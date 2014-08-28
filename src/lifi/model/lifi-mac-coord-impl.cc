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
				  m_coordAssocHandler (new LifiCoordAssocHandler),
				  m_transcHandler (new LifiTransactionHandler)


{
	m_trxHandler->SetLifiMacImpl(this);
	m_trxHandler->SetMacPibAttributes(&m_attributes);
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
		Mac16Address assocShortAddr, MacOpStatus status,
		MacOpStatus capResponse) {
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

void LifiMacCoordImpl::Scan(ScanType scanType, LogicChannelId channel,
		uint32_t scanDuration) {
}

void LifiMacCoordImpl::SendData(TypeId srcAddrMode, TypeId dstAddrMode,
		uint16_t dstVPANId, Address dstAddr, uint32_t msduLength,
		Ptr<Packet> msdu, uint8_t msduHanle, TxOption option, DataRateId rate) {
}

void LifiMacCoordImpl::StartVPAN(uint16_t vpanId, LogicChannelId channel,
		uint32_t startTime, uint32_t beaconOrder, uint32_t supframeOrder,
		bool vpanCoord) {
	NS_LOG_FUNCTION (this);
	NS_ASSERT (vpanCoord);
	m_attributes.macVPANId = vpanId;
	m_attributes.macBeaconOrder = beaconOrder;
	m_attributes.macSuperframeOrder = supframeOrder;
	m_mac->GetPlmeSapProvider()->PlmeSetRequest(PHY_CURRENT_CHANNEL, channel);
	m_trxHandler->Start();
}

void LifiMacCoordImpl::Disassociate(TypeId devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
}

Ptr<Packet> LifiMacCoordImpl::ConstructBeacon() const
{
	LifiMacBeacon beacon;
	beacon.SetAssocPermit(true);
	beacon.SetBcnOrder((uint8_t)m_attributes.macBeaconOrder);
	beacon.SetCellSearchEn(true);
	beacon.SetCellSearchLenth(0);

	beacon.SetFinalCapSlot(16 - m_gtsSlotCount);

	beacon.SetGtsPermit(true);
	beacon.SetSupframeOrder((uint8_t)m_attributes.macSuperframeOrder);
	beacon.SetVpanCoord(true);

	beacon.SetGtsDescripCount(GetGtsDesCount());
	beacon.AddGtsList(GetGtsLists());
	beacon.SetGtsDirMask(GetGtsDirectionsMark());
	beacon.SetAddrs(GetPendingAddresses());

	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetDstAddress(Address(Mac16Address("ff:ff")));
	header.SetDstVPANId(m_attributes.macVPANId);
	header.SetFrameType(LIFI_BEACON);
	header.SetSequenceNumber(m_attributes.macBSN);
	header.SetSrcAddress(m_mac->GetDevice()->GetAddress());
	std::cout << m_mac->GetDevice()->GetAddress() << std::endl;
//	header.SetSrcVPANId(m_attributes.macVPANId);
	Ptr<Packet> p = beacon.GetPacket();
	p->AddHeader(header);
	return p;
}

void LifiMacCoordImpl::SetCFPLenth(uint32_t gtsLenth){
	m_gtsSlotCount = gtsLenth;
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
}

void LifiMacCoordImpl::DataConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << status);
	m_trxHandler->TxConfirm (status);
}

void LifiMacCoordImpl::SetMcpsSapUser(Ptr<McpsSapUser> u)
{
}

void LifiMacCoordImpl::AddGtsTransactionPacket(GtsTransactionInfo& gtsTransInfo)
{
	NS_LOG_FUNCTION(this);
	m_gtsHandler->AddGtsTransaction(gtsTransInfo);
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

} /* namespace ns3 */

