/*
 * lifi-node-dev.cc
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */
#include "lifi-node-dev.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiNodeDev");

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED (LifiNodeDev);

LifiNodeDev::LifiNodeDev() {
}

LifiNodeDev::~LifiNodeDev() {
}

TypeId LifiNodeDev::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiNodeDev")
						.SetParent<LifiNode> ()
						.AddConstructor<LifiNodeDev> ();
	return tid;
}

void LifiNodeDev::Start() {
	m_mlmeSapProvider->MlmeResetRequest();
}

void LifiNodeDev::StartAssoc(LogicChannelId channel, AddrMode coordAddrMode,
							 uint16_t coordVPANId, Mac64Address coordAddr, CapabilityInfo info){
	m_mlmeSapProvider->MlmeAssociateRequest(channel, coordAddrMode, coordVPANId, coordAddr, info);
}

void LifiNodeDev::DataConfirm(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
	NS_LOG_FUNCTION(this);
	SendDataInfo dataInfo;
	dataInfo.srcAddrMode = SHORT;
	dataInfo.dstAddrMode = EXTENDED;
	dataInfo.dstVPANId = 0x01;
	dataInfo.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
	dataInfo.msduLength = 10;
	dataInfo.msdu = Create<Packet> (10);
	dataInfo.msduHandle = 45;
	dataInfo.txOption.ackTx = true;
	dataInfo.txOption.indirectTx = false;
	dataInfo.txOption.gtsTx = false;
	dataInfo.rate = PHY_III_24_00_MBPS;
	dataInfo.burstMode = false;

	UniformVariable m_random;

	Simulator::Schedule(Seconds(m_random.GetValue()), &LifiNodeDev::SendData, this, dataInfo);

}

void LifiNodeDev::DataIndication(DataIndicaDescriptor dataDesc){

}

void LifiNodeDev::AssociateConfirm(Mac16Address assocShortAddr,
		MacOpStatus status, MacColorStabCapab capNigoResponse) {
//	uint16_t tempShortAddr ;
//	assocShortAddr.CopyTo((uint8_t*)&tempShortAddr);
//	if((tempShortAddr != 0xffff) && (tempShortAddr != 0xfffe)){
//		TxOption op;
//		op.ackTx = true;
//		op.indirectTx = false;
//		op.gtsTx = false;
//		m_mcpsSapProvider->McpsDataRequest(SHORT, EXTENDED, 0x01,
//										   Mac64Address ("00:00:00:00:00:00:00:01"), 10, Create<Packet> (10),
//										   45, op, PHY_III_24_00_MBPS, false);
//	}
}

void LifiNodeDev::AssociateIndication(Mac64Address devAddr,
		CapabilityInfo capInfo) {
}

void LifiNodeDev::BeaconNotify(uint8_t bsn, VPANDescriptor vpanDiscriptor,
		PendAddrSpec pendAddrSpec, AddrList addrList, uint32_t payloadSize,
		uint8_t* payload) {
}

void LifiNodeDev::CommStatusIndication(uint16_t vpanId,
		AddrMode srcAddrMode, Address srcAddr, AddrMode dsAddrMode,
		Address dstAddr, MacOpStatus status) {
}

void LifiNodeDev::DisassociateConfirm(MacOpStatus status,
		AddrMode devAddrMode, uint16_t devVPANId, Address devAddr) {
}

void LifiNodeDev::DisassociateIndication(Mac64Address devAddr,
		DisassocReason disassociateReason) {
}

void LifiNodeDev::GtsConfirm(GTSCharacteristics characteristics,
		MacOpStatus status) {
}

void LifiNodeDev::GtsIndication(Mac16Address devAddr,
		GTSCharacteristics charateristic) {
}

void LifiNodeDev::PollConfirm(MacOpStatus status) {
}

void LifiNodeDev::ResetConfirm(MacOpStatus status) {
}

void LifiNodeDev::RxEnableConfirm(MacOpStatus status) {
}

void LifiNodeDev::ScanConfirm(MacOpStatus status, ScanType scanType,
		uint8_t unscanChannels, uint32_t resultListSize,
		VpanDescriptors& descriptors) {
}

void LifiNodeDev::SetConfirm(MacOpStatus ststus, MacPIBAttributeId id,
		uint32_t index) {
}

void LifiNodeDev::StartConfirm(MacOpStatus status) {
}

void LifiNodeDev::SyncLossIndication(MacOpStatus lossReason,
		uint16_t vpanId, LogicChannelId channel) {
}

void LifiNodeDev::DataConfirmMcps(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNodeDev::DataIndicationMcps(TypeId srcAddrMode, uint16_t srcVPANId,
		Address srcAddr, TypeId dstAddrMode, Address dstAddr,
		uint32_t msduLength, Ptr<Packet> msdu, uint8_t mpduLinkQuality,
		uint8_t dsn, uint32_t timestamp, DataRateId dataRate, bool burstMode) {
}

void LifiNodeDev::PurgeConfirmMcps(uint8_t msduHandle, MacOpStatus status) {
}

void LifiNodeDev::SendData(SendDataInfo dataInfo){
	m_mcpsSapProvider->McpsDataRequest(dataInfo.srcAddrMode, dataInfo.dstAddrMode, dataInfo.dstVPANId, dataInfo.dstAddr,dataInfo.msduLength,
									   dataInfo.msdu, dataInfo.msduHandle, dataInfo.txOption, dataInfo.rate, dataInfo.burstMode);
}

void LifiNodeDev::GtsRequest(GTSCharacteristics characteristics, Address dstAddr){
	m_mlmeSapProvider->MlmeGtsRequest(characteristics, dstAddr);
}

void LifiNodeDev::DisassocRequst(AddrMode devAddrMode, uint16_t devVPANId,
							     Address devAddr, DisassocReason reason, bool txIndirect){
	m_mlmeSapProvider->MlmeDisassociateRequest(devAddrMode, devVPANId, devAddr, reason, txIndirect);
}

}
