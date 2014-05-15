/*
 * ifi-tx-handler.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "lifi-trx-handler.h"
#include "lifi-assoc-handler.h"
#include "lifi-gts-handler.h"
#include "lifi-data-handler.h"
#include "lifi-transaction-handler.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"
#include "ns3/log.h"
#include "ns3/timer.h"

NS_LOG_COMPONENT_DEFINE("LifiTrxHandler");

namespace ns3 {

bool operator< (TranceiverTask t1, TranceiverTask t2)
{
	return (t1.priority > t2.priority);
}

LifiTrxHandler::LifiTrxHandler()
{
	NS_LOG_FUNCTION (this);
	m_curTranceiverTask.Clear();
	m_impl = 0;
	m_listens.clear();
	m_plmeProvider = 0;
	m_pdProvider = 0;
	m_opticalPeriod = 0;
	m_opStatus = IDLE;
	AddTrigger(LifiTrxHandler::TxConfirm, false);
	AddTrigger(LifiTrxHandler::ReceivePacket, true);
}

LifiTrxHandler::~LifiTrxHandler() {
	NS_LOG_FUNCTION (this);
}

void LifiTrxHandler::TxConfirm(PhyOpStatus status) {
	if (CheckTrigger(LifiTrxHandler::TxConfirm))
	{
		onTxConfirm(status);
	}else
	{
		NS_LOG_WARN ("Ignore LifiTrxHandler::TxConfirm.");
	}
}

void LifiTrxHandler::ReceivePacket(uint32_t timestamp, Ptr<Packet> p)
{
	if (CheckTrigger(LifiTrxHandler::ReceivePacket))
	{
		onReceivePacket(timestamp, p);
	}else
	{
		NS_LOG_WARN ("Ignore LifiTrxHandler::ReceivePacket.");
	}
}

void LifiTrxHandler::Reset()
{
	NS_LOG_FUNCTION (this);
	m_curTranceiverTask.Clear();
}

void LifiTrxHandler::ServiceRequest(TranceiverTask task)
{
	NS_LOG_FUNCTION (this << task.priority);
	NS_ASSERT ((task.priority > 0) && (task.listener != 0));
	m_tranceiverTasks.push(task);
	if ((m_opStatus == LifiTrxHandler::IDLE)
	  && m_superframeStruct.m_synchronized)
//		Simulator::ScheduleNow(&LifiTrxHandler::Fetch, this);
		Fetch ();
}

bool LifiTrxHandler::Transmit(PacketInfo& info)
{
	NS_LOG_FUNCTION (this);
	PhyOpStatus phyStatus = m_plmeProvider->PlmeSetTRXStateRequest(DEFAULT);
	NS_ASSERT (phyStatus == ns3::IDLE);
//	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);
	m_curTransmission.Reset();
	m_curTransmission.m_info = info;
//	NS_ASSERT (!info.m_option.gtsTx);

	if (!info.m_option.gtsTx)
		NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);

	if (info.m_option.gtsTx)
		NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CFP);

	if (info.m_force || info.m_option.gtsTx)
		return DoTransmitData();
	else
		StartRandomAccess(info);

	return true;
}

void LifiTrxHandler::Receceive(bool enable)
{
	NS_LOG_FUNCTION (this << enable);
	PhyOpStatus phyStatus = m_plmeProvider->PlmeSetTRXStateRequest(DEFAULT);
	NS_ASSERT ((phyStatus != TX_BUSY));
	if (enable)
		m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	else
		m_plmeProvider->PlmeSetTRXStateRequest(TRX_OFF);
}

void LifiTrxHandler::Suspend()
{
//	NS_LOG_FUNCTION (this);
//	NS_ASSERT (m_curTask.Available());
//	m_suspendedTasks.push(m_curTask);
//	ClearCurTask();
//	Simulator:: ScheduleNow(&LifiTrxHandler::Fetch, this);
}

void LifiTrxHandler::Release()
{
	NS_LOG_FUNCTION (this);
	m_opStatus = IDLE;
	if (m_superframeStruct.m_synchronized)
		Simulator:: ScheduleNow(&LifiTrxHandler::Fetch, this);
}

void LifiTrxHandler::Fetch()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_opStatus == LifiTrxHandler::IDLE);
	NS_ASSERT (!m_curTranceiverTask.Available());

	if ((!m_tranceiverTasks.empty()) &&
		(m_superframeStruct.m_state == SuperframeStrcut::CAP))
	{
		m_curTranceiverTask = m_tranceiverTasks.top();
		m_tranceiverTasks.pop();
		NS_ASSERT (m_curTranceiverTask.listener);
		Simulator::ScheduleNow(&TrxHandlerListener::AllocNotification,
				m_curTranceiverTask.listener,DataService::Create(this));
		return;
	}
	switch (m_superframeStruct.m_state)
	{
	case SuperframeStrcut::CAP:
		if (m_raTasks.empty()) return;
		m_curTransmission.Reset();
		m_curTransmission.m_info = m_raTasks.front();
		StartRandomAccess(m_curTransmission.m_info);
		m_raTasks.pop();
		break;
	case SuperframeStrcut::CFP:
		// In the CFP, GtsHandler will occupy all the resource of TrxHandler.
		break;
	default:
		return;
	}
}

void LifiTrxHandler::SetLifiMacImpl(LifiMacImpl* impl)
{
	NS_LOG_FUNCTION (this << impl);
	m_impl = impl;
	Initialize(&(impl->m_attributes));
}

Ptr<LifiMacImpl> LifiTrxHandler::GetLifiMacImpl() const
{
	NS_LOG_FUNCTION (this);
	return m_impl;
}

void LifiTrxHandler::SetPlmeSapProvider(Ptr<PlmeSapProvider> provider)
{
	NS_LOG_FUNCTION (this << provider);
	m_plmeProvider = provider;
}

Ptr<PlmeSapProvider> LifiTrxHandler::GetPlmeSapProvider() const
{
	NS_LOG_FUNCTION (this);
	return m_plmeProvider;
}

void LifiTrxHandler::SetPdSapProvider(Ptr<PdSapProvider> provider)
{
	NS_LOG_FUNCTION (this << provider);
	m_pdProvider = provider;
}

Ptr<PdSapProvider> LifiTrxHandler::GetPdSapProvider() const
{
	NS_LOG_FUNCTION (this);
	return m_pdProvider;
}

void LifiTrxHandler::SetOpticalPeriod(const Time* oc)
{
	m_opticalPeriod = oc;
}

const Time* LifiTrxHandler::GetOpticalPeriod() const
{
	return m_opticalPeriod;
}

void LifiTrxHandler::ClearCurTask()
{
	NS_LOG_FUNCTION (this);
//	m_curTask.Clear();
	PhyOpStatus tranceiverState = m_plmeProvider->PlmeSetTRXStateRequest(DEFAULT);
	NS_ASSERT ((tranceiverState != TX_BUSY) || (tranceiverState != RX_BUSY));
	// There is something more to do for clearing random access process.
	m_plmeProvider->PlmeSetTRXStateRequest(TRX_OFF);
}

void LifiTrxHandler::AddListener(TypeId id, TrxHandlerListener* listener)
{
	NS_LOG_FUNCTION (this << (uint32_t)id.GetUid() << (uint64_t)(&listener));
	TrxHandlerListeners::iterator it = m_listens.find(id.GetUid());
	if (it == m_listens.end())
	{
		m_listens.insert(std::pair<uint16_t, TrxHandlerListener*>(id.GetUid(), listener));
	}else
	{
		NS_LOG_WARN ("Duplicated registration of TrxHandlerListener.");
	}
}

void LifiTrxHandler::RemoveListener(TypeId id, TrxHandlerListener* listener)
{
	NS_LOG_FUNCTION (this << (uint32_t)id.GetUid() << (uint64_t)(listener));
	TrxHandlerListeners::iterator it = m_listens.find(id.GetUid());
	if (it != m_listens.end())
	{
		m_listens.erase(it);
	}else
	{
		NS_LOG_WARN ("TrxHandlerListener not exist.");
	}
}

void LifiTrxHandler::onReceivePacket(uint32_t timestamp, Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << timestamp <<p);
	LifiMacHeader header;
	p->PeekHeader(header);
	if (header.GetFrameType() == LIFI_BEACON)
	{
		NS_LOG_INFO(this << " receive beacon frame.");
//		BuildSuperframeStruct(p);
		Broadcast(TrxHandlerListener::ReceiveBeacon, timestamp, p);
	}else if (header.GetFrameType() == LIFI_DATA)
	{
		NS_LOG_INFO(this << " receive data frame.");
		Broadcast(TrxHandlerListener::ReceiveData, timestamp, p);
	}else if (header.GetFrameType() == LIFI_ACK)
	{
		onReceiveAck(timestamp, p);
	}else if (header.GetFrameType() == LIFI_COMMAND)
	{
		NS_LOG_INFO(this << " receive command frame.");
		LifiMacHeader temp;
		p->RemoveHeader(temp);
		CommId commId = (CommId)0;
		p->CopyData((uint8_t*)(&commId), 1);
		p->AddHeader(temp);
		switch (commId)
		{
		case ASSOC_REQUEST:
			Broadcast(TrxHandlerListener::ReceiveAssocRequest, timestamp, p);
			break;
		case ASSOC_RESPONSE:
			Broadcast(TrxHandlerListener::ReceiveAssocResponse, timestamp, p);
			break;
		case DISASSOC_NOTIFICATION:
			Broadcast(TrxHandlerListener::ReceiveDisassocNotification, timestamp, p);
			break;
		case DATA_REQUEST:
			Broadcast(TrxHandlerListener::ReceiveDataRequest, timestamp, p);
			break;
		case BEACON_REQUEST:
			Broadcast(TrxHandlerListener::ReceiveBeaconRequest, timestamp, p);
			break;
		case GTS_REQUEST:
			Broadcast(TrxHandlerListener::ReceiveGtsRequest, timestamp, p);
			break;
		case GTS_RESPONSE:
			Broadcast(TrxHandlerListener::ReceiveGtsResponse, timestamp, p);
			break;
		case INFORMANTION_ELEMENT:
			NS_FATAL_ERROR("INFORMANTION_ELEMENT not realized.");
			break;
		default:
			NS_FATAL_ERROR("Command frame type error: Unsupported type.");
			break;
		}
	}else
	{
		NS_FATAL_ERROR("Frame type error: receive frame with reserved type.");
	}
}

void LifiTrxHandler::Backoff()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_curTransmission.IsAvailable());
	if (m_curTransmission.m_backoff.IsReachMaxRetry())
	{
		EndTransmission(CHANNEL_ACCESS_FAILURE, 0);
		return;
	}
	Time opticalPeriod = *(m_impl->m_mac->GetOpticalPeriod());
	Time nextBackOffset =  NanoSeconds(m_superframeStruct.m_capEnd.GetDelayLeft().GetNanoSeconds()
					% opticalPeriod.GetNanoSeconds());
	Time backoff = NanoSeconds(m_curTransmission.m_backoff.GetBackoffTime()
							* LifiMac::aUnitBackoffPeriod * opticalPeriod.GetNanoSeconds());
	NS_LOG_INFO ("Backoff: " << backoff);
	m_curTransmission.m_backoff.m_backoffTimer.SetFunction
						   (&LifiTrxHandler::onBackoffTimeout, this);
	m_curTransmission.m_backoff.m_backoffTimer.Schedule(backoff + nextBackOffset);
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::ReceivePacket);
	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	m_opStatus = LifiTrxHandler::BACKOFF;
}

bool LifiTrxHandler::ClearChannelAccessment()
{
	NS_LOG_FUNCTION (this);
	return m_plmeProvider->PlmeCcaRequset();
}

void LifiTrxHandler::AcknowledgmentTimeout()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_curTransmission.m_ackTimer.IsExpired()
			&& m_curTransmission.m_info.m_option.ackTx);
	m_curTransmission.IncrePacketRetry();
	if (m_curTransmission.IsReachMaxRetry())
	{
		EndTransmission(NO_ACK, 0);
	}else
	{
		if ((!m_curTransmission.m_info.m_force)
		&& (!m_curTransmission.m_info.m_option.gtsTx))
		{
			m_curTransmission.m_backoff.IncreNumRetries();
			Backoff();
		}
		if (!DoTransmitData())
		{
			EndTransmission(CHANNEL_ACCESS_FAILURE, 0);
		}
	}
}

bool LifiTrxHandler::RetransmitData()
{
	NS_LOG_FUNCTION (this);
	return DoTransmitData();
}

void LifiTrxHandler::EndTransmission(MacOpStatus status, Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << status << ack);
//	m_curTransmission.m_info.m_listener->TxResultNotification(status, ack);
	Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
							m_curTransmission.m_info.m_listener,
							status, m_curTransmission.m_info, ack);

	if (m_opStatus == BACKOFF)
	{
		m_opStatus = IDLE;
		m_curTransmission.Reset();
		if (m_superframeStruct.m_synchronized)
			Simulator::ScheduleNow (&LifiTrxHandler::Fetch, this);
	}
	m_curTransmission.Reset();

}

bool LifiTrxHandler::DoTransmitData() {
	NS_LOG_FUNCTION (this);
	m_plmeProvider->PlmeSetTRXStateRequest(TX_ON);
	NS_ASSERT (m_curTransmission.IsAvailable());

	/*
	 * If check whether there is enough time before the end of CAP.
	 * */
	uint8_t mcsid = m_plmeProvider->PlmeGetRequset<uint8_t>(PHY_MCSID);
	double dataRateKbps = LifiPhy::GetRate(mcsid);
	Time txDuration = NanoSeconds(((double) m_curTransmission.m_info.m_packet
								->GetSize()*8)/(dataRateKbps*1000)*1e9);
	Time ackWaitTime;
	if (m_curTransmission.m_info.m_option.ackTx)
		ackWaitTime = NanoSeconds(m_attributes->macAckWaitDuration
				*m_impl->m_opticalPeriod->GetNanoSeconds());
	else
		ackWaitTime = NanoSeconds(0);

	bool enough = false;

	if (m_superframeStruct.m_state == SuperframeStrcut::BEACON)
	{
		NS_ASSERT (m_curTransmission.m_info.m_handle == 1);
		enough = true;
	}else if (m_superframeStruct.m_state == SuperframeStrcut::CAP)
	{
		NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
		enough = (m_superframeStruct.m_capEnd.GetDelayLeft() > (txDuration+ackWaitTime));
	}else if (m_superframeStruct.m_state == SuperframeStrcut::CFP)
	{
		NS_ASSERT (m_curTransmission.m_info.m_option.gtsTx);
		enough = (m_superframeStruct.m_cfpEnd.GetDelayLeft() > (txDuration+ackWaitTime));
	}else
	{
		NS_FATAL_ERROR("Error transmit timing.");
	}

	if (!enough)
	{
		if ((!m_curTransmission.m_info.m_option.gtsTx)
		&& (!m_curTransmission.m_info.m_force))
		{
			m_suspendedTransmission = m_curTransmission;
		}
		m_curTransmission.Reset();
		return false;
	}


	m_pdProvider->DataRequest(m_curTransmission.m_info.m_packet->GetSize(),
							  m_curTransmission.m_info.m_packet,
							  m_curTransmission.m_info.m_band);
	// Enable the external trigger to onTxConfirm.
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::TxConfirm);
	return true;
}
void LifiTrxHandler::onTxConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t)status);
//	NS_ASSERT (m_curTask.Available());
	NS_ASSERT (status == PHY_SUCCESS);
	if (m_curTransmission.m_info.m_option.ackTx)
	{
		m_curTransmission.m_ackTimer.SetFunction(&LifiTrxHandler::AcknowledgmentTimeout, this);
		m_curTransmission.m_ackTimer.Schedule(NanoSeconds(m_attributes->macAckWaitDuration
							*m_curTransmission.m_opticalPeriod->GetNanoSeconds()));
		DisableAllTrigger();
		EnableTrigger(LifiTrxHandler::ReceivePacket);
		m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	}
	else{
		EndTransmission(MAC_SUCCESS, 0);
	}
}

