/*
 * lifi-gts-dev-handler.h
 *
 *  Created on: 2014年8月19日
 *      Author: wanghongqiao
 */

#ifndef LIFI_GTS_DEV_HANDLER_H_
#define LIFI_GTS_DEV_HANDLER_H_

#include "lifi-gts-handler.h"
#include "lifi-trx-handler.h"

namespace ns3{

class LifiGtsDevHandler : public LifiGtsHandler{

public:
	LifiGtsDevHandler();
	virtual ~LifiGtsDevHandler();
	static TypeId GetTypeId ();
	void StartGtsRequest(GTSCharacteristics gtsCharacter, Address dstAddr);
	void SendGtsRequest();
	void ReceiveBeacon(uint32_t timestamp, Ptr<Packet> p);
	void AllocNotification (Ptr<DataService> service);
	void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	void SendGtsDatas();
	void EndGtsTransmit();
	void OpenGtsDataReceive(uint16_t devAddr);
	void CloseGtsDataReceive();
	void ReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

protected:
	void SendAck();
	void SetGtsOffset(uint8_t startSlot, uint8_t gtsLength, uint8_t gtsCount);
	void SetGtsDirection(GTSDirection direction);
	void onReceiveBeacon(uint32_t timestamp, Ptr<Packet> p);
	void onReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void onAllocNotification1 (Ptr<DataService> service);
	void onAllocNotification2 (Ptr<DataService> service);
	void onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	Callback<void, Ptr<DataService> > m_allocNotification;

	GTSCharacteristics m_gtsRequestCharacter;
	GTSCharacteristics m_gtsBeaconCharacter;
	Address m_dstAddress;
	GtsList m_beaconGtsList;
	bool m_gtsTransmitState;
	GtsTransactions m_gtsTransactions;
	GtsTransactionInfo m_curGtsTransmit;
	GtsTransactions::iterator m_curGtsTransIterator;
	Ptr<LifiTrxHandler> m_trxHandler;
};

}

#endif /* LIFI_GTS_DEV_HANDLER_H_ */
