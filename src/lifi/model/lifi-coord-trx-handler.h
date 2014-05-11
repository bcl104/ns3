/*
 * lifi-coord-trx-handler.h
 *
 *  Created on: 2014年5月3日
 *      Author: will
 */

#ifndef LIFI_COORD_TRX_HANDLER_H_
#define LIFI_COORD_TRX_HANDLER_H_

#include "lifi-trx-handler.h"
#include "lifi-transaction-handler.h"

namespace ns3
{

class LifiCoordTrxHandler: public LifiTrxHandler
{
public:
	LifiCoordTrxHandler();
	virtual ~LifiCoordTrxHandler();

	static TypeId GetTypeId ();

	void Start ();

	void Stop ();

protected:
	virtual void BeaconStart ();
	virtual void SuperframeEnd ();
	virtual void TxResultNotification (MacOpStatus status, Ptr<Packet> ack);

private:
	void TransmitBeacon ();

	bool m_running;

};

} /* namespace ns3 */

#endif /* LIFI_COORD_TRX_HANDLER_H_ */
