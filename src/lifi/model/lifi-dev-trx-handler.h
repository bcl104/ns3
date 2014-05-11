/*
 * lifi-dev-trx-handler.h
 *
 *  Created on: 2014年5月3日
 *      Author: will
 */

#ifndef LIFI_DEV_TRX_HANDLER_H_
#define LIFI_DEV_TRX_HANDLER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-trx-handler.h"

namespace ns3
{

class LifiDevTrxHandler: public LifiTrxHandler
{
public:
	LifiDevTrxHandler();
	virtual ~LifiDevTrxHandler();
	static TypeId GetTypeId ();
protected:

	virtual void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> p);
	void BuildSuperframeStruct (Ptr<Packet> beacon);

};

} /* namespace ns3 */

#endif /* LIFI_DEV_TRX_HANDLER_H_ */
