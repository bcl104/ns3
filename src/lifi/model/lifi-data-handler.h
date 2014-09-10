/*
 * lifi-data-handler.h
 *
 *  Created on: 2014年8月10日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DATA_HANDLER_H_
#define LIFI_DATA_HANDLER_H_
#include "lifi-mac-general.h"
#include "data-service.h"
#include "lifi-mac-handler.h"

namespace ns3 {

class LifiDataHandler : public LifiMacHandler, public TrxHandlerListener
{

public:
	LifiDataHandler();
	virtual ~LifiDataHandler();
	static TypeId GetTypeId ();
	virtual void StartTransmit(DataDescriptor DataDesc);
	virtual void ReceiveData (uint32_t timestamp, Ptr<Packet> p);
	virtual void AllocNotification (Ptr<DataService> service);
	virtual void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

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

};

} /* namespace ns3 */

#endif /* LIFI_DATA_HANDLER_H_ */
