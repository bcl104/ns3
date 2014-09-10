/*
 * lifi-coord-trx-handler.cc
 *
 *  Created on: 2014年5月3日
 *      Author: will
 */

#include "lifi-coord-trx-handler.h"
#include "lifi-mac-beacon.h"
#include "lifi-mac-coord-impl.h"
#include "lifi-mac-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiCoordTrxHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiCoordTrxHandler);

LifiCoordTrxHandler::LifiCoordTrxHandler()
						: m_running (false),
						  m_gtsCount(0),
						  m_curGtsCount(0)
{
	NS_LOG_FUNCTION (this);
	AddListener(GetTypeId(), this);
}

LifiCoordTrxHandler::~LifiCoordTrxHandler()
{
	NS_LOG_FUNCTION (this);
}

TypeId LifiCoordTrxHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiCoordTrxHandler")
								.SetParent<LifiTrxHandler> ()
								.AddConstructor<LifiCoordTrxHandler>();
	return tid;
}

void LifiCoordTrxHandler::SuperframeEnd()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (!m_curTransmission.IsAvailable());
	NS_ASSERT (!m_curTranceiverTask.Available());
	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::ReceivePacket);
	m_superframeStruct.m_synchronized = false;
	m_superframeStruct.m_state = SuperframeStrcut::DEFAULT;
	if (m_running)
		Simulator::ScheduleNow (&LifiCoordTrxHandler::BeaconStart, this);
}

void LifiCoordTrxHandler::Start()
{
	NS_LOG_FUNCTION (this);
	m_running = true;
	m_superframeStruct.m_synchronized = true;
	BeaconStart();
}

void LifiCoordTrxHandler::TxResultNotification(MacOpStatus status, PacketInfo info,
		Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << status << ack);
	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::BEACON);
	NS_ASSERT (status == MAC_SUCCESS);
	LifiMacHeader header;
	Ptr<Packet> p = info.m_packet;
	p->RemoveHeader(header);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(p);

//	uint16_t tempCapSlot = beacon.GetFinalCapSlot();
//	std::cout<<tempCapSlot << std::endl;

	m_superframeStruct.m_capEnd.Schedule();
	if (m_superframeStruct.m_contentionFreePeriod)
		m_superframeStruct.m_cfpEnd.Schedule();

//	m_gtsCount = m_gtsCoordHandler->GetGtsCount();
//    m_superframGtsList = m_gtsCoordHandler->GetGtsDescList();
    Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
    m_gtsCount = coordImpl->GetGtsDesCount();
	m_superframGtsList = coordImpl->GetGtsLists();

	Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());

	for(GtsList::reverse_iterator it = m_superframGtsList.rbegin(); it != m_superframGtsList.rend(); it ++){
		m_curGtsTimer.shortAddr = (*it).deviceShortAddr;
		m_curGtsTimer.gtsdirc = (GTSDirection)beacon.GetGtsDirection((*it).deviceShortAddr);
		uint32_t curGts = (1+(*it).gtsStartSlot) * LifiMac::aBaseSlotDuration * pow (2, m_attributes->macSuperframeOrder);
		Time curGtsStart = NanoSeconds(curGts * op.GetNanoSeconds());
		m_curGtsTimer.gtsStartTime.SetFunction(&LifiCoordTrxHandler::StartOneGts, this);
		m_curGtsTimer.gtsStartTime.SetArguments(m_curGtsTimer.shortAddr, m_curGtsTimer.gtsdirc);
		m_curGtsTimer.gtsStartTime.SetDelay(curGtsStart);
		if(it != m_superframGtsList.rbegin())
			m_curGtsTimer.gtsStartTime.Schedule();
		m_gtsTimerList.push_back(m_curGtsTimer);
	}

	m_superframeStruct.m_supframeEnd.Schedule();
	Simulator::ScheduleNow(&LifiCoordTrxHandler::SuperframeStart, this);
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::ReceivePacket);
}

