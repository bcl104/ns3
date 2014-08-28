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
#include "lifi-gts-coord-handler.h"

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
	virtual void TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack);
	virtual void ContentionFreePeriodEnd();
	virtual void ContentionFreePeriodStart();
//	virtual void GtsTransmitStart();
//	virtual void GtsTransmitEnd();
	void StartOneGts(uint16_t shortAddr, GTSDirection gtsDirec);
private:
	void TransmitBeacon ();
	bool m_running;

	uint8_t m_gtsCount;
	uint8_t m_curGtsCount;
	GtsList m_superframGtsList;
	GtsStructure m_curGtsTimer;
	GtsTimer m_gtsTimerList;
	Ptr<LifiGtsCoordHandler> m_gtsCoordHandler;

};

} /* namespace ns3 */

#endif /* LIFI_COORD_TRX_HANDLER_H_ */
