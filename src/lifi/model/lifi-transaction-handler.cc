/*
 * lifi-transaction-handler.cc
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#include "lifi-transaction-handler.h"

NS_LOG_COMPONENT_DEFINE("LifiTransactionHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiTransactionHandler);

LifiTransactionHandler::LifiTransactionHandler()
{
	NS_LOG_FUNCTION(this);
	m_transationNumbers = 0 ;
	m_IsSevenTransactions = false;
//	memset(&m_curTransactionInfo,0,sizeof(m_curTransactionInfo));
	AddTrigger(LifiTransactionHandler::AddTransaction, true);
	AddTrigger(LifiTransactionHandler::PetchTransaction, true);
	AddTrigger(LifiTransactionHandler::DelTransaction, true);
	AddTrigger(LifiTransactionHandler::GetPendingAddress, true);
	AddTrigger(LifiTransactionHandler::TxResultNotification, false);
	AddTrigger(LifiTransactionHandler::AllocNotification, false);
}

LifiTransactionHandler::~LifiTransactionHandler()
{
	NS_LOG_FUNCTION(this);
}

TypeId LifiTransactionHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiTransactionHandler")
						.AddConstructor<LifiTransactionHandler> ()
						.SetParent<LifiMacHandler> ();
	return tid;
}

void LifiTransactionHandler::AddTransaction(TransactionInfo& transInfo)
{
	NS_LOG_FUNCTION(this);
	if(CheckTrigger(LifiTransactionHandler::AddTransaction))
	{
		onAddTransaction(transInfo);
	}else
	{
		NS_LOG_WARN("invalid LifiTransactionHandler::AddTransaction.");
	}
}

void LifiTransactionHandler::onAddTransaction(TransactionInfo& transInfo) {
	NS_LOG_FUNCTION(this);
	std::pair<Mac64Address, TransactionInfo> m_curTransactionPair;
	m_curTransactionPair.first = transInfo.m_extendDevAddress;
	m_curTransactionPair.second = transInfo;
	m_curTransactionPair.second.m_listener = this;
	uint32_t transPer = m_attributes->macTransactionPersistenceTime;
	uint32_t transPerTimes = transPer * LifiMac::aBaseSuperframeDuration * pow(2, m_attributes->macBeaconOrder);
	int64_t op = m_impl->GetLifiMac()->GetOpticalPeriod()->GetNanoSeconds();
	Time delay = NanoSeconds(transPerTimes * op);
	Ptr<Packet> temp = 0;

	(*m_curTransactionPair.second.m_timer).SetFunction(&LifiTransactionHandler::TimeOutTransaction, this);
	(*m_curTransactionPair.second.m_timer).SetArguments(transInfo.m_extendDevAddress, TRANSACTION_EXPIRED, temp);
	(*m_curTransactionPair.second.m_timer).SetDelay(delay);
	(*m_curTransactionPair.second.m_timer).Schedule();
	m_transactions.push_back(m_curTransactionPair);
	std::cout << m_transactions.size() << std::endl;
	m_transationNumbers ++;
}

void LifiTransactionHandler::PetchTransaction(Mac64Address DevAddress) {
	NS_LOG_FUNCTION(this);
	if(CheckTrigger(LifiTransactionHandler::PetchTransaction))
	{
		onPetchTransaction(DevAddress);
	}else
	{
		NS_LOG_WARN("invalid LifiTransactionHandler::PetchTransaction.");
	}
}

void LifiTransactionHandler::onPetchTransaction(Mac64Address DevAddress) {
	NS_LOG_FUNCTION(this);
    if(m_IsSevenTransactions){
    	for(Transactions::iterator it = m_transactions.begin();
			it != m_transactions.begin() + MAX_PENDING_ADDRESS; it++)
			{
				if((*it).first == DevAddress){
					memset(&m_curTransactionInfo,0,sizeof(m_curTransactionInfo));
					m_curTransactionInfo = (*it).second;
					m_curTransactionIterator = it;
					m_curTranmitTransactionInfo = m_curTransactionInfo;
					m_curTranmitTransactionInfo.m_packetInfo.m_listener = m_curTransactionInfo.m_listener;
					SendTransaction();
					break;
				}
			}
    }else{
    	for(Transactions::iterator it = m_transactions.begin();
			it != m_transactions.end(); it++)
			{
				if((*it).first == DevAddress){
					memset(&m_curTransactionInfo,0,sizeof(m_curTransactionInfo));
					m_curTransactionInfo = (*it).second;
					m_curTransactionIterator = it;
					m_curTranmitTransactionInfo = m_curTransactionInfo;
					m_curTranmitTransactionInfo.m_packetInfo.m_listener = m_curTransactionInfo.m_listener;
					SendTransaction();
					break;
				}
			}
    }
}

void LifiTransactionHandler::SendTransaction() {
	NS_LOG_FUNCTION(this);

	TranceiverTask task;
	task.Clear();
	task.id = TX_COMMAND;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_COMMAND);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiTransactionHandler::AllocNotification);
}

void LifiTransactionHandler::AllocNotification(Ptr<DataService> service) {
		NS_LOG_FUNCTION(this << service);
		if(CheckTrigger(LifiTransactionHandler::AllocNotification)){

			onAllocNotification(service);
		}
		else{
			NS_LOG_ERROR("Ignore LifiTransactionHandler::AllocNotification");
		}

}

void LifiTransactionHandler::onAllocNotification(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service << m_curTranmitTransactionInfo.m_packetInfo.m_listener << m_curTransactionInfo.m_packetInfo.m_listener);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiTransactionHandler::AllocNotification);

	m_dataService->Transmit(m_curTranmitTransactionInfo.m_packetInfo);
	EnableTrigger(LifiTransactionHandler::TxResultNotification);
	m_curTranmitTransactionInfo.Reset();
}

void LifiTransactionHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);
	if(CheckTrigger(LifiTransactionHandler::TxResultNotification))
	{
		onTxResultNotification(status, info, ack);
	}else{
		NS_LOG_WARN("ignore LifiTransactionHandler::TxResultNotification.");
	}

}

void LifiTransactionHandler::onTxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);

	NS_ASSERT(status == MAC_SUCCESS || status == CHANNEL_ACCESS_FAILURE || status == NO_ACK || status == TRANSACTION_EXPIRED);
	//here is no primitive to the upper layer.we need to notify the handler which invoked this handler.
	Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
							m_curTransactionInfo.m_packetInfo.m_listener,
							status, m_curTransactionInfo.m_packetInfo, ack);
	std::cout << m_transactions.size() << std::endl;
	if(status == MAC_SUCCESS || status == NO_ACK){
		m_transactions.erase(m_curTransactionIterator);
		m_transationNumbers --;
		std::cout << m_transactions.size() << std::endl;
	}
	m_dataService->Release();
	m_dataService = 0;
	m_curTransactionInfo.Reset();
}


void LifiTransactionHandler::DelTransaction(Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);
	if(CheckTrigger(LifiTransactionHandler::DelTransaction))
	{
		onDelTransaction(DevAddress, status, ack);
	}else{
		NS_LOG_WARN("ignore LifiTransactionHandler::DelTransaction.");
	}
}

void LifiTransactionHandler::onDelTransaction(Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack) {
	//satisfy some requirements to delete one transaction.we need to perfect it later.
	NS_LOG_FUNCTION(this);
	if(m_IsSevenTransactions){
		for(Transactions::iterator it = m_transactions.begin();
			it != m_transactions.begin() + MAX_PENDING_ADDRESS; it++)
			{
				if((*it).first == DevAddress){
					if(!(*it).second.m_packetInfo.m_option.gtsTx)
					{
						Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
												(*it).second.m_packetInfo.m_listener,
												status, (*it).second.m_packetInfo, ack);
					}else{
						//gts need to write later
					}
					m_transactions.erase(it);
					m_transationNumbers --;
					break;
				}
			}
	}else{
		for(Transactions::iterator it = m_transactions.begin();
			it != m_transactions.end(); it++)
			{
				if((*it).first == DevAddress){
					if(!(*it).second.m_packetInfo.m_option.gtsTx)
					{
						Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
												(*it).second.m_packetInfo.m_listener,
												status, (*it).second.m_packetInfo, ack);
					}else{
						//gts need to write later
					}
					m_transactions.erase(it);
					m_transationNumbers --;
					break;
				}
			}
	}
}

AddrList LifiTransactionHandler::GetPendingAddress() {
	NS_LOG_FUNCTION(this);
	m_pendingAddress.clear();
	if(m_transationNumbers >= MAX_PENDING_ADDRESS){
		for(Transactions::iterator it = m_transactions.begin();
			it != m_transactions.begin() + MAX_PENDING_ADDRESS; it++)
		{
			m_pendingAddress.push_back((*it).first);
		}
		m_IsSevenTransactions = true;
	}else{
		for(Transactions::iterator it = m_transactions.begin();
								   it != m_transactions.end(); it++)
			{
				m_pendingAddress.push_back((*it).first);
			}
		m_IsSevenTransactions = false;
	}
	return m_pendingAddress;
}

void LifiTransactionHandler::TimeOutTransaction(Mac64Address DevAddress, MacOpStatus status, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);
	Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
							(*m_transactions.begin()).second.m_listener,
							status, (*m_transactions.begin()).second.m_packetInfo, ack);
	std::cout << m_transactions.size() << std::endl;
//	std::cout << m_transactions.
	m_transactions.pop_front();
	m_transationNumbers --;
	NS_ASSERT(m_transationNumbers >= 0);

}

void LifiTransactionHandler::SetLifiMacImpl(LifiMacImpl* impl) {
	NS_LOG_FUNCTION(this);
	m_impl = impl;
}

Ptr<LifiMacImpl> LifiTransactionHandler::GetLifiMacImpl() const {
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiTransactionHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider) {
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiTransactionHandler::GetPlmeSapProvider() const {
	NS_LOG_FUNCTION(this);
	return m_plmeProvider;
}

void LifiTransactionHandler::SetDataService(Ptr<DataService> service) {
	NS_LOG_FUNCTION(this << service);
	m_dataService = service;
}

Ptr<DataService> LifiTransactionHandler::GetDataService() const {
	NS_LOG_FUNCTION(this);
	return m_dataService;
}

void LifiTransactionHandler::SetLifiMacPibAttribute(Ptr<LifiMacPibAttribute> pib) {
	NS_LOG_FUNCTION(this << pib);
	m_attributes = pib;
}

Ptr<LifiMacPibAttribute> LifiTransactionHandler::GetLifiMacPibAttribute() const {
	NS_LOG_FUNCTION(this);
	return m_attributes;
}

void LifiTransactionHandler::SetMlmeSapUser(Ptr<MlmeSapUser> user) {
	NS_LOG_FUNCTION(this << user);
	m_user = user;
}

Ptr<MlmeSapUser> LifiTransactionHandler::GetMlmeSapUser() const {
	NS_LOG_FUNCTION(this);
	return m_user;
}

void LifiTransactionHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

} /* namespace ns3 */
