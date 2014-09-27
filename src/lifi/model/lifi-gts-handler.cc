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
	NS_LOG_FUNCTION(this);
}

LifiGtsHandler::~LifiGtsHandler() {
}

TypeId LifiGtsHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiGtsHandler")
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
	std::cout << m_gtsTransactions.size() << std::endl;
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

void LifiGtsHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiGtsHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiGtsHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiGtsHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiGtsHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiGtsHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiGtsHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiGtsHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiGtsHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiGtsHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}

} /* namespace ns3 */
