/*
 * lifi-disassoc-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
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
						.SetParent<Object> ()
						.AddConstructor<LifiDisassocHandler> ();
	return tid;
}

void LifiDisassocHandler::TxResultNotification(PhyOpStatus status) {
}

} /* namespace ns3 */
