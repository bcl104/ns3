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
	LifiMacHeader header;
	p->PeekHeader(header);
	std::cout << "its type:"<<  header.GetFrameType() << std::endl;
	std::cout <<"its dstAddr:" << header.GetDstAddress() << std::endl;
	std::cout <<"its srcAddr:" << header.GetSrcAddress() << std::endl;
	std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
	if(m_attributes->macReceivePacketPermit){
		if (CheckTrigger(LifiTrxHandler::ReceivePacket)){
			onReceivePacket(timestamp, p);
		}else{
			NS_LOG_WARN ("Ignore LifiTrxHandler::ReceivePacket.");
		}
	}
}

void LifiTrxHandler::Reset()
{
//	NS_LOG_FUNCTION (this);
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
	NS_LOG_FUNCTION (this << m_plmeProvider->PlmeSetTRXStateRequest(TX_ON));
	PhyOpStatus phyStatus = m_plmeProvider->PlmeSetTRXStateRequest(TX_ON);
//	NS_ASSERT (phyStatus == ns3::IDLE);
//	NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);
	if(phyStatus == TX_ON){
		std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
		m_curTransmission.Reset();
		m_curTransmission.m_info = info;
		std::cout << info.m_msduSize << std::endl;
//		NS_ASSERT (!info.m_option.gtsTx);
		NS_ASSERT(info.m_force == true);
		if (!info.m_option.gtsTx)
			NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CAP);

		if (info.m_option.gtsTx)
			NS_ASSERT (m_superframeStruct.m_state == SuperframeStrcut::CFP);

		if (info.m_force || info.m_option.gtsTx)
//			std::cout << m_curTransmission.IsAvailable() << std::endl;
			return DoTransmitData();
//		else
//			StartRandomAccess(info);

		}
	else{
		//phyStatus maybe TX_BUSY or RX_BUSY,too. we need handle these.
	}

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
//	NS_LOG_FUNCTION (this);
	m_opStatus = IDLE;
	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
	m_curTranceiverTask.Clear();
	if (m_superframeStruct.m_synchronized)
		Simulator:: ScheduleNow(&LifiTrxHandler::Fetch, this);
}

