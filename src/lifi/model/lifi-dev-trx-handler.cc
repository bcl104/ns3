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
		Time gtsStartTime = NanoSeconds(gtsStart * op.GetNanoSeconds() -1);
		Time gtsEndTime = NanoSeconds(gtsEnd * op.GetNanoSeconds() -1);
		m_superframeStruct.m_gtsStart.SetDelay(gtsStartTime);
		m_superframeStruct.m_gtsEnd.SetDelay(gtsEndTime);
		m_superframeStruct.m_gtsStart.SetFunction(&LifiDevTrxHandler::GtsTransmitStart, this);
		m_superframeStruct.m_gtsEnd.SetFunction(&LifiDevTrxHandler::GtsTransmitEnd, this);

		if(startSlot == capEndSlot - 1){
			m_superframeStruct.m_gtsEnd.Schedule();
			m_gtsIsCapEnd = true;
		}
		if(startSlot + gtsLength == 15){
			m_superframeStruct.m_gtsStart.Schedule();
			m_gtsIsCfpEnd = true;
		}
	}
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
		m_gtsHandler->SendGtsDatas();
	}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
		m_gtsHandler->OpenGtsDataReceive(0);
	}
}
void LifiDevTrxHandler::GtsTransmitEnd(){
	NS_LOG_FUNCTION (this);
	if(!m_gtsIsCfpEnd){
		if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
			m_gtsHandler->EndGtsTransmit();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_gtsHandler->CloseGtsDataReceive();
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
			m_gtsHandler->SendGtsDatas();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_gtsHandler->OpenGtsDataReceive(0);
		}
	}
}

void LifiDevTrxHandler::ContentionFreePeriodEnd()
{
	NS_LOG_FUNCTION (this);
	if(m_gtsIsCfpEnd){
		if(m_superframeStruct.m_gtsState == SuperframeStrcut::TX_tranceiver){
			m_gtsHandler->EndGtsTransmit();
		}else if(m_superframeStruct.m_gtsState == SuperframeStrcut::RX_tranceiver){
			m_gtsHandler->CloseGtsDataReceive();
		}
	}
	m_curTransmission.Reset();
	m_curTranceiverTask.Clear();
	if (m_superframeStruct.m_inactivePortion)
	{
		InactionPortionStart ();
	}
}

} /* namespace ns3 */
