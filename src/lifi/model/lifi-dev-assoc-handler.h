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

namespace ns3 {

class LifiDevAssocHandler : public LifiAssocHandler
{

public:
	static const uint32_t MAX_MAC_RESPONSE_WAIT_TIME = 61440;

	LifiDevAssocHandler ();

	LifiDevAssocHandler(DataService* service, LifiMacImpl* impl, PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user);

	virtual ~LifiDevAssocHandler();

	static TypeId GetTypeId ();

	void Start(VPANDescriptor &vpandescri, bool trackbeacon);

	void SetMacPibAttribtes(LifiMacPibAttribute* attributes);

	virtual void TxResultNotification(MacOpStatus status);

	void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu);

	void ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

	void ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu);

	void Reset ();

protected:
	void SendAssocRequest();

	void SendDataRequest();

	void SendAck();

	void EndAssoc(MacOpStatus status);

	void Initialize ();
	void InitializeTxNotificationCallback (MacOpStatus status);

	void WaitForAck1 ();
	void WaitForAck1AckNotificationCallback (uint32_t timestamp, Ptr<Packet> msdu);

	void WaitForResponWithTrack ();
	void WaitForResponWithTrackBeaconNotificationCallback (uint32_t, Ptr<Packet> msdu);
	void WaitForResponWithTrackTxNotification (MacOpStatus status);

	void WaitForResponWithoutTrack ();
	void WaitForResponWithoutTrackTxNotification (MacOpStatus status);
	void WaitForResponWithoutTrackTimeout ();

	void WaitForAck2 ();
	void WaitForAck2AckNotificationCallback (uint32_t timestamp, Ptr<Packet> msdu);

	void WaitForResponse ();
	void OnAssocRspNotification (uint32_t, Ptr<Packet> msdu);

	enum State
	{
		IDLE,
		INITIALIZE,
		WAIT_FOR_ACK1,
		WAIT_FOR_RESPON_WITH_TRACK,
		WAIT_FOR_RESPON_WITHOUT_TRACK,
		WAIT_FOR_ACK2,
		WAIT_FOR_ASSOC_RESPON,
	} m_state;

private:
	Address m_coordAddr;
	Mac16Address m_allocAddr;
	Timer m_timer;
	uint16_t m_VPANId;
	LogicChannelId m_curChannel;
	bool m_run;
	bool m_trackBeacon;

	Callback<void, MacOpStatus> m_txRstNotification;
	Callback<void, uint32_t, Ptr<Packet> > m_ackNotification;
	Callback<void, uint32_t, Ptr<Packet> > m_beaconNotification;
	Callback<void, uint32_t, Ptr<Packet> > m_assocRspNotification;

};

} /* namespace ns3 */


#endif /* LIFI_DEV_ASSO_HANDLER_H_ */
