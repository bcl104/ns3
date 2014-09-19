/*
 * lifi-gts-handler.h
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#ifndef LIFI_GTS_HANDLER_H_
#define LIFI_GTS_HANDLER_H_

#include "lifi-mac-handler.h"
#include "data-service.h"
#include "lifi-mac-general.h"

namespace ns3 {

typedef std::vector<GtsDescriptor> GtsDescriptors;

class LifiGtsHandler : public LifiMacHandler, public TrxHandlerListener
{

public:
	LifiGtsHandler();
	virtual ~LifiGtsHandler();
	static TypeId GetTypeId ();
	bool GetGtsPermission ();
	uint8_t GetGtsCount ();
	uint8_t GetGtsDirMask ();
	GtsDescriptors GetGtsDescritors () const;
	void AllocNotification (Ptr<DataService> service);
	void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	virtual void ReportTransmission(MacOpStatus status);
	virtual void AddGtsTransaction(GtsTransactionInfo& gtsTransInfo);
	virtual void SetGtsOffset(uint8_t startSlot, uint8_t gtsLength);
	virtual void SendGtsDatas();
	virtual void EndGtsTransmit();
	virtual void OpenGtsDataReceive(uint16_t devAddr);
	virtual void CloseGtsDataReceive();
	virtual void ReceiveData (uint32_t timestamp, Ptr<Packet> p);
	virtual void SetGtsTransmitArgument(uint16_t shortAddr, bool transmitState);

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

private:
	GtsDescriptors m_descriptors;
	GtsTransactions m_gtsTransactions;
	GtsTransactionInfo m_curGtsTransaction;

};
} /* namespace ns3 */

#endif /* LIFI_GTS_HANDLER_H_ */
