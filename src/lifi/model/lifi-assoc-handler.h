/*
 * lifi-asso-handler.h
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#ifndef LIFI_ASSOC_HANDLER_H_
#define LIFI_ASSOC_HANDLER_H_
#include "ns3/object.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"
#include <string>

namespace ns3 {

class LifiAssocHandler : public Object, public OpStatusCallback
{

public:
	static const uint32_t MAX_MAC_RESPONSE_WAIT_TIME = 61440;
//	macResponseWaitTime :numbers of optical clocks
	LifiAssocHandler();
	virtual ~LifiAssocHandler();

	static TypeId GetTypeId ();

	virtual void TxResultNotification(MacOpStatus status);

	void AssociationStart(VPANDescriptor &vpandescri, bool trackbeacon);

	void SetMacPibAttribtes(LifiMacPibAttribute* attributes);

	void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu);

	void ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

	void ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu);

protected:

	void onTxRuesult(MacOpStatus status);

	void SendAssocRequest();
	void SendDataRequest();
	void SendAck();
	void sendPacket();
	void EndAssoc();

	void DoRun();

	void onReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu);

	void onReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

	void onReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu);



	enum State
		{
			IDLE,
			WAIT_FOR_ACK1,
			WAIT_FOR_RESPO_WITH_TRACK,
			WAIT_FOR_RESPO_WITHOUT_TRACK,
			WAIT_FOR_ACK2,
			WAIT_FOR_ASSOC_RESPO,
			END,

		} m_state;

	DataService* m_dataService;
	LifiMacImpl* m_impl;
	MlmeSapProvider* m_provider;
	LifiMacPibAttribute* m_attributes;
	MlmeSapUser* m_user;

private:
	TypeId m_addrMode;
	Address m_CoordAddr;
	MacOpStatus m_reson;
//	bool m_txIndirect;                      //coordinator function
	uint16_t m_VPANId;
	LogicChannelId m_curChannel;
//	CapabilityInfo m_capainfo;
	bool m_trackBeacon;
	Timer m_timer;
	MacColorStabCapab m_colorStab;
	FrameType m_type;
	bool flag;

};

} /* namespace ns3 */

#endif /* LIFI_ASSO_CHANDLER_H_ */
