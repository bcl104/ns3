/*
 * lifi-coord-assoc-handler.cc
 *
 *  Created on: 2014年4月28日
 *      Author: will
 */

#include "lifi-coord-assoc-handler.h"
//#include "lifi-mac-general.h"
#include "lifi-transaction-handler.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiCoordAssocHandler");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(LifiCoordAssocHandler);

LifiCoordAssocHandler::LifiCoordAssocHandler()
{
	NS_LOG_FUNCTION (this);
	m_AckState = SEND_ACK1;
	m_curChannel = CHANNEL1;//how to get the current channel?
//	NS_FATAL_ERROR("Unavailable instantiation with this constructor.");
	AddTrigger(LifiCoordAssocHandler::AllocNotification, false);
	AddTrigger(LifiCoordAssocHandler::ReceiveAssocRequest, true);
	AddTrigger(LifiCoordAssocHandler::ReceiveDataRequest, false);
	AddTrigger(LifiCoordAssocHandler::MlmeAssocResponse,true);
	AddTrigger(LifiCoordAssocHandler::TxResultNotification, false);
}

LifiCoordAssocHandler::LifiCoordAssocHandler(DataService* service,LifiMacImpl* impl,
					 PlmeSapProvider* p, LifiMacPibAttribute* a, MlmeSapUser* user)
{
	NS_LOG_FUNCTION (this);
	m_dataService = service;
	m_user = user;
	m_plmeProvider = p;
	m_impl = impl;
	m_AckState = SEND_ACK1;
	m_coordAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
	AddTrigger(LifiCoordAssocHandler::AllocNotification, false);
	AddTrigger(LifiCoordAssocHandler::ReceiveAssocRequest, true);
	AddTrigger(LifiCoordAssocHandler::ReceiveDataRequest, false);
	AddTrigger(LifiCoordAssocHandler::MlmeAssocResponse,true);
	AddTrigger(LifiCoordAssocHandler::TxResultNotification, false);
}

LifiCoordAssocHandler::~LifiCoordAssocHandler()
{
	NS_LOG_FUNCTION (this);
}

TypeId LifiCoordAssocHandler::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::LifiCoordAssocHandler")
						.AddConstructor<LifiCoordAssocHandler> ()
						.SetParent<LifiAssocHandler> ();
	return tid;
}

void LifiCoordAssocHandler::ReceiveAssocRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	if (CheckTrigger(LifiCoordAssocHandler::ReceiveAssocRequest))
	{
		onReceiveAssocRequest(timestamp, msdu);
	}else
	{
		NS_LOG_WARN("Ignore LifiCoordAssocHandler::ReceiveAssocRequest.");
	}
}

void LifiCoordAssocHandler::onReceiveAssocRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);

//	m_temp->printffff();

	LifiMacHeader header;
	Ptr<Packet> tempPacket = msdu->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_COMMAND);
	AssocRqstComm comm = AssocRqstComm::Construct(tempPacket);
	NS_ASSERT (comm.GetCommId() == ASSOC_REQUEST);
	DisableTrigger(LifiCoordAssocHandler::ReceiveAssocRequest);  //occupation sometime
	m_curDeviceAddress = Mac64Address::ConvertFrom(header.GetSrcAddress());
	m_srcAddrMode = header.GetSrcAddressMode();
	m_dstAddrMode = header.GetDstAddressMode();//assign coordinatorAddress to shortAddress
//	std::cout << m_impl->GetLifiMac()->GetDevice()->GetAddress() << std::endl;
//	std::cout << header.GetDstAddress() << std::endl;
//	std::cout << m_curDeviceAddress << std::endl;
	m_coordAddress = m_impl->GetLifiMac()->GetDevice()->GetAddress();
//	std::cout << m_coordAddress << std::endl;
	NS_ASSERT(header.GetDstAddress() == m_impl->GetLifiMac()->GetDevice()->GetAddress());
												//local device associate to local coordinator.
	m_vpanId = header.GetDstVPANId();

 	m_AckState = SEND_ACK1;

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);

	EnableTrigger(LifiCoordAssocHandler::AllocNotification);
	m_trxHandler->ServiceRequest(task);
}

