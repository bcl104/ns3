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
	case TX_DATA:
	case TX_COMMAND:
		return 3;
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

Time LifiBackoff::GetBackoffTime()
{
	return Time();
}

LifiBackoff::LifiBackoff() : m_backoffTimer (Timer::CANCEL_ON_DESTROY)
{
}

void LifiBackoff::Reset()
{
}

void PacketInfo::Reset()
{
	this->m_band = 0;
	this->m_handle = 0;
	this->m_listener = 0;
	this->m_msduSize = 0;
	this->m_packet = 0;
}

bool PacketInfo::Available()
{
	return (this->m_band !=0) && (this->m_handle != 0) && (this->m_listener !=0) &&
	(this->m_msduSize != 0) && (this->m_packet != 0);
}
} /* namespace ns3 */