void LifiTrxHandler::onReceiveAck(uint32_t timestamp, Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << timestamp << ack);
	LifiMacHeader header;
	ack->PeekHeader(header);
	NS_ASSERT (header.GetSequenceNumber() == m_curTransmission.m_sequenceNum);
	// Report the acknowledgment to the upper handler.
	EndTransmission(MAC_SUCCESS, ack);
}





void LifiTrxHandler::TransmissionInfo::IncrePacketRetry()
{
	if (this->m_packetRetry < *(this->maxPacketRetry))
		this->m_packetRetry ++;
}

bool LifiTrxHandler::TransmissionInfo::IsReachMaxRetry()
{
	return this->m_packetRetry >= *(this->maxPacketRetry);
}

LifiTrxHandler::TransmissionInfo::TransmissionInfo(uint32_t* maxRt)
									: maxPacketRetry(maxRt),
									  m_ackTimer (Timer::CANCEL_ON_DESTROY)
{
}

bool LifiTrxHandler::TransmissionInfo::IsAvailable()
{
	return (this->m_info.Available());
}

LifiTrxHandler::TransmissionInfo::TransmissionInfo()
				: m_ackTimer (Timer::CANCEL_ON_DESTROY)
{
}

LifiTrxHandler::TransmissionInfo::~TransmissionInfo()
{
}

