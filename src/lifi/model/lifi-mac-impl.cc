/*
 * lifi-mac-impl.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-mac-impl.h"
#include "ns3/core-module.h"

namespace ns3 {

LifiMacImpl::LifiMacImpl()
			: m_attributes (LifiMacPibAttribute())
{
}

LifiMacImpl::~LifiMacImpl() {

}

TypeId LifiMacImpl::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiMacImpl")
			.SetParent<Object> ()
			.AddConstructor<LifiMacImpl>();
	return tid;
}

void LifiMacImpl::Associate(LogicChannelId channel, TypeId coordAddrMode,
		uint16_t coordVPANId, Address coordAddr, CapabilityInfo info) {
	NS_FATAL_ERROR("LifiMacImpl::Associate can not be invoked.");
}

void LifiMacImpl::Disassociate(TypeId devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
	NS_FATAL_ERROR("LifiMacImpl::Disassociate can not be invoked.");
}

void LifiMacImpl::AssociateResponse(Mac64Address devAddr,
		Mac16Address assocShortAddr, MacOpStatus status,
		MacOpStatus capResponse) {
	NS_FATAL_ERROR("LifiMacImpl::AssociateResponse can not be invoked.");
}

void LifiMacImpl::GtsRequest(GTSCharacteristics characteristic) {
}

void LifiMacImpl::Polling(TypeId coordAddrMode, uint16_t coordVPANId,
		Address coordAddr) {
	NS_FATAL_ERROR("LifiMacImpl::Polling can not be invoked.");
}

void LifiMacImpl::PurgeTrancsion(uint8_t handle) {

}

void LifiMacImpl::Receive(uint32_t size, Ptr<Packet> p, uint8_t quality) {

}

void LifiMacImpl::DataConfirm(PhyOpStatus status)
{
}


void LifiMacImpl::Reset() {

}

void LifiMacImpl::RxEnable(bool deferPermit, uint32_t rxOnTime,
		uint32_t rxOnDuration) {
}

void LifiMacImpl::Scan(ScanType scanType, uint8_t channel,
		uint32_t scanDuration) {
}

void LifiMacImpl::SendData(TypeId srcAddrMode, TypeId dstAddrMode,
		uint16_t dstVPANId, Address dstAddr, uint32_t size, Ptr<Packet> msdu,
		uint8_t handle, TxOption option, DataRateId rate) {
}

void LifiMacImpl::StartVPAN(uint16_t vpanId, LogicChannelId channel,
		uint32_t startTime, uint32_t beaconOrder, uint32_t supframeOrder,
		bool vpanCoord) {
	NS_FATAL_ERROR("LifiMacImpl::Associate can not be invoked.");

}

void LifiMacImpl::Synchronize(LogicChannelId channel, bool trackBeacon) {

}

void LifiMacImpl::SetLifiMac(Ptr<LifiMac> mac) {
	m_mac = mac;
}

Ptr<LifiMac> LifiMacImpl::GetLifiMac() const {
	return m_mac;
}

void LifiMacImpl::SetOpticalPeriod(const Time* oc)
{
	m_opticalPeriod = oc;
}

const Time* LifiMacImpl::GetOpticalPeriod() const
{
	return m_opticalPeriod;
}

LifiMacPibAttribute& LifiMacImpl::GetLifiMacPibAttribute()
{
	return m_attributes;
}

void LifiMacImpl::SetPdSapProvider(Ptr<PdSapProvider> p)
{
}

void LifiMacImpl::SetPlmeSapProvider(Ptr<PlmeSapProvider> p)
{
}

void LifiMacImpl::SetMlmeSapUser(Ptr<MlmeSapUser> u)
{
}

void LifiMacImpl::SetMcpsSapUser(Ptr<McpsSapUser> u)
{
}

} /* namespace ns3 */
