/*
 * lifi-net-device.cc
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#include "lifi-net-device.h"

namespace ns3 {

LifiNetDevice::LifiNetDevice() {
	// TODO Auto-generated constructor stub

}

LifiNetDevice::~LifiNetDevice() {
	// TODO Auto-generated destructor stub
}

void LifiNetDevice::SetMac(Ptr<LifiMac> pmac) {
}

void LifiNetDevice::SetPhy(Ptr<LifiPhy> phy) {
}

Ptr<LifiMac> LifiNetDevice::GetMac() const {
	return 0;
}

Ptr<LifiPhy> LifiNetDevice::GetPhy() const {
	return 0;
}

void LifiNetDevice::SetIfIndex(const uint32_t index) {
}

uint32_t LifiNetDevice::GetIfIndex()const {
	return 0;
}

Ptr<Channel> LifiNetDevice::GetChannel() const {
	return 0;
}

void LifiNetDevice::SetAddress(Address address) {
}

Address LifiNetDevice::GetAddress() const {
	return Address ();
}

bool LifiNetDevice::SetMtu(const uint16_t mtu) {
	return 0;
}

uint16_t LifiNetDevice::GetMtu() const {
	return 0;
}

bool LifiNetDevice::IsLinkUp() const {
	return false;
}

void LifiNetDevice::AddLinkChangeCallback(Callback<void> claaback) {
}

bool LifiNetDevice::IsBroadcast() const {
	return false;
}

Address LifiNetDevice::GetBroadcast() const {
	return Address();
}

bool LifiNetDevice::IsMulticast() const {
	return 0;
}

Address LifiNetDevice::GetMulticast(Ipv4Address multicastGroup) const {
	return multicastGroup;
}

Address LifiNetDevice::GetMulticast(Ipv6Address addr) const {
	return addr;
}

bool LifiNetDevice::IsBridge() const {
	return false;
}

bool LifiNetDevice::IsPointToPoint() const {
	return false;
}

bool LifiNetDevice::Send(Ptr<Packet> packet, const Address& dest,
		uint16_t protocolNumber) {
	return false;
}

bool LifiNetDevice::SendFrom(Ptr<Packet> packet, const Address& source,
		const Address& dest, uint16_t protocolNumber) {
	return false;
}

Ptr<Node> LifiNetDevice::GetNode() const {
	return 0;
}

void LifiNetDevice::SetNode(Ptr<Node> pnode) {
}

bool LifiNetDevice::NeedsArp() const {
	return false;
}

void LifiNetDevice::SetReceiveCallback(ReceiveCallback cb) {
}

void LifiNetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback cb) {
}

bool LifiNetDevice::SupportsSendFrom() const {
	return false;
}

void LifiNetDevice::DoInitialize() {
}

void LifiNetDevice::DoDispose() {
}

void LifiNetDevice::LinkDown() {
}

void LifiNetDevice::LinkUp() {
}

} /* namespace ns3 */
