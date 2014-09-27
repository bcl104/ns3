/*
 * lifi-mac-coord-impl.h
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#ifndef LIFI_MAC_COORD_IMPL_H_
#define LIFI_MAC_COORD_IMPL_H_
#include "lifi-mac-impl.h"
#include "lifi-gts-handler.h"
#include "lifi-coord-assoc-handler.h"
#include "lifi-coord-trx-handler.h"
#include "lifi-data-coord-handler.h"
#include "lifi-beacon-handler.h"
#include "lifi-disassoc-handler.h"
#include "lifi-gts-coord-handler.h"
#include "lifi-disassoc-coord-handler.h"
#include "lifi-channel-scan-handler.h"

namespace ns3 {

class LifiTxHandler;
class LifiAssocHandler;
class LifiChannelScanHandler;
class LifiGtsHandler;
class LifiCoordAssocHandler;

class LifiMacCoordImpl : public LifiMacImpl
{
	friend class LifiCoordTrxHandler;
	friend class LifiCoordAssocHandler;
	friend class LifiDevAssocHandler;
	friend class LifiMacBeacon;
public:
	LifiMacCoordImpl();
	virtual ~LifiMacCoordImpl();
	static TypeId GetTypeId ();

//	virtual void Associate(LogicChannelId channel, AddrMode coordAddrMode,
//						   uint16_t coordVPANId, Address coordAddr, CapabilityInfo info);
	virtual void AssociateResponse(Mac64Address devAddr, Mac16Address assocShortAddr,
									MacOpStatus status, MacColorStabCapab capResponse);
	virtual void Disassociate(AddrMode devAddrMode, uint16_t devVPANId, Address devAddr,
									DisassocReason reason, bool txIndirect);
	virtual void GtsRequest(GTSCharacteristics characteristic, Address dstAddr);
	virtual void PurgeTrancsion(uint8_t handle);
	virtual void Receive(uint32_t size, Ptr<Packet> p, uint8_t quality);
	virtual void Reset();
	virtual void RxEnable(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);
	virtual void Scan(ScanType scanType, uint32_t scanDuration);
	virtual void SendData(AddrMode srcAddrMode, AddrMode dstAddrMode, uint16_t dstVPANId,
							Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu,
							uint8_t msduHanle, TxOption option, DataRateId rate, bool burstMode);
	virtual void StartVPAN(uint16_t vpanId, LogicChannelId channel, uint32_t startTime,
						uint32_t beaconOrder, uint32_t supframeOrder, bool vpanCoord);

	virtual void DataConfirm (PhyOpStatus status);

	Ptr<Packet> ConstructBeacon () const;

	virtual void SetCFPLenth(uint32_t gtsLenth);

	virtual void SetLifiMac (Ptr<LifiMac> mac);

	virtual void SetOpticalPeriod (const Time* oc);

	virtual void SetPdSapProvider (Ptr<PdSapProvider> p);

	virtual void SetPlmeSapProvider (Ptr<PlmeSapProvider> p);

	virtual void SetMlmeSapUser (Ptr<MlmeSapUser> u);

	virtual void SetMcpsSapUser (Ptr<McpsSapUser> u);

private:

	void AddGtsTransactionPacket (GtsTransactionInfo& gtsTransInfo);
	void AddTransactionPacket (TransactionInfo& transInfo);
	void PetchTransactionPacket (Mac64Address DevAddress);
	GtsList GetGtsLists() const;
	GtsList GetBeaconGtsLists() const;
	uint8_t GetGtsDesCount()const;
	uint8_t GetGtsDirectionsMark() const;
	AddrList GetPendingAddresses () const;

	virtual void SetGtsTransmitArgument (uint16_t shortAddr, bool transmitState);
	virtual void OpenGtsDataReceive(uint16_t devAddr);
	virtual void CloseGtsDataReceive();
	virtual void EndGtsTransmit();
	virtual void SendGtsDatas();

	uint32_t m_gtsSlotCount;
	GtsList m_gtsList;
	Ptr<LifiCoordTrxHandler> m_trxHandler;
	Ptr<LifiChannelScanHandler> m_channelScanHandler;
	Ptr<LifiCoordAssocHandler> m_coordAssocHandler;
	Ptr<LifiTransactionHandler> m_transcHandler;
	Ptr<LifiDataCoordHandler> m_dataCoordHandler;
	Ptr<LifiDisassocCoordHandler> m_disassocCoordHandler;
	Ptr<LifiGtsCoordHandler> m_gtsCoordHandler;
};

} /* namespace ns3 */

#endif /* LIFI_MAC_COORD_IMPL_H_ */
