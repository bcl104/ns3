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
	// TODO Auto-generated constructor stub

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
	beacon->RemoveHeader(header);
	LifiMacBeacon bcn = LifiMacBeacon::Construct(beacon);
	uint8_t bcnOrder = bcn.GetBcnOrder();
	uint8_t superframeOrder = bcn.GetSupframeOrder();
	uint8_t capSlot = bcn.GetFinalCapSlot();

	// Only support beacon-enable VPAN.
	NS_ASSERT ((bcnOrder >=0) && (bcnOrder <= 14));
	NS_ASSERT (bcnOrder >= superframeOrder);
	if (m_attributes != 0)
	{
		m_attributes->macBeaconOrder = (uint32_t)bcnOrder;
		m_attributes->macSuperframeOrder = (uint32_t)superframeOrder;
	}
	uint32_t bcnIntval = LifiMac::aBaseSuperframeDuration * pow(2, (uint32_t)bcnOrder);
	uint32_t superframDur = LifiMac::aBaseSuperframeDuration * pow (2, (uint32_t)superframeOrder);
	uint32_t capDur = (capSlot + 1) * LifiMac::aBaseSlotDuration * pow (2, (uint32_t)superframeOrder);

	Time op = *m_opticalPeriod;
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

} /* namespace ns3 */
