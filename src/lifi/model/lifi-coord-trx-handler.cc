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
	m_gtsCount = 0;
	m_curGtsCount = 0;
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
	if (m_superframeStruct.m_contentionFreePeriod){
		m_superframeStruct.m_cfpEnd.Schedule();

		//	m_gtsCount = m_gtsCoordHandler->GetGtsCount();
		//    m_superframGtsList = m_gtsCoordHandler->GetGtsDescList();
		Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
		m_gtsCount = coordImpl->GetGtsDesCount();
		m_superframGtsList = coordImpl->GetBeaconGtsLists();

		Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());

		for(GtsList::reverse_iterator it = m_superframGtsList.rbegin(); it != m_superframGtsList.rend(); it ++){
			std::cout << m_superframGtsList.size() << std::endl;
			GtsStructure m_curGtsTimer;
			m_curGtsTimer.shortAddr = (*it).deviceShortAddr;
			m_curGtsTimer.gtsdirc = (GTSDirection)beacon.GetGtsDirection((*it).deviceShortAddr);
			uint32_t curGts = (1+(*it).gtsStartSlot) * LifiMac::aBaseSlotDuration * pow (2, m_attributes->macSuperframeOrder);
			Time curGtsStart = NanoSeconds(curGts * op.GetNanoSeconds());
			(*m_curGtsTimer.gtsStartTime).SetFunction(&LifiCoordTrxHandler::StartOneGts, this);
			(*m_curGtsTimer.gtsStartTime).SetArguments(m_curGtsTimer.shortAddr, m_curGtsTimer.gtsdirc);
			(*m_curGtsTimer.gtsStartTime).SetDelay(curGtsStart);
			if(it != m_superframGtsList.rbegin())
				(*m_curGtsTimer.gtsStartTime).Schedule();
			std::cout << (*m_curGtsTimer.gtsStartTime).GetDelayLeft() << std::endl;
			m_gtsTimerList.push_back(m_curGtsTimer);
		}
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
	coordImpl->SetGTSCount();
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
	NS_ASSERT (!m_curTransmission.IsAvailable());std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
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
		m_impl->EndGtsTransmit();
	}else if((*it).gtsdirc == (GTSDirection)SuperframeStrcut::RX_tranceiver){
		m_impl->CloseGtsDataReceive();
	}
	std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
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
////gtsDirec is only point at Device(仅仅针对Device而言，对Device来说是Transmit_GTS,对Coord来说就是Receive_GTS)
//	if(gtsDirec == (GTSDirection)SuperframeStrcut::TX_tranceiver){
//		m_impl->SetGtsTransmitArgument(shortAddr, true);
//	}else if(gtsDirec == (GTSDirection)SuperframeStrcut::RX_tranceiver){
//		m_impl->OpenGtsDataReceive(shortAddr);
//	}
	if(gtsDirec == (GTSDirection)SuperframeStrcut::RX_tranceiver){
		m_impl->SetGtsTransmitArgument(shortAddr, true);
	}else if(gtsDirec == (GTSDirection)SuperframeStrcut::TX_tranceiver){
		m_impl->OpenGtsDataReceive(shortAddr);
	}
}

bool LifiCoordTrxHandler::DoTransmitData() {
	NS_LOG_FUNCTION (this << m_impl->m_opticalPeriod->GetNanoSeconds() << m_curTransmission.m_info.m_listener);

	m_plmeProvider->PlmeSetTRXStateRequest(TX_ON);
	NS_ASSERT (m_curTransmission.IsAvailable());

	/*
	 * If check whether there is enough time before the end of CAP.
	 * */
	uint8_t mcsid = m_plmeProvider->PlmeGetRequset<uint8_t>(PHY_MCSID);
	double dataRateKbps = LifiPhy::GetRate(mcsid);
//	std::cout << m_curTransmission.m_info.m_packet->GetSize() << std::endl;
//	std::cout << m_impl->m_opticalPeriod->GetNanoSeconds() << std::endl;
	Time txDuration = NanoSeconds(((double) m_curTransmission.m_info.m_packet
								->GetSize()*8)/(dataRateKbps*1000)*1e9);
	Time ackWaitTime;
	if (m_curTransmission.m_info.m_option.ackTx)
		ackWaitTime = NanoSeconds(m_attributes->macAckWaitDuration
				*m_impl->m_opticalPeriod->GetNanoSeconds());
	else
		ackWaitTime = NanoSeconds(0);

	bool enough = false;

	if (m_superframeStruct.m_state == SuperframeStrcut::BEACON)
	{
		NS_ASSERT (m_curTransmission.m_info.m_handle == 1);
		enough = true;
	}else if (m_superframeStruct.m_state == SuperframeStrcut::CAP)
	{
		NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
		enough = (m_superframeStruct.m_capEnd.GetDelayLeft() > (txDuration+ackWaitTime));
	}else if (m_superframeStruct.m_state == SuperframeStrcut::CFP)
	{
		NS_ASSERT (m_curTransmission.m_info.m_option.gtsTx);
		std::cout << m_superframeStruct.m_cfpEnd.GetDelayLeft() << std::endl;
		std::cout << m_gtsTimerList.size() << std::endl;
		GtsTimer::iterator iter = m_gtsTimerList.begin() + m_curGtsCount;
		if(iter != m_gtsTimerList.end()){
			std::cout << (*(*iter).gtsStartTime).GetState() << std::endl;
			std::cout << (*(*iter).gtsStartTime).GetDelayLeft() << std::endl;
			enough = ((*(*iter).gtsStartTime).GetDelayLeft() > (txDuration+ackWaitTime));
		}else{
			enough = (m_superframeStruct.m_cfpEnd.GetDelayLeft() > (txDuration+ackWaitTime));
		}

	}else
	{
		NS_FATAL_ERROR("Error transmit timing.");
	}

	if (!enough)
	{
		if ((!m_curTransmission.m_info.m_option.gtsTx)
		&& (!m_curTransmission.m_info.m_force))
		{
			m_suspendedTransmission = m_curTransmission;
		}
		std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
		m_curTransmission.Reset();
		return false;
	}

//	std::cout << Simulator::Now() << std::endl;
//	std::cout << m_curTransmission.m_info.m_packet->GetSize() << std::endl;
	m_pdProvider->DataRequest(m_curTransmission.m_info.m_packet->GetSize(),
							  m_curTransmission.m_info.m_packet,
							  m_curTransmission.m_info.m_band);
	// Enable the external trigger to onTxConfirm.
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::ReceivePacket);

	EnableTrigger(LifiTrxHandler::TxConfirm);
	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);

	return true;
}

} /* namespace ns3 */