void LifiTrxHandler::Fetch()
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_opStatus == LifiTrxHandler::IDLE);
//	NS_ASSERT (!m_curTranceiverTask.Available());
	if(!m_curTranceiverTask.Available()){
		//	if (m_opStatus == TRANCEIVER_TASK) return;

		if ((!m_tranceiverTasks.empty()))
		{
			m_curTranceiverTask = m_tranceiverTasks.top();
			m_tranceiverTasks.pop();
			NS_ASSERT (m_curTranceiverTask.listener);
			Simulator::ScheduleNow(&TrxHandlerListener::AllocNotification,
					m_curTranceiverTask.listener,DataService::Create(this));
			m_taskType = TRANCEIVER_TASK;
			return;
		}
		switch (m_superframeStruct.m_state)
		{
		case SuperframeStrcut::CAP:
			if (m_raTasks.empty()) return;
//			std::cout << m_raTasks.size() << std::endl;
			std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
			m_curTransmission.Reset();
			m_curTransmission.m_info = m_raTasks.front();
			StartRandomAccess(m_curTransmission.m_info);
			m_taskType = GENERAL_RANDOM_ACCESS;
			m_raTasks.pop();
			break;
		case SuperframeStrcut::CFP:
			// In the CFP, GtsHandler will occupy all the resource of TrxHandler.
			break;
		default:
			return;
		}
	}
	else{
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

void LifiTrxHandler::SetMlmeSapUser (Ptr<MlmeSapUser> user){
	NS_LOG_FUNCTION (this << user);
	m_user = user;
}

void LifiTrxHandler::SetMcpsSapUser (Ptr<McpsSapUser> mcps){
	NS_LOG_FUNCTION (this << mcps);
	m_mcps = mcps;
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

void LifiTrxHandler::PermitReceivePacket(){
	m_attributes->macReceivePacketPermit = true;
}

LogicChannelId LifiTrxHandler::GetChannelId(){
	return GetPlmeSapProvider()->PlmeGetRequset<LogicChannelId>(PHY_CURRENT_CHANNEL);
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
	std::cout << m_attributes->macVPANId << std::endl;
	std::cout << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
	if((m_attributes->macVPANId == 0xffff)
	 ||((m_attributes->macVPANId != 0xffff)&&(m_attributes->macVPANId == header.GetDstVPANId()))){
		if (header.GetFrameType() == LIFI_BEACON)
		{
			NS_LOG_INFO(this << " receive beacon frame.");
	//		BuildSuperframeStruct(p);
			Broadcast(TrxHandlerListener::ReceiveBeacon, timestamp, p);
		}else{

			uint8_t* tempP = (uint8_t*)(&m_attributes->macShortAddress);
			uint16_t tempAddr = *tempP;
			std::cout << tempAddr << std::endl;
			tempP ++;
			std::cout << (uint32_t)*tempP << std::endl;
			tempAddr = (tempAddr<<8)|(*tempP);
			Mac16Address tempAddr1;
			tempAddr1.CopyFrom((uint8_t*)(&(tempAddr)));

			std::cout << m_attributes->macShortAddress << std::endl;
			std::cout << tempAddr << std::endl;
			std::cout << tempAddr1 << std::endl;
			std::cout << header.GetDstAddress() << std::endl;

			if(((header.GetDstAddressMode() == EXTENDED) && (header.GetDstAddress() == m_impl->GetLifiMac()->GetDevice()->GetAddress()))
			  || ((header.GetDstAddressMode() == SHORT) && (header.GetDstAddress() == Address(tempAddr1)))){
				if (header.GetFrameType() == LIFI_DATA)
				{
					NS_LOG_INFO(this << " receive data frame.");
					Broadcast(TrxHandlerListener::ReceiveData, timestamp, p);
				}else if (header.GetFrameType() == LIFI_ACK)
				{
					NS_LOG_INFO(this << " receive ack frame.");
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
				}else{
					NS_FATAL_ERROR("Frame type error: receive frame with reserved type.");
				}
			}else{
				NS_LOG_ERROR("Not my device Packet!");
			}
		}
	}else{
			std::cout << header.GetDstVPANId() << std::endl;
			NS_LOG_ERROR("Not the packet of this VPAN !");
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
//	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
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
	NS_LOG_FUNCTION (this << status << ack << m_curTransmission.m_info.m_listener);
//	m_curTransmission.m_info.m_listener->TxResultNotification(status, ack);

	Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
							m_curTransmission.m_info.m_listener,
							status, m_curTransmission.m_info, ack);

	if (m_taskType == GENERAL_RANDOM_ACCESS || m_taskType == TRANCEIVER_TASK)
	{
//		NS_ASSERT (m_opStatus == BACKOFF);
		m_opStatus = IDLE;
		m_taskType = TASK_IDLE;
		m_curTransmission.Reset();
		std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
		if (m_superframeStruct.m_synchronized)
			Simulator::ScheduleNow (&LifiTrxHandler::Fetch, this);
	}
	m_curTransmission.Reset();
//	NS_LOG_FUNCTION(this << m_curTransmission.m_backoff.m_backoffTimer.GetState());
}

bool LifiTrxHandler::DoTransmitData() {
//	NS_LOG_FUNCTION (this << m_impl->m_opticalPeriod->GetNanoSeconds() << m_curTransmission.m_info.m_listener);
//
//	m_plmeProvider->PlmeSetTRXStateRequest(TX_ON);
//	NS_ASSERT (m_curTransmission.IsAvailable());
//
//	/*
//	 * If check whether there is enough time before the end of CAP.
//	 * */
//	uint8_t mcsid = m_plmeProvider->PlmeGetRequset<uint8_t>(PHY_MCSID);
//	double dataRateKbps = LifiPhy::GetRate(mcsid);
////	std::cout << m_curTransmission.m_info.m_packet->GetSize() << std::endl;
////	std::cout << m_impl->m_opticalPeriod->GetNanoSeconds() << std::endl;
//	Time txDuration = NanoSeconds(((double) m_curTransmission.m_info.m_packet
//								->GetSize()*8)/(dataRateKbps*1000)*1e9);
//	Time ackWaitTime;
//	if (m_curTransmission.m_info.m_option.ackTx)
//		ackWaitTime = NanoSeconds(m_attributes->macAckWaitDuration
//				*m_impl->m_opticalPeriod->GetNanoSeconds());
//	else
//		ackWaitTime = NanoSeconds(0);
//
//	bool enough = false;
//
//	if (m_superframeStruct.m_state == SuperframeStrcut::BEACON)
//	{
//		NS_ASSERT (m_curTransmission.m_info.m_handle == 1);
//		enough = true;
//	}else if (m_superframeStruct.m_state == SuperframeStrcut::CAP)
//	{
//		NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
//		enough = (m_superframeStruct.m_capEnd.GetDelayLeft() > (txDuration+ackWaitTime));
//	}else if (m_superframeStruct.m_state == SuperframeStrcut::CFP)
//	{
//		NS_ASSERT (m_curTransmission.m_info.m_option.gtsTx);
//		std::cout << m_superframeStruct.m_cfpEnd.GetDelayLeft() << std::endl;
//		std::cout << m_superframeStruct.m_gtsEndDev.GetDelayLeft() << std::endl;
//		if(m_gtsIsCfpEnd){
//			enough = (m_superframeStruct.m_cfpEnd.GetDelayLeft() > (txDuration+ackWaitTime));
//		}else{
//			enough = (m_superframeStruct.m_gtsEndDev.GetDelayLeft() > (txDuration+ackWaitTime));
//		}
//
//	}else
//	{
//		NS_FATAL_ERROR("Error transmit timing.");
//	}
//
//	if (!enough)
//	{
//		if ((!m_curTransmission.m_info.m_option.gtsTx)
//		&& (!m_curTransmission.m_info.m_force))
//		{
//			m_suspendedTransmission = m_curTransmission;
//		}
//		std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
//		m_curTransmission.Reset();
//		return false;
//	}
//
////	std::cout << Simulator::Now() << std::endl;
////	std::cout << m_curTransmission.m_info.m_packet->GetSize() << std::endl;
//	m_pdProvider->DataRequest(m_curTransmission.m_info.m_packet->GetSize(),
//							  m_curTransmission.m_info.m_packet,
//							  m_curTransmission.m_info.m_band);
//	// Enable the external trigger to onTxConfirm.
//	DisableAllTrigger();
//	EnableTrigger(LifiTrxHandler::ReceivePacket);
//
//	EnableTrigger(LifiTrxHandler::TxConfirm);
//	m_plmeProvider->PlmeSetTRXStateRequest(RX_ON);
//
	return true;
}
void LifiTrxHandler::onTxConfirm(PhyOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t)status << NanoSeconds(m_attributes->macAckWaitDuration
			*m_impl->m_opticalPeriod->GetNanoSeconds()));
//	NS_ASSERT (m_curTask.Available());
	NS_ASSERT (status == PHY_SUCCESS);
	if (m_curTransmission.m_info.m_option.ackTx)
	{
		m_curTransmission.m_ackTimer.SetFunction(&LifiTrxHandler::AcknowledgmentTimeout, this);
		m_curTransmission.m_ackTimer.Schedule(NanoSeconds(m_attributes->macAckWaitDuration
							*m_impl->m_opticalPeriod->GetNanoSeconds()));
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
	std::cout << header.GetSequenceNumber() << std::endl;
	std::cout << m_curTransmission.m_sequenceNum << std::endl;
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
//	NS_LOG_FUNCTION(this);
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

void LifiTrxHandler::SetGtsDuration(uint8_t startSlot, uint8_t gtsLength, uint8_t gtsCount){

}

void LifiTrxHandler::DeleteGtsDuration(){

}

void LifiTrxHandler::SetGtsTrxState(GTSDirection direction){

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
				  m_gtsStartDev(Timer::CANCEL_ON_DESTROY),
				  m_gtsEndDev(Timer::CANCEL_ON_DESTROY),
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
	Time t = Simulator::Now ();
//	NS_LOG_FUNCTION (this << m_curTransmission.m_backoff.m_backoffTimer.GetState());
	if (m_curTransmission.IsAvailable())
	{
		NS_ASSERT (!m_curTransmission.m_info.m_option.gtsTx);
		NS_ASSERT (m_curTransmission.m_backoff.m_backoffTimer.IsRunning());//maybe all transmitted.
//		if(m_curTransmission.m_backoff.m_backoffTimer.IsRunning()){
		m_curTransmission.m_backoff.m_backoffTimer.Suspend();
		m_suspendedTransmission = m_curTransmission;
//		}
		std::cout << "my address:" << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
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
}

void LifiTrxHandler::GtsTransmitStart(){

}

void LifiTrxHandler::GtsTransmitEnd(){

}

void LifiTrxHandler::ContentionFreePeriodEnd()
{
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

bool LifiTrxHandler::IsCfpDuration(){
	if(m_superframeStruct.m_state == SuperframeStrcut::CFP)
		return true;
	else
		return false;
}

} /* namespace ns3 */