void LifiCoordTrxHandler::BeaconStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_synchronized = true;
	m_superframeStruct.m_state = SuperframeStrcut::BEACON;
	Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
	uint32_t bcnIntval = LifiMac::aBaseSuperframeDuration * pow(2, m_attributes->macBeaconOrder);
	uint32_t superframDur = LifiMac::aBaseSuperframeDuration * pow (2, m_attributes->macSuperframeOrder);
	uint32_t capDur = (16 - coordImpl->m_gtsSlotCount) * LifiMac::aBaseSlotDuration * pow (2, m_attributes->macSuperframeOrder);

	Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());
	Time bcnIntvalTime = NanoSeconds(bcnIntval * op.GetNanoSeconds());
	Time capEnd = NanoSeconds(capDur * op.GetNanoSeconds());
	Time cfpEnd = NanoSeconds(superframDur * op.GetNanoSeconds());

	m_superframeStruct.m_capEnd.SetFunction(&LifiCoordTrxHandler::ContentionAccessPeriodEnd, this);
	m_superframeStruct.m_cfpEnd.SetFunction(&LifiCoordTrxHandler::ContentionFreePeriodEnd, this);
	m_superframeStruct.m_supframeEnd.SetFunction(&LifiCoordTrxHandler::SuperframeEnd, this);

	m_superframeStruct.m_contentionFreePeriod = (cfpEnd > capEnd);
	m_superframeStruct.m_inactivePortion = (bcnIntvalTime > cfpEnd);

	NS_ASSERT ((bcnIntvalTime >= capEnd) && (bcnIntvalTime >= cfpEnd));
	NS_ASSERT ((cfpEnd >= capEnd));
	NS_ASSERT (capEnd > 0);

	m_superframeStruct.m_capEnd.SetDelay (capEnd);
	m_superframeStruct.m_cfpEnd.SetDelay (cfpEnd);
	m_superframeStruct.m_supframeEnd.SetDelay (bcnIntvalTime);

	TransmitBeacon();
}

void LifiCoordTrxHandler::Stop()
{
	NS_LOG_FUNCTION (this);
	m_running = false;
}

void LifiCoordTrxHandler::TransmitBeacon()
{
	NS_LOG_FUNCTION (this);
//	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::BEACON);
	Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
	Ptr<Packet> beacon = coordImpl->ConstructBeacon();
	NS_ASSERT (!m_curTransmission.IsAvailable());
	m_curTransmission.Reset();
	m_curTransmission.m_info.m_band = CHANNEL1;
	m_curTransmission.m_info.m_bust = false;
	m_curTransmission.m_info.m_force = true;
	m_curTransmission.m_info.m_handle = 1;	// handle number 1 is for beacon transmission.
	m_curTransmission.m_info.m_listener = this;
	m_curTransmission.m_info.m_option.ackTx = false;
	m_curTransmission.m_info.m_option.indirectTx = false;
	m_curTransmission.m_info.m_msduSize = beacon->GetSize();
	m_curTransmission.m_info.m_packet = beacon;

	DoTransmitData();
}

void LifiCoordTrxHandler::ContentionFreePeriodStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_state = SuperframeStrcut::CFP;
	NS_ASSERT (!m_curTransmission.IsAvailable());
	GtsTimer::iterator it = m_gtsTimerList.begin();
	StartOneGts((*it).shortAddr, (*it).gtsdirc);
}

void LifiCoordTrxHandler::ContentionFreePeriodEnd()
{
	NS_LOG_FUNCTION (this);
	GtsTimer::reverse_iterator it = m_gtsTimerList.rbegin();
	if((*it).gtsdirc == (GTSDirection)SuperframeStrcut::TX_tranceiver){
		m_gtsHandler->EndGtsTransmit();
	}else if((*it).gtsdirc == (GTSDirection)SuperframeStrcut::RX_tranceiver){
		m_gtsHandler->CloseGtsDataReceive();
	}
	m_curTransmission.Reset();
	m_curTranceiverTask.Clear();
	if (m_superframeStruct.m_inactivePortion)
	{
		InactionPortionStart ();
	}
}

void LifiCoordTrxHandler::StartOneGts(uint16_t shortAddr, GTSDirection gtsDirec){
	NS_LOG_FUNCTION(this);
	m_curGtsCount ++;
	if(gtsDirec == (GTSDirection)SuperframeStrcut::TX_tranceiver){
		m_gtsHandler->SetGtsTransmitArgument(shortAddr, true);
	}else if(gtsDirec == (GTSDirection)SuperframeStrcut::RX_tranceiver){
		m_gtsHandler->OpenGtsDataReceive(shortAddr);
	}
}

} /* namespace ns3 */
