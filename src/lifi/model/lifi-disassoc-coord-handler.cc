/*
 * lifi-disassoc-coord-handler.cc
 *
 *  Created on: 2014年8月15日
 *      Author: wanghongqiao
 */
#include "lifi-disassoc-coord-handler.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"

NS_LOG_COMPONENT_DEFINE ("LifiDisassocCoordHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDisassocCoordHandler);

LifiDisassocCoordHandler::LifiDisassocCoordHandler() {
	NS_LOG_FUNCTION(this);
//	m_CoordAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
//	m_impl = 0,"m_impl" is a NULL pointer,we set this parameter after the "m_impl" has been set.
	AddTrigger(LifiDisassocCoordHandler::AllocNotification, false);
	AddTrigger(LifiDisassocCoordHandler::TxResultNotification, false);
	AddTrigger(LifiDisassocCoordHandler::ReceiveDisassocNotification, true);
	AddTrigger(LifiDisassocCoordHandler::ReceiveDataRequest, false);
}

LifiDisassocCoordHandler::~LifiDisassocCoordHandler() {
}

TypeId LifiDisassocCoordHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDisassocCoordHandler")
								.AddConstructor<LifiDisassocCoordHandler> ()
								.SetParent<LifiDisassocHandler> ();
	return tid;
}

void LifiDisassocCoordHandler::StartDisassoc(DisassocDescriptor disassocDes){
	NS_LOG_FUNCTION(this);
	m_disassocDes = disassocDes;
	if(m_attributes->macVPANId != m_disassocDes.DeviceVPANId){
		m_user->MlmeDisassociateConfirm(MAC_INVALID_PARAMETER, GetCurAddressMode(m_disassocDes.DeviceAddr),
										m_attributes->macVPANId, m_disassocDes.DeviceAddr);
	}else{
		if(((m_disassocDes.DeviceAddrMode == 0x03) && (m_attributes->macCoordExtendedAddress != m_disassocDes.DeviceAddr))
				||((m_disassocDes.DeviceAddrMode == 0x02) && (m_attributes->macCoordShortAddress != m_disassocDes.DeviceAddr)))
		{
			if(m_disassocDes.TxIndirect == true){
				SendToIndirectTransaction();
			}
			else{
				SendToCCA();
			}
		}else
			NS_LOG_ERROR("Ignore LifiDisassocDevHandler::StartDisassoc");

	}

}

void LifiDisassocCoordHandler::SendToCCA(){
	NS_LOG_FUNCTION(this);
	DisassocNotificationComm disassoc;
	disassoc.SetDissassocReason(m_disassocDes.DisassociationReason);
	Ptr<Packet> p = disassoc.GetPacket();

	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetDstAddress(m_disassocDes.DeviceAddr);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());

	//the SrcAddress which gets in this way is Mac64Address?we need Mac64Address!

	header.SetAckRequest(true);
	header.SetDstVPANId(m_attributes->macVPANId);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_bust = false;
	info.m_handle = 0x24;
	info.m_force = false;
	info.m_listener = this;
	info.m_isAck = false;
	info.m_msduSize = p->GetSize();

	info.m_option.ackTx = true;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;

	m_trxHandler->Send(info);
	EnableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocCoordHandler::onTxResultNotification2);
}

void LifiDisassocCoordHandler::SendToIndirectTransaction(){
	NS_LOG_FUNCTION(this);
	DisassocNotificationComm disassoc;
	disassoc.SetDissassocReason(m_disassocDes.DisassociationReason);
	Ptr<Packet> p = disassoc.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetDstAddress(m_disassocDes.DeviceAddr);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());

	//the SrcAddress which gets in this way is Mac64Address?we need Mac64Address!

	header.SetAckRequest(true);
	header.SetDstVPANId(m_attributes->macVPANId);

	p->AddHeader(header);

	TransactionInfo transInfo;
	transInfo.m_extendDevAddress = Mac64Address::ConvertFrom(m_disassocDes.DeviceAddr);
	transInfo.m_packetInfo.m_band = m_trxHandler->GetChannelId();
	transInfo.m_packetInfo.m_packet = p;
	transInfo.m_packetInfo.m_listener = this;
	transInfo.m_packetInfo.m_bust = false;
	transInfo.m_packetInfo.m_force = true;
	transInfo.m_packetInfo.m_handle = 0x55;
	transInfo.m_packetInfo.m_msduSize = p->GetSize();

	transInfo.m_packetInfo.m_option.ackTx = true;
	transInfo.m_packetInfo.m_option.gtsTx = false;
	transInfo.m_packetInfo.m_option.indirectTx = true;
	transInfo.m_listener = this;

	m_impl->AddTransactionPacket(transInfo);
	EnableTrigger(LifiDisassocCoordHandler::ReceiveDataRequest);
//	we have no return information, for example ,the transaction capacity is full.
}

void LifiDisassocCoordHandler::ReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	if (CheckTrigger(LifiDisassocCoordHandler::ReceiveDataRequest))
	{
		onReceiveDataRequest(timestamp, msdu);
	}else
	{
		NS_LOG_WARN("Ignore LifiDisassocCoordHandler::ReceiveDataRequest.");
	}
}

