/*
 * lifi-coord-assoc-handler.cc
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#include "lifi-coord-assoc-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiCoordAssocHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiCoordAssocHandler);

LifiCoordAssocHandler::LifiCoordAssocHandler()
{
	NS_LOG_FUNCTION (this);
	AddTrigger(LifiCoordAssocHandler::ReceiveAssocRequest, true);
//	AddTrigger(LifiCoordAssocHandler::ReceiveDataRequest, true);
//	AddTrigger(LifiCoordAssocHandler::ReceiveAck, true);
	AddTrigger(LifiCoordAssocHandler::TxResultNotification, false);
}

LifiCoordAssocHandler::~LifiCoordAssocHandler()
{
	NS_LOG_FUNCTION (this);
}

TypeId LifiCoordAssocHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiCoordAssocHandler")
						.AddConstructor<LifiCoordAssocHandler> ()
						.SetParent<LifiAssocHandler> ();
	return tid;
}

void LifiCoordAssocHandler::ReceiveAssocRequest(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	if (CheckTrigger(LifiCoordAssocHandler::ReceiveAssocRequest))
	{
		onReceiveAssocRequest(timestamp, msdu);
	}else
	{
		NS_LOG_WARN("Ignore LifiCoordAssocHandler::ReceiveAssocRequest.");
	}
}

//void LifiCoordAssocHandler::ReceiveDataRequest(uint32_t timestamp,
//		Ptr<Packet> msdu)
//{
//	NS_LOG_FUNCTION (this << timestamp << msdu);
//	if (CheckTrigger(LifiCoordAssocHandler::ReceiveDataRequest))
//	{
//		onReceiveDataRequest(timestamp, msdu);
//	}else
//	{
//		NS_LOG_WARN("Ignore LifiCoordAssocHandler::ReceiveDataRequest.");
//	}
//}

void LifiCoordAssocHandler::ReceiveAck(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	if (CheckTrigger(LifiCoordAssocHandler::ReceiveAck))
	{
		onReceiveAck(timestamp, msdu);
	}else
	{
		NS_LOG_WARN("Ignore LifiCoordAssocHandler::ReceiveAck.");
	}
}

void LifiCoordAssocHandler::TxResultNotification(MacOpStatus status)
{
	NS_FATAL_ERROR("LifiCoordAssocHandler::TxResultNotification can not be invoked.");
}

void LifiCoordAssocHandler::onReceiveAssocRequest(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);

}

void LifiCoordAssocHandler::onReceiveDataRequest(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	LifiMacHeader header;
	msdu->PeekHeader(header);
	NS_ASSERT (header.GetSrcAddressMode() == EXTENDED);
	Address extendedAddress = header.GetSrcAddress();
	AssocSessions::iterator it = m_sessions.find(extendedAddress);
	NS_ASSERT (it != m_sessions.end());
	it->second.ReceiveDataRequest(timestamp, msdu);
}

void LifiCoordAssocHandler::onReceiveAck(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
}

} /* namespace ns3 */
