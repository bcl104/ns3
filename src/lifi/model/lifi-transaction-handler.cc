/*
 * lifi-transaction-handler.cc
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#include "lifi-transaction-handler.h"

namespace ns3
{

LifiTransactionHandler::LifiTransactionHandler()
{
	// TODO Auto-generated constructor stub

}

LifiTransactionHandler::~LifiTransactionHandler()
{
	// TODO Auto-generated destructor stub
}

uint8_t LifiTransactionHandler::AddTransaction(Ptr<Packet> p)
{
	return 0;
}

void LifiTransactionHandler::DelTransaction(uint8_t handle)
{
}

std::vector<Address> LifiTransactionHandler::GetPendingAddress() const
{
	std::vector<Address> address;
	return address;
}

void LifiTransactionHandler::ReceiveDataReuqest(uint32_t timestamp,
		Ptr<Packet> msdu)
{
}

void LifiTransactionHandler::ReceiveAck(uint32_t timestamp, Ptr<Packet> msdu)
{
}

void LifiTransactionHandler::TxResultNotification(MacOpStatus status)
{
}

uint8_t LifiTransactionHandler::AllocHandle()
{
	return 0;
}

void LifiTransactionHandler::DoDelTransaction(uint8_t handle)
{
}

} /* namespace ns3 */

ns3::Transaction::Transaction()
{
}

ns3::Transaction::~Transaction()
{
}
