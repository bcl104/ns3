/*
 * lifi-gts-handler.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */
#include "ns3/log.h"
#include "lifi-gts-handler.h"

NS_LOG_COMPONENT_DEFINE ("LifiGtsHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiGtsHandler);

LifiGtsHandler::LifiGtsHandler() {
}

LifiGtsHandler::~LifiGtsHandler() {
}

TypeId LifiGtsHandler::GetTypeId()
{
	TypeId tid = TypeId ("ns3::LifiGtsHandler")
			.SetParent<LifiMacHandler> ()
			.AddConstructor<LifiGtsHandler> ();
	return tid;
}

bool LifiGtsHandler::GetGtsPermission()
{
	return false;
}

uint8_t LifiGtsHandler::GetGtsCount()
{
	return 0;
}

uint8_t LifiGtsHandler::GetGtsDirMask()
{
	return 0x00;
}

GtsDescriptors LifiGtsHandler::GetGtsDescritors() const
{
	return m_descriptors;
}

void LifiGtsHandler::ReportTransmission(MacOpStatus status)
{
}

void LifiGtsHandler::AllocNotification (Ptr<DataService> service){

}
void LifiGtsHandler::TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){

}

void LifiGtsHandler::AddGtsTransaction(GtsTransactionInfo& gtsTransInfo){
	NS_LOG_FUNCTION(this);
	m_curGtsTransaction = gtsTransInfo;
	m_curGtsTransaction.m_listener = this;
	m_gtsTransactions.insert(std::pair<uint16_t, GtsTransactionInfo>(gtsTransInfo.m_ShortAddress, m_curGtsTransaction));

}

void LifiGtsHandler::SetGtsOffset(uint8_t startSlot, uint8_t gtsLength){

}

void LifiGtsHandler::SendGtsDatas(){

}

void LifiGtsHandler::EndGtsTransmit(){

}

void LifiGtsHandler::OpenGtsDataReceive(uint16_t devAddr){

}
void LifiGtsHandler::CloseGtsDataReceive(){

}

void LifiGtsHandler::ReceiveData (uint32_t timestamp, Ptr<Packet> p) {

}

void LifiGtsHandler::SetGtsTransmitArgument(uint16_t shortAddr, bool transmitState){

}

} /* namespace ns3 */
