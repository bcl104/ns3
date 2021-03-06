/*
 * lifi-general.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-mac-general.h"

namespace ns3 {


uint16_t GetTrxTaskPriority(TranceiverTaskId task)
{
	switch (task)
	{
	case TRX_BEACON:
		return 1;
	case TX_ACK:
		return 2;
	case TX_COMMAND:
	case RX_ASSOC_RESPONSE:
	case RX_DISASSOC_NOTIFICATION:
		return 3;
	case TX_DATA:
		return 4;
	default:
		return 0;
	}
}

void LifiBackoff::IncreBackoffExp()
{
	if (this->m_backoffExponential < *(this->maxBackoffExponential))
		this->m_backoffExponential ++;
}

void LifiBackoff::IncreNumRetries()
{
	if (this->m_backoffRetries < *(this->maxBackoffRetry))
		this->m_backoffRetries ++;
}

bool LifiBackoff::IsReachMaxRetry()
{
	return this->m_backoffRetries >= *(this->maxBackoffRetry);
}

int64_t LifiBackoff::GetBackoffTime()
{
	UniformVariable v;
	return v.GetInteger(0, pow(2, this->m_backoffExponential) - 1);
}

LifiBackoff::LifiBackoff() : m_backoffTimer (Timer::CANCEL_ON_DESTROY)
{
	m_backoffRetries = 0;
}

LifiBackoff::~LifiBackoff()
{
}

void LifiBackoff::Reset()
{
	this->m_backoffExponential = *minBackoffExponential;
	this->m_backoffRetries = 0;
//	this->m_backoffTimer.Cancel();
}

void PacketInfo::Reset()
{
	this->m_band = CHANNEL1;
	this->m_handle = 0;
	this->m_listener = 0;
	this->m_msduSize = 0;
	this->m_packet = 0;
}

bool PacketInfo::Available()
{
	return (this->m_band !=0) && (this->m_handle != 0) &&
		   (this->m_msduSize != 0) && (this->m_packet != 0);
}

void TransactionInfo::Reset()
{
	this->m_listener = 0;
	this->m_extendDevAddress = Mac64Address("ff:ff:ff:ff:ff:ff:ff:ff");
	this->m_handle = 0;
	(*this->m_timer).Cancel();
	this->m_packetInfo.Reset();
}

} /* namespace ns3 */


