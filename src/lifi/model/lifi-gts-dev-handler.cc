/*
 * lifi-gts-dev-handler.cc
 *
 *  Created on: 2014年8月19日
 *      Author: wanghongqiao
 */

#include "lifi-gts-dev-handler.h"
#include "ns3/log.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-header.h"
#include "lifi-mac-beacon.h"

NS_LOG_COMPONENT_DEFINE ("LifiGtsDevHandler");

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED (LifiGtsDevHandler);

LifiGtsDevHandler::LifiGtsDevHandler() {
	m_curChannel = CHANNEL1;
	m_gtsTransmitState = 1;
	AddTrigger(LifiGtsDevHandler::AllocNotification, false);
	AddTrigger(LifiGtsDevHandler::TxResultNotification, false);
	AddTrigger(LifiGtsDevHandler::ReceiveBeacon, true);
	AddTrigger(LifiGtsDevHandler::ReceiveData, false);
}

LifiGtsDevHandler::~LifiGtsDevHandler() {
}

TypeId LifiGtsDevHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiGtsDevHandler")
						.SetParent<LifiGtsHandler> ()
						.AddConstructor<LifiGtsDevHandler> ();
	return tid;

}

void LifiGtsDevHandler::OpenGtsDataReceive(uint16_t devAddr){
	NS_LOG_FUNCTION(this);
	EnableTrigger(LifiGtsDevHandler::ReceiveData);
}

void LifiGtsDevHandler::CloseGtsDataReceive(){
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiGtsDevHandler::ReceiveData);
}

void LifiGtsDevHandler::StartGtsRequest(GTSCharacteristics gtsCharacter, Address dstAddr){
	NS_LOG_FUNCTION(this);
	m_gtsRequestCharacter = gtsCharacter;
	m_dstAddress = dstAddr;
////	VPAN initialization  has the info of the dstAddrMode?
	m_attributes->macShortAddress = 0x5555;
	m_attributes->macVPANId = 0x01;
////set the shortAddress ,macVPANId temporary because it is not associated now.
	if((m_attributes->macShortAddress != 0xffff)
	 &&(m_attributes->macShortAddress != 0xfffe)){
		SendGtsRequest();
	}else{
//		m_user->MlmeGtsConfirm(m_gtsRequestCharacter, NO_SHORT_ADDRESS);
	}

}

void LifiGtsDevHandler::SendGtsRequest(){
	NS_LOG_FUNCTION(this);
	GtsRequestComm gtsRequest;
	gtsRequest.SetgtsCharType(m_gtsRequestCharacter.charType);
	gtsRequest.SetgtsDirection(m_gtsRequestCharacter.gtsDirection);
	gtsRequest.SetgtsLenth(m_gtsRequestCharacter.gtsLength);
	Ptr<Packet> p = gtsRequest.GetPacket();

	LifiMacHeader header;
	header.SetFrameType(LIFI_COMMAND);
	header.SetFramePending(false);
	header.SetAckRequest(true);
	header.SetSrcVPANId(m_attributes->macVPANId);

	Mac16Address tempSrcAddr;
	tempSrcAddr.CopyFrom((uint8_t*) (&(m_attributes->macShortAddress)));
	header.SetSrcAddress(Address(tempSrcAddr));
	header.SetDstAddress(m_dstAddress);

	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_bust = false;
	info.m_force = false;
	info.m_handle = 0x44;
	info.m_isAck = false;
	info.m_listener = this;
	info.m_msduSize = p->GetSize();
	info.m_option.ackTx = true;
	info.m_option.gtsTx = false;
	info.m_option.indirectTx = false;
	info.m_packet = p;

	m_trxHandler->Send(info);
	EnableTrigger(LifiGtsDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsDevHandler::onTxResultNotification1);
}

