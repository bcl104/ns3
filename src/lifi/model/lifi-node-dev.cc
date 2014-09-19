/*
 * lifi-node-dev.cc
 *
 *  Created on: 2014年9月16日
 *      Author: wanghongqiao
 */
#include "lifi-node-dev.h"
#include "ns3/log.h"

namespace ns3{

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
}

void LifiNodeDev::DataConfirm(uint8_t msduHandle, MacOpStatus status,
		uint32_t timestamp) {
}

void LifiNodeDev::DataIndication(DataIndicaDescriptor dataDesc){

}

void LifiNodeDev::AssociateConfirm(Mac16Address assocShortAddr,
		MacOpStatus status, MacColorStabCapab capNigoResponse) {
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

}



