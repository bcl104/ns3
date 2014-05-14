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
						: m_running (false)
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
	if (m_running)
		Simulator::ScheduleNow (&LifiCoordTrxHandler::BeaconStart, this);
}

void LifiCoordTrxHandler::Start()
{
	NS_LOG_FUNCTION (this);
	m_running = true;
	BeaconStart();
}

void LifiCoordTrxHandler::TxResultNotification(MacOpStatus status, PacketInfo info,
		Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << status << ack);
	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::BEACON);
	NS_ASSERT (status == MAC_SUCCESS);
	Simulator::ScheduleNow(&LifiCoordTrxHandler::SuperframeStart, this);
}

void LifiCoordTrxHandler::BeaconStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_state = SuperframeStrcut::BEACON;
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
	m_curTransmission.m_info.m_band = 0xff;
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

} /* namespace ns3 */
