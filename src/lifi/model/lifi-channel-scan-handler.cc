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

NS_OBJECT_ENSURE_REGISTERED(LifiChannelScanHandler);

LifiChannelScanHandler::LifiChannelScanHandler() {
	NS_LOG_FUNCTION (this);
	m_UnscannedChannel = 0x7f;
	m_curChannel = CHANNEL1;
	m_channelScanRunning = false;
	m_receiveBeaconNum = 0;
	m_storedVPANId = 0xff;
	m_scanDuration = 0;
	m_scanType = ACTIVE_SCAN;
	AddTrigger(LifiChannelScanHandler::ReceiveBeacon, false);
}

LifiChannelScanHandler::LifiChannelScanHandler(LifiMacImpl* impl, DataService* service, MlmeSapUser* user) {
	NS_LOG_FUNCTION (this);
	m_UnscannedChannel = 0x7f;
	m_curChannel = CHANNEL1;
	m_channelScanRunning = false;
	m_receiveBeaconNum = 0;
	m_scanDuration = 0;
	m_storedVPANId = 0xff;
	m_scanType = ACTIVE_SCAN;
	m_dataService = service;
	m_impl = impl;
	m_user = user;
	AddTrigger(LifiChannelScanHandler::ReceiveBeacon, false);
}

LifiChannelScanHandler::~LifiChannelScanHandler() {
}

TypeId LifiChannelScanHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiChannelScanHandler")
						.SetParent<LifiMacHandler> ()
						.AddConstructor<LifiChannelScanHandler> ();
	return tid;
}

void LifiChannelScanHandler::StartChannelScan(ScanType scanType, uint32_t duration) {
	NS_LOG_FUNCTION (this);
	if(m_channelScanRunning){
		VpanDescriptors tempVPANDes;
		tempVPANDes.clear();
//		m_user->MlmeScanConfirm(SCAN_IN_PROGRESS, scanType, m_channelUnscanned, 0, tempVPANDes);
	}else{
		m_channelScanRunning = true;
////    0xffff is the default value of "m_attributes->macVPANId "if device is not associated.
		m_storedVPANId = m_attributes->macVPANId;
		m_attributes->macVPANId = MAC_SCAN_VPANID;
		m_scanType = scanType;
		NS_ASSERT(duration <= MAX_SCAN_DURATION);
		m_scanDuration = LifiMac::aBaseSuperframeDuration * (2 * duration + 1);

		if(m_scanType == ACTIVE_SCAN){
			SendBeaconRequest();
		}
		StartNewChannel();
	}

}

void LifiChannelScanHandler::StartNewChannel(){
	NS_LOG_FUNCTION(this);
	Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());
	Time tempScanTime = NanoSeconds(m_scanDuration * op.GetNanoSeconds());
	m_timer.SetFunction(&LifiChannelScanHandler::EndScannOnOneChannel, this);
	m_timer.SetDelay(tempScanTime);
	m_timer.Schedule();
	EnableTrigger(LifiChannelScanHandler::ReceiveBeacon);
}

void LifiChannelScanHandler::EndScannOnOneChannel() {
	NS_LOG_FUNCTION(this << "currentScanChannel:" << (uint8_t)m_curChannel);
	m_UnscannedChannel = (~m_curChannel) & m_UnscannedChannel;
	m_curChannel = (LogicChannelId)((uint8_t)m_curChannel << 1);
	if(!m_UnscannedChannel){
		FinishScan();
	}else{
		StartNewChannel();
	}
	DisableTrigger(LifiChannelScanHandler::ReceiveBeacon);
}

void LifiChannelScanHandler::SendBeaconRequest() {
////we don't need this function! if so you can do it later.
}

void LifiChannelScanHandler::ReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu) {
	if(CheckTrigger(LifiChannelScanHandler::ReceiveBeacon)){
		NS_LOG_FUNCTION (this << timestamp << msdu);
		onReceiveBeacon(timestamp, msdu);
	}else{
		NS_LOG_ERROR("Ignore LifiChannelScanHandler::ReceiveBeacon");
	}
}

void LifiChannelScanHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION(this << timestamp << msdu);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = msdu->Copy();
	tempPacket->RemoveHeader(header);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(tempPacket);
	VPANDescriptor Descri;
	Descri.coordAddr = header.GetSrcAddress();
	Descri.coordAddrMode = header.GetSrcAddressMode();
	Descri.coordVPANId = header.GetSrcVPANId();
	Descri.gtsPermit = beacon.GetGtsPermit();
	Descri.supframeSpec.assocPermit = beacon.GetAssocPermit();
	Descri.supframeSpec.beaconOrder = beacon.GetBcnOrder();
	Descri.supframeSpec.finalCapSlot = beacon.GetFinalCapSlot();
	Descri.supframeSpec.supframeOrder = beacon.GetSupframeOrder();
	Descri.supframeSpec.vpanCoord = beacon.GetVpanCoord();
	Descri.logicChannel = m_curChannel;
	if (m_attributes->macAutoRequest)
	{
		m_VPANDescriptors.push_back(Descri);
	}else
	{
//		PendAddrSpec tempSpec;
//		tempSpec.numOfExtendedAddr = beacon.GetExtendedAddressCount();
//		tempSpec.numOfShortAddr = beacon.GetShortAddressCount();
//		uint32_t payloadSize = beacon.GetPaylaodSize();
//		uint8_t *payload = beacon.GetPayload(&payloadSize);
//		m_user->MlmeBeaconNotify(m_attributes->macBSN, Descri, tempSpec, beacon.GetAddrs(), payloadSize, payload);
	}
	m_receiveBeaconNum ++;
	if (m_receiveBeaconNum > MAX_BEACON_PER_CHANNEL)
	{
		NS_LOG_DEBUG ("m_receiveBeaconNum > MAX_BEACON_PER_CHANNEL, terminate scanning in current channel: "
				<< (uint32_t)m_curChannel);
		m_receiveBeaconNum = 0;
		if (!m_timer.IsExpired())
				m_timer.Cancel();
		EndScannOnOneChannel ();
	}

}

void LifiChannelScanHandler::FinishScan() {
	NS_LOG_FUNCTION(this);
	if(m_VPANDescriptors.empty()){
//		m_user->MlmeScanConfirm(NO_BEACON, m_scanType, (uint8_t)m_UnscannedChannel,
//								m_VPANDescriptors.size(), m_VPANDescriptors);
	}else if(m_attributes->macAutoRequest){
//		m_user->MlmeScanConfirm(MAC_SUCCESS, m_scanType, (uint8_t)m_UnscannedChannel,
//								m_VPANDescriptors.size(), m_VPANDescriptors);
	}
	Reset();
}

void LifiChannelScanHandler::Reset() {
	NS_LOG_FUNCTION (this);
	m_UnscannedChannel = 0x7f;
	m_curChannel = CHANNEL1;
	m_channelScanRunning = false;
	m_scanDuration = 0;
	m_attributes->macVPANId = m_storedVPANId;
	m_storedVPANId = 0;
	m_scanType = ACTIVE_SCAN;
	m_receiveBeaconNum = 0;
}

void LifiChannelScanHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiChannelScanHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiChannelScanHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiChannelScanHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiChannelScanHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiChannelScanHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiChannelScanHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiChannelScanHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiChannelScanHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiChannelScanHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}

void LifiChannelScanHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

} /* namespace ns3 */
