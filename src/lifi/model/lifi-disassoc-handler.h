/*
 * lifi-disassoc-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_DISASSOC_HANDLER_H_
#define LIFI_DISASSOC_HANDLER_H_
#include "ns3/object.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiDisassocHandler : public Object, public OpStatusCallback
{

public:
	LifiDisassocHandler();
	virtual ~LifiDisassocHandler();
	static TypeId GetTypeId ();

	virtual void TxResultNotification(PhyOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;

};

} /* namespace ns3 */

#endif /* LIFI_DISASSOC_HANDLER_H_ */
