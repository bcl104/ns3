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
#include "lifi-trx-handler.h"
#include "lifi-disassoc-handler.h"
#include "lifi-beacon-handler.h"
#include "lifi-gts-handler.h"
#include "lifi-data-handler.h"
#include "lifi-assoc-handler.h"
#include "lifi-channel-scan-handler.h"

namespace ns3 {

//class LifiTxHandler;
class LifiAssocHandler;
class LifiChannelScanHandler;
//class LifiDisassocHandler;
//class LifiBeaconHandler;
//class LifiGtsHandler;
//class LifiDataHandler;

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

	virtual void SetPdSapProvider (Ptr<PdSapProvider> p);

	virtual void SetPlmeSapProvider (Ptr<PlmeSapProvider> p);

	virtual void SetMlmeSapUser (Ptr<MlmeSapUser> u);

	virtual void SetMcpsSapUser (Ptr<McpsSapUser> u);

protected:

	Ptr<LifiTrxHandler> m_trxHandler;
	Ptr<LifiAssocHandler> m_assocHandler;
	Ptr<LifiChannelScanHandler> m_channelScanHandler;
	Ptr<LifiDisassocHandler> m_disassocHandler;
	Ptr<LifiBeaconHandler> m_beaconHandler;
	Ptr<LifiGtsHandler> m_gtsHandler;
	Ptr<LifiDataHandler> m_dataHandler;
};

} /* namespace ns3 */

#endif /* LIFI_MAC_DEV_IMPL_H_ */
