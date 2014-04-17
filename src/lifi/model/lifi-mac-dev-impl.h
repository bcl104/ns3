/*
 * lifi-mac-dev-impl.h
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#ifndef LIFI_MAC_DEV_IMPL_H_
#define LIFI_MAC_DEV_IMPL_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiTxHandler;
class LifiAssocHandler;
class LifiChannelScanHandler;
class LifiDisassocHandler;
class LifiBeaconHandler;
class LifiGtsHandler;
class LifiDataHandler;

class LifiMacDevImpl : public LifiMacImpl
{

public:
	LifiMacDevImpl();
	virtual ~LifiMacDevImpl();

	static TypeId GetTypeId ();


	virtual void Associate(LogicChannelId channel, TypeId coordAddrMode, uint16_t coordVPANId,
							Address coordAddr, CapabilityInfo info);
	virtual void Disassociate(TypeId devAddrMode, uint16_t devVPANId, Address devAddr,
									DisassocReason reason, bool txIndirect);
	virtual void GtsRequest(GTSCharacteristics characteristic);
	virtual void Polling(TypeId coordAddrMode, uint16_t coordVPANId, Address coordAddr);
	virtual void PurgeTrancsion(uint8_t handle);
	virtual void Receive(Ptr<PacketBurst> pb);
	virtual void Reset();
	virtual void RxEnable(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);
	virtual void Scan(ScanType scanType, uint8_t channel, uint32_t scanDuration);
	virtual void SendData(TypeId srcAddrMode, TypeId dstAddrMode, uint16_t dstVPANId,
							Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu,
							uint8_t msduHanle, TxOption option, bool rate);
	virtual void Synchronize(LogicChannelId channel, bool trackBeacon);

protected:
	bool DoChannelClearAccessment();
	void DoTransmitData(uint32_t size, Ptr<PacketBurst> pb, uint8_t band);
	virtual void onAcknowledge(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onAssocResponse(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onBandHopping(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onBeacon(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onData(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onDataConfirm(PhyOpStatus status);
	virtual void onDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onGTSResponse(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onImformationElement(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void onMultipleChannelAssignment(uint32_t timestamp, Ptr<Packet> msdu);

	LifiTxHandler* m_txHandler;
	LifiAssocHandler* m_assocHandler;
	LifiChannelScanHandler* m_channelScanHandler;
	LifiDisassocHandler* m_disassocHandler;
	LifiBeaconHandler* m_beaconHandler;
	LifiGtsHandler* m_gtsHandler;
	LifiDataHandler* m_dataHandler;
};

} /* namespace ns3 */

#endif /* LIFI_MAC_DEV_IMPL_H_ */