void LifiDisassocCoordHandler::onReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);

	LifiMacHeader header;
	msdu->PeekHeader(header);
	NS_ASSERT (header.GetSrcAddressMode() == EXTENDED);
	DisableTrigger(LifiDisassocCoordHandler::ReceiveDataRequest);
	m_curDeviceAddress_T = Mac64Address::ConvertFrom(header.GetSrcAddress());
	NS_ASSERT(header.GetDstAddress() == m_impl->GetLifiMac()->GetDevice()->GetAddress());

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiDisassocCoordHandler::AllocNotification);
	ReplaceTriggerCallback (m_allocNotification,
							LifiDisassocCoordHandler::onAllocNotification2);
}

void LifiDisassocCoordHandler::ReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION(this << timestamp << msdu);
	if (CheckTrigger(LifiDisassocCoordHandler::ReceiveDisassocNotification))
	{
		onReceiveDisassocNotification(timestamp, msdu);
	}
	else{
		NS_LOG_ERROR("Ignore LifiDisassocCoordHandler::ReceiveDisassocNotification");
	}
}

void LifiDisassocCoordHandler::onReceiveDisassocNotification(uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION(this);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = msdu->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT(header.GetFrameType() == LIFI_COMMAND);
	m_curDeviceAddress_R = header.GetSrcAddress();
	DisassocNotificationComm disassocNotify = DisassocNotificationComm::Construct(tempPacket);
	m_disassocReason = disassocNotify.GetDissassocReason();
	NS_ASSERT(m_disassocReason == DEVICE);

	if(header.GetSrcAddress() != m_attributes->macCoordExtendedAddress){

////	verify that the source address corresponds to its associated device//
////		consider the device disassociated .                            //
////		how to coding ? this layer or the next upper layer ?           //

		DisableTrigger(LifiDisassocCoordHandler::ReceiveDisassocNotification);

		TranceiverTask task;
		task.Clear();
		task.id = TX_ACK;
		task.listener = this;
		task.priority = GetTrxTaskPriority(TX_ACK);
		m_trxHandler->ServiceRequest(task);
		EnableTrigger(LifiDisassocCoordHandler::AllocNotification);
		ReplaceTriggerCallback (m_allocNotification,
								LifiDisassocCoordHandler::onAllocNotification1);
	}
}

void LifiDisassocCoordHandler::AllocNotification(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiDisassocCoordHandler::AllocNotification))
	{
		if (!m_allocNotification.IsNull())
				m_allocNotification (service);
		else
			NS_LOG_ERROR ("LifiDisassocCoordHandler is null.");
	}else {
		NS_LOG_ERROR("Ignore LifiDisassocCoordHandler::AllocNotification");
	}
}

void LifiDisassocCoordHandler::onAllocNotification1(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;

	SendAck1();
}

void LifiDisassocCoordHandler::onAllocNotification2(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;

	SendAck2();
}

void LifiDisassocCoordHandler::SendAck1(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_curDeviceAddress_R);
	header.SetFramePending(false);
	header.SetDstVPANId(m_attributes->macVPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_handle = 0x27;
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

	EnableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
			                LifiDisassocCoordHandler::onTxResultNotification1);
}

void LifiDisassocCoordHandler::SendAck2(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_curDeviceAddress_T);
	header.SetFramePending(true);
	header.SetDstVPANId(m_attributes->macVPANId);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_handle = 0x28;
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
	EnableTrigger(LifiDisassocCoordHandler::ReceiveDataRequest);
	EnableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocCoordHandler::onTxResultNotification3);
}

void LifiDisassocCoordHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	if (CheckTrigger(LifiDisassocCoordHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDisassocCoordHandler::onTxRuesult");
	}
}

void LifiDisassocCoordHandler::onTxResultNotification1(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	NS_ASSERT(info.m_isAck == true);
	DisableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	m_dataService->Release();
	m_dataService = 0;

	EnableTrigger(LifiDisassocCoordHandler::ReceiveDisassocNotification);

	m_user->MlmeDisassociateIndication(Mac64Address::ConvertFrom(m_curDeviceAddress_R), DEVICE);
}

void LifiDisassocCoordHandler::onTxResultNotification2(MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == MAC_SUCCESS));
	if(status == NO_ACK || status == MAC_SUCCESS){
		m_user->MlmeDisassociateConfirm(MAC_SUCCESS, GetCurAddressMode(m_disassocDes.DeviceAddr),
										m_attributes->macVPANId, m_disassocDes.DeviceAddr);
	}else
	{
		m_user->MlmeDisassociateConfirm(status, GetCurAddressMode(m_disassocDes.DeviceAddr),
										m_attributes->macVPANId, m_disassocDes.DeviceAddr);
	}
}

void LifiDisassocCoordHandler::onTxResultNotification3(MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this);
	m_dataService->Release();
	m_dataService = 0;
	DisableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	m_impl->PetchTransactionPacket(Mac64Address::ConvertFrom(m_curDeviceAddress_T));
	EnableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDisassocCoordHandler::onTxResultNotification4);
}

void LifiDisassocCoordHandler::onTxResultNotification4 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiDisassocCoordHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == MAC_SUCCESS));
	if(status == NO_ACK || status == MAC_SUCCESS){
		m_user->MlmeDisassociateConfirm(MAC_SUCCESS, GetCurAddressMode(m_curDeviceAddress_T),
										m_attributes->macVPANId, m_curDeviceAddress_T);
	}else
	{
		m_user->MlmeDisassociateConfirm(status, GetCurAddressMode(m_curDeviceAddress_T),
										m_attributes->macVPANId, m_curDeviceAddress_T);
	}
}

void LifiDisassocCoordHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

}
