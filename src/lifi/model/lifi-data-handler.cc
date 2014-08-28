/*
 * lifi-data-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: wanghongqiao
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
						.AddConstructor<LifiDataHandler> ()
						.SetParent<LifiMacHandler> ();
	return tid;
}

void LifiDataHandler::StartTransmit(DataDescriptor DataDesc){
}

void LifiDataHandler::ReceiveDataFrame(uint32_t timestamp, Ptr<Packet> msdu){

}
void LifiDataHandler::AllocNotification (Ptr<DataService> service) {

}

void LifiDataHandler::TxResultNotification(MacOpStatus status,
			          PacketInfo info, Ptr<Packet> ack) {
}

} /* namespace ns3 */
