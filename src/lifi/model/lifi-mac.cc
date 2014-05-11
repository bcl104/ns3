/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "lifi-mac.h"
#include "lifi-mac-coord-impl.h"
#include "lifi-mac-dev-impl.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiMac);

LifiMac::LifiMac()
{
	NS_LOG_FUNCTION (this);

	m_mcpsSapProvider = Create<McpsSpecificSapProvider<LifiMac> > (this);
	m_mlmeSapProvider = Create<MlmeSpecificSapProvider<LifiMac> > (this);
	m_pdSapUser = Create<PdSpecificSapUser<LifiMac> > (this);
	m_plmeSapUser = Create<PlmeSpecificSapUser<LifiMac> > (this);
	m_lifiMacImpl = CreateObject<LifiMacDevImpl> ();
	m_lifiMacImpl->SetLifiMac(this);
}

LifiMac::~LifiMac()
{
	NS_LOG_FUNCTION (this);
}

TypeId LifiMac::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiMac")
						.SetParent<Object> ()
						.AddConstructor<LifiMac> ();
	return tid;
}

Ptr<LifiMacPibAttribute> LifiMac::GetMacPibAttributes()
{
	NS_LOG_FUNCTION (this);
	return &m_lifiMacImpl->GetLifiMacPibAttribute();
}

void LifiMac::Reset()
{
	NS_LOG_FUNCTION (this);
	m_lifiMacImpl->Reset();
}

void LifiMac::DoDataConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << status);
}

void LifiMac::DoReceive(uint32_t size, Ptr<Packet> p,
		uint8_t quality)
{
	NS_LOG_FUNCTION (this << size << p << (uint32_t)quality);
//	m_lifiMacImpl->Receive();
}

void LifiMac::Send(TypeId srcAddrMode, TypeId dstAddrMode, uint16_t dstVPANId,
		Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu, uint8_t handle,
		const TxOption& txOption, DataRateId rate, bool burstMode) {
//	NS_LOG_FUNCTION (this << srcAddrMode << dstAddrMode << dstVPANId << dstAddr << msduLength << handle
//					<< txOption << rate << burstMode);
	NS_LOG_FUNCTION (this);
	m_lifiMacImpl->SendData(srcAddrMode, dstAddrMode, dstVPANId, dstAddr, msduLength, msdu, handle,
							txOption, rate);

}

void LifiMac::Purge(uint8_t handle) {
	NS_LOG_FUNCTION (this << (uint32_t)handle);
	m_lifiMacImpl->PurgeTrancsion(handle);
}

void LifiMac::AssociateResponse(Mac64Address devAddr,Mac16Address assocShortAddr, MacOpStatus status,
		MacOpStatus capNegoResponse) {
	NS_LOG_FUNCTION (this << devAddr << assocShortAddr << (uint32_t)status << (uint32_t)capNegoResponse);
	m_lifiMacImpl->AssociateResponse(devAddr, assocShortAddr, status, capNegoResponse);
}

void LifiMac::Associate(LogicChannelId channel, TypeId coordAddrMode,uint16_t coordVPANId,
		Mac64Address coordAddr, CapabilityInfo capInfo) {
	NS_LOG_FUNCTION(this << (uint32_t)channel << coordAddrMode << coordVPANId << coordAddr /*<< capInfo*/);

	m_lifiMacImpl->Associate(channel, coordAddrMode, coordVPANId, coordAddr, capInfo);
}

void LifiMac::Disassociate(TypeId devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
	NS_LOG_FUNCTION (this << devAddrMode << devVPANId << devAddr << (uint32_t)reason << txIndirect);

	m_lifiMacImpl->Disassociate(devAddrMode, devVPANId, devAddr, reason, txIndirect);
}

void LifiMac::GtsRequest(GTSCharacteristics characteristics) {
//	NS_LOG_FUNCTION (this << (uint32_t) characteristics);
	NS_LOG_FUNCTION (this);
	m_lifiMacImpl->GtsRequest(characteristics);
}

void LifiMac::Poll(TypeId coordAddrMode, uint16_t coordVPANId, Address coordAddr) {
	NS_LOG_FUNCTION (this << coordAddrMode << coordVPANId << coordAddr);

	m_lifiMacImpl->Polling(coordAddrMode, coordVPANId, coordAddr);
}

