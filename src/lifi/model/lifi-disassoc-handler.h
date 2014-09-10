/*
 * lifi-disassoc-handler.h
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */

#ifndef LIFI_DISASSOC_HANDLER_H_
#define LIFI_DISASSOC_HANDLER_H_
#include "lifi-mac-handler.h"
#include "data-service.h"
#include "lifi-mac-general.h"

namespace ns3 {

class LifiDisassocHandler : public LifiMacHandler, public TrxHandlerListener
{

public:
	LifiDisassocHandler();
	virtual ~LifiDisassocHandler();
	static TypeId GetTypeId ();
	void AllocNotification(Ptr<DataService> service);
	void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

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

protected:
	AddrMode GetCurAddressMode(Address address) const;
};

} /* namespace ns3 */

#endif /* LIFI_DISASSOC_HANDLER_H_ */
