/*
 * lifi-node.cc
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */

#include "lifi-node.h"
#include "ns3/log.h"

namespace ns3{

LifiNode::LifiNode() {
	m_mlmeSapUser = Create<MlmeSpecificSapUser <LifiNode> >(this);
	m_mcpsSapUser = Create<McpsSpecificSapUser <LifiNode> >(this);

//	m_mlmeSapUser = new MlmeSpecificSapUser <LifiNode>  (this);

}

LifiNode::~LifiNode() {
}

TypeId LifiNode::GetTypeId (){
	static TypeId tid = TypeId ("ns3::LifiNode")
						.SetParent<Object> ()
						.AddConstructor<LifiNode> ();
	return tid;
}

void LifiNode::Start(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,	uint32_t bcnOrder,
					 uint32_t supframeOrder, bool vpanCoord) {
}

void LifiNode::DataConfirm(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNode::DataIndication(DataIndicaDescriptor dataDesc){

}

void LifiNode::AssociateConfirm(Mac16Address assocShortAddr,
		MacOpStatus status, MacColorStabCapab capNigoResponse) {
}

void LifiNode::AssociateIndication(Mac64Address devAddr,
		CapabilityInfo capInfo) {
}

void LifiNode::BeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor,
		PendAddrSpec pendAddrSpec, AddrList addrList, uint32_t payloadSize,
		uint8_t* payload) {
}

void LifiNode::CommStatusIndication(uint16_t vpanId,
		AddrMode srcAddrMode, Address srcAddr, AddrMode dsAddrMode,
		Address dstAddr, MacOpStatus status) {
}

void LifiNode::DisassociateConfirm(MacOpStatus status,
		AddrMode devAddrMode, uint16_t devVPANId, Address devAddr) {
}

void LifiNode::DisassociateIndication(Mac64Address devAddr,
		DisassocReason disassociateReason) {
}

void LifiNode::GtsConfirm(GTSCharacteristics characteristics,
		MacOpStatus status) {
}

void LifiNode::GtsIndication(Mac16Address devAddr,
		GTSCharacteristics charateristic) {
}

void LifiNode::PollConfirm(MacOpStatus status) {
}

void LifiNode::ResetConfirm(MacOpStatus status) {
}

void LifiNode::RxEnableConfirm(MacOpStatus status) {
}

void LifiNode::ScanConfirm(MacOpStatus status, ScanType scanType,
		uint8_t unscanChannels, uint32_t resultListSize,
		VpanDescriptors& descriptors) {
}

void LifiNode::SetConfirm(MacOpStatus ststus, MacPIBAttributeId id,
		uint32_t index) {
}

void LifiNode::StartConfirm(MacOpStatus status) {
}

void LifiNode::SyncLossIndication(MacOpStatus lossReason,
		uint16_t vpanId, LogicChannelId channel) {
}

void LifiNode::DataConfirmMcps(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNode::DataIndicationMcps(TypeId srcAddrMode, uint16_t srcVPANId,
		Address srcAddr, TypeId dstAddrMode, Address dstAddr,
		uint32_t msduLength, Ptr<Packet> msdu, uint8_t mpduLinkQuality,
		uint8_t dsn, uint32_t timestamp, DataRateId dataRate, bool burstMode) {
}

void LifiNode::PurgeConfirmMcps(uint8_t msduHandle, MacOpStatus status) {
}

void LifiNode::SendData(SendDataInfo dataInfo){

}

void LifiNode::GtsRequest(GTSCharacteristics characteristics, Address dstAddr){

}

void LifiNode::DisassocRequst(AddrMode devAddrMode, uint16_t devVPANId,
							  Address devAddr, DisassocReason reason, bool txIndirect){

}

void LifiNode::SetMlmeSapProvider (Ptr<MlmeSapProvider> p){
	m_mlmeSapProvider = p;
}

void LifiNode::SetMcpsSapProvider (Ptr<McpsSapProvider> p){
	m_mcpsSapProvider = p;
}

Ptr<MlmeSapUser> LifiNode::GetMlmeSapUser (){
	return m_mlmeSapUser;
}
Ptr<McpsSapUser> LifiNode::GetMcpsSapUser (){
	return m_mcpsSapUser;
}

void LifiNode::SetLifiMac (Ptr<LifiMac> p){
	m_lifiMac = p;
}

}
