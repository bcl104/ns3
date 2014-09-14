/*
 * lifi-data-dev-handler.cc
 *
 *  Created on: 2014年8月10日
 *      Author: wanghongqiao
 */

#include "lifi-data-dev-handler.h"
#include "ns3/log.h"
#include "lifi-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("LifiDataDevHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiDataDevHandler);

LifiDataDevHandler::LifiDataDevHandler() {
	NS_LOG_FUNCTION (this);
	m_curChannel = CHANNEL1;
	AddTrigger(LifiDataDevHandler::TxResultNotification, false);
	AddTrigger(LifiDataDevHandler::AllocNotification, false);
}

LifiDataDevHandler::~LifiDataDevHandler() {

}

TypeId LifiDataDevHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiDataDevHandler")
						.AddConstructor<LifiDataDevHandler> ()
						.SetParent<LifiDataHandler> ();
	return tid;
}

void LifiDataDevHandler::StartTransmit(DataDescriptor DataDesc){
	NS_LOG_FUNCTION(this);
	m_dataDesc = DataDesc;
	NS_ASSERT(m_dataDesc.Options.indirectTx == false);
	if(m_dataDesc.Options.gtsTx == true)
			SendToGtsTransaction();
	else SendToCCA();
}

void LifiDataDevHandler::SendToCCA() {
	NS_LOG_FUNCTION(this);
	Ptr<Packet> p = m_dataDesc.Msdu;
	LifiMacHeader header;
	header.SetFrameType(LIFI_DATA);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
//	std::cout << srcAddress << std::endl;
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_dataDesc.DstAddr);
	header.SetDstVPANId(m_dataDesc.DstVPANId);
	header.SetAckRequest(m_dataDesc.Options.ackTx);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_bust = false;
	info.m_handle = m_dataDesc.MsduHandle;
	info.m_force = false;
	info.m_listener = this;
	info.m_isAck = false;
	info.m_msduSize = m_dataDesc.MsduLenth;

	info.m_option.ackTx = m_dataDesc.Options.ackTx;
	info.m_option.gtsTx = m_dataDesc.Options.gtsTx;
	info.m_option.indirectTx = m_dataDesc.Options.indirectTx;
	info.m_packet = p;
	m_trxHandler->Send(info);

	EnableTrigger(LifiDataDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDataDevHandler::onTxResultNotification1);

}

void LifiDataDevHandler::SendToGtsTransaction(){
	NS_LOG_FUNCTION(this);
	Ptr<Packet> p = m_dataDesc.Msdu;
	LifiMacHeader header;
	header.SetFrameType(LIFI_DATA);
	Address srcAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
//	std::cout << srcAddress << std::endl;
	header.SetSrcAddress(srcAddress);
	header.SetDstAddress(m_dataDesc.DstAddr);
	header.SetDstVPANId(m_dataDesc.DstVPANId);
	header.SetAckRequest(m_dataDesc.Options.ackTx);
	p->AddHeader(header);

	GtsTransactionInfo gtsTransInfo;
	gtsTransInfo.m_ShortAddress = m_attributes->macShortAddress;
	gtsTransInfo.m_packetInfo.m_band = m_curChannel;
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

void LifiDataDevHandler::AllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION(this << service);
	if(CheckTrigger(LifiDataDevHandler::AllocNotification)){
		onAllocNotification(service);
	}else{
		NS_LOG_ERROR("igore LifiDataDevHandler::AllocNotification.");
	}
}

void LifiDataDevHandler::onAllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION(this << service);
	NS_ASSERT(service != 0);
	m_dataService = service;

	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_receiveCoordAddr);
	header.SetFramePending(false);

	std::cout << m_impl->GetLifiMac()->GetDevice()->GetAddress() <<std::endl;
	std::cout << m_receiveCoordAddr <<std::endl;
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
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

	EnableTrigger(LifiDataDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiDataDevHandler::onTxResultNotification1);

}

void LifiDataDevHandler::TxResultNotification(MacOpStatus status,
			          PacketInfo info, Ptr<Packet> ack) {
	if (CheckTrigger(LifiDataDevHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiDataDevHandler::onTxRuesult");
	}
}

void LifiDataDevHandler::ReceiveData(uint32_t timestamp, Ptr<Packet> msdu){
	if(CheckTrigger(LifiDataDevHandler::ReceiveData)){
		NS_LOG_FUNCTION(this << timestamp << msdu);
			onReceiveData(timestamp, msdu);
	}else{
		NS_LOG_ERROR("Ignore LifiDataDevHandler::ReceiveData");
	}
//	if(CheckTrigger(LifiDataDevHandler::ReceiveData)){
//		NS_LOG_FUNCTION(this << timestamp << msdu);
//		LifiMacHeader header;
//		msdu->PeekHeader(header);
//		if(header.GetDstVPANId() == m_attributes->macVPANId){
//			onReceiveData(timestamp, msdu);
//		}else{
//			NS_LOG_ERROR("Not this VPAN packet,Ignore LifiDataDevHandler::ReceiveData.");
//		}
//	}else{
//		NS_LOG_ERROR("Ignore LifiDataDevHandler::ReceiveData");
//	}
}

void LifiDataDevHandler::onReceiveData(uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION(this);
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

	m_receiveCoordAddr = header.GetSrcAddress();

	if(header.GetAckRequest()){
		TranceiverTask task;
		task.Clear();
		task.id = TX_ACK;
		task.listener = this;
		task.priority = GetTrxTaskPriority(TX_ACK);
		m_trxHandler->ServiceRequest(task);
		EnableTrigger(LifiDataDevHandler::AllocNotification);
	}
//	    m_user->MlmeDataIndication(m_dataIndicaDes);
}

void LifiDataDevHandler::onTxResultNotification1(MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	DisableTrigger(LifiDataDevHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == MAC_SUCCESS));
//	m_user->MlmeDataConfirm(info.m_handle, status, 0);
	m_dataService->Release();
	m_dataService = 0;

	EnableTrigger(LifiDataHandler::TxResultNotification);
}

void LifiDataDevHandler::onTxResultNotification2 (MacOpStatus status,
		PacketInfo info, Ptr<Packet> ack) {
	NS_LOG_FUNCTION (this << status);
	DisableTrigger(LifiDataDevHandler::TxResultNotification);
	NS_ASSERT ((status == CHANNEL_ACCESS_FAILURE)
			 ||(status == NO_ACK)
			 ||(status == MAC_SUCCESS));
//	m_user->MlmeDataConfirm(info.m_handle, status, 0);

	EnableTrigger(LifiDataHandler::TxResultNotification);
}

void LifiDataDevHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

} /* namespace ns3 */
