/*
 * lifi-channel-scan-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_CHANNEL_SCAN_HANDLER_H_
#define LIFI_CHANNEL_SCAN_HANDLER_H_
#include "data-service.h"
#include "lifi-mac-handler.h"
#include "lifi-trx-handler.h"

namespace ns3 {

class LifiChannelScanHandler : public LifiMacHandler, public TrxHandlerListener
{
public:

	LifiChannelScanHandler ();
	LifiChannelScanHandler (LifiMacImpl* impl, DataService* service, MlmeSapUser* user);
	virtual ~LifiChannelScanHandler();
	static TypeId GetTypeId ();
	virtual void ReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu);
	void StartChannelScan(ScanType scanType, uint32_t duration);

	void SetLifiMacImpl (LifiMacImpl* impl);
	Ptr<LifiMacImpl> GetLifiMacImpl () const;

	void SetPlmeSapProvider (Ptr<PlmeSapProvider> provider);
	Ptr<PlmeSapProvider> GetPlmeSapProvider () const;

	void SetDataService (Ptr<DataService> service);
	Ptr<DataService> GetDataService () const;

	void SetLifiMacPibAttribute (Ptr<LifiMacPibAttribute> pib);
	Ptr<LifiMacPibAttribute> GetLifiMacPibAttribute () const;

	void SetMlmeSapUser (Ptr<MlmeSapUser> user);
	Ptr<MlmeSapUser> GetMlmeSapUser () const;

	void SetTrxHandler(Ptr<LifiTrxHandler> trxHandler);

protected:
	void EndScannOnOneChannel();
	void onReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu);
	void SendBeaconRequest();
	void StartNewChannel();
	void FinishScan();
	void Reset ();

	bool m_channelScanRunning;
	uint8_t m_UnscannedChannel;
	LogicChannelId m_curChannel;
	uint32_t m_scanDuration;
	ScanType m_scanType;
	uint8_t m_storedVPANId;
	VpanDescriptors m_VPANDescriptors;
	uint8_t m_receiveBeaconNum;
	Timer m_timer;
	Ptr<LifiTrxHandler> m_trxHandler;
};

} /* namespace ns3 */

#endif /* LIFI_CHANNEL_SCAN_HANDLER_H_ */
