/*
 * lifi-beacon-handle.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-beacon-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiBeaconHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiBeaconHandler);

LifiBeaconHandler::LifiBeaconHandler() {
}

LifiBeaconHandler::~LifiBeaconHandler() {
}

TypeId LifiBeaconHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiBeaconHandler")
						.SetParent<Object> ()
						.AddConstructor<LifiBeaconHandler> ();
	return tid;
}

void LifiBeaconHandler::TxResultNotification(MacOpStatus status) {
}

} /* namespace ns3 */
