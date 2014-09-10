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


void LifiDisassocHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiDisassocHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiDisassocHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiDisassocHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiDisassocHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiDisassocHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiDisassocHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiDisassocHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiDisassocHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiDisassocHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}

} /* namespace ns3 */


