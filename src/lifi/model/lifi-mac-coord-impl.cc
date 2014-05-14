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
				: m_trxHandler (new LifiCoordTrxHandler)
{
	m_trxHandler->SetLifiMacImpl(this);
	m_trxHandler->SetMacPibAttributes(&m_attributes);
}

LifiMacCoordImpl::~LifiMacCoordImpl() {

}

TypeId LifiMacCoordImpl::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiMacCoordImpl")
						.SetParent<LifiMacImpl> ()
						.AddConstructor<LifiMacCoordImpl> ();
	return tid;
}

void LifiMacCoordImpl::AssociateResponse(Mac64Address devAddr,
		Mac16Address assocShortAddr, MacOpStatus status,
		MacOpStatus capResponse) {
}

void LifiMacCoordImpl::PurgeTrancsion(uint8_t handle) {
}

void LifiMacCoordImpl::Receive(Ptr<PacketBurst> pb) {
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
		Ptr<Packet> msdu, uint8_t msduHanle, TxOption option, bool rate) {
}

void LifiMacCoordImpl::StartVPAN(uint16_t vpanId, LogicChannelId channel,
		uint32_t startTime, uint32_t beaconOrder, uint32_t supframeOrder,
		bool vpanCoord) {
	NS_LOG_FUNCTION (this);
	NS_ASSERT (vpanCoord);
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
	beacon.SetFinalCapSlot(15);
	beacon.SetGtsDescripCount(0);
	beacon.SetGtsPermit(false);
	beacon.SetSupframeOrder((uint8_t)m_attributes.macSuperframeOrder);
	beacon.SetVpanCoord(true);
	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetDstAddress(Address(Mac16Address("ff:ff")));
	header.SetDstVPANId(m_attributes.macVPANId);
	header.SetFrameType(LIFI_BEACON);
	header.SetSequenceNumber(m_attributes.macDSN);
	header.SetSrcAddress(m_mac->GetDevice()->GetAddress());
	header.SetSrcVPANId(m_attributes.macVPANId);
	Ptr<Packet> p = beacon.GetPacket();
	p->AddHeader(header);
	return p;
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

void LifiMacCoordImpl::SetMcpsSapUser(Ptr<McpsSapUser> u)
{
}

} /* namespace ns3 */
