/*
 * lifi-mac-coord-impl.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-coord-impl.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiMacCoordImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiMacCoordImpl);

LifiMacCoordImpl::LifiMacCoordImpl() {

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
}

bool LifiMacCoordImpl::DoChannelClearAccessment() {
	return false;
}

void LifiMacCoordImpl::DoTransmitData(uint32_t size, Ptr<PacketBurst> pb,
		uint8_t band) {
}

void LifiMacCoordImpl::onAcknowledge(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onAssocRequest(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onBeaconRequest(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onData(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onDataConfirm(MacOpStatus status) {
}

void LifiMacCoordImpl::onDataRequest(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onDisassocNotification(uint32_t timestamp,
		Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onGTSRequest(uint32_t timestamp, Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::onImformationElement(uint32_t timestamp,
		Ptr<Packet> msdu) {
}

void LifiMacCoordImpl::Disassociate(TypeId devAddrMode, uint16_t devVPANId,
		Address devAddr, DisassocReason reason, bool txIndirect) {
}

void LifiMacCoordImpl::onMultipleChannelAssignment(uint32_t timestamp,
		Ptr<Packet> msdu) {
}

} /* namespace ns3 */