void LifiCoordAssocHandler::sendAck1() {
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_coordAddress);
	header.SetDstAddress(m_curDeviceAddress);
	header.SetFramePending(false);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_handle = 0x25;
	info.m_bust = false;
	info.m_force = true;
	info.m_isAck = true;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	std::cout << info.m_msduSize << std::endl;
	info.m_option.ackTx = false;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
//	std::cout << (info.m_band !=0) << std::endl;
//	std::cout << (info.m_handle != 0) << std::endl;
//	std::cout << (info.m_msduSize != 0) << std::endl;
//	std::cout << (info.m_packet !=0) << std::endl;

	m_dataService->Transmit(info);
}

void LifiCoordAssocHandler::sendAck2() {
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetAckRequest(false);
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_coordAddress);
	header.SetDstAddress(m_curDeviceAddress);
	header.SetFramePending(true);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_handle = 0x25;
	info.m_bust = false;
	info.m_force = true;
	info.m_isAck = true;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	std::cout << info.m_msduSize << std::endl;
	info.m_option.ackTx = false;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;
//	std::cout << (info.m_band !=0) << std::endl;
//	std::cout << (info.m_handle != 0) << std::endl;
//	std::cout << (info.m_msduSize != 0) << std::endl;
//	std::cout << (info.m_packet !=0) << std::endl;

	m_dataService->Transmit(info);
}

void LifiCoordAssocHandler::MlmeAssocResponse(Mac64Address devAddress,
		Mac16Address allocAssocShortAddr, MacOpStatus status)
{
	NS_LOG_FUNCTION (this << devAddress << allocAssocShortAddr << status);
	AssocResponseComm rescom;
//	Mac16Address address = Mac16Address::ConvertFrom(allocAssocShortAddr);
	rescom.SetShortAddr(allocAssocShortAddr);
	rescom.SetAssocStatus(status);//not set capability negotiation response
	std::cout << rescom.GetAssocStatus() << std::endl;
	Ptr<Packet> p = rescom.GetPacket();

	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetAckRequest(true);
	header.SetDstAddress(m_curDeviceAddress);
	header.SetSrcAddress(m_coordAddress);
	//how to get its extendedAddress/shortAddress.
	p->AddHeader(header);

	TransactionInfo transInfo;
	transInfo.m_extendDevAddress = m_curDeviceAddress;
	transInfo.m_packetInfo.m_band = m_curChannel;
	transInfo.m_packetInfo.m_packet = p;
	transInfo.m_packetInfo.m_listener = this;
	transInfo.m_packetInfo.m_bust = false;
	transInfo.m_packetInfo.m_force = true;
	transInfo.m_packetInfo.m_msduSize = p->GetSize();
	transInfo.m_packetInfo.m_option.ackTx = true;
	transInfo.m_packetInfo.m_option.gtsTx = false;
	transInfo.m_packetInfo.m_option.indirectTx = true;
	transInfo.m_listener = this;

//	LifiMacHeader headerer;
//	transInfo.m_packetInfo.m_packet->RemoveHeader(headerer);
//	AssocResponseComm response = AssocResponseComm::Construct(transInfo.m_packetInfo.m_packet);
//	std::cout << response.GetAssocStatus() << std::endl;
//	std::cout << response.GetShortAddr() << std::endl;
//	m_temp->AddTransaction(transInfo);
//	m_coordImpl->m_transcHandler->AddTransaction(p);
	m_impl->AddTransactionPacket(transInfo);//transctionHandler need the info of this packet and return it.
}

void LifiCoordAssocHandler::ReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	if (CheckTrigger(LifiCoordAssocHandler::ReceiveDataRequest))
	{
		onReceiveDataRequest(timestamp, msdu);
	}else
	{
		NS_LOG_WARN("Ignore LifiCoordAssocHandler::ReceiveDataRequest.");
	}
}