void LifiTrxHandler::TransmissionInfo::Reset()
{
	this->m_info.Reset();
	this->m_packetRetry = 0;
	this->m_sequenceNum = 0;
	this->m_backoff.Reset();
	if (!this->m_ackTimer.IsExpired())
		this->m_ackTimer.Cancel();
}

void LifiTrxHandler::Send(PacketInfo& p)
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (!p.m_option.gtsTx);
	m_raTasks.push(p);

	if ((m_opStatus == LifiTrxHandler::IDLE)
	  && m_superframeStruct.m_synchronized
	  && m_superframeStruct.m_state != SuperframeStrcut::INACTIVE)
//		Simulator::ScheduleNow (&LifiTrxHandler::Fetch, this);
		Fetch ();
}

void LifiTrxHandler::StartRandomAccess(PacketInfo& info)
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_opStatus == LifiTrxHandler::IDLE);
	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);
	NS_ASSERT (m_curTransmission.m_info.Available());
	NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
	Backoff();
}

void LifiTrxHandler::StartGtsTransmit(PacketInfo& task)
{
}

void LifiTrxHandler::onBackoffTimeout()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Backoff timeout: " << Simulator::Now());
	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);
	if (ClearChannelAccessment ())
	{
		DoTransmitData();
	}else
	{
		m_curTransmission.m_backoff.IncreNumRetries();
		Backoff ();
	}
}

