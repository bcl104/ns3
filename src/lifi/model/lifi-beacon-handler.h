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
#include "data-service.h"
#include "lifi-mac-impl.h"

namespace ns3 {

class LifiBeaconHandler : public Object, public TrxHandlerListener
{

public:
	LifiBeaconHandler();

	virtual ~LifiBeaconHandler();

	static TypeId GetTypeId ();

	virtual void AllocNotification (Ptr<DataService> service);

	virtual void TxResultNotification (MacOpStatus status, Ptr<Packet> ack);

	virtual void ReceiveBeaconRequest (uint32_t timestamp, Ptr<Packet> p);

	Ptr<Packet> GetBeacon ();

protected:
	DataService* m_dataService;
	LifiMacImpl* m_impl;
};

} /* namespace ns3 */

#endif /* LIFI_BEACON_HANDLER_H_ */
