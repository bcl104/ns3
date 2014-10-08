/*
 * lifi-dev-trx-handler.cc
 *
 *  Created on: 2014年5月3日
 *      Author: will
 */

#include "lifi-dev-trx-handler.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiDevTrxHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiDevTrxHandler);

LifiDevTrxHandler::LifiDevTrxHandler()
{
	NS_LOG_FUNCTION (this);
	m_gtsIsCapEnd = false;
	m_gtsIsCfpEnd = false;
	AddListener(LifiDevTrxHandler::GetTypeId(), this);
}

LifiDevTrxHandler::~LifiDevTrxHandler()
{
	// TODO Auto-generated destructor stub
}

TypeId LifiDevTrxHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiDevTrxHandler")
						.SetParent<LifiTrxHandler> ()
						.AddConstructor<LifiDevTrxHandler> ();
	return tid;
}

void LifiDevTrxHandler::BuildSuperframeStruct(Ptr<Packet> beacon)
{
	NS_LOG_FUNCTION (this);
	LifiMacHeader header;
	Ptr<Packet> tempPacket = beacon->Copy();
	tempPacket->RemoveHeader(header);
	LifiMacBeacon bcn = LifiMacBeacon::Construct(tempPacket);
	uint8_t bcnOrder = bcn.GetBcnOrder();
	uint8_t superframeOrder = bcn.GetSupframeOrder();
	capEndSlot = bcn.GetFinalCapSlot();

	// Only support beacon-enable VPAN.
	NS_ASSERT ((bcnOrder >=0) && (bcnOrder <= 14));
	NS_ASSERT (bcnOrder >= superframeOrder);
	if (m_attributes != 0)
	{
		m_impl->m_attributes.macBeaconOrder = (uint32_t)bcnOrder;
		m_impl->m_attributes.macSuperframeOrder = (uint32_t)superframeOrder;
	}
	uint32_t bcnIntval = LifiMac::aBaseSuperframeDuration * pow(2, (uint32_t)bcnOrder);
	uint32_t superframDur = LifiMac::aBaseSuperframeDuration * pow (2, (uint32_t)superframeOrder);
	uint32_t capDur = ((uint32_t)capEndSlot + 1) * LifiMac::aBaseSlotDuration * pow (2, (uint32_t)superframeOrder);

	Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());
	Time bcnIntvalTime = NanoSeconds(bcnIntval * op.GetNanoSeconds() -1);
	Time capEnd = NanoSeconds(capDur * op.GetNanoSeconds() -1);
	Time cfpEnd = NanoSeconds(superframDur * op.GetNanoSeconds() -1);

	NS_ASSERT ((bcnIntvalTime >= capEnd) && (bcnIntvalTime >= cfpEnd));
	NS_ASSERT ((cfpEnd >= capEnd));
	NS_ASSERT (capEnd > 0);

	m_superframeStruct.m_contentionFreePeriod = (cfpEnd > capEnd);
	m_superframeStruct.m_inactivePortion = (bcnIntvalTime > cfpEnd);

	m_superframeStruct.m_capEnd.SetDelay(capEnd);
	m_superframeStruct.m_cfpEnd.SetDelay(cfpEnd);
	m_superframeStruct.m_supframeEnd.SetDelay(bcnIntvalTime);

	m_superframeStruct.m_capEnd.SetFunction(&LifiDevTrxHandler::ContentionAccessPeriodEnd, this);
	m_superframeStruct.m_cfpEnd.SetFunction(&LifiDevTrxHandler::ContentionFreePeriodEnd, this);
	m_superframeStruct.m_supframeEnd.SetFunction(&LifiDevTrxHandler::SuperframeEnd, this);

	m_superframeStruct.m_capEnd.Schedule();
	if (m_superframeStruct.m_contentionFreePeriod)
		m_superframeStruct.m_cfpEnd.Schedule();
	m_superframeStruct.m_supframeEnd.Schedule();

	m_superframeStruct.m_synchronized = true;
	Simulator::ScheduleNow (&LifiDevTrxHandler::SuperframeStart, this);
}

void LifiDevTrxHandler::ReceiveBeacon(uint32_t timestamp, Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << timestamp << p);
	BuildSuperframeStruct (p);
}

