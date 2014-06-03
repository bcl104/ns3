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
#include "data-service.h"
#include "lifi-trx-handler.h"
#include <map>

namespace ns3
{

//class LifiTrxHandler;

class LifiTransactionHandler : public LifiMacHandler, public TrxHandlerListener
{
public:
	friend class LifiCoordAssocHandler;
	LifiTransactionHandler();

	virtual ~LifiTransactionHandler();

	static TypeId GetTypeId ();

	void AddTransaction (TransactionInfo& transInfo);

	void PetchTransaction (Mac64Address DevAddress);

	void DelTransaction (Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack);

	AddrList GetPendingAddress ();

	virtual void TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	virtual void AllocNotification (Ptr<DataService> service);

	void SetLifiMacImpl (LifiMacImpl* impl);

	Ptr<LifiMacImpl> GetLifiMacImpl () const;

	void SetPlmeSapProvider (Ptr<PlmeSapProvider> provider);

	Ptr<PlmeSapProvider> GetPlmeSapProvider () const;

	void SetDataService (Ptr<DataService> service);

	Ptr<DataService> GetDataService () const;

	void SetLifiMacPibAttribute (Ptr<LifiMacPibAttribute> pib);

	Ptr<LifiMacPibAttribute> GetLifiMacPibAttribute () const;

	void SetMlmeSapUser (Ptr<MlmeSapUser> user);

	Ptr<MlmeSapUser> GetMlmeSapUser () const;

	void SetTrxHandler (Ptr<LifiTrxHandler> trxHandler);

private:

	static uint8_t AllocHandle ();

	void onAddTransaction (TransactionInfo& transInfo);

	void onPetchTransaction (Mac64Address DevAddress);

//	void DoDelTransaction (uint8_t handle);

	void SendTransaction(PacketInfo& packet);

	void TimeOutTransaction(Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack);

	void onAllocNotification (Ptr<DataService> service);

	void onTxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack);

	void onDelTransaction (Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack);

	Transactions m_transactions;
	AddrList m_pendingAddress;
	uint64_t m_transationNumbers;
	bool m_IsSevenTransactions;
	TransactionInfo m_curTransactionInfo;
	Transactions::iterator m_curTransactionIterator;
	std::pair<Mac64Address, TransactionInfo> m_curTransactionPair;

	Ptr<LifiTrxHandler> m_trxHandler;
//	Ptr<DataService> m_service;
//	MlmeSapUser* m_user;
};

} /* namespace ns3 */

#endif /* LIFI_TRANSACTION_HANDLER_H_ */
