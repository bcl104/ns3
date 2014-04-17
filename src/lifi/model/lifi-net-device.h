/*
 * lifi-net-device.h
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#ifndef LIFI_NET_DEVICE_H_
#define LIFI_NET_DEVICE_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac.h"
#include "lifi-phy.h"

namespace ns3 {

class Node;
class Packet;
class LifiPhy;
class LifiMac;

class LifiNetDevice : public NetDevice
{

public:
	LifiNetDevice();
	virtual ~LifiNetDevice();

//	void SetTxPower(double tx_power);
//	void SetCSmode1Th(unsigned int th);
//	void SetCSmode2Th(unsigned int th);
	virtual void SetMac(Ptr<LifiMac> pmac);
	virtual void SetPhy(Ptr<LifiPhy> phy);
	virtual Ptr<LifiMac> GetMac() const;
	virtual Ptr<LifiPhy> GetPhy() const;
	virtual void SetIfIndex(const uint32_t index);
	virtual uint32_t GetIfIndex() const;
	virtual Ptr<Channel> GetChannel() const;
	virtual void SetAddress(Address address);
	virtual Address GetAddress() const;
	virtual bool SetMtu(const uint16_t mtu);
	virtual uint16_t GetMtu() const;
	virtual bool IsLinkUp() const;
	virtual void AddLinkChangeCallback(Callback<void> claaback);
	virtual bool IsBroadcast() const;
	virtual Address GetBroadcast() const;
	virtual bool IsMulticast() const;
	virtual Address GetMulticast(Ipv4Address multicastGroup) const;
	virtual Address GetMulticast(Ipv6Address addr) const;
	virtual bool IsBridge() const;
	virtual bool IsPointToPoint() const;
	virtual bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
	virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
//	void Receive(Ptr<Packet> packet);
	virtual Ptr<Node> GetNode() const;
	virtual void SetNode(Ptr<Node> pnode);
	virtual bool NeedsArp() const;
	virtual void SetReceiveCallback(ReceiveCallback cb);
	virtual void SetPromiscReceiveCallback(PromiscReceiveCallback cb);
	virtual bool SupportsSendFrom() const;

protected:
	virtual void DoInitialize();
	virtual void DoDispose();

private:
	/**
	 * BUSY_RX, BUSY_TX, RX_ON, TRX_OFF, TX_ON.
	 */
	PhyOpStatus m_nodeStatus;
//	point m_location;
//	unsigned float m_txPower;
//	uint_8 m_currentChannel;
	TypeId m_nodeType;
	uint32_t m_ifIndex;
	Ptr<Node> m_node;
	Ptr<LifiPhy> m_phy;
	Ptr<LifiMac> m_mac;
	NetDevice::ReceiveCallback m_recv;
	NetDevice::PromiscReceiveCallback m_promiscRx;
	bool m_linkUp;
	uint16_t m_mtu;

	void LinkDown();
	void LinkUp();

};

} /* namespace ns3 */

#endif /* LIFI_NET_DEVICE_H_ */