void LifiDevTrxHandler::SetGtsDuration(uint8_t startSlot, uint8_t gtsLength, uint8_t gtsCount){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(gtsCount != 0);
	if(gtsCount == 1){
		m_gtsIsCapEnd = true;
		m_gtsIsCfpEnd = true;
	}else if(gtsCount > 1){
		uint32_t gtsStart = (1 + startSlot) * LifiMac::aBaseSlotDuration * pow (2, m_attributes->macSuperframeOrder);
		uint32_t gtsEnd = (1 + startSlot + gtsLength) * LifiMac::aBaseSlotDuration * pow (2, m_attributes->macSuperframeOrder);
		Time op = *(m_impl->GetLifiMac()->GetOpticalPeriod());
		Time gtsStartTime = NanoSeconds(gtsStart * op.GetNanoSeconds() + 1);
		Time gtsEndTime = NanoSeconds(gtsEnd * op.GetNanoSeconds() - 1);
		m_superframeStruct.m_gtsStartDev.SetDelay(gtsStartTime);
		m_superframeStruct.m_gtsEndDev.SetDelay(gtsEndTime);
		m_superframeStruct.m_gtsStartDev.SetFunction(&LifiDevTrxHandler::GtsTransmitStart, this);
		m_superframeStruct.m_gtsEndDev.SetFunction(&LifiDevTrxHandler::GtsTransmitEnd, this);

		if(startSlot == capEndSlot){
			m_superframeStruct.m_gtsEndDev.Schedule();
			m_gtsIsCapEnd = true;
		}
		if(startSlot + gtsLength == 15){
			m_superframeStruct.m_gtsStartDev.Schedule();
			m_gtsIsCfpEnd = true;
		}else if((startSlot != capEndSlot) && (startSlot + gtsLength != 15)){
			m_superframeStruct.m_gtsStartDev.Schedule();
			m_superframeStruct.m_gtsEndDev.Schedule();
			}
	}
}

void LifiDevTrxHandler::DeleteGtsDuration(){
	m_gtsIsCapEnd = false;
	m_gtsIsCfpEnd = false;
	m_superframeStruct.m_gtsStartDev.Cancel();
	m_superframeStruct.m_gtsEndDev.Cancel();
}

void LifiDevTrxHandler::SetGtsTrxState(GTSDirection direction){
	NS_LOG_FUNCTION(this);
	if(direction == Transmit_GTS)
		m_superframeStruct.m_gtsState = SuperframeStrcut::TX_tranceiver;
	else
		m_superframeStruct.m_gtsState = SuperframeStrcut::RX_tranceiver;
}

void LifiDevTrxHandler::GtsTransmitStart(){
	NS_LOG_FUNCTION (this);
	if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
		m_impl->SendGtsDatas();
	}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
		m_impl->OpenGtsDataReceive(0);
	}
}
void LifiDevTrxHandler::GtsTransmitEnd(){
	NS_LOG_FUNCTION (this);
	if(!m_gtsIsCfpEnd){
		if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
			m_impl->EndGtsTransmit();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_impl->CloseGtsDataReceive();
		}
	}
}

void LifiDevTrxHandler::ContentionFreePeriodStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_state = SuperframeStrcut::CFP;
	NS_ASSERT (!m_curTransmission.IsAvailable());
	if(m_gtsIsCapEnd){
		if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
			m_impl->SendGtsDatas();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_impl->OpenGtsDataReceive(0);
		}
	}
}

void LifiDevTrxHandler::ContentionFreePeriodEnd()
{
	NS_LOG_FUNCTION (this);
	if(m_gtsIsCfpEnd){
		if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
			m_impl->EndGtsTransmit();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_impl->CloseGtsDataReceive();
		}
	}
	std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
	m_curTransmission.Reset();
	m_curTranceiverTask.Clear();
	if (m_superframeStruct.m_inactivePortion)
	{
		InactionPortionStart ();
	}
}

bool LifiDevTrxHandler::DoTransmitData() {
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
		std::cout << m_superframeStruct.m_gtsEndDev.GetDelayLeft() << std::endl;
		if(m_gtsIsCfpEnd){
			enough = (m_superframeStruct.m_cfpEnd.GetDelayLeft() > (txDuration+ackWaitTime));
		}else{
			enough = (m_superframeStruct.m_gtsEndDev.GetDelayLeft() > (txDuration+ackWaitTime));
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
