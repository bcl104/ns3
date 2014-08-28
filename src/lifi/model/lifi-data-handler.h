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
	virtual void ReceiveDataFrame(uint32_t timestamp, Ptr<Packet> msdu);
	virtual void AllocNotification (Ptr<DataService> service);
	virtual void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

};

} /* namespace ns3 */

#endif /* LIFI_DATA_HANDLER_H_ */
