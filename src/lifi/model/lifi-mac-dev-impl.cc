/*
 * lifi-mac-dev-impl.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-dev-impl.h"
#include "lifi-dev-trx-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiMacDevImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiMacDevImpl);

LifiMacDevImpl::LifiMacDevImpl()
				: m_trxHandler (new LifiDevTrxHandler)
{
	NS_LOG_FUNCTION (this);
	m_trxHandler->SetLifiMacImpl(this);
}

LifiMacDevImpl::~LifiMacDevImpl() {
	NS_LOG_FUNCTION (this);
}

TypeId LifiMacDevImpl::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDevMacImpl")
						.AddConstructor<LifiMacDevImpl> ()
						.SetParent<LifiMacImpl> ();
	return tid;
}

void LifiMacDevImpl::Associate(LogicChannelId channel,
		TypeId coordAddrMode, uint16_t coordVPANId, Address coordAddr,
		CapabilityInfo info) {
	NS_LOG_FUNCTION (this);

}

void LifiMacDevImpl::Disassociate(TypeId devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::GtsRequest(GTSCharacteristics characteristic) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::Polling(TypeId coordAddrMode, uint16_t coordVPANId,
		Address coordAddr) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::PurgeTrancsion(uint8_t handle) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::Receive(uint32_t size, Ptr<Packet> p, uint8_t quality) {
	NS_LOG_FUNCTION (this);
	m_trxHandler->ReceivePacket (0, p);
}

void LifiMacDevImpl::Reset() {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::RxEnable(bool deferPermit, uint32_t rxOnTime,
		uint32_t rxOnDuration) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::Scan(ScanType scanType, uint8_t channel,
		uint32_t scanDuration) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::SendData(TypeId srcAddrMode, TypeId dstAddrMode,
		uint16_t dstVPANId, Address dstAddr, uint32_t msduLength,
		Ptr<Packet> msdu, uint8_t msduHanle, TxOption option, bool rate) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::Synchronize(LogicChannelId channel,
		bool trackBeacon) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::SetPdSapProvider(Ptr<PdSapProvider> p)
{
	m_trxHandler->SetPdSapProvider(p);
}

void LifiMacDevImpl::SetPlmeSapProvider(Ptr<PlmeSapProvider> p)
{
	m_trxHandler->SetPlmeSapProvider(p);
}

void LifiMacDevImpl::SetMlmeSapUser(Ptr<MlmeSapUser> u)
{
}

void LifiMacDevImpl::SetMcpsSapUser(Ptr<McpsSapUser> u)
{
}

} /* namespace ns3 */