void LifiMac::RxEnable(bool deferPermit, uint32_t rxOnTime,	uint32_t rxOnDuration) {
	NS_LOG_FUNCTION (this << deferPermit << rxOnTime << rxOnDuration);

	m_lifiMacImpl->RxEnable(deferPermit, rxOnTime, rxOnDuration);
}

void LifiMac::ScanChannel(ScanType scanType, uint8_t scanChannels, uint32_t scanDuration) {
	NS_LOG_FUNCTION (this << (uint32_t)scanType << (uint32_t)scanChannels << scanDuration);

	m_lifiMacImpl->Scan(scanType, scanChannels, scanDuration);
}

void LifiMac::StartVPAN(uint8_t vpanId, LogicChannelId channel, uint32_t startTime,	uint32_t bcnOrder,
						uint32_t supframeOrder, bool vpanCoord) {
	NS_LOG_FUNCTION (this << (uint32_t)vpanId << (uint32_t) channel << startTime << bcnOrder
					<< supframeOrder << vpanCoord);
	const Time* op = m_lifiMacImpl->GetOpticalPeriod();
	NS_ASSERT (op != 0);
	m_lifiMacImpl = CreateObject<LifiMacCoordImpl> ();
	m_lifiMacImpl->SetOpticalPeriod(op);
	m_lifiMacImpl->SetLifiMac(this);
}

void LifiMac::Synchronize(LogicChannelId channel, bool trackBeacon) {
	NS_LOG_FUNCTION(this << (uint32_t) channel << trackBeacon);

	m_lifiMacImpl->Synchronize(channel, trackBeacon);
}

void LifiMac::SwitchConfirm(PhyOpStatus status) {
	NS_LOG_FUNCTION (this);
}

const Ptr<LifiMacImpl>& LifiMac::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_lifiMacImpl;
}

void LifiMac::SetLifiMacImpl(const Ptr<LifiMacImpl>& impl) {
	NS_LOG_FUNCTION (this);
	m_lifiMacImpl = impl;
}

void LifiMac::SetPdSapProvider(const Ptr<PdSapProvider>& provider) {
	m_pdSapProvider = provider;
}

const Ptr<PdSapUser>& LifiMac::GetPdSapUser() const {
	return m_pdSapUser;
}

void LifiMac::SetPlmeSapProvider(const Ptr<PlmeSapProvider>& provider) {
	m_plmeSapProvider = provider;
}

const Ptr<PlmeSapUser>& LifiMac::GetPlmeSapUser() {
	return m_plmeSapUser;
}

void LifiMac::DoTransmit(uint32_t size, Ptr<Packet> pb, uint8_t band)
{
	NS_LOG_FUNCTION (this << size << pb << (uint32_t) band);
	m_pdSapProvider->DataRequest (size, pb, band);
}

const Ptr<McpsSapProvider>& LifiMac::GetMcpsSapProvider() const {
	NS_LOG_FUNCTION (this);
	return m_mcpsSapProvider;
}

void LifiMac::SetMcpsSapUser(const Ptr<McpsSapUser>& mcpsSapUser) {
	NS_LOG_FUNCTION (this);
	m_mcpsSapUser = mcpsSapUser;
}

const Ptr<MlmeSapProvider>& LifiMac::GetMlmeSapProvider() const {
	NS_LOG_FUNCTION (this);
	return m_mlmeSapProvider;
}

void LifiMac::SetMlmeSapUser(const Ptr<MlmeSapUser>& mlmeSapUser) {
	NS_LOG_FUNCTION (this);
	m_mlmeSapUser = mlmeSapUser;
}

void LifiMac::SetDevice(Ptr<LifiNetDevice> device) {
	m_device = device;
}

Ptr<LifiNetDevice> LifiMac::GetDevice() const {
	return m_device;
}

void LifiMac::SetOpticalClock(const double* oc)
{
//	m_lifiMacImpl->SetOpticalClock(oc);
}

const double* LifiMac::GetOpticalClock() const
{
//	return m_lifiMacImpl->GetOpticalClock();
	return 0;
}

}// ns3 name space


