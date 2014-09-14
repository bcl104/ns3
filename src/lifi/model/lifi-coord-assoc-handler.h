/*
 * lifi-coord-assoc-handler.h
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#ifndef LIFI_COORD_ASSOC_HANDLER_H_
#define LIFI_COORD_ASSOC_HANDLER_H_

#include "lifi-assoc-handler.h"
#include "lifi-mac-header.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-coord-impl.h"
#include "lifi-trx-handler.h"
#include "lifi-transaction-handler.h"

namespace ns3
{

class LifiCoordAssocHandler: public LifiAssocHandler
{
public:
	LifiCoordAssocHandler();
	LifiCoordAssocHandler(DataService* service, LifiMacImpl* impl,
			              PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user);
	virtual ~LifiCoordAssocHandler();
	static TypeId GetTypeId ();
	virtual void AllocNotification (Ptr<DataService> service);
	virtual void ReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> msdu);
	virtual void ReceiveDataRequest (uint32_t timestamp, Ptr<Packet> msdu);
	virtual void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void MlmeAssocResponse(Mac64Address devAddress, Mac16Address allocAssocShortAddr,
			               MacOpStatus status);
	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

private:
	void onReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> msdu);
	void onAllocNotification (Ptr<DataService> service);
	void sendAck1();
	void sendAck2();
	void onReceiveDataRequest (uint32_t timestamp, Ptr<Packet> msdu);
	void onTxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	void getTransctionPacket(Mac64Address devAddress);
	void reset();

	enum {
		SEND_ACK1,
		SEND_ACK2,
	}m_AckState;

	AddrMode m_srcAddrMode;
	AddrMode m_dstAddrMode;
	Address m_coordAddress;
	Mac64Address m_curDeviceAddress;
	LogicChannelId m_curChannel; ////how to get m_curChannel?
	uint16_t m_vpanId;
	uint32_t m_AckPacketSize;
	CapabilityInfo m_capInfo;
	uint8_t m_handle;
	Mac16Address m_allocShortAddr;
	MacOpStatus status;
	Ptr<LifiTrxHandler> m_trxHandler;
};

} /* namespace ns3 */

#endif /* LIFI_COORD_ASSOC_HANDLER_H_ */
