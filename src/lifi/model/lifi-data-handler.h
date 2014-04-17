/*
 * lifi-data-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_DATA_HANDLER_H_
#define LIFI_DATA_HANDLER_H_
#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"


namespace ns3 {

class LifiDataHandler : public Object, public OpStatusCallback
{

public:
	LifiDataHandler();
	virtual ~LifiDataHandler();
	static TypeId GetTypeId ();

	virtual void TxResultNotification(PhyOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;

};

} /* namespace ns3 */

#endif /* LIFI_DATA_HANDLER_H_ */