/*
 * lifi-coord-assoc-handler.h
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#ifndef LIFI_COORD_ASSOC_HANDLER_H_
#define LIFI_COORD_ASSOC_HANDLER_H_

#include "lifi-assoc-handler.h"

namespace ns3
{

//typedef std::map<Address, AssocSession> AssocSessions;

class LifiCoordAssocHandler: public LifiAssocHandler
{
public:
	LifiCoordAssocHandler();

	virtual ~LifiCoordAssocHandler();

	static TypeId GetTypeId ();

	void AddAssocResponse (Mac64Address devAddr, Mac16Address assocShortAddr,
						MacOpStatus status, MacOpStatus capNegoResponse);



	void ReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> msdu);

//	void ReceiveDataRequest (uint32_t timestamp, Ptr<Packet> msdu);

//	void ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

	void TransactionNotification (MacOpStatus status);

	virtual void TxResultNotification(MacOpStatus status);

private:

	void onReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> msdu);

	void onReceiveDataRequest (uint32_t timestamp, Ptr<Packet> msdu);

	void onReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

//	AssocSessions m_sessions;


};

} /* namespace ns3 */

#endif /* LIFI_COORD_ASSOC_HANDLER_H_ */
