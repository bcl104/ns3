/*
 * lifi-disassoc-coord-handler.h
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DISASSOC_COORD_HANDLER_H_
#define LIFI_DISASSOC_COORD_HANDLER_H_

#include "lifi-disassoc-handler.h"
#include "lifi-trx-handler.h"

namespace ns3 {
class LifiDisassocCoordHandler : public LifiDisassocHandler{
public:
	LifiDisassocCoordHandler();
	virtual ~LifiDisassocCoordHandler();
	static TypeId GetTypeId ();
	void StartDisassoc(DisassocDescriptor disassocDes);
	void ReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);
	void ReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu);
	void AllocNotification (Ptr<DataService> service);
	void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

protected:
	void SendToIndirectTransaction();
	void SendToCCA();
	void SendAck1();
	void SendAck2();
	void onReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);
	void onReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu);
	void onAllocNotification1 (Ptr<DataService> service);
	void onAllocNotification2 (Ptr<DataService> service);
	void onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification4 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	Callback<void, Ptr<DataService> > m_allocNotification;

	DisassocDescriptor m_disassocDes;
	LogicChannelId m_curChannel;
	Address m_CoordAddress;
	Address m_curDeviceAddress_R;
	Address m_curDeviceAddress_T;
	DisassocReason m_disassocReason;
	Ptr<LifiTrxHandler> m_trxHandler;
};

}
#endif /* LIFI_DISASSOC_COORD_HANDLER_H_ */
