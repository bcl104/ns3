/*
 * lifi-data-coord-handler.cc
 *
 *  Created on: 2014年8月10日
 *      Author: wanghongqiao
 */

#include "lifi-data-coord-handler.h"
#include "ns3/log.h"
#include "lifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("LifiDataCoordHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDataCoordHandler);

LifiDataCoordHandler::LifiDataCoordHandler() {
	NS_LOG_FUNCTION (this);
	m_curDeviceAddress = Mac64Address("ff:ff:ff:ff:ff:ff:ff:ff");
	AddTrigger(LifiDataCoordHandler::TxResultNotification, false);
	AddTrigger(LifiDataCoordHandler::AllocNotification, false);
	AddTrigger(LifiDataCoordHandler::ReceiveData, true);
}

LifiDataCoordHandler::~LifiDataCoordHandler() {

}

TypeId LifiDataCoordHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDataCoordHandler")
						.SetParent<LifiDataHandler> ()
						.AddConstructor<LifiDataCoordHandler> ();
	return tid;
}

void LifiDataCoordHandler::StartTransmit(DataDescriptor DataDesc){
	NS_LOG_FUNCTION(this);
	m_dataDesc = DataDesc;
	if(m_dataDesc.Options.gtsTx == true)
		SendToGtsTransaction();
	else if(m_dataDesc.Options.indirectTx == true)
		   	   SendToIndirectTransaction();
		 else SendToCCA();
}

void LifiDataCoordHandler::SendToGtsTransaction() {
	Ptr<Packet> p = m_dataDesc.Msdu;
	LifiMacHeader header;
	header.SetFrameType(LIFI_DATA);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_dataDesc.DstAddr);
	header.SetDstVPANId(m_dataDesc.DstVPANId);
	header.SetAckRequest(m_dataDesc.Options.ackTx);
	p->AddHeader(header);

	GtsTransactionInfo gtsTransInfo;
	gtsTransInfo.m_ShortAddress = m_attributes->macShortAddress;
	gtsTransInfo.m_packetInfo.m_band = m_trxHandler->GetChannelId();
	gtsTransInfo.m_packetInfo.m_packet = p;
	gtsTransInfo.m_packetInfo.m_listener = this;
	gtsTransInfo.m_packetInfo.m_bust = false;
	gtsTransInfo.m_packetInfo.m_force = true;
	gtsTransInfo.m_packetInfo.m_msduSize = m_dataDesc.MsduLenth;
	gtsTransInfo.m_packetInfo.m_handle = m_dataDesc.MsduHandle;

	NS_ASSERT(m_dataDesc.Options.gtsTx == true);
	NS_ASSERT(m_dataDesc.Options.indirectTx == false);

	gtsTransInfo.m_packetInfo.m_option.ackTx = m_dataDesc.Options.ackTx;
	gtsTransInfo.m_packetInfo.m_option.gtsTx = m_dataDesc.Options.gtsTx;
	gtsTransInfo.m_packetInfo.m_option.indirectTx = m_dataDesc.Options.indirectTx;
	gtsTransInfo.m_listener = this;

	m_impl->AddGtsTransactionPacket(gtsTransInfo);
}

void LifiDataCoordHandler::SendToIndirectTransaction() {
	Ptr<Packet> p = m_dataDesc.Msdu;
	LifiMacHeader header;
	header.SetFrameType(LIFI_DATA);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_dataDesc.DstAddr);
	header.SetDstVPANId(m_dataDesc.DstVPANId);
	header.SetAckRequest(m_dataDesc.Options.ackTx);
	p->AddHeader(header);

	TransactionInfo transInfo;
	transInfo.m_extendDevAddress = Mac64Address::ConvertFrom(srcAddress);
	transInfo.m_packetInfo.m_band = m_trxHandler->GetChannelId();
	transInfo.m_packetInfo.m_packet = p;
	transInfo.m_packetInfo.m_listener = this;
	transInfo.m_packetInfo.m_bust = false;
	transInfo.m_packetInfo.m_force = true;
	transInfo.m_packetInfo.m_msduSize = m_dataDesc.MsduLenth;
	transInfo.m_packetInfo.m_handle = m_dataDesc.MsduHandle;

	NS_ASSERT(m_dataDesc.Options.gtsTx == false);
	NS_ASSERT(m_dataDesc.Options.indirectTx == true);

	transInfo.m_packetInfo.m_option.ackTx = m_dataDesc.Options.ackTx;
	transInfo.m_packetInfo.m_option.gtsTx = m_dataDesc.Options.gtsTx;
	transInfo.m_packetInfo.m_option.indirectTx = m_dataDesc.Options.indirectTx;
	transInfo.m_listener = this;

	m_impl->AddTransactionPacket(transInfo);
	EnableTrigger(LifiDataCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDataCoordHandler::onTxResultNotification2);

}

