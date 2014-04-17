/*
 * lifi-mac-dev-impl.cc
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#include "lifi-mac-dev-impl.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiMacDevImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiMacDevImpl);

LifiMacDevImpl::LifiMacDevImpl() {
	NS_LOG_FUNCTION (this);
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

void LifiMacDevImpl::Receive(Ptr<PacketBurst> pb) {
	NS_LOG_FUNCTION (this);
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

bool LifiMacDevImpl::DoChannelClearAccessment() {
	NS_LOG_FUNCTION (this);
	return 0;
}

void LifiMacDevImpl::DoTransmitData(uint32_t size, Ptr<PacketBurst> pb,
		uint8_t band) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onAcknowledge(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onAssocResponse(uint32_t timestamp,
		Ptr<Packet> msdu) {
}

void LifiMacDevImpl::onBandHopping(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onBeacon(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onData(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onDataConfirm(PhyOpStatus status) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onDisassocNotification(uint32_t timestamp,
		Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onGTSResponse(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onImformationElement(uint32_t timestamp,
		Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}

void LifiMacDevImpl::onMultipleChannelAssignment(uint32_t timestamp,
		Ptr<Packet> msdu) {
	NS_LOG_FUNCTION (this);
}
} /* namespace ns3 */


