/*
 * lifi-dev-asso-handler.cc
 *
 *  Created on: 2014年4月20日
 *      Author: wanghongqiao
 */

#include "lifi-dev-assoc-handler.h"
#include "ns3/log.h"
#include "lifi-mac-comm.h"
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
	NS_FATAL_ERROR("Unavailable instantiation with this constructor.");
	AddTrigger (LifiDevAssocHandler::TxResultNotification, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAck, false);
	AddTrigger (LifiDevAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAssocResponse, false);
}


LifiDevAssocHandler::LifiDevAssocHandler(DataService* service, LifiMacImpl* impl, PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user) {
	m_run = false;
	m_state = IDLE;
	m_dataService = service;
	m_user = user;
	m_provider = p;
	m_impl = impl;
	AddTrigger (LifiDevAssocHandler::TxResultNotification, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAck, false);
	AddTrigger (LifiDevAssocHandler::ReceiveBeacon, false);
	AddTrigger (LifiDevAssocHandler::ReceiveAssocResponse, false);
}

LifiDevAssocHandler::~LifiDevAssocHandler() {
}

TypeId LifiDevAssocHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDevAssocHandler")
						.SetParent<LifiAssocHandler> ()
						.AddConstructor<LifiDevAssocHandler> ();
	return tid;
}

void LifiDevAssocHandler::SetMacPibAttribtes(LifiMacPibAttribute* attributes) {
	m_attributes = attributes;
}

void Send(uint32_t size, Ptr<Packet> p, uint8_t band, bool contention)
{
	return ;
}

void LifiDevAssocHandler::TxResultNotification(MacOpStatus status)
{
	if (CheckTrigger(LifiDevAssocHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::onTxRuesult");
	}
}

void LifiDevAssocHandler::ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDevAssocHandler::ReceiveBeacon))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		if (!m_beaconNotification.IsNull())
			m_beaconNotification (timestamp,msdu);
		else
			NS_LOG_ERROR ("m_beaconNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::ReceiveBeacon");
	}
}

void LifiDevAssocHandler::ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDevAssocHandler::ReceiveAck))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		if (!m_ackNotification.IsNull())
			m_ackNotification (timestamp,msdu);
		else
			NS_LOG_ERROR ("m_ackNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::ReceiveAck");
	}
}

void LifiDevAssocHandler::ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDevAssocHandler::ReceiveAssocResponse))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		if (!m_assocRspNotification.IsNull())
			m_assocRspNotification (timestamp,msdu);
		else
			NS_LOG_ERROR ("m_assocRspNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDevAssocHandler::ReceiveAssocResponse");
	}
}

void LifiDevAssocHandler::Start(VPANDescriptor &vpandescri, bool trackbeacon){
	NS_LOG_FUNCTION (this);
	NS_ASSERT (!m_run);
	m_run = true;
	m_curChannel = vpandescri.logicChannel;
	m_VPANId = vpandescri.coordVPANId;
	m_coordAddr = vpandescri.coordAddr;
	m_trackBeacon = trackbeacon;
	SwitchState(LifiDevAssocHandler::Initialize);
}

void LifiDevAssocHandler::EndAssoc(MacOpStatus status){
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

void LifiDevAssocHandler::SendAssocRequest() {
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

void LifiDevAssocHandler::SendDataRequest(){
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

void LifiDevAssocHandler::SendAck(){
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


void LifiDevAssocHandler::Reset()
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
		m_state = IDLE;
	}else
	{
		return;
	}
}

void LifiDevAssocHandler::Initialize()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiDevAssocHandler::ReceiveAck);
	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	EnableTrigger(LifiDevAssocHandler::TxResultNotification);
	ReplaceTriggerCallback(m_txRstNotification,
	LifiDevAssocHandler::InitializeTxNotificationCallback);

	SendAssocRequest();

	m_state = INITIALIZE;
}

void LifiDevAssocHandler::InitializeTxNotificationCallback(MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	NS_ASSERT ((status == MAC_SUCCESS)
			|| (status == CHANNEL_ACCESS_FAILURE));
	if (status == MAC_SUCCESS)
		SwitchState(LifiDevAssocHandler::WaitForAck1);
	else
		EndAssoc(CHANNEL_ACCESS_FAILURE);
}

void LifiDevAssocHandler::WaitForAck1()
{
	NS_LOG_FUNCTION (this);

	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	EnableTrigger(LifiDevAssocHandler::ReceiveAck);
	ReplaceTriggerCallback(m_ackNotification,
	LifiDevAssocHandler::WaitForAck1AckNotificationCallback);

	m_timer.SetFunction(&LifiDevAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_ACK);
	m_timer.Schedule(Seconds(10));
	m_state = WAIT_FOR_ACK1;
}

