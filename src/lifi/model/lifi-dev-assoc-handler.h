/*
 * lifi-dev-asso-handler.h
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DEV_ASSOC_HANDLER_H_
#define LIFI_DEV_ASSOC_HANDLER_H_
#include "ns3/object.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"
#include "lifi-assoc-handler.h"
#include "lifi-trx-handler.h"
#include "lifi-mac-comm.h"

namespace ns3 {

//class AssocResponseComm;
class LifiDevAssocHandler : public LifiAssocHandler
{

public:
//	static const uint32_t MAX_MAC_RESPONSE_WAIT_TIME = 61440;

	LifiDevAssocHandler ();

	LifiDevAssocHandler(DataService* service, LifiMacImpl* impl, PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user);

	virtual ~LifiDevAssocHandler();

	static TypeId GetTypeId ();

	void Start(VPANDescriptor &vpandescri);

	void SetMacPibAttribtes(LifiMacPibAttribute* attributes);

	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

	// Inherit from TrxHandlerListener.
	virtual void AllocNotification (Ptr<DataService> service);

	virtual void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> p);

	virtual void TxResultNotification (MacOpStatus status,
									PacketInfo info, Ptr<Packet> ack);

	virtual void ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> p);

	void Reset ();

protected:
	void onAllocNotification1 (Ptr<DataService> service);
	void onAllocNotification2 (Ptr<DataService> service);

	void onReceiveBeacon (uint32_t timestamp, Ptr<Packet> p);

	void onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	void onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	void onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	void onReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> p);

	void SendAssocRequest();

	void SendDataRequest();

	void SendAck();

	void EndAssoc(MacOpStatus status);

private:
	Address m_coordAddr;
	AddrMode m_coordAddrMode;
	uint16_t m_VPANId;
	LogicChannelId m_curChannel;
	bool m_trackBeacon;
	AssocResponseComm m_assocResponse;


	bool m_run;
	Timer m_timer;
	Mac16Address m_allocAddr;
	Ptr<LifiTrxHandler> m_trxHandler;
	Ptr<DataService> m_service;

//	enum {
//		RECEIVE_ASSOC_RESPONSE,
//		SEND_ACK,
//	} m_occupationState;

	Callback<void, MacOpStatus, PacketInfo, Ptr<Packet> > m_txRstNotification;
	Callback<void, Ptr<DataService> > m_allocNotification;
//	Callback<void, uint32_t, Ptr<Packet> > m_ackNotification;
//	Callback<void, uint32_t, Ptr<Packet> > m_beaconNotification;
//	Callback<void, uint32_t, Ptr<Packet> > m_assocRspNotification;

};

} /* namespace ns3 */


#endif /* LIFI_DEV_ASSO_HANDLER_H_ */
