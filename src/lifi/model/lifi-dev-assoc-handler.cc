/*
 * lifi-dev-asso-handler.cc
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#include "lifi-dev-assoc-handler.h"
#include "ns3/log.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"
#include "lifi-mac-ack.h"
#include "lifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("LifiDevAssocHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDevAssocHandler);

LifiDevAssocHandler::LifiDevAssocHandler()
{
	NS_LOG_FUNCTION (this);
	m_run = false;
	m_trackBeacon = false;
//	NS_FATAL_ERROR("Unavailable instantiation with this constructor.");
	AddTrigger (LifiDevAssocHandler::TxResultNotification, false);
	AddTrigger (LifiDevAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAssocResponse, false);
	AddTrigger (LifiDevAssocHandler::AllocNotification, false);
}

LifiDevAssocHandler::LifiDevAssocHandler(DataService* service, LifiMacImpl* impl, PlmeSapProvider* p,
										 LifiMacPibAttribute* a, MlmeSapUser* user) {
	NS_LOG_FUNCTION (this);
	m_run = false;
	m_dataService = service;
	m_user = user;
	m_plmeProvider = p;
	m_impl = impl;
	m_trackBeacon = false;//MLME_SYNC.request primitive has this para.we should get it somewhere.
	AddTrigger (LifiDevAssocHandler::TxResultNotification, false);
	AddTrigger (LifiDevAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAssocResponse, false);
	AddTrigger (LifiDevAssocHandler::AllocNotification, false);
}

LifiDevAssocHandler::~LifiDevAssocHandler() {
}

TypeId LifiDevAssocHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDevAssocHandler")
						.SetParent<LifiAssocHandler> ()
						.AddConstructor<LifiDevAssocHandler> ();
	return tid;
}

void LifiDevAssocHandler::StartAssoc(VPANDescriptor &vpandescri){
	NS_LOG_FUNCTION (this << m_run);
	NS_ASSERT (!m_run);
	m_run = true;
	m_curChannel = vpandescri.logicChannel;
	m_VPANId = vpandescri.coordVPANId;
	m_coordAddr = vpandescri.coordAddr;
	m_coordAddrMode = vpandescri.coordAddrMode;

	SendAssocRequest();
}

void LifiDevAssocHandler::SendAssocRequest() {
	NS_LOG_FUNCTION(this);

	AssocRqstComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	std::cout << srcAddress << std::endl;
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_coordAddr);
	header.SetDstVPANId(m_VPANId);
	header.SetAckRequest(true);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_bust = false;
	info.m_handle = 0x31;
	info.m_force = false;
	info.m_listener = this;
	info.m_isAck = false;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = true;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
	m_trxHandler->Send(info);

	EnableTrigger(LifiDevAssocHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDevAssocHandler::onTxResultNotification1);
}

void LifiDevAssocHandler::TxResultNotification(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack)
{
	if (CheckTrigger(LifiDevAssocHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::onTxRuesult");
	}
}

void LifiDevAssocHandler::onTxResultNotification1(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << status);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	if (status == MAC_SUCCESS)
	{
		if (m_trackBeacon)
		{
			EnableTrigger(LifiDevAssocHandler::ReceiveBeacon);
			m_timer.SetFunction(&LifiDevAssocHandler::EndAssoc, this);
			m_timer.SetArguments(NO_DATA);
			uint32_t resWaitTime = m_impl->GetLifiMacPibAttribute().macResponseWaitTime;
			m_timer.Schedule(NanoSeconds(resWaitTime * m_impl->GetLifiMac()->GetOpticalPeriod()->GetNanoSeconds()));
		}else {
			m_timer.SetFunction(&LifiDevAssocHandler::SendDataRequest, this);
			int64_t op = m_impl->GetLifiMac()->GetOpticalPeriod()->GetNanoSeconds();
			uint32_t resWaitTime = m_attributes->macResponseWaitTime * LifiMac::aBaseSlotDuration * LifiMac::aNumSuperframeSlots;
			m_timer.Schedule(NanoSeconds(op * resWaitTime));
			std::cout << NanoSeconds(op * resWaitTime) << std::endl;
		}
	}else
	{
		EndAssoc(status);
	}
}

void LifiDevAssocHandler::ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDevAssocHandler::ReceiveBeacon))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		onReceiveBeacon(timestamp, msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::ReceiveBeacon");
	}
}

void LifiDevAssocHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << timestamp << p);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = p->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_BEACON);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(tempPacket);
	Address address = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	if (beacon.CheckPendingAddress(address))
	{
		if (!m_timer.IsExpired())
			m_timer.Cancel();
		DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
		SendDataRequest();
	}
}

void LifiDevAssocHandler::SendDataRequest(){
	NS_LOG_FUNCTION(this);
	DataRequestComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_coordAddr);
	header.SetDstVPANId(m_VPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_handle = 0x33;
	info.m_bust = false;
	info.m_force = false;
	info.m_isAck = false;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = true;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
	m_trxHandler->Send(info);

	EnableTrigger(LifiDevAssocHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDevAssocHandler::onTxResultNotification2);
}

void LifiDevAssocHandler::onTxResultNotification2(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	if (status == MAC_SUCCESS)
	{
		DisableTrigger(LifiDevAssocHandler::TxResultNotification);
		LifiMacHeader header;
		Ptr<Packet> tempPacket = ack->Copy();
		tempPacket->RemoveHeader(header);
		if (header.GetFramePending())
		{
			TranceiverTask task;
			task.Clear();
			task.id = RX_ASSOC_RESPONSE;
			task.listener = this;
			task.priority = GetTrxTaskPriority(RX_ASSOC_RESPONSE);

			EnableTrigger(LifiDevAssocHandler::AllocNotification);
			ReplaceTriggerCallback (m_allocNotification,
									LifiDevAssocHandler::onAllocNotification1);
			m_trxHandler->ServiceRequest(task);
		}
	}else
	{
		EndAssoc(status);
	}
}

void LifiDevAssocHandler::ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDevAssocHandler::ReceiveAssocResponse))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		onReceiveAssocResponse(timestamp, msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::ReceiveAssocResponse");
	}
}

void LifiDevAssocHandler::onReceiveAssocResponse(uint32_t timestamp, Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << timestamp << p);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = p->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_COMMAND);
	m_assocResponse = AssocResponseComm::Construct(tempPacket);
	if(m_assocResponse.GetAssocStatus() == MAC_SUCCESS)
	{
		uint16_t allocShortAddress = m_assocResponse.GetShortAddr();
		m_allocAddr.CopyFrom((uint8_t*) (&allocShortAddress));
		m_attributes->macShortAddress = allocShortAddress;
		NS_ASSERT (header.GetSrcAddressMode () == EXTENDED);
		m_attributes->macCoordExtendedAddress = Mac64Address::ConvertFrom(header.GetSrcAddress());
	}
	else if (m_assocResponse.GetAssocStatus() == DENIED)
	{
		m_attributes->macShortAddress = 0xffff;
	}else
	{
		NS_FATAL_ERROR("Unavailable association response status.");
	}
	m_service->Release();
	m_service = 0;

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiDevAssocHandler::AllocNotification);
	ReplaceTriggerCallback (m_allocNotification,
							LifiDevAssocHandler::onAllocNotification2);
}

void LifiDevAssocHandler::AllocNotification(Ptr<DataService> service)
{
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiDevAssocHandler::AllocNotification))
	{
		if (!m_allocNotification.IsNull())
				m_allocNotification (service);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}else {
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::AllocNotification");
	}
}

void LifiDevAssocHandler::onAllocNotification1(Ptr<DataService> service)
{
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_service = service;

	EnableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
}

void LifiDevAssocHandler::onAllocNotification2(Ptr<DataService> service)
{
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_service = service;
	SendAck();
}

void LifiDevAssocHandler::SendAck(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_coordAddr);
	header.SetFramePending(false);
	header.SetDstVPANId(m_VPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_handle = 21;
	info.m_bust = false;
	info.m_force = true;
	info.m_listener = this;
	info.m_isAck = true;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = false;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
	m_service->Transmit(info);
	EnableTrigger(LifiDevAssocHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDevAssocHandler::onTxResultNotification3);
}

void LifiDevAssocHandler::onTxResultNotification3(MacOpStatus status, PacketInfo info, Ptr<Packet> ack)
{
	NS_LOG_FUNCTION (this << status);
	NS_ASSERT(info.m_isAck == true);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	if(m_assocResponse.GetAssocStatus() == MAC_SUCCESS)
	{
		EndAssoc(MAC_SUCCESS);
	}
	else if (m_assocResponse.GetAssocStatus() == DENIED)
	{
		EndAssoc(DENIED);
	}else
	{
		////not success ,retransmission coding.
	}

}

void LifiDevAssocHandler::EndAssoc(MacOpStatus status){
	NS_LOG_FUNCTION(this << status);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == NO_DATA)
			 ||(status == MAC_SUCCESS)
			 ||(status == DENIED));

	if (status == CHANNEL_ACCESS_FAILURE)
	{
		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"), CHANNEL_ACCESS_FAILURE, NO_COLOR_STABI);
	}else if (status == NO_ACK)
	{
		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"), NO_ACK, NO_COLOR_STABI);
	}else if (status == NO_DATA)
	{
		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"), NO_DATA, NO_COLOR_STABI);
	}else if (status == MAC_SUCCESS)
	{
		m_trxHandler->GetPlmeSapProvider()->PlmeSetRequest(PHY_CURRENT_CHANNEL, m_curChannel);
		m_attributes->macVPANId = m_VPANId;
		m_user->MlmeAssociateConfirm(m_allocAddr, MAC_SUCCESS, NO_COLOR_STABI);
	}else
	{
		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"), DENIED, NO_COLOR_STABI);
	}
	m_run = false;
	m_service->Release();
	m_service = 0;
	Reset ();
}

void LifiDevAssocHandler::Reset()
{
	NS_LOG_FUNCTION (this);
	if (!m_run)
	{
		m_VPANId = 0xff;
		m_allocAddr = Mac16Address("ff:ff");
		m_curChannel = CHANNEL_UNAVAILABLE;
		m_trackBeacon = false;
//		m_txRstNotification.Nullify();
	}
}


void LifiDevAssocHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

} /* namespace ns3 */