LifiTrxHandler::SuperframeStrcut::SuperframeStrcut()
				: m_synchronized (false),
				  m_contentionFreePeriod (false),
				  m_inactivePortion (false),
				  m_capEnd (Timer::CANCEL_ON_DESTROY),
				  m_cfpEnd (Timer::CANCEL_ON_DESTROY),
				  m_supframeEnd (Timer::CANCEL_ON_DESTROY),
				  m_state (DEFAULT)
{
}

void LifiTrxHandler::SuperframeStart()
{
	NS_LOG_FUNCTION (this);
	m_impl->GetLifiMac()->GetPlmeSapProvider()->PlmeSetTRXStateRequest(RX_ON);
	m_superframeStruct.m_state = SuperframeStrcut::CAP;
	NS_ASSERT (!m_curTransmission.IsAvailable());
	NS_ASSERT (!m_curTranceiverTask.Available());
	if (m_suspendedTransmission.IsAvailable())
	{
		NS_ASSERT (!m_suspendedTransmission.m_info.m_option.gtsTx);
		if (m_suspendedTransmission.m_backoff.m_backoffTimer.IsSuspended())
		{
			m_curTransmission = m_suspendedTransmission;
			m_curTransmission.m_backoff.m_backoffTimer.Resume();
			m_suspendedTransmission.Reset();
			return;
		}else if (m_suspendedTransmission.m_backoff.m_backoffTimer.IsExpired())
		{
			m_curTransmission = m_suspendedTransmission;
			Backoff ();
		}
		return;
	}
	NS_ASSERT (m_superframeStruct.m_synchronized);
	Fetch ();
}

