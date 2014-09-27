/*
 * lifi-node-coord.cc
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */

#include "lifi-node-coord.h"
#include "ns3/log.h"

namespace ns3{

LifiNodeCoord::LifiNodeCoord() {
//	m_mlmeSapUser = Create<MlmeSpecificSapUser <LifiNode> >(this);
//	m_mcpsSapUser = Create<McpsSpecificSapUser <LifiNode> >(this);
//	m_lifiMac = CreateObject<LifiMac> ();
}

LifiNodeCoord::~LifiNodeCoord() {
}

TypeId LifiNodeCoord::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiNodeCoord")
						.SetParent<LifiNode> ()
						.AddConstructor<LifiNodeCoord> ();
	return tid;
}

void LifiNodeCoord::Start(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,	uint32_t bcnOrder,
						uint32_t supframeOrder, bool vpanCoord) {
	m_mlmeSapProvider->MlmeStartRequest(vpanId, channel, startTime, bcnOrder, supframeOrder, vpanCoord);
}

void LifiNodeCoord::DataConfirm(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNodeCoord::DataIndication(DataIndicaDescriptor dataDesc){

}

void LifiNodeCoord::AssociateConfirm(Mac16Address assocShortAddr,
		MacOpStatus status, MacColorStabCapab capNigoResponse) {
}

void LifiNodeCoord::AssociateIndication(Mac64Address devAddr,
		CapabilityInfo capInfo) {
	m_mlmeSapProvider->MlmeAssocaiteResponse(devAddr, Mac16Address("12:34"), MAC_SUCCESS, NO_COLOR_STABI);
}

void LifiNodeCoord::BeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor,
		PendAddrSpec pendAddrSpec, AddrList addrList, uint32_t payloadSize,
		uint8_t* payload) {
}

void LifiNodeCoord::CommStatusIndication(uint16_t vpanId,
		AddrMode srcAddrMode, Address srcAddr, AddrMode dsAddrMode,
		Address dstAddr, MacOpStatus status) {
}

void LifiNodeCoord::DisassociateConfirm(MacOpStatus status,
		AddrMode devAddrMode, uint16_t devVPANId, Address devAddr) {
}

void LifiNodeCoord::DisassociateIndication(Mac64Address devAddr,
		DisassocReason disassociateReason) {
}

void LifiNodeCoord::GtsConfirm(GTSCharacteristics characteristics,
		MacOpStatus status) {
}

void LifiNodeCoord::GtsIndication(Mac16Address devAddr,
		GTSCharacteristics charateristic) {
}

void LifiNodeCoord::PollConfirm(MacOpStatus status) {
}

void LifiNodeCoord::ResetConfirm(MacOpStatus status) {
}

void LifiNodeCoord::RxEnableConfirm(MacOpStatus status) {
}

void LifiNodeCoord::ScanConfirm(MacOpStatus status, ScanType scanType,
		uint8_t unscanChannels, uint32_t resultListSize,
		VpanDescriptors& descriptors) {
}

void LifiNodeCoord::SetConfirm(MacOpStatus ststus, MacPIBAttributeId id,
		uint32_t index) {
}

void LifiNodeCoord::StartConfirm(MacOpStatus status) {
}

void LifiNodeCoord::SyncLossIndication(MacOpStatus lossReason,
		uint16_t vpanId, LogicChannelId channel) {
}

void LifiNodeCoord::DataConfirmMcps(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNodeCoord::DataIndicationMcps(TypeId srcAddrMode, uint16_t srcVPANId,
		Address srcAddr, TypeId dstAddrMode, Address dstAddr,
		uint32_t msduLength, Ptr<Packet> msdu, uint8_t mpduLinkQuality,
		uint8_t dsn, uint32_t timestamp, DataRateId dataRate, bool burstMode) {
}

void LifiNodeCoord::PurgeConfirmMcps(uint8_t msduHandle, MacOpStatus status) {
}

void LifiNodeCoord::SendData(SendDataInfo dataInfo){
	m_mcpsSapProvider->McpsDataRequest(dataInfo.srcAddrMode, dataInfo.dstAddrMode, dataInfo.dstVPANId, dataInfo.dstAddr,dataInfo.msduLength,
							    	   dataInfo.msdu, dataInfo.msduHandle, dataInfo.txOption, dataInfo.rate, dataInfo.burstMode);
}

void LifiNodeCoord::GtsRequest(GTSCharacteristics characteristics, Address dstAddr){
	m_mlmeSapProvider->MlmeGtsRequest(characteristics, dstAddr);
}

void LifiNodeCoord::DisassocRequst(AddrMode devAddrMode, uint16_t devVPANId,
							     Address devAddr, DisassocReason reason, bool txIndirect){
	m_mlmeSapProvider->MlmeDisassociateRequest(devAddrMode, devVPANId, devAddr, reason, txIndirect);
}

}
