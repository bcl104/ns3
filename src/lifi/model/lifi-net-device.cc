/*
 * lifi-net-device.cc
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#include "lifi-net-device.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE("LifiNetDevice");

namespace ns3 {

LifiNetDevice::LifiNetDevice() {
	// TODO Auto-generated constructor stub

}

LifiNetDevice::~LifiNetDevice() {
	// TODO Auto-generated destructor stub
}

void LifiNetDevice::SetMac(Ptr<LifiMac> pmac) {
	NS_LOG_FUNCTION(this);
	m_mac = pmac;
}

void LifiNetDevice::SetPhy(Ptr<LifiPhy> phy) {
	NS_LOG_FUNCTION(this);
	m_phy = phy;
}

Ptr<LifiMac> LifiNetDevice::GetMac() const {
	NS_LOG_FUNCTION(this);
	return m_mac;
}

Ptr<LifiPhy> LifiNetDevice::GetPhy() const {
	NS_LOG_FUNCTION(this);
	return m_phy;
}

void LifiNetDevice::SetIfIndex(const uint32_t index) {
	NS_LOG_FUNCTION(this);
	m_ifIndex = index;
}

uint32_t LifiNetDevice::GetIfIndex()const {
	NS_LOG_FUNCTION(this);
	return m_ifIndex;
}

Ptr<Channel> LifiNetDevice::GetChannel() const {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiNetDevice::SetAddress(Address address) {
	NS_LOG_FUNCTION(this);
}

Address LifiNetDevice::GetAddress() const {
	NS_LOG_FUNCTION(this);
	return Address ();
}

bool LifiNetDevice::SetMtu(const uint16_t mtu) {
	NS_LOG_FUNCTION(this);
	return 0;
}

uint16_t LifiNetDevice::GetMtu() const {
	NS_LOG_FUNCTION(this);
	return 0;
}

bool LifiNetDevice::IsLinkUp() const {
	NS_LOG_FUNCTION(this);
	return false;
}

void LifiNetDevice::AddLinkChangeCallback(Callback<void> claaback) {
	NS_LOG_FUNCTION(this);
}

bool LifiNetDevice::IsBroadcast() const {
	NS_LOG_FUNCTION(this);
	return false;
}

Address LifiNetDevice::GetBroadcast() const {
	NS_LOG_FUNCTION(this);
	return Address();
}

bool LifiNetDevice::IsMulticast() const {
	NS_LOG_FUNCTION(this);
	return 0;
}

Address LifiNetDevice::GetMulticast(Ipv4Address multicastGroup) const {
	NS_LOG_FUNCTION(this);
	return multicastGroup;
}

Address LifiNetDevice::GetMulticast(Ipv6Address addr) const {
	NS_LOG_FUNCTION(this);
	return addr;
}

bool LifiNetDevice::IsBridge() const {
	NS_LOG_FUNCTION(this);
	return false;
}

bool LifiNetDevice::IsPointToPoint() const {
	NS_LOG_FUNCTION(this);
	return false;
}

bool LifiNetDevice::Send(Ptr<Packet> packet, const Address& dest,
		uint16_t protocolNumber) {
	NS_LOG_FUNCTION(this);
	return false;
}

bool LifiNetDevice::SendFrom(Ptr<Packet> packet, const Address& source,
		const Address& dest, uint16_t protocolNumber) {
	NS_LOG_FUNCTION(this);
	return false;
}

Ptr<Node> LifiNetDevice::GetNode() const {
	NS_LOG_FUNCTION(this);
	return m_node;
}

void LifiNetDevice::SetNode(Ptr<Node> pnode) {
	NS_LOG_FUNCTION(this);
	m_node = pnode;
}

bool LifiNetDevice::NeedsArp() const {
	NS_LOG_FUNCTION(this);
	return false;
}

void LifiNetDevice::SetReceiveCallback(ReceiveCallback cb) {
	NS_LOG_FUNCTION(this);
}

void LifiNetDevice::SetPromiscReceiveCallback(PromiscReceiveCallback cb) {
	NS_LOG_FUNCTION(this);
}

bool LifiNetDevice::SupportsSendFrom() const {
	NS_LOG_FUNCTION(this);
	return false;
}

void LifiNetDevice::DoInitialize() {
	NS_LOG_FUNCTION(this);
}

void LifiNetDevice::DoDispose() {
	NS_LOG_FUNCTION(this);
}

void LifiNetDevice::LinkDown() {
	NS_LOG_FUNCTION(this);
}

void LifiNetDevice::LinkUp() {
	NS_LOG_FUNCTION(this);
}

} /* namespace ns3 */