void LifiTrxHandler::ContentionAccessPeriodEnd()
{
	NS_LOG_FUNCTION (this);
	if (m_curTransmission.IsAvailable())
	{
		NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
		NS_ASSERT (m_curTransmission.m_backoff.m_backoffTimer.IsRunning());
		m_curTransmission.m_backoff.m_backoffTimer.Suspend();
		m_suspendedTransmission = m_curTransmission;
		m_curTransmission.Reset();
	}
	if (m_superframeStruct.m_contentionFreePeriod)
	{
		ContentionFreePeriodStart();
		return;
	}else if (m_superframeStruct.m_inactivePortion)
	{
		InactionPortionStart ();
	}
}

void LifiTrxHandler::ContentionFreePeriodStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_state = SuperframeStrcut::CFP;
	NS_ASSERT (!m_curTransmission.IsAvailable());
	TrxHandlerListeners::iterator it = m_listens.find(LifiGtsHandler::GetTypeId().GetUid());
	if (it != m_listens.end())
		it->second->AllocNotification(DataService::Create(this));
}

void LifiTrxHandler::ContentionFreePeriodEnd()
{
	NS_LOG_FUNCTION (this);
	m_curTransmission.Reset();
	m_curTranceiverTask.Clear();
	if (m_superframeStruct.m_inactivePortion)
	{
		InactionPortionStart ();
	}
}