void LifiDevAssocHandler::WaitForAck1AckNotificationCallback(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	NS_ASSERT (!m_timer.IsExpired());

	m_timer.Cancel();

	if (m_trackBeacon)
	{
		SwitchState(LifiDevAssocHandler::WaitForResponWithTrack);
	}
	else{
		SwitchState(LifiDevAssocHandler::WaitForResponWithoutTrack);
	}
}

void LifiDevAssocHandler::WaitForResponWithTrack()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	DisableTrigger(LifiDevAssocHandler::ReceiveAck);
	EnableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	ReplaceTriggerCallback(m_beaconNotification,
	LifiDevAssocHandler::WaitForResponWithTrackBeaconNotificationCallback);

	m_timer.SetFunction(&LifiDevAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_DATA);
	m_timer.Schedule(Seconds(11));
	m_provider->PlmeSetTRXStateRequest(RX_ON);

	m_state = WAIT_FOR_RESPON_WITH_TRACK;
}

void LifiDevAssocHandler::WaitForResponWithTrackBeaconNotificationCallback(
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
		DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
		DisableTrigger(LifiDevAssocHandler::ReceiveAck);
		DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
		EnableTrigger(LifiDevAssocHandler::TxResultNotification);
		ReplaceTriggerCallback(m_txRstNotification,
		LifiDevAssocHandler::WaitForResponWithTrackTxNotification);
	}
}

void LifiDevAssocHandler::WaitForResponWithTrackTxNotification(MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	if (status == MAC_SUCCESS)
	{
		SwitchState(LifiDevAssocHandler::WaitForAck2);
	}
	else if (status == CHANNEL_ACCESS_FAILURE){
		EndAssoc(CHANNEL_ACCESS_FAILURE);
	}
}

void LifiDevAssocHandler::WaitForResponWithoutTrack()
{
	NS_LOG_FUNCTION (this);
	// Disable all trigger.
	DisableTrigger(LifiDevAssocHandler::ReceiveAck);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);

	NS_ASSERT (!m_timer.IsRunning());
	m_timer.SetFunction(&LifiDevAssocHandler::WaitForResponWithoutTrackTimeout, this);
	m_timer.Schedule(Seconds (10));

	m_state = WAIT_FOR_RESPON_WITHOUT_TRACK;
}

void LifiDevAssocHandler::WaitForResponWithoutTrackTxNotification(
		MacOpStatus status)
{
	NS_LOG_FUNCTION (this << (uint32_t) status);
	if (status == MAC_SUCCESS)
	{
		SwitchState(LifiDevAssocHandler::WaitForAck2);
	}
	else if (status == CHANNEL_ACCESS_FAILURE){
		EndAssoc(CHANNEL_ACCESS_FAILURE);
	}
}

void LifiDevAssocHandler::WaitForResponWithoutTrackTimeout()
{
	NS_LOG_FUNCTION (this);

	SendDataRequest();
	DisableTrigger(LifiDevAssocHandler::ReceiveAck);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	EnableTrigger(LifiDevAssocHandler::TxResultNotification);
	ReplaceTriggerCallback(m_txRstNotification,
	LifiDevAssocHandler::WaitForResponWithoutTrackTxNotification);
}

void LifiDevAssocHandler::WaitForAck2()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	EnableTrigger(LifiDevAssocHandler::ReceiveAck);
	ReplaceTriggerCallback(m_ackNotification,
	LifiDevAssocHandler::WaitForAck2AckNotificationCallback);

	NS_ASSERT (!m_timer.IsRunning());
	m_timer.SetFunction(&LifiDevAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_ACK);
	m_timer.Schedule(Seconds (10));

	m_state = WAIT_FOR_ACK2;
}

void LifiDevAssocHandler::WaitForAck2AckNotificationCallback(uint32_t timestamp,
		Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);

	NS_ASSERT (!m_timer.IsExpired());
	m_timer.Cancel();

	LifiMacHeader header;
	msdu->RemoveHeader(header);

	if (header.GetFramePending())
	{
		SwitchState(LifiDevAssocHandler::WaitForResponse);
	}else
	{
		EndAssoc(NO_DATA);
	}
}

void LifiDevAssocHandler::WaitForResponse()
{
	NS_LOG_FUNCTION (this);
	DisableTrigger(LifiDevAssocHandler::TxResultNotification);
	DisableTrigger(LifiDevAssocHandler::ReceiveBeacon);
	DisableTrigger(LifiDevAssocHandler::ReceiveAck);
	EnableTrigger(LifiDevAssocHandler::ReceiveAssocResponse);
	ReplaceTriggerCallback(m_assocRspNotification,
	LifiDevAssocHandler::OnAssocRspNotification);

	m_timer.SetFunction(&LifiDevAssocHandler::EndAssoc, this);
	m_timer.SetArguments(NO_DATA);
	m_timer.Schedule(Seconds (10));

	m_provider->PlmeSetTRXStateRequest(RX_ON);
	m_state = WAIT_FOR_ASSOC_RESPON;
}

void LifiDevAssocHandler::OnAssocRspNotification(uint32_t unsignedInt,
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
