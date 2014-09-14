/*
 * lifi-assoc-handler.cc
 *
 *  Created on: 2014年4月25日
 *      Author: will
 */

#include "lifi-assoc-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiAssocHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiAssocHandler);
LifiAssocHandler::LifiAssocHandler(){
}

LifiAssocHandler::~LifiAssocHandler(){
}

TypeId LifiAssocHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiAssocHandler")
						.AddConstructor<LifiAssocHandler> ()
						.SetParent<LifiMacHandler> ();
	return tid;
}

void LifiAssocHandler::AllocNotification(Ptr<DataService> service){
}

void LifiAssocHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
}

void LifiAssocHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiAssocHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiAssocHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiAssocHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiAssocHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiAssocHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiAssocHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiAssocHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiAssocHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiAssocHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}

} /* namespace ns3 */
