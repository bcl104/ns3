/*
 * lifi-disassoc-dev-handler.h
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DISASSOC_DEV_HANDLER_H_
#define LIFI_DISASSOC_DEV_HANDLER_H_

#include "lifi-disassoc-handler.h"
#include "lifi-trx-handler.h"

namespace ns3{

class LifiDisassocDevHandler : public LifiDisassocHandler{
public:
	LifiDisassocDevHandler();
	virtual ~LifiDisassocDevHandler();
	static TypeId GetTypeId ();
	void StartDisassoc(DisassocDescriptor disassocDes);
	virtual void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> p);
	void ReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);

	void AllocNotification(Ptr<DataService> service);
	void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

protected:
	void Reset();
	void onReceiveBeacon (uint32_t timestamp, Ptr<Packet> p);
	void SendDataRequest();
	void SendDisassocCCA();
	void SendAck();

	void onReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu);

	void onAllocNotification1(Ptr<DataService> service);
	void onAllocNotification2(Ptr<DataService> service);
	void onTxResultNotification1(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification2(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void onTxResultNotification3(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	Callback<void, Ptr<DataService> > m_allocNotification;

	DisassocDescriptor m_disassocDes;
	Address m_CoordAddress;
	DisassocReason m_disassocReason;
	Ptr<LifiTrxHandler> m_trxHandler;
	bool m_receiveDisassocNotify_Indirect;
};

}
#endif /* LIFI_DISASSOC_DEV_HANDLER_H_ */
