/*
 * lifi-asso-handler.cc
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#include "lifi-assoc-handler.h"
#include "ns3/log.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"
#include "lifi-mac-ack.h"
#include "lifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("LifiAssocHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiAssocHandler);

LifiAssocHandler::LifiAssocHandler()
{
	NS_LOG_FUNCTION (this);
//	NS_FATAL_ERROR("Unavailable instantialize with this constructor.");
	AddTrigger (LifiAssocHandler::TxResultNotification, false);
	AddTrigger (LifiAssocHandler::ReceiveAck, false);
	AddTrigger (LifiAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiAssocHandler::ReceiveAssocResponse, false);
}


LifiAssocHandler::LifiAssocHandler(DataService* service, LifiMacImpl* impl, PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user) {
	m_run = false;
	m_state = IDLE;
	m_dataService = service;
	m_user = user;
	m_provider = p;
	m_impl = impl;
	AddTrigger (LifiAssocHandler::TxResultNotification, false);
	AddTrigger (LifiAssocHandler::ReceiveAck, false);
	AddTrigger (LifiAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiAssocHandler::ReceiveAssocResponse, false);
}

LifiAssocHandler::~LifiAssocHandler() {
}

TypeId LifiAssocHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiAssocHandler")
						.SetParent<Object> ()
						.AddConstructor<LifiAssocHandler> ();
	return tid;
}

void LifiAssocHandler::SetMacPibAttribtes(LifiMacPibAttribute* attributes) {
	m_attributes = attributes;
}

void Send(uint32_t size, Ptr<Packet> p, uint8_t band, bool contention)
{
	return ;
}

void LifiAssocHandler::TxResultNotification(MacOpStatus status)
{
	if (CheckTrigger(LifiAssocHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		m_txRstNotification (status);
	}
	else{
		NS_LOG_ERROR("Ignore LifiAssocHandler::onTxRuesult");
	}
}

void LifiAssocHandler::ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiAssocHandler::ReceiveBeacon))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		m_beaconNotification (timestamp,msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiAssocHandler::ReceiveBeacon");
	}
}

void LifiAssocHandler::ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiAssocHandler::ReceiveAck))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		m_ackNotification (timestamp,msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiAssocHandler::ReceiveAck");
	}
}

void LifiAssocHandler::ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiAssocHandler::ReceiveAssocResponse))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		m_assocRspNotification (timestamp,msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiAssocHandler::ReceiveAssocResponse");
	}
}

void LifiAssocHandler::Start(VPANDescriptor &vpandescri, bool trackbeacon){
	NS_LOG_FUNCTION (this);
	NS_ASSERT (!m_run);
	m_run = true;
	m_curChannel = vpandescri.logicChannel;
	m_VPANId = vpandescri.coordVPANId;
	m_coordAddr = vpandescri.coordAddr;
	m_trackBeacon = trackbeacon;
	SwitchState(LifiAssocHandler::Initialize);
}

void LifiAssocHandler::EndAssoc(MacOpStatus status){
	NS_LOG_FUNCTION(this);
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
		m_user->MlmeAssociateConfirm(m_allocAddr, MAC_SUCCESS, NO_COLOR_STABI);
	}else
	{
		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"), DENIED, NO_COLOR_STABI);
	}
	m_run = false;
	Reset ();
}

void LifiAssocHandler::SendAssocRequest() {
	AssocRqstComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(COMMAND);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_coordAddr);
	p->AddHeader(header);

	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
	return;
}

void LifiAssocHandler::SendDataRequest(){
	DataRequestComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(COMMAND);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_coordAddr);
	p->AddHeader(header);

//	sendPacket();
	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
}

void LifiAssocHandler::SendAck(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_coordAddr);
	p->AddHeader(header);
//	sendPacket();
	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
}


void LifiAssocHandler::Reset()
{
	NS_LOG_FUNCTION (this);
	if (!m_run)
	{
		m_VPANId = 0;
		m_allocAddr = 0;
		m_curChannel = (LogicChannelId)0;
		m_trackBeacon = false;
		m_ackNotification.Nullify();
		m_txRstNotification.Nullify();
		m_beaconNotification.Nullify();
		m_assocRspNotification.Nullify();
	}else
	{
		return;
	}
}

void LifiAssocHandler::Initialize()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiAssocHandler::ReceiveAck);
	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	EnableTrigger(LifiAssocHandler::TxResultNotification);
	ReplaceTriggerCallback(m_txRstNotification,
	LifiAssocHandler::InitializeTxNotificationCallback);

	SendAssocRequest();

	m_state = INITIALIZE;
}

void LifiAssocHandler::InitializeTxNotificationCallback(MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	SwitchState(LifiAssocHandler::WaitForAck1);
}

void LifiAssocHandler::WaitForAck1()
{
	NS_LOG_FUNCTION (this);

	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiAssocHandler::TxResultNotification);
	EnableTrigger(LifiAssocHandler::ReceiveAck);
	ReplaceTriggerCallback(m_ackNotification,
	LifiAssocHandler::WaitForAck1AckNotificationCallback);

	m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_ACK);
	m_timer.Schedule(Seconds(10));
	m_state = WAIT_FOR_ACK1;
}

void LifiAssocHandler::WaitForAck1AckNotificationCallback(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	NS_ASSERT (!m_timer.IsExpired());

	m_timer.Cancel();

	if (m_trackBeacon)
	{
		SwitchState(LifiAssocHandler::WaitForResponWithTrack);
	}
	else{
		SwitchState(LifiAssocHandler::WaitForResponWithoutTrack);
	}
}

void LifiAssocHandler::WaitForResponWithTrack()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiAssocHandler::TxResultNotification);
	DisableTrigger(LifiAssocHandler::ReceiveAck);
	EnableTrigger(LifiAssocHandler::ReceiveBeacon);
	ReplaceTriggerCallback(m_beaconNotification,
	LifiAssocHandler::WaitForResponWithTrackBeaconNotificationCallback);

	m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_DATA);
	m_timer.Schedule(Seconds(11));
	m_provider->PlmeSetTRXStateRequest(RX_ON);

	m_state = WAIT_FOR_RESPON_WITH_TRACK;
}

void LifiAssocHandler::WaitForResponWithTrackBeaconNotificationCallback(
		uint32_t unsignedInt, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << unsignedInt << msdu);
	NS_ASSERT (!m_timer.IsExpired());

	LifiMacHeader header;
	msdu->RemoveHeader(header);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(msdu);
	NS_ASSERT (header.GetFrameType() == BEACON);
	if ((header.GetFramePending())
	  && beacon.CheckPendingAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress()))
	{
		m_timer.Cancel();
		SendDataRequest();
		DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
		DisableTrigger(LifiAssocHandler::ReceiveAck);
		DisableTrigger(LifiAssocHandler::ReceiveBeacon);
		EnableTrigger(LifiAssocHandler::TxResultNotification);
		ReplaceTriggerCallback(m_txRstNotification,
		LifiAssocHandler::WaitForResponWithTrackTxNotification);
	}
}

void LifiAssocHandler::WaitForResponWithTrackTxNotification(MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	if (status == MAC_SUCCESS)
	{
		SwitchState(LifiAssocHandler::WaitForAck2);
	}
	else if (status == CHANNEL_ACCESS_FAILURE){
		EndAssoc(CHANNEL_ACCESS_FAILURE);
	}
}

void LifiAssocHandler::WaitForResponWithoutTrack()
{
	NS_LOG_FUNCTION (this);
	// Disable all trigger.
	DisableTrigger(LifiAssocHandler::ReceiveAck);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiAssocHandler::TxResultNotification);

	NS_ASSERT (!m_timer.IsRunning());
	m_timer.SetFunction(&LifiAssocHandler::WaitForResponWithoutTrackTimeout, this);
	m_timer.Schedule(Seconds (10));

	m_state = WAIT_FOR_RESPON_WITHOUT_TRACK;
}

void LifiAssocHandler::WaitForResponWithoutTrackTxNotification(
		MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	if (status == MAC_SUCCESS)
	{
		SwitchState(LifiAssocHandler::WaitForAck2);
	}
	else if (status == CHANNEL_ACCESS_FAILURE){
		EndAssoc(CHANNEL_ACCESS_FAILURE);
	}
}

void LifiAssocHandler::WaitForResponWithoutTrackTimeout()
{
	NS_LOG_FUNCTION (this);

	SendDataRequest();
	DisableTrigger(LifiAssocHandler::ReceiveAck);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	EnableTrigger(LifiAssocHandler::TxResultNotification);
	ReplaceTriggerCallback(m_txRstNotification,
	LifiAssocHandler::WaitForResponWithoutTrackTxNotification);
}

void LifiAssocHandler::WaitForAck2()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiAssocHandler::TxResultNotification);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	EnableTrigger(LifiAssocHandler::ReceiveAck);
	ReplaceTriggerCallback(m_ackNotification,
	LifiAssocHandler::WaitForAck2AckNotificationCallback);

	NS_ASSERT (!m_timer.IsRunning());
	m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_ACK);
	m_timer.Schedule(Seconds (10));

	m_state = WAIT_FOR_ACK2;
}

void LifiAssocHandler::WaitForAck2AckNotificationCallback(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);

	NS_ASSERT (!m_timer.IsExpired());
	m_timer.Cancel();

	LifiMacHeader header;
	msdu->RemoveHeader(header);

	if (header.GetFramePending())
	{
		SwitchState(LifiAssocHandler::WaitForResponse);
	}else
	{
		EndAssoc(NO_DATA);
	}
}

void LifiAssocHandler::WaitForResponse()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiAssocHandler::TxResultNotification);
	DisableTrigger(LifiAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiAssocHandler::ReceiveAck);
	EnableTrigger(LifiAssocHandler::ReceiveAssocResponse);
	ReplaceTriggerCallback(m_assocRspNotification,
	LifiAssocHandler::OnAssocRspNotification);

	m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_DATA);
	m_timer.Schedule(Seconds (10));

	m_provider->PlmeSetTRXStateRequest(RX_ON);
	m_state = WAIT_FOR_ASSOC_RESPON;
}

void LifiAssocHandler::OnAssocRspNotification(uint32_t unsignedInt,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << unsignedInt << msdu);
	NS_ASSERT (!m_timer.IsExpired());

	m_timer.Cancel();
	LifiMacHeader header;
	msdu->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == COMMAND);

	AssocResponseComm response = AssocResponseComm::Construct(msdu);

	if(response.GetAssocStatus() == MAC_SUCCESS)
	{
		uint16_t allocShortAddress = response.GetShortAddr();
		m_allocAddr.CopyFrom((uint8_t*) (&allocShortAddress));
		m_attributes->macShortAddress = allocShortAddress;
		NS_ASSERT (header.GetSrcAddressMode () == EXTENDED);
		m_attributes->macCoordExtendedAddress = Mac64Address::ConvertFrom(header.GetSrcAddress());
		EndAssoc(MAC_SUCCESS);
	}
	else if (response.GetAssocStatus() == DENIED)
	{
		m_attributes->macShortAddress = 0xffff;
		EndAssoc(DENIED);
	}else
	{
		NS_FATAL_ERROR("Unavailable association response status.");
	}
}

} /* namespace ns3 */
