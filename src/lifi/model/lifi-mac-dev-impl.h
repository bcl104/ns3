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
#include "lifi-assoc-handler.h"
#include "lifi-channel-scan-handler.h"
#include "lifi-dev-assoc-handler.h"
#include "lifi-data-dev-handler.h"
#include "lifi-disassoc-dev-handler.h"
#include "lifi-gts-dev-handler.h"
#include "lifi-transaction-handler.h"

namespace ns3 {

//class LifiTxHandler;
//class LifiAssocHandler;
//class LifiChannelScanHandler;
//class LifiTransactionHandler;
//class LifiDevAssocHandler;
//class LifiDisassocHandler;
//class LifiBeaconHandler;
//class LifiGtsHandler;
class LifiDataDevHandler;

class LifiMacDevImpl : public LifiMacImpl
{

public:
	LifiMacDevImpl();
	virtual ~LifiMacDevImpl();

	static TypeId GetTypeId ();

	virtual void Associate(LogicChannelId channel, AddrMode coordAddrMode, uint16_t coordVPANId,
						   Mac64Address coordAddr, CapabilityInfo info);
	virtual void Disassociate(AddrMode devAddrMode, uint16_t devVPANId, Address devAddr,
									DisassocReason reason, bool txIndirect);
	virtual void GtsRequest(GTSCharacteristics characteristic, Address dstAddr);
	virtual void Polling(TypeId coordAddrMode, uint16_t coordVPANId, Address coordAddr);
	virtual void PurgeTrancsion(uint8_t handle);
	virtual void Receive(uint32_t size, Ptr<Packet> p, uint8_t quality);
	virtual void Reset();
	virtual void RxEnable(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);
	virtual void Scan(ScanType scanType, uint32_t scanDuration);

	virtual void SendData(AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId, Address dstAddr,
						  uint32_t size, Ptr<Packet> msdu, uint8_t handle, TxOption option,
						  DataRateId rate, bool burstMode);
	virtual void Synchronize(LogicChannelId channel, bool trackBeacon);

	virtual void DataConfirm (PhyOpStatus status);

	virtual void SetPdSapProvider (Ptr<PdSapProvider> p);

	virtual void SetPlmeSapProvider (Ptr<PlmeSapProvider> p);

	virtual void SetMlmeSapUser (Ptr<MlmeSapUser> u);

	virtual void SetMcpsSapUser (Ptr<McpsSapUser> m);

	virtual void AddGtsTransactionPacket (GtsTransactionInfo& gtsTransInfo);

	virtual void SetGtsTransmitArgument (uint16_t shortAddr, bool transmitState);
	virtual void OpenGtsDataReceive(uint16_t devAddr);
	virtual void CloseGtsDataReceive();
	virtual void EndGtsTransmit();
	virtual void SendGtsDatas();
protected:

	Ptr<LifiTrxHandler> m_trxHandler;
	Ptr<LifiChannelScanHandler> m_channelScanHandler;
	Ptr<LifiAssocHandler> m_assocHandler;
//	Ptr<LifiDisassocHandler> m_disassocHandler;
//	Ptr<LifiBeaconHandler> m_beaconHandler;
//	Ptr<LifiGtsHandler> m_gtsHandler;

	Ptr<LifiDevAssocHandler> m_devAssocHandler;
	Ptr<LifiDataDevHandler> m_dataDevHandler;
	Ptr<LifiTransactionHandler>  m_transcHandler;
	Ptr<LifiDisassocDevHandler> m_disassocDevHandler;
	Ptr<LifiGtsDevHandler> m_gtsDevHandler;

};

} /* namespace ns3 */

#endif /* LIFI_MAC_DEV_IMPL_H_ */
