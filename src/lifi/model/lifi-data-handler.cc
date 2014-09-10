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

void LifiDataHandler::ReceiveData(uint32_t timestamp, Ptr<Packet> p){

}
void LifiDataHandler::AllocNotification (Ptr<DataService> service) {

}

void LifiDataHandler::TxResultNotification(MacOpStatus status,
			          PacketInfo info, Ptr<Packet> ack) {
}


void LifiDataHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiDataHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiDataHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiDataHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiDataHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiDataHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiDataHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiDataHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiDataHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiDataHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}



} /* namespace ns3 */
