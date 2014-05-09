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

namespace ns3 {

class LifiTxHandler;
class LifiAssocHandler;
class LifiChannelScanHandler;
class LifiDisassocHandler;
class LifiBeaconHandler;
class LifiGtsHandler;
class LifiDataHandler;

class LifiMacCoordImpl : public LifiMacImpl
{

public:
	LifiMacCoordImpl();
	virtual ~LifiMacCoordImpl();
	static TypeId GetTypeId ();

	virtual void AssociateResponse(Mac64Address devAddr, Mac16Address assocShortAddr,
									MacOpStatus status, MacOpStatus capResponse);
	virtual void Disassociate(TypeId devAddrMode, uint16_t devVPANId, Address devAddr,
									DisassocReason reason, bool txIndirect);
	virtual void PurgeTrancsion(uint8_t handle);
	virtual void Receive(Ptr<PacketBurst> pb);
	virtual void Reset();
	virtual void RxEnable(bool deferPermit, uint32_t rxOnTime, uint32_t rxOnDuration);
	virtual void Scan(ScanType scanType, LogicChannelId channel, uint32_t scanDuration);
	virtual void SendData(TypeId srcAddrMode, TypeId dstAddrMode, uint16_t dstVPANId,
							Address dstAddr, uint32_t msduLength, Ptr<Packet> msdu,
							uint8_t msduHanle, TxOption option, bool rate);
	virtual void StartVPAN(uint16_t vpanId, LogicChannelId channel, uint32_t startTime,
						uint32_t beaconOrder, uint32_t supframeOrder, bool vpanCoord);

	Ptr<Packet> ConstructBeacon () const;

	virtual void SetLifiMac (Ptr<LifiMac> mac);

	virtual void SetOpticalPeriod (const Time* oc);

protected:

	LifiCoordTrxHandler* m_trxHandler;
	LifiCoordAssocHandler* m_assocHandler;
	LifiDisassocHandler* m_disassocHandler;
	LifiBeaconHandler* m_beaconHandler;
	LifiGtsHandler* m_gtsHandler;
	LifiDataHandler* m_dataHandler;

};

} /* namespace ns3 */

#endif /* LIFI_MAC_COORD_IMPL_H_ */