void LifiCoordAssocHandler::onReceiveDataRequest(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION (this << timestamp << msdu);
	LifiMacHeader header;
	msdu->PeekHeader(header);
	NS_ASSERT (header.GetSrcAddressMode() == EXTENDED);
	m_curDeviceAddress = Mac64Address::ConvertFrom(header.GetSrcAddress());
	m_srcAddrMode = header.GetSrcAddressMode();
	m_dstAddrMode = header.GetDstAddressMode();
	NS_ASSERT(header.GetDstAddress() == m_impl->GetLifiMac()->GetDevice()->GetAddress());
	m_vpanId = header.GetDstVPANId();

	m_AckState = SEND_ACK2;

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiCoordAssocHandler::AllocNotification);
}

void LifiCoordAssocHandler::AllocNotification (Ptr<DataService> service)
{
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiCoordAssocHandler::AllocNotification))
	{
		onAllocNotification(service);
	}else {

		NS_LOG_ERROR("Ignore LifiDevAssocHandler::AllocNotification");
	}
}


void LifiCoordAssocHandler::onAllocNotification(Ptr<DataService> service) {
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiCoordAssocHandler::AllocNotification);

	NS_ASSERT(m_AckState == SEND_ACK1 || m_AckState == SEND_ACK2);
	EnableTrigger(LifiCoordAssocHandler::TxResultNotification);
	if(m_AckState == SEND_ACK1){
		sendAck1();//rewrite the info of the device.
		MlmeAssocResponse(m_curDeviceAddress, Mac16Address("12:34"), MAC_SUCCESS);
		EnableTrigger(LifiCoordAssocHandler::ReceiveDataRequest);
//		m_user->MlmeAssociateIndication(m_curDeviceAddress, m_capInfo);
	}else{
		sendAck2();
//		uint8_t mcsid = m_impl->GetLifiMac()->GetPlmeSapProvider()->PlmeGetRequset<uint8_t>(PHY_MCSID);
//		double dataRateKbps = LifiPhy::GetRate(mcsid);
//		Time AcktxDuration = NanoSeconds(((double)m_AckPacketSize*8)/(dataRateKbps*1000)*1e9);
//		Simulator::Schedule(AcktxDuration, &LifiCoordAssocHandler::getTransctionPacket, this, m_curDeviceAddress);
		m_impl->PetchTransactionPacket(m_curDeviceAddress);

	}

}

void LifiCoordAssocHandler::TxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack)
{
	NS_LOG_FUNCTION(this);
	if(CheckTrigger(LifiCoordAssocHandler::TxResultNotification))
	{
		onTxResultNotification(status, info, ack);
	}else{
		NS_LOG_WARN("ignore LifiCoordAssocHandler::TxResultNotification.");
	}
}

void LifiCoordAssocHandler::onTxResultNotification(MacOpStatus status, PacketInfo info, Ptr<Packet> ack) {//para needs modified.
	NS_LOG_FUNCTION(this);
	NS_ASSERT(status == MAC_SUCCESS
		   || status == TRANSACTION_OVERFLOW
		   || status == TRANSACTION_EXPIRED
		   || status == NO_ACK
		   || status == CHANNEL_ACCESS_FAILURE);
	if(info.m_isAck){
		// we can do retransmission in the future.
		m_dataService->Release();
		m_dataService = 0;
	}
	else{
		DisableTrigger(LifiCoordAssocHandler::TxResultNotification);
//		m_user->MlmeCommStatusIndication(m_vpanId, m_srcAddrMode, m_curDeviceAddress,
//										 m_dstAddrMode, m_coordAddress, status);
	}
	reset();
//	m_dataService->Release();
//	m_dataService = 0;
//	reset();

}

void LifiCoordAssocHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

void LifiCoordAssocHandler::getTransctionPacket(Mac64Address devAddress) {
	NS_LOG_FUNCTION(this);
	m_impl->PetchTransactionPacket(m_curDeviceAddress);
}

void LifiCoordAssocHandler::reset() {
	NS_LOG_FUNCTION(this);
	m_srcAddrMode = ERROR;
	m_dstAddrMode = ERROR;
	m_vpanId = 0;
	m_curDeviceAddress = Mac64Address ("ff:ff:ff:ff:ff:ff:ff:ff");
}

} /* namespace ns3 */
