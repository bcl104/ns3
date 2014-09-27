/*
 * lifi-disassoc-dev-handler.cc
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */
#include "lifi-disassoc-dev-handler.h"
#include "ns3/log.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"

NS_LOG_COMPONENT_DEFINE ("LifiDisassocDevHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDisassocDevHandler);

LifiDisassocDevHandler::LifiDisassocDevHandler() {
	NS_LOG_FUNCTION(this);
	m_receiveDisassocNotify_Indirect = false;
	AddTrigger(LifiDisassocDevHandler::AllocNotification, false);
	AddTrigger(LifiDisassocDevHandler::TxResultNotification, false);
	AddTrigger(LifiDisassocDevHandler::ReceiveBeacon, true);
	AddTrigger(LifiDisassocDevHandler::ReceiveDisassocNotification, true);
}

LifiDisassocDevHandler::~LifiDisassocDevHandler() {
}

TypeId LifiDisassocDevHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDisassocDevHandler")
							.AddConstructor<LifiDisassocDevHandler> ()
							.SetParent<LifiDisassocHandler> ();
		return tid;
}
void LifiDisassocDevHandler::StartDisassoc(DisassocDescriptor disassocDes){
	NS_LOG_FUNCTION(this);
	m_disassocDes = disassocDes;
	NS_ASSERT(m_attributes->macVPANId != 0xffff);
//	m_attributes->macCoordExtendedAddress = Mac64Address("00:00:00:00:00:00:00:01");
////    here the "m_attributes->macVPANId" is not initialized because we test it when this device is not associated!
////	we can delete this when it is associated.
	if(m_attributes->macVPANId != m_disassocDes.DeviceVPANId){
		m_user->MlmeDisassociateConfirm(MAC_INVALID_PARAMETER, GetCurAddressMode(m_impl->GetLifiMac()->GetDevice()->GetAddress()),
										m_attributes->macVPANId, m_impl->GetLifiMac()->GetDevice()->GetAddress());
	}else{
		if(((m_disassocDes.DeviceAddrMode == EXTENDED) && (m_attributes->macCoordExtendedAddress == m_disassocDes.DeviceAddr))
			    ||((m_disassocDes.DeviceAddrMode == SHORT) && (m_attributes->macCoordShortAddress == m_disassocDes.DeviceAddr)))
		{//allow associating with the coordinator but not allocate the short address .
			SendDisassocCCA();
		}else
			NS_LOG_ERROR("Ignore LifiDisassocDevHandler::StartDisassoc");

	}

}

void LifiDisassocDevHandler::SendDisassocCCA(){
	NS_LOG_FUNCTION(this);
	DisassocNotificationComm disassoc;
	disassoc.SetDissassocReason(m_disassocDes.DisassociationReason);
	Ptr<Packet> p = disassoc.GetPacket();

	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetDstAddress(m_disassocDes.DeviceAddr);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	//the SrcAddress which gets in this way is Mac64Address? we need Mac64Address!
	header.SetAckRequest(true);
	header.SetDstVPANId(m_attributes->macVPANId);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_bust = false;
	info.m_force = false;
	info.m_handle = 0x34;
	info.m_isAck = false;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = true;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx =false;
	info.m_packet = p;

	m_trxHandler->Send(info);
	EnableTrigger(LifiDisassocDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocDevHandler::onTxResultNotification1);
}

void LifiDisassocDevHandler::ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDisassocDevHandler::ReceiveBeacon))
	{
		//must be associated ,than it can be disassociated!
		if((m_attributes->macShortAddress != 0xffff) && (m_attributes->macShortAddress != 0xfffe)){
			NS_LOG_FUNCTION (this << timestamp << msdu);
			onReceiveBeacon(timestamp, msdu);
		}else{
			NS_LOG_ERROR("Ignore LifiDisassocDevHandler::ReceiveBeacon");
		}
	}
	else{
		NS_LOG_ERROR("Ignore LifiDisassocDevHandler::ReceiveBeacon");
	}
}

void LifiDisassocDevHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << timestamp << p);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = p->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_BEACON);
	m_CoordAddress = header.GetSrcAddress();
	LifiMacBeacon beacon = LifiMacBeacon::Construct(tempPacket);
	if (beacon.CheckPendingAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress()))
	{
		DisableTrigger(LifiDisassocDevHandler::ReceiveBeacon);
		SendDataRequest();
	}
}

void LifiDisassocDevHandler::SendDataRequest(){
	NS_LOG_FUNCTION(this);
	DataRequestComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_CoordAddress);
	header.SetDstVPANId(m_attributes->macVPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_handle = 0x36;
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

	EnableTrigger(LifiDisassocDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocDevHandler::onTxResultNotification2);
}

void LifiDisassocDevHandler::AllocNotification(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiDisassocDevHandler::AllocNotification))
	{
		if (!m_allocNotification.IsNull())
				m_allocNotification (service);
		else
			NS_LOG_ERROR ("LifiDisassocDevHandler is null.");
	}else {
		NS_LOG_ERROR("Ignore LifiDisassocDevHandler::AllocNotification");
	}
}

void LifiDisassocDevHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);
	if (CheckTrigger(LifiDisassocDevHandler::TxResultNotification))
		{
			NS_LOG_FUNCTION (this << (uint32_t) status);
			if (!m_txRstNotification.IsNull())
				m_txRstNotification (status, info, ack);
			else
				NS_LOG_ERROR ("m_txRstNotification is null.");
		}
		else{
			NS_LOG_ERROR("Ignore LifiDisassocDevHandler::onTxRuesult");
		}
}

void LifiDisassocDevHandler::onAllocNotification1(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;

	m_receiveDisassocNotify_Indirect = true;
	EnableTrigger(LifiDisassocDevHandler::ReceiveDisassocNotification);

}

void LifiDisassocDevHandler::onAllocNotification2(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;

	SendAck();
}

void LifiDisassocDevHandler::ReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu){
	if (CheckTrigger(LifiDisassocDevHandler::ReceiveDisassocNotification))
	{
		NS_LOG_FUNCTION (this << timestamp << msdu);
		onReceiveDisassocNotification(timestamp, msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiDisassocDevHandler::ReceiveDisassocNotification");
	}
}

void LifiDisassocDevHandler::onReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION(this);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = msdu->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT(header.GetFrameType() == LIFI_COMMAND);
	DisableTrigger(LifiDisassocDevHandler::ReceiveDisassocNotification);
	DisassocNotificationComm disassocNotify = DisassocNotificationComm::Construct(tempPacket);
	m_disassocReason = disassocNotify.GetDissassocReason();
	NS_ASSERT(m_disassocReason == COORD);
	if(m_receiveDisassocNotify_Indirect){
		m_dataService->Release();
		m_dataService = 0;
	}
	if(header.GetSrcAddress() == m_attributes->macCoordExtendedAddress){
		TranceiverTask task;
		task.Clear();
		task.id = TX_ACK;
		task.listener = this;
		task.priority = GetTrxTaskPriority(TX_ACK);
		m_trxHandler->ServiceRequest(task);
		EnableTrigger(LifiDisassocDevHandler::AllocNotification);
		ReplaceTriggerCallback (m_allocNotification,
								LifiDisassocDevHandler::onAllocNotification2);
	}

}

void LifiDisassocDevHandler::SendAck(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_CoordAddress);
	header.SetFramePending(false);
	header.SetDstVPANId(m_attributes->macVPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_handle = 24;
	info.m_bust = false;
	info.m_force = true;
	info.m_listener = this;
	info.m_isAck = true;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = false;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
	m_dataService->Transmit(info);
	EnableTrigger(LifiDisassocDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocDevHandler::onTxResultNotification3);

}

void LifiDisassocDevHandler::onTxResultNotification1(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiDisassocDevHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == MAC_SUCCESS));
	if(status == NO_ACK || status == MAC_SUCCESS){
		m_user->MlmeDisassociateConfirm(MAC_SUCCESS, GetCurAddressMode(m_impl->GetLifiMac()->GetDevice()->GetAddress()),
										m_attributes->macVPANId, m_impl->GetLifiMac()->GetDevice()->GetAddress());
		Reset();
	}else
	{
		m_user->MlmeDisassociateConfirm(status, GetCurAddressMode(m_impl->GetLifiMac()->GetDevice()->GetAddress()),
										m_attributes->macVPANId, m_impl->GetLifiMac()->GetDevice()->GetAddress());
	}
}

void LifiDisassocDevHandler::onTxResultNotification2(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	if (status == MAC_SUCCESS)
	{
		DisableTrigger(LifiDisassocDevHandler::TxResultNotification);
		LifiMacHeader header;
		Ptr<Packet> tempPacket = ack->Copy();
		tempPacket->RemoveHeader(header);
		if (header.GetFramePending())
		{
			TranceiverTask task;
			task.Clear();
			task.id = RX_DISASSOC_NOTIFICATION;
			task.listener = this;
			task.priority = GetTrxTaskPriority(RX_DISASSOC_NOTIFICATION);

			EnableTrigger(LifiDisassocDevHandler::AllocNotification);
			ReplaceTriggerCallback (m_allocNotification,
									LifiDisassocDevHandler::onAllocNotification1);
			m_trxHandler->ServiceRequest(task);
		}
	}else
		NS_LOG_ERROR("Ignore LifiDisassocDevHandler::ReceiveBeacon.");

}

void LifiDisassocDevHandler::onTxResultNotification3(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	NS_ASSERT(info.m_isAck == true);
	DisableTrigger(LifiDisassocDevHandler::TxResultNotification);
	m_dataService->Release();
	m_dataService = 0;
	Reset();
	m_user->MlmeDisassociateIndication(Mac64Address::ConvertFrom(m_impl->GetLifiMac()->GetDevice()->GetAddress()), COORD);
}

void LifiDisassocDevHandler::Reset(){
	NS_LOG_FUNCTION(this);
	m_attributes->macVPANId = 0xffff;
	m_attributes->macShortAddress = 0xffff;
	m_attributes->macAssociatedVPANCoord = false;
	m_attributes->macCoordExtendedAddress = Mac64Address ("ff:ff:ff:ff:ff:ff:ff:ff");
	m_attributes->macCoordShortAddress = Mac16Address ("ff:ff");
}

void LifiDisassocDevHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

}
