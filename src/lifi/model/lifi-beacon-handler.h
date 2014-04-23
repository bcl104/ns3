/*
 * lifi-beacon-handler.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef LIFI_BEACON_HANDLER_H_
#define LIFI_BEACON_HANDLER_H_
#include "ns3/object.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiBeaconHandler : public Object, public OpStatusCallback
{

public:
	LifiBeaconHandler();
	virtual ~LifiBeaconHandler();
	static TypeId GetTypeId ();

	virtual void TxResultNotification(MacOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;
};

} /* namespace ns3 */

#endif /* LIFI_BEACON_HANDLER_H_ */
