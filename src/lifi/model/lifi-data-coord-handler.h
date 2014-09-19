/*
 * lifi-data-coord-handler.h
 *
 *  Created on: 2014年8月10日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DATA_DEV_HANDLER_H_
#define LIFI_DATA_DEV_HANDLER_H_
#include "lifi-trx-handler.h"
#include "lifi-data-handler.h"

namespace ns3 {

class LifiDataCoordHandler : public LifiDataHandler
{

public:
	LifiDataCoordHandler();
	virtual ~LifiDataCoordHandler();
	static TypeId GetTypeId ();
	void StartTransmit(DataDescriptor DataDesc);
	void ReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void AllocNotification (Ptr<DataService> service);
	void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

protected:
	void SendToGtsTransaction();
	void SendToIndirectTransaction();
	void SendToCCA();
	void onReceiveData (uint32_t timestamp, Ptr<Packet> p);
	void onAllocNotification (Ptr<DataService> service);
	void onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	DataDescriptor m_dataDesc;
	DataIndicaDescriptor m_dataIndicaDes;
	Mac64Address m_curDeviceAddress;
	Ptr<LifiTrxHandler> m_trxHandler;
};

}

#endif /* LIFI_DATA_DEV_HANDLER_H_ */
