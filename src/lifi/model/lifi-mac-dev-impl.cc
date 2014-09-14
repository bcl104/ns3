/*
 * lifi-mac-dev-impl.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-dev-impl.h"
#include "lifi-dev-trx-handler.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiMacDevImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiMacDevImpl);

LifiMacDevImpl::LifiMacDevImpl()
				: m_trxHandler (new LifiDevTrxHandler),
				  m_channelScanHandler (new LifiChannelScanHandler),
				  m_devAssocHandler (new LifiDevAssocHandler),
				  m_dataDevHandler (new LifiDataDevHandler),
				  m_disassocDevHandler (new LifiDisassocDevHandler),
				  m_gtsDevHandler (new LifiGtsDevHandler)
{
	NS_LOG_FUNCTION (this);
	m_trxHandler->SetLifiMacImpl(this);
	m_trxHandler->SetMacPibAttributes(&m_attributes);

	m_channelScanHandler->SetLifiMacImpl(this);
	m_channelScanHandler->SetMacPibAttributes(&m_attributes);
	m_channelScanHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiChannelScanHandler::GetTypeId(),
							  GetPointer (m_channelScanHandler));

	m_devAssocHandler->SetLifiMacImpl(this);
	m_devAssocHandler->SetMacPibAttributes(&m_attributes);
	m_devAssocHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiDevAssocHandler::GetTypeId(),
								  GetPointer (m_devAssocHandler));

	m_dataDevHandler->SetLifiMacImpl(this);
	m_dataDevHandler->SetMacPibAttributes(&m_attributes);
	m_dataDevHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiDataDevHandler::GetTypeId(),
								  GetPointer (m_dataDevHandler));

	m_disassocDevHandler->SetLifiMacImpl(this);
	m_disassocDevHandler->SetMacPibAttributes(&m_attributes);
	m_disassocDevHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiDisassocDevHandler::GetTypeId(),
								  GetPointer (m_disassocDevHandler));

	m_gtsDevHandler->SetLifiMacImpl(this);
	m_gtsDevHandler->SetMacPibAttributes(&m_attributes);
	m_gtsDevHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiGtsDevHandler::GetTypeId(),
								  GetPointer (m_gtsDevHandler));
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

void LifiMacDevImpl::Associate(LogicChannelId channel, AddrMode coordAddrMode,
							   uint16_t coordVPANId, Mac64Address coordAddr, CapabilityInfo info) {
	NS_LOG_FUNCTION (this << (uint32_t)channel << coordAddrMode << coordVPANId << coordAddr);
	VPANDescriptor vpanDesr;
	vpanDesr.coordAddr = coordAddr;
	vpanDesr.coordAddrMode = coordAddrMode;
	vpanDesr.coordVPANId = coordVPANId;
	vpanDesr.logicChannel = channel;
	m_devAssocHandler->Start(vpanDesr);

}

void LifiMacDevImpl::Disassociate(AddrMode devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
	NS_LOG_FUNCTION (this);
	DisassocDescriptor m_disassocDes;
	m_disassocDes.DeviceAddrMode = devAddrMode;
	m_disassocDes.DeviceVPANId = devVPANId;
	m_disassocDes.DeviceAddr = devAddr;
	m_disassocDes.DisassociationReason = reason;
	m_disassocDes.TxIndirect = txIndirect;

	m_disassocDevHandler->StartDisassoc(m_disassocDes);

}

void LifiMacDevImpl::GtsRequest(GTSCharacteristics characteristic, Address dstAddr) {
	NS_LOG_FUNCTION (this);
	m_gtsDevHandler->StartGtsRequest(characteristic, dstAddr);
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

void LifiMacDevImpl::Scan(ScanType scanType, uint32_t scanDuration) {
	NS_LOG_FUNCTION (this);
	m_channelScanHandler->StartChannelScan(scanType, scanDuration);
}

void LifiMacDevImpl::SendData(AddrMode srcAddrMode, AddrMode dstAddrMode,
		uint16_t dstVPANId, Address dstAddr, uint32_t msduLength,
		Ptr<Packet> msdu, uint8_t msduHanle, TxOption option, DataRateId rate, bool burstMode) {
	NS_LOG_FUNCTION (this);
	DataDescriptor m_dataDescriptor;
	m_dataDescriptor.SrcAddrMode = srcAddrMode;
	m_dataDescriptor.DstAddrMode = dstAddrMode;
	m_dataDescriptor.DstVPANId = dstVPANId;
	m_dataDescriptor.DstAddr = dstAddr;
	m_dataDescriptor.MsduLenth = msduLength;
	m_dataDescriptor.Msdu = msdu;
	m_dataDescriptor.MsduHandle = msduHanle;
	m_dataDescriptor.Options = option;
	m_dataDescriptor.Rate = rate;
	m_dataDescriptor.BurstMode = burstMode;
	m_dataDevHandler->StartTransmit(m_dataDescriptor);
}

void LifiMacDevImpl::Synchronize(LogicChannelId channel,
		bool trackBeacon) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::DataConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << status);
	m_trxHandler->TxConfirm(status);
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

void LifiMacDevImpl::AddGtsTransactionPacket (GtsTransactionInfo& gtsTransInfo){
	NS_LOG_FUNCTION(this);
	m_gtsHandler->AddGtsTransaction(gtsTransInfo);
}

void LifiMacDevImpl::SetGtsTransmitArgument (uint16_t shortAddr, bool transmitState){
	NS_LOG_FUNCTION(this);
	m_gtsDevHandler->SetGtsTransmitArgument(shortAddr, transmitState);
}

void LifiMacDevImpl::OpenGtsDataReceive(uint16_t devAddr){
	NS_LOG_FUNCTION(this);
	m_gtsDevHandler->OpenGtsDataReceive(devAddr);
}

void LifiMacDevImpl::CloseGtsDataReceive(){
	NS_LOG_FUNCTION(this);
	m_gtsDevHandler->CloseGtsDataReceive();
}

void LifiMacDevImpl::EndGtsTransmit(){
	NS_LOG_FUNCTION(this);
	m_gtsDevHandler->EndGtsTransmit();
}

void LifiMacDevImpl::SendGtsDatas(){
	NS_LOG_FUNCTION(this);
	m_gtsDevHandler->SendGtsDatas();
}

} /* namespace ns3 */


