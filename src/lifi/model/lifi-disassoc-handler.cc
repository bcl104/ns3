/*
 * lifi-disassoc-handler.cc
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */

#include "lifi-disassoc-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiDisassocHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDisassocHandler);

LifiDisassocHandler::LifiDisassocHandler() {

}

LifiDisassocHandler::~LifiDisassocHandler() {

}

TypeId LifiDisassocHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDisassocHandler")
						.SetParent<LifiMacHandler> ()
						.AddConstructor<LifiDisassocHandler> ();
	return tid;
}

void LifiDisassocHandler::AllocNotification(Ptr<DataService> service) {
}

void LifiDisassocHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
}

AddrMode LifiDisassocHandler::GetCurAddressMode(Address address) const {
	if (Mac16Address::IsMatchingType(address))
	{
		return SHORT;
	}else if (Mac64Address::IsMatchingType(address))
	{
		return EXTENDED;
	}else
	{
		NS_FATAL_ERROR("Invalid address mode.");
	}
	return ERROR;
}

} /* namespace ns3 */


