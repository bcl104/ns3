/*
 * lifi-channel-scan-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-channel-scan-handler.h"
#include "ns3/log.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"

NS_LOG_COMPONENT_DEFINE ("LifiChannelScanHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiChannelScanHandler);

LifiChannelScanHandler::LifiChannelScanHandler() {
	NS_LOG_FUNCTION (this);
	m_attributes = 0;
	m_channelUnscanned = 0xff;
	m_curChannel = CHANNEL1;
	m_dataService = 0;
	m_lifiMacImpl = 0;
	m_provider = 0;
	m_run = false;
	m_bcnRcved = 0;
}

LifiChannelScanHandler::LifiChannelScanHandler(LifiMacImpl* impl, DataService* service, MlmeSapUser* user) {
	NS_LOG_FUNCTION (this);
	LifiChannelScanHandler ();
	m_dataService = service;
	m_lifiMacImpl = impl;
	m_user = user;
	m_timer = Timer(Timer::CANCEL_ON_DESTROY);
}

LifiChannelScanHandler::~LifiChannelScanHandler() {
	NS_LOG_FUNCTION (this);
}

TypeId LifiChannelScanHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiChannelScanHandler")
						.SetParent<Object> ()
						.AddConstructor<LifiChannelScanHandler> ();
	return tid;
}

void LifiChannelScanHandler::TxResultNotification(MacOpStatus status) {
	NS_LOG_FUNCTION (this);
	onTxRuesult(status);
}

bool LifiChannelScanHandler::Reset() {
	NS_LOG_FUNCTION (this);
	if (m_run)
	{
		// 	If the channel scan handler is running, it will finish scanning current channel and reset
		// 	afterwards.
		m_run = false;
		return false;
	}
	m_channelUnscanned = 0;
	m_curChannel = CHANNEL1;
	m_scanDuration = 0;
	m_storedVPANId = 0;
	m_scanType = INACTIVE_SCAN;
	return true;
}

void LifiChannelScanHandler::onTxRuesult(MacOpStatus status) {
	NS_LOG_FUNCTION (this);
}

bool LifiChannelScanHandler::Start(ScanType scanType, uint32_t duration) {
	NS_LOG_FUNCTION (this);
	NS_ASSERT_MSG (m_run == false, "LifiChannelScanHandler::Start can not be called when the handler is running.");
	m_state = NEW_CHANNEL;
	m_run = true;
	m_storedVPANId = m_attributes->macVPANId;
	m_attributes->macVPANId = MAC_FILTER_ALL;
	m_scanType = scanType;
	m_curChannel = CHANNEL1;

	if (duration > MAX_CHANNEL_DURATION)
	{
		m_scanDuration = MAX_CHANNEL_DURATION;
	}else
	{
		m_scanDuration = duration;
	}
	DoRun();
	return true;
}

void LifiChannelScanHandler::SetMacPibAttribtes(LifiMacPibAttribute* attributes) {
	m_attributes = attributes;
}

void LifiChannelScanHandler::DoRun()
{
	// DoRun method is first invoked after the initialization.
	if (m_state == NEW_CHANNEL)
	{
		//	Channel scanning of current channel will stop in number of m_scanDuration
		//	optical clock.
		m_timer.SetFunction(&LifiChannelScanHandler::EndScannOnOneChannel, this);

		// 	The value of delay will be modified later.
		m_timer.Schedule(Seconds(10));

		if (m_scanType == ACTIVE_SCAN)
		{
			SendBeaconRequest();
			m_state = WAIT_FOR_BCN;
			return;
		}

		if (m_scanType == INACTIVE_SCAN)
		{
			m_state = WAIT_FOR_BCN;
			return;
		}
	}

	if (m_state == WAIT_FOR_BCN)
	{
		NS_FATAL_ERROR("DoRun should not be called when handler in state WAIT_FOR_BCN.");
	}
}

void LifiChannelScanHandler::EndScannOnOneChannel() {
	NS_LOG_FUNCTION(this);
	NS_LOG_INFO(Simulator::Now()<<" Channel scanning-current channel: " << (uint32_t)m_curChannel);
	// When the Reset() method is called and the handler is scanning channel, the handler
	// will stop and reset its parameters after it finishing scanning on current channel.
	if (!m_run)
	{
		Reset();
		if (!m_timer.IsExpired())
			m_timer.Remove();
		m_state = IDLE;
		return;
	}

	// 7 logic channel has already been scanned.
	if (!SwitchChannel())
	{
		Complete();
		return;
	}

	// Reset the timer.
	if (!m_timer.IsExpired())
		m_timer.Remove();

	DoRun();
}

void LifiChannelScanHandler::SendBeaconRequest() {
	BeaconRequestComm comm;
	Ptr<Packet> p = comm.GetPacket();
	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
}

void LifiChannelScanHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu) {
	LifiMacHeader header;
	msdu->RemoveHeader(header);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(msdu);
	VPANDescriptor d;
	d.coordAddr = header.GetSrcAddress();
	d.coordAddrMode = header.GetSrcAddressMode();
	d.coordVPANId = header.GetSrcVPANId();
	d.gtsPermit = beacon.GetGtsPermit();
	d.supframeSpec.assocPermit = beacon.GetAssocPermit();
	d.supframeSpec.beaconOrder = beacon.GetBcnOrder();
	d.supframeSpec.finalCapSlot = beacon.GetFinalCapSlot();
	d.supframeSpec.supframeOrder = beacon.GetSupframeOrder();
	d.supframeSpec.vpanCoord = beacon.GetVpanCoord();
	d.logicChannel = m_curChannel;
	PendAddrSpec pas;
	pas.numOfExtendedAddr = beacon.GetExtendedAddressCount();
	pas.numOfShortAddr = beacon.GetShortAddressCount();
	uint32_t payloadSize;
	uint8_t *payload = beacon.GetPayload(&payloadSize);
	if (m_attributes->macAutoRequest)
	{
		m_VPANDescriptors.push_back(d);
	}else
	{
		m_user->MlmeBeaconNotify(m_attributes->macBSN, d, pas, beacon.GetAddrs(), payloadSize, payload);
	}
	m_bcnRcved ++;
	if (m_bcnRcved >= MAX_BEACON_PER_CHANNEL)
	{
		NS_LOG_DEBUG ("m_bcnRcved >= MAX_BEACON_PER_CHANNEL, terminate scanning current channel "
				<< (uint32_t)m_curChannel);
		m_bcnRcved = 0;
		EndScannOnOneChannel ();
	}

}

void LifiChannelScanHandler::Complete() {
	NS_LOG_FUNCTION(this);
	if (m_attributes->macAutoRequest == true)
	{
		MacOpStatus status = (m_VPANDescriptors.empty()== true)? NO_BEACON : MAC_SUCCESS;
		m_user->MlmeScanConfirm(status, m_scanType, (uint8_t)m_channelUnscanned,
								m_VPANDescriptors.size(), m_VPANDescriptors);
	}
}

void LifiChannelScanHandler::ReceiveBeacon(uint32_t timestamp,
		Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this << timestamp << msdu);
	onReceiveBeacon(timestamp, msdu);
}

bool LifiChannelScanHandler::IsRun() {
	return m_run;
}

bool LifiChannelScanHandler::SwitchChannel() {
	NS_LOG_FUNCTION (this);
	if ((uint8_t)m_curChannel == 0x00)
		return false;
	m_curChannel = (LogicChannelId)((uint8_t)m_curChannel << 1);
	m_channelUnscanned = (~m_curChannel) & m_channelUnscanned;
	m_state = NEW_CHANNEL;
	return true;
}

} /* namespace ns3 */
