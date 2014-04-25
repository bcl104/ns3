/*
 * lifi-assoc-handler.h
 *
 *  Created on: 2014年4月25日
 *      Author: will
 */

#ifndef LIFI_ASSOC_HANDLER_H_
#define LIFI_ASSOC_HANDLER_H_

#include "lifi-mac-handler.h"
#include "op-status-callback.h"

namespace ns3
{

class DataService;
class LifiMacImpl;
class PlmeSapProvider;
class LifiMacPibAttribute;
class MlmeSapUser;

class LifiAssocHandler: public LifiMacHandler, public OpStatusCallback
{
public:
	LifiAssocHandler();

	virtual ~LifiAssocHandler();

	static TypeId GetTypeId ();

	virtual void TxResultNotification(MacOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;
	PlmeSapProvider* m_provider;
	LifiMacPibAttribute* m_attributes;
	MlmeSapUser* m_user;

};

} /* namespace ns3 */

#endif /* LIFI_ASSOC_HANDLER_H_ */
