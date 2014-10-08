/*
 * lifi-gts-coord-handler.h
 *
 *  Created on: 2014年8月19日
 *      Author: wanghongqiao
 */

#ifndef LIFI_GTS_COORD_HANDLER_H_
#define LIFI_GTS_COORD_HANDLER_H_

#include "lifi-gts-handler.h"
#include "lifi-trx-handler.h"

namespace ns3{

typedef std::vector<GTSDesInfo> GtsDesInfos;

class LifiGtsCoordHandler : public LifiGtsHandler{

public:
	LifiGtsCoordHandler();
	virtual ~LifiGtsCoordHandler();
	static TypeId GetTypeId ();
	void StartGtsDealloc(GTSCharacteristics gtsCharacter, Address dstAddr);
	void ReceiveGtsRequest(uint32_t timestamp, Ptr<Packet> p);
	void AllocNotification (Ptr<DataService> service);
	void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	GtsList GetGtsDescList();
	GtsList GetBeaconGtsDescList();
	uint8_t GetGtsDesCount();
	uint8_t GetGtsDirectionsMark();

	void SendGtsDatas();
	void EndGtsTransmit();
	void OpenGtsDataReceive(uint16_t devAddr);
	void CloseGtsDataReceive();
	void ReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void SetGtsTransmitArgument(uint16_t shortAddr, bool transmitState);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);
	void SetGTSCount();

protected:
	void onReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void SendAck();
	void SendAck2();
	void onReceiveGtsRequest(uint32_t timestamp, Ptr<Packet> p);
	void onAllocNotification1 (Ptr<DataService> service);
	void onAllocNotification2 (Ptr<DataService> service);
	void onAllocNotification3 (Ptr<DataService> service);
	void onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	Callback<void, Ptr<DataService> > m_allocNotification;

	GTSCharacteristics m_gtsDeviceCharacter;
	GtsDescriptor m_gtsDesc;
	GtsDesInfos m_gtsDesInfo;
	GtsList m_gtsList;
	uint16_t m_deviceAddress;
	uint16_t m_curStartSlot;
	Mac16Address m_allocGtsDevAddr;
	bool m_gtsTransmitState;
	Address m_dataAddr;
	GtsTransactions::iterator m_curGtsIterator;
	GtsTransactionInfo m_curGts;
	GtsTransactionInfo m_curGtsTransmit;
	DataIndicaDescriptor m_dataIndicaDes;
	Ptr<LifiTrxHandler> m_trxHandler;
};

}

#endif /* LIFI_GTS_COORD_HANDLER_H_ */