void LifiDataCoordHandler::SendToCCA() {
	NS_LOG_FUNCTION(this);
	Ptr<Packet> p = m_dataDesc.Msdu;
	LifiMacHeader header;
	header.SetFrameType(LIFI_DATA);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_dataDesc.DstAddr);
	header.SetDstVPANId(m_dataDesc.DstVPANId);
	header.SetAckRequest(m_dataDesc.Options.ackTx);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_bust = false;
	info.m_handle = m_dataDesc.MsduHandle;
	info.m_force = false;
	info.m_listener = this;
	info.m_isAck = false;
	info.m_msduSize = m_dataDesc.MsduLenth;

	NS_ASSERT(m_dataDesc.Options.gtsTx == false);
	NS_ASSERT(m_dataDesc.Options.indirectTx == false);

	info.m_option.ackTx = m_dataDesc.Options.ackTx;
	info.m_option.gtsTx = m_dataDesc.Options.gtsTx;
	info.m_option.indirectTx = m_dataDesc.Options.indirectTx;
	info.m_packet = p;
	m_trxHandler->Send(info);

	EnableTrigger(LifiDataCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDataCoordHandler::onTxResultNotification2);

}

void LifiDataCoordHandler::TxResultNotification(MacOpStatus status,
			          PacketInfo info, Ptr<Packet> ack) {
	if (CheckTrigger(LifiDataCoordHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDataCoordHandler::onTxRuesult");
	}
}

void LifiDataCoordHandler::ReceiveData(uint32_t timestamp, Ptr<Packet> msdu){
	if(CheckTrigger(LifiDataCoordHandler::ReceiveData)){
		NS_LOG_FUNCTION(this << timestamp << msdu);
		onReceiveData(timestamp, msdu);
	}else{
		NS_LOG_ERROR("Ignore LifiDataDevHandler::ReceiveData");
	}
}

void LifiDataCoordHandler::onReceiveData(uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiDataCoordHandler::ReceiveData);
	LifiMacHeader header;
	msdu->PeekHeader(header);
	m_dataIndicaDes.DstAddrMode = header.GetDstAddressMode();
	m_dataIndicaDes.SrcAddrMode = header.GetSrcAddressMode();
	m_dataIndicaDes.DstVPANId = header.GetDstVPANId();
	m_dataIndicaDes.SrcVPANId = header.GetSrcVPANId();
	m_dataIndicaDes.DstAddr = header.GetDstAddress();
	m_dataIndicaDes.SrcAddr = header.GetSrcAddress();
	m_dataIndicaDes.DSN = header.GetSequenceNumber();
	m_dataIndicaDes.TimeStamp = timestamp;
	m_dataIndicaDes.Msdu = msdu;
	m_dataIndicaDes.MsduLenth = msdu->GetSize();

	if(header.GetAckRequest()){
		TranceiverTask task;
		task.Clear();
		task.id = TX_ACK;
		task.listener = this;
		task.priority = GetTrxTaskPriority(TX_ACK);
		m_trxHandler->ServiceRequest(task);
		EnableTrigger(LifiDataCoordHandler::AllocNotification);
	}
}

void LifiDataCoordHandler::AllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION(this << service);
	if(CheckTrigger(LifiDataCoordHandler::AllocNotification)){
		onAllocNotification(service);
	}else{
		NS_LOG_ERROR("Igore LifiDataCoordHandler::AllocNotification.");
	}
}

void LifiDataCoordHandler::onAllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION(this << service);
	NS_ASSERT(service != 0);
	m_dataService = service;

	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_dataIndicaDes.SrcAddr);
	header.SetFramePending(false);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_trxHandler->GetChannelId();
	info.m_handle = 0x25;
	info.m_bust = false;
	info.m_force = true;
	info.m_isAck = true;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = false;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;

	m_dataService->Transmit(info);
	EnableTrigger(LifiDataCoordHandler::ReceiveData);
	EnableTrigger(LifiDataCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDataCoordHandler::onTxResultNotification1);
}

void LifiDataCoordHandler::onTxResultNotification1(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	DisableTrigger(LifiDataCoordHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == MAC_SUCCESS));
	m_dataService->Release();
	m_dataService = 0;

//	m_user->MlmeDataIndication(m_dataIndicaDes);

	EnableTrigger(LifiDataCoordHandler::TxResultNotification);
}

void LifiDataCoordHandler::onTxResultNotification2(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	DisableTrigger(LifiDataCoordHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == TRANSACTION_OVERFLOW)
			 ||(status == TRANSACTION_EXPIRED)
			 ||(status == MAC_SUCCESS));

//	m_user->MlmeDataConfirm(info.m_handle, status, 0);

	EnableTrigger(LifiDataCoordHandler::TxResultNotification);
}

void LifiDataCoordHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

} /* namespace ns3 */
