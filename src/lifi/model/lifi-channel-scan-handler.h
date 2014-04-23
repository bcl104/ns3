/*
 * lifi-channel-scan-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_CHANNEL_SCAN_HANDLER_H_
#define LIFI_CHANNEL_SCAN_HANDLER_H_
#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "lifi-phy-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"


namespace ns3 {


class LifiChannelScanHandler : public Object, public OpStatusCallback
{

public:
	static const uint32_t MAX_BEACON_PER_CHANNEL = 64;
	static const uint32_t MAX_CHANNEL_DURATION = 65535;

	LifiChannelScanHandler ();

	LifiChannelScanHandler (LifiMacImpl* impl, DataService* service, MlmeSapUser* user);

	virtual ~LifiChannelScanHandler();

	static TypeId GetTypeId ();

	void ReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu);

	virtual void TxResultNotification(MacOpStatus status);

	bool Start(ScanType scanType, uint32_t duration);

	OpStatusCallback* GetOpStatusCallback ();

	bool IsRun ();

	bool Reset ();

	void SetMacPibAttribtes (LifiMacPibAttribute* attributes);

protected:

	void EndScannOnOneChannel();

	void DoRun();

	void Complete();

	void onReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu);

	void onTxRuesult(MacOpStatus status);

	void SendBeaconRequest();

	bool SwitchChannel();

	enum State
	{
		IDLE,
		WAIT_FOR_BCN,
		NEW_CHANNEL,
	} m_state;

	bool m_run;
	uint8_t m_channelUnscanned;
	LogicChannelId m_curChannel;
	uint32_t m_scanDuration;
	ScanType m_scanType;
	uint8_t m_storedVPANId;

	VpanDescriptors m_VPANDescriptors;

	Timer m_timer;
	uint32_t m_bcnRcved;

	LifiMacImpl* m_lifiMacImpl;
	DataService* m_dataService;
	MlmeSapUser* m_user;
	PlmeSapProvider* m_provider;
	LifiMacPibAttribute* m_attributes;

};

} /* namespace ns3 */

#endif /* LIFI_CHANNEL_SCAN_HANDLER_H_ */
