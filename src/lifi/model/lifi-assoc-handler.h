/*
 * lifi-assoc-handler.h
 *
 *  Created on: 2014年4月25日
 *      Author: will
 */

#ifndef LIFI_ASSOC_HANDLER_H_
#define LIFI_ASSOC_HANDLER_H_

#include "lifi-mac-handler.h"
#include "lifi-mac-general.h"
#include "data-service.h"

namespace ns3
{

//class DataService;
//class LifiMacImpl;
//class PlmeSapProvider;
//class LifiMacPibAttribute;
//class MlmeSapUser;

class LifiAssocHandler: public LifiMacHandler, public TrxHandlerListener
{
public:
	LifiAssocHandler();

	virtual ~LifiAssocHandler();

	static TypeId GetTypeId ();

	virtual void AllocNotification (Ptr<DataService> service);

	virtual void TxResultNotification (MacOpStatus status, Ptr<Packet> ack);

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


//protected:
//	DataService* m_dataService;
//	LifiMacImpl* m_impl;
//	PlmeSapProvider* m_provider;
//	LifiMacPibAttribute* m_attributes;
//	MlmeSapUser* m_user;
};

} /* namespace ns3 */

#endif /* LIFI_ASSOC_HANDLER_H_ */
