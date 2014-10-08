/*
 * lifi-mac-coord-impl.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-coord-impl.h"
#include "lifi-mac-beacon.h"
#include "lifi-mac-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiMacCoordImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiMacCoordImpl);

LifiMacCoordImpl::LifiMacCoordImpl()
				: m_gtsSlotCount (0),
				  m_trxHandler (new LifiCoordTrxHandler),
				  m_channelScanHandler (new LifiChannelScanHandler),
				  m_coordAssocHandler (new LifiCoordAssocHandler),
				  m_transcHandler (new LifiTransactionHandler),
				  m_dataCoordHandler (new LifiDataCoordHandler),
				  m_disassocCoordHandler (new LifiDisassocCoordHandler),
				  m_gtsCoordHandler (new LifiGtsCoordHandler)
{
	m_trxHandler->SetLifiMacImpl(this);
	m_trxHandler->SetMacPibAttributes(&m_attributes);
	m_trxHandler->PermitReceivePacket();

	m_channelScanHandler->SetLifiMacImpl(this);
	m_channelScanHandler->SetMacPibAttributes(&m_attributes);
	m_channelScanHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiChannelScanHandler::GetTypeId(),
							  GetPointer (m_channelScanHandler));

	m_coordAssocHandler->SetLifiMacImpl(this);
	m_coordAssocHandler->SetMacPibAttributes(&m_attributes);
	m_coordAssocHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiCoordAssocHandler::GetTypeId(),
							  GetPointer (m_coordAssocHandler));

	m_transcHandler->SetLifiMacImpl(this);
	m_transcHandler->SetMacPibAttributes(&m_attributes);
	m_transcHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiTransactionHandler::GetTypeId(),
							  GetPointer (m_transcHandler));

	m_dataCoordHandler->SetLifiMacImpl(this);
	m_dataCoordHandler->SetMacPibAttributes(&m_attributes);
	m_dataCoordHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiDataCoordHandler::GetTypeId(),
							  GetPointer (m_dataCoordHandler));

	m_disassocCoordHandler->SetLifiMacImpl(this);
	m_disassocCoordHandler->SetMacPibAttributes(&m_attributes);
	m_disassocCoordHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiDisassocCoordHandler::GetTypeId(),
							  GetPointer (m_disassocCoordHandler));

	m_gtsCoordHandler->SetLifiMacImpl(this);
	m_gtsCoordHandler->SetMacPibAttributes(&m_attributes);
	m_gtsCoordHandler->SetTrxHandler(m_trxHandler);
	m_trxHandler->AddListener(LifiGtsCoordHandler::GetTypeId(),
							  GetPointer (m_gtsCoordHandler));

}

LifiMacCoordImpl::~LifiMacCoordImpl() {

}

TypeId LifiMacCoordImpl::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiMacCoordImpl")
						.SetParent<LifiMacImpl> ()
						.AddConstructor<LifiMacCoordImpl> ();
	return tid;
}

//void LifiMacCoordImpl::Associate(LogicChannelId channel, AddrMode coordAddrMode,
//			uint16_t coordVPANId, Address coordAddr, CapabilityInfo info){
//	NS_LOG_FUNCTION(this);
//}

void LifiMacCoordImpl::AssociateResponse(Mac64Address devAddr,
			Mac16Address assocShortAddr, MacOpStatus status, MacColorStabCapab capResponse) {
	m_coordAssocHandler->MlmeAssocResponse(devAddr, assocShortAddr, status, capResponse);
}

void LifiMacCoordImpl::GtsRequest(GTSCharacteristics characteristic, Address dstAddr){
	NS_LOG_FUNCTION (this);
	m_gtsCoordHandler->StartGtsDealloc(characteristic, dstAddr);
}

void LifiMacCoordImpl::PurgeTrancsion(uint8_t handle) {
}

void LifiMacCoordImpl::Receive(uint32_t size, Ptr<Packet> p, uint8_t quality) {
	NS_LOG_FUNCTION (p);
	m_trxHandler->ReceivePacket(0, p);
}

void LifiMacCoordImpl::Reset() {
}

void LifiMacCoordImpl::RxEnable(bool deferPermit, uint32_t rxOnTime,
		uint32_t rxOnDuration) {
}

void LifiMacCoordImpl::Scan(ScanType scanType, uint32_t scanDuration) {
	NS_LOG_FUNCTION(this);
	m_channelScanHandler->StartChannelScan(scanType, scanDuration);
}

void LifiMacCoordImpl::SendData(AddrMode srcAddrMode, AddrMode dstAddrMode,
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
	m_dataCoordHandler->StartTransmit(m_dataDescriptor);
}

void LifiMacCoordImpl::StartVPAN(uint16_t vpanId, LogicChannelId channel,
		uint32_t startTime, uint32_t beaconOrder, uint32_t supframeOrder,
		bool vpanCoord) {
	NS_LOG_FUNCTION (this);
	NS_ASSERT (vpanCoord);
	m_trxHandler->PermitReceivePacket();
	m_trxHandler->GetPlmeSapProvider()->PlmeSetRequest(PHY_CURRENT_CHANNEL, channel);
	m_attributes.macVPANId = vpanId;
	m_attributes.macBeaconOrder = beaconOrder;
	m_attributes.macSuperframeOrder = supframeOrder;
	m_mac->GetPlmeSapProvider()->PlmeSetRequest(PHY_CURRENT_CHANNEL, channel);
	m_trxHandler->Start();
}

void LifiMacCoordImpl::Disassociate(AddrMode devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
	NS_LOG_FUNCTION (this);
	DisassocDescriptor m_disassocDes;
	m_disassocDes.DeviceAddrMode = devAddrMode;
	m_disassocDes.DeviceVPANId = devVPANId;
	m_disassocDes.DeviceAddr = devAddr;
	m_disassocDes.DisassociationReason = reason;
	m_disassocDes.TxIndirect = txIndirect;

	m_disassocCoordHandler->StartDisassoc(m_disassocDes);
}

Ptr<Packet> LifiMacCoordImpl::ConstructBeacon() const
{
	LifiMacBeacon beacon;
	beacon.SetAssocPermit(true);
	beacon.SetBcnOrder((uint8_t)m_attributes.macBeaconOrder);
	beacon.SetCellSearchEn(true);
	beacon.SetCellSearchLenth(0);

	beacon.SetFinalCapSlot(15 - m_gtsSlotCount);

	beacon.SetGtsPermit(true);
	beacon.SetSupframeOrder((uint8_t)m_attributes.macSuperframeOrder);
	beacon.SetVpanCoord(true);

	beacon.SetGtsDescripCount(GetGtsDesCount());
	beacon.AddGtsList(GetGtsLists());
	beacon.SetGtsDirMask(GetGtsDirectionsMark());
	beacon.SetAddrs(GetPendingAddresses());
	beacon.SetPendingAddrSpec();

	Ptr<Packet> p = beacon.GetPacket();

	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetDstAddress(Address(Mac16Address("ff:ff")));
	header.SetDstVPANId(m_attributes.macVPANId);
	header.SetFrameType(LIFI_BEACON);
	header.SetSequenceNumber(m_attributes.macBSN);
	header.SetSrcAddress(m_mac->GetDevice()->GetAddress());
//	std::cout << m_mac->GetDevice()->GetAddress() << std::endl;
//	Mac16Address temp = m_attributes.macCoordShortAddress;
//	std::cout <<  (uint16_t)temp << std::endl;
//	header.SetSrcVPANId(m_attributes.macVPANId);

	p->AddHeader(header);

	return p;
}

void LifiMacCoordImpl::SetCFPLenth(uint32_t gtsLenth){
	m_gtsSlotCount = gtsLenth;
}

void LifiMacCoordImpl::SetGTSCount(){
	m_gtsCoordHandler->SetGTSCount();
}

void LifiMacCoordImpl::SetLifiMac(Ptr<LifiMac> mac)
{
	m_mac = mac;
	m_trxHandler->SetPdSapProvider(GetPointer (mac->m_pdSapProvider));
	m_trxHandler->SetPlmeSapProvider(GetPointer (mac->m_plmeSapProvider));
}

void LifiMacCoordImpl::SetOpticalPeriod(const Time* oc)
{
	m_opticalPeriod = oc;
	m_trxHandler->SetOpticalPeriod(m_opticalPeriod);
}

void LifiMacCoordImpl::SetPdSapProvider(Ptr<PdSapProvider> p)
{
	m_trxHandler->SetPdSapProvider(p);
}

void LifiMacCoordImpl::SetPlmeSapProvider(Ptr<PlmeSapProvider> p)
{
	m_trxHandler->SetPlmeSapProvider(p);
}

void LifiMacCoordImpl::SetMlmeSapUser(Ptr<MlmeSapUser> u)
{
	m_trxHandler->SetMlmeSapUser(u);
	m_channelScanHandler->SetMlmeSapUser(u);
	m_coordAssocHandler->SetMlmeSapUser(u);
    m_transcHandler->SetMlmeSapUser(u);
    m_dataCoordHandler->SetMlmeSapUser(u);
    m_disassocCoordHandler->SetMlmeSapUser(u);
    m_gtsCoordHandler->SetMlmeSapUser(u);

}

void LifiMacCoordImpl::DataConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << status);
	m_trxHandler->TxConfirm (status);
}

void LifiMacCoordImpl::SetMcpsSapUser(Ptr<McpsSapUser> u)
{
	m_trxHandler->SetMcpsSapUser(u);
}

void LifiMacCoordImpl::AddGtsTransactionPacket(GtsTransactionInfo& gtsTransInfo)
{
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->AddGtsTransaction(gtsTransInfo);
}

void LifiMacCoordImpl::AddTransactionPacket(TransactionInfo& transInfo)
{
	NS_LOG_FUNCTION(this);
	m_transcHandler->AddTransaction(transInfo);
}

void LifiMacCoordImpl::PetchTransactionPacket(Mac64Address DevAddress) {
	NS_LOG_FUNCTION(this);
	m_transcHandler->PetchTransaction(DevAddress);
}

GtsList LifiMacCoordImpl::GetGtsLists() const{
	NS_LOG_FUNCTION(this);
	GtsList m_tempGtsList;
	m_tempGtsList = m_gtsCoordHandler->GetGtsDescList();
	return m_tempGtsList;
}

GtsList LifiMacCoordImpl::GetBeaconGtsLists() const{
	NS_LOG_FUNCTION(this);
	GtsList m_tempGtsList;
	m_tempGtsList = m_gtsCoordHandler->GetBeaconGtsDescList();
	return m_tempGtsList;
}

uint8_t LifiMacCoordImpl::GetGtsDesCount() const{
	NS_LOG_FUNCTION(this);
	return m_gtsCoordHandler->GetGtsDesCount();
}

uint8_t LifiMacCoordImpl::GetGtsDirectionsMark() const{
	NS_LOG_FUNCTION(this);
	return m_gtsCoordHandler->GetGtsDirectionsMark();
}

AddrList LifiMacCoordImpl::GetPendingAddresses () const{
	NS_LOG_FUNCTION(this);
	return m_transcHandler->GetPendingAddress();
}

void LifiMacCoordImpl::SetGtsTransmitArgument (uint16_t shortAddr, bool transmitState){
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->SetGtsTransmitArgument(shortAddr, transmitState);
}

void LifiMacCoordImpl::OpenGtsDataReceive(uint16_t devAddr){
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->OpenGtsDataReceive(devAddr);
}

void LifiMacCoordImpl::CloseGtsDataReceive(){
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->CloseGtsDataReceive();
}

void LifiMacCoordImpl::EndGtsTransmit(){
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->EndGtsTransmit();
}

void LifiMacCoordImpl::SendGtsDatas(){
	NS_LOG_FUNCTION(this);
	m_gtsCoordHandler->SendGtsDatas();
}

} /* namespace ns3 */