void LifiGtsDevHandler::ReceiveBeacon(uint32_t timestamp, Ptr<Packet> p){
	if (CheckTrigger(LifiGtsDevHandler::ReceiveBeacon)){
		//must be associated ,than it can use the GTS!
		if((m_attributes->macShortAddress != 0xffff) && (m_attributes->macShortAddress != 0xfffe)){
			NS_LOG_FUNCTION (this << timestamp << p);
			onReceiveBeacon(timestamp, p);
		}else{
			NS_LOG_ERROR("Ignore LifiGtsDevHandler::ReceiveBeacon");
		}
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsDevHandler::ReceiveBeacon");
	}
}

void LifiGtsDevHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> p){
	NS_LOG_FUNCTION (this << timestamp << p);

	LifiMacHeader header;
	Ptr<Packet> tempPacket = p->Copy();
	tempPacket->RemoveHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_BEACON);
	LifiMacBeacon beacon = LifiMacBeacon::Construct(tempPacket);

	if (beacon.AssignedGtsExist(m_attributes->macShortAddress))
	{
		GtsDescriptor tempDes;
		tempDes = beacon.GetGtsDescriptor(m_attributes->macShortAddress);
		NS_ASSERT(tempDes.deviceShortAddr != 0);
		if(tempDes.gtsStartSlot != 0){
				m_gtsBeaconCharacter.gtsLength = tempDes.gtsLenth;
				m_gtsBeaconCharacter.gtsDirection = (GTSDirection)beacon.GetGtsDirection(m_attributes->macShortAddress);
				if(m_gtsRequestCharacter == m_gtsBeaconCharacter){
					SetGtsDirection(m_gtsBeaconCharacter.gtsDirection);
					SetGtsOffset(tempDes.gtsStartSlot, tempDes.gtsLenth, beacon.GetGtsDescripCount());

//					m_user->MlmeGtsConfirm(m_gtsRequestCharacter, MAC_SUCCESS);
				}
				else {
//					m_user->MlmeGtsConfirm(m_gtsRequestCharacter, MAC_INVALID_PARAMETER);
				}
			}
		else{
			if(m_gtsRequestCharacter.gtsLength != 0){
//			m_user->MlmeGtsConfirm(m_gtsRequestCharacter, DENIED);
			}else{
				GTSCharacteristics tempChar;
				tempChar.gtsLength = tempDes.gtsLenth;
				tempChar.charType = DEALLOCATION;
				tempChar.gtsDirection = (GTSDirection)beacon.GetGtsDirection(m_attributes->macShortAddress);
//				m_user->MlmeGtsIndication(m_attributes->macShortAddress, tempChar);
////				how to clear the GTS information?
			}
		}

	}
	else {
//		m_user->MlmeGtsConfirm(m_gtsRequestCharacter, NO_DATA);
	}
}

void LifiGtsDevHandler::AllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiGtsDevHandler::AllocNotification))
	{
		if (!m_allocNotification.IsNull())
				m_allocNotification (service);
		else
			NS_LOG_ERROR ("LifiGtsDevHandler is null.");
	}else {
		NS_LOG_ERROR("Ignore LifiGtsDevHandler::AllocNotification");
	}
}

void LifiGtsDevHandler::onAllocNotification1 (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiGtsDevHandler::AllocNotification);
	GtsTransactions::iterator gts_beg = m_gtsTransactions.lower_bound(m_attributes->macShortAddress);
	GtsTransactions::iterator gts_end = m_gtsTransactions.upper_bound(m_attributes->macShortAddress);
	if(gts_beg != gts_end){
		m_curGtsTransmit = gts_beg->second;
		m_curGtsTransIterator = gts_beg;
		m_dataService->Transmit(m_curGtsTransmit.m_packetInfo);
	EnableTrigger(LifiGtsDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsDevHandler::onTxResultNotification2);
	}else {
		NS_LOG_ERROR("there is no packet for this device.");
	}

}

void LifiGtsDevHandler::onAllocNotification2 (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiGtsDevHandler::AllocNotification);

	SendAck();
}

void LifiGtsDevHandler::TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	if (CheckTrigger(LifiGtsDevHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsDevHandler::onTxRuesult");
	}
}

