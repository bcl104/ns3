/*
 * lifi-asso-chandler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-assoc-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiAssocHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiAssocHandler);

LifiAssocHandler::LifiAssocHandler() {

}

LifiAssocHandler::~LifiAssocHandler() {
}

TypeId LifiAssocHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiAssocHandler")
						.SetParent<Object> ()
						.AddConstructor<LifiAssocHandler> ();
	return tid;
}

void LifiAssocHandler::TxResultNotification(PhyOpStatus status){
}

} /* namespace ns3 */


