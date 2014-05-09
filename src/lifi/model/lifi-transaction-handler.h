/*
 * lifi-transaction-handler.h
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#ifndef LIFI_TRANSACTION_HANDLER_H_
#define LIFI_TRANSACTION_HANDLER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-mac-general.h"
#include "lifi-mac-handler.h"
#include <map>

namespace ns3
{

struct Transaction
{
	Transaction ();
	virtual ~Transaction ();
	uint8_t handle;
	Address address;
	Ptr<Packet> packet;
	EventId event;
	Callback<void, MacOpStatus> callback;
};

typedef std::map<uint8_t, Transaction> Transactions;

class LifiTransactionHandler : public LifiMacHandler
{
public:
	LifiTransactionHandler();

	virtual ~LifiTransactionHandler();

	uint8_t AddTransaction (Ptr<Packet> p);


	void DelTransaction (uint8_t handle);

	std::vector<Address> GetPendingAddress () const;

	void ReceiveDataReuqest (uint32_t timestamp, Ptr<Packet> msdu);

	void ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu);

	virtual void TxResultNotification(MacOpStatus status);


private:
	static uint8_t AllocHandle ();

	void DoDelTransaction (uint8_t handle);

	Transactions m_transactions;


};

} /* namespace ns3 */

#endif /* LIFI_TRANSACTION_HANDLER_H_ */