void LifiGtsDevHandler::onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this << status);
	DisableTrigger(LifiGtsDevHandler::TxResultNotification);
	NS_ASSERT(status == NO_ACK
			||status == MAC_SUCCESS
			||status == CHANNEL_ACCESS_FAILURE);
	if(status == MAC_SUCCESS){
		if(m_gtsRequestCharacter.charType == ALLOCATION){
		EnableTrigger(LifiGtsDevHandler::ReceiveBeacon);
////          how to process the GTS?
		}else if(m_gtsRequestCharacter.charType == DEALLOCATION){
//			m_user->MlmeGtsConfirm(m_gtsCharacter, status);
		}
	}else{
//		m_user->MlmeGtsConfirm(m_gtsCharacter, status);
	}
}

void LifiGtsDevHandler::onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this << status);
	NS_ASSERT(status == MAC_SUCCESS || status == CHANNEL_ACCESS_FAILURE || status == NO_ACK);
	Simulator::ScheduleNow(&TrxHandlerListener::TxResultNotification,
							m_curGtsTransmit.m_packetInfo.m_listener,
							status, m_curGtsTransmit.m_packetInfo, ack);
	m_gtsTransactions.erase(m_curGtsTransIterator);
	m_dataService->Release();
	m_dataService = 0;
	if(m_gtsTransmitState)
		SendGtsDatas();
}

void LifiGtsDevHandler::onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this << status);
	DisableTrigger(LifiGtsDevHandler::TxResultNotification);
	NS_ASSERT(status == MAC_SUCCESS || status == CHANNEL_ACCESS_FAILURE || status == NO_ACK);
//	how to pass the dataFrame to the upper layer?
	m_dataService->Release();
	m_dataService = 0;
}

void LifiGtsDevHandler::SendGtsDatas(){
	NS_LOG_FUNCTION(this);

	TranceiverTask task;
	task.Clear();
	task.id = TX_DATA;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_DATA);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiGtsDevHandler::AllocNotification);
	ReplaceTriggerCallback (m_allocNotification,
							LifiGtsDevHandler::onAllocNotification1);
}

void LifiGtsDevHandler::EndGtsTransmit(){
	NS_LOG_FUNCTION(this);
	m_gtsTransmitState = 0;
}

void LifiGtsDevHandler::ReceiveData (uint32_t timestamp, Ptr<Packet> p){
	if (CheckTrigger(LifiGtsDevHandler::ReceiveData))
	{
		NS_LOG_FUNCTION (this << timestamp << p);
		onReceiveData(timestamp, p);
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsDevHandler::ReceiveData");
	}
}

void LifiGtsDevHandler::onReceiveData (uint32_t timestamp, Ptr<Packet> p){
	NS_LOG_FUNCTION(this);

	LifiMacHeader header;
	p->PeekHeader(header);
	NS_ASSERT (header.GetFrameType() == LIFI_DATA);
	//pass this packet to the upper layer.

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiGtsDevHandler::AllocNotification);
	ReplaceTriggerCallback (m_allocNotification,
							LifiGtsDevHandler::onAllocNotification2);
}

void LifiGtsDevHandler::SendAck(){
	NS_LOG_FUNCTION (this);
	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
	header.SetDstAddress(m_dstAddress);
	header.SetFramePending(false);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
	info.m_handle = 4;
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
	EnableTrigger(LifiGtsDevHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsDevHandler::onTxResultNotification3);
}

void LifiGtsDevHandler::SetGtsOffset(uint8_t startSlot, uint8_t gtsLength, uint8_t gtsCount){
	NS_LOG_FUNCTION(this);
	m_trxHandler->SetGtsDuration(startSlot, gtsLength, gtsCount);
}

void LifiGtsDevHandler::SetGtsDirection(GTSDirection direction){
	NS_LOG_FUNCTION(this);
	m_trxHandler->SetGtsTrxState(direction);
}

void LifiGtsDevHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

}