void LifiTrxHandler::InactionPortionStart()
{
	NS_LOG_FUNCTION (this);
	m_superframeStruct.m_state = SuperframeStrcut::INACTIVE;
}

void LifiTrxHandler::Initialize(LifiMacPibAttribute* attrubutes)
{
	m_attributes = attrubutes;
	m_curTransmission.m_backoff.maxBackoffExponential = &attrubutes->macMaxBE;
	m_curTransmission.m_backoff.minBackoffExponential = &attrubutes->macMinBE;
	m_curTransmission.m_backoff.maxBackoffRetry = &attrubutes->macMaxCSMABackoffs;
	m_curTransmission.maxPacketRetry = &attrubutes->macMaxFrameRetries;

	m_suspendedTransmission.m_backoff.maxBackoffExponential = &attrubutes->macMaxBE;
	m_suspendedTransmission.m_backoff.minBackoffExponential = &attrubutes->macMinBE;
	m_suspendedTransmission.m_backoff.maxBackoffRetry = &attrubutes->macMaxCSMABackoffs;
	m_suspendedTransmission.maxPacketRetry = &attrubutes->macMaxFrameRetries;
}

void LifiTrxHandler::SuperframeEnd()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (!m_curTransmission.IsAvailable());
	NS_ASSERT (!m_curTranceiverTask.Available());
	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	DisableAllTrigger();
	EnableTrigger(LifiTrxHandler::ReceivePacket);
	m_superframeStruct.m_synchronized = false;
	m_superframeStruct.m_state = SuperframeStrcut::DEFAULT;
}

} /* namespace ns3 */


