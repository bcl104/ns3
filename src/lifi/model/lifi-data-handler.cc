/*
 * lifi-data-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-data-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiDataHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDataHandler);

LifiDataHandler::LifiDataHandler() {

}

LifiDataHandler::~LifiDataHandler() {

}

TypeId LifiDataHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDataHandler")
						.SetParent<Object> ()
						.AddConstructor<LifiDataHandler> ();
	return tid;
}

void LifiDataHandler::TxResultNotification(PhyOpStatus status) {
}

} /* namespace ns3 */
