/*
 * lifi-gts-handler.h
 *
 *  Created on: 2014年3月31日
 *      Author: will
 */

#ifndef LIFI_GTS_HANDLER_H_
#define LIFI_GTS_HANDLER_H_

#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "op-status-callback.h"
#include "data-service.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiMacImpl;

class LifiGtsHandler : public Object, public OpStatusCallback
{

public:
	LifiGtsHandler();
	virtual ~LifiGtsHandler();

	static TypeId GetTypeId ();

	virtual void ReportTransmission(MacOpStatus status);

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;

};
} /* namespace ns3 */

#endif /* LIFI_GTS_HANDLER_H_ */
