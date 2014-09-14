/*
 * lifi-gts-coord-handler.cc
 *
 *  Created on: 2014年8月19日
 *      Author: wanghongqiao
 */
#include "lifi-gts-coord-handler.h"
#include "ns3/log.h"
#include "lifi-mac-header.h"
#include "lifi-mac-comm.h"
#include "lifi-mac-coord-impl.h"

NS_LOG_COMPONENT_DEFINE ("LifiGtsCoordHandler");

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED (LifiGtsCoordHandler);

LifiGtsCoordHandler::LifiGtsCoordHandler() {
	m_curStartSlot = 15;
	m_gtsTransmitState = true;
	m_curChannel = CHANNEL1;
	AddTrigger(LifiGtsCoordHandler::ReceiveGtsRequest, true);
	AddTrigger(LifiGtsCoordHandler::ReceiveData, false);
	AddTrigger(LifiGtsCoordHandler::AllocNotification, false);
	AddTrigger(LifiGtsCoordHandler::TxResultNotification, false);
}

LifiGtsCoordHandler::~LifiGtsCoordHandler() {
}

TypeId LifiGtsCoordHandler::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiGtsCoordHandler")
						.SetParent<LifiGtsHandler> ()
						.AddConstructor<LifiGtsCoordHandler> ();
	return tid;
}

void LifiGtsCoordHandler::StartGtsDealloc(GTSCharacteristics gtsCharacter, Address dstAddr){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(Mac16Address::IsMatchingType(dstAddr));
	GTSDesInfo tempGtsDesInfo;

	Mac16Address tempAddr;
	tempAddr = Mac16Address::ConvertFrom(dstAddr);
	tempAddr.CopyTo((uint8_t*)&tempGtsDesInfo.gtsDescriptor.deviceShortAddr);
	tempGtsDesInfo.gtsDescriptor.gtsLenth = gtsCharacter.gtsLength;
	tempGtsDesInfo.gtsDescriptor.gtsStartSlot = 0;
	tempGtsDesInfo.gtsDirection = gtsCharacter.gtsDirection;

	GtsDesInfos::iterator it = std::find_if(m_gtsDesInfo.begin(), m_gtsDesInfo.end(), tempGtsDesInfo);
	if(it != m_gtsDesInfo.end()){
		it = m_gtsDesInfo.erase(it);
		m_curStartSlot = 15;
		for(GtsDesInfos::iterator iter = m_gtsDesInfo.begin(); iter != m_gtsDesInfo.end(); iter ++){
			(*iter).gtsDescriptor.gtsStartSlot = m_curStartSlot - (*iter).gtsDescriptor.gtsLenth;
			m_curStartSlot = m_curStartSlot - (*iter).gtsDescriptor.gtsLenth;
		}
		m_gtsDesInfo.push_back(tempGtsDesInfo);
		Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
		coordImpl->SetCFPLenth(15 - m_curStartSlot);
	}
//		must <= MAX_GTS_DESCRIPTOR
//		m_user->MlmeGtsConfirm(gtsCharacter, MAC_SUCCESS);

}

void LifiGtsCoordHandler::ReceiveGtsRequest(uint32_t timestamp, Ptr<Packet> p){
	if (CheckTrigger(LifiGtsCoordHandler::ReceiveGtsRequest))
	{
		NS_LOG_FUNCTION (this << timestamp << p);
		onReceiveGtsRequest(timestamp, p);
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsCoordHandler::ReceiveGtsRequest");
	}
}

void LifiGtsCoordHandler::onReceiveGtsRequest(uint32_t timestamp, Ptr<Packet> p){
	NS_LOG_FUNCTION(this);
	LifiMacHeader header;
	Ptr<Packet> tempPacket = p->Copy();
	tempPacket->RemoveHeader(header);

	m_allocGtsDevAddr = Mac16Address::ConvertFrom(header.GetSrcAddress());
////this address should be checked in the associated dataBase.
	NS_ASSERT (header.GetFrameType() == LIFI_COMMAND);
	GtsRequestComm gtsRequest = GtsRequestComm::Construct(tempPacket);
	m_gtsDeviceCharacter.charType = (GTSCharType)gtsRequest.GetgtsCharType();
	m_gtsDeviceCharacter.gtsLength = gtsRequest.GetgtsLenth();
	m_gtsDeviceCharacter.gtsDirection = (GTSDirection)gtsRequest.GetDirection();
//	m_gtsDeviceCharacter is used in the indication primitive.

	if(m_gtsDeviceCharacter.charType == ALLOCATION){
//		Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
		NS_ASSERT(m_gtsDeviceCharacter.gtsLength <= 16-LifiMac::aMinCapLength);
			if(m_gtsDeviceCharacter.gtsLength <= m_curStartSlot - LifiMac::aMinCapLength){
				if(m_gtsDesInfo.size() <= MAX_GTS_DESCRIPTOR){
					GTSDesInfo tempGtsDesInfo;
					tempGtsDesInfo.gtsDescriptor.gtsStartSlot = m_curStartSlot - m_gtsDeviceCharacter.gtsLength;
					m_curStartSlot = m_curStartSlot - m_gtsDeviceCharacter.gtsLength;
					m_allocGtsDevAddr.CopyTo((uint8_t*)&tempGtsDesInfo.gtsDescriptor.deviceShortAddr);
					tempGtsDesInfo.gtsDescriptor.gtsLenth = m_gtsDeviceCharacter.gtsLength;
					m_gtsDesInfo.push_back(tempGtsDesInfo);
					Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
					coordImpl->SetCFPLenth(15 - m_curStartSlot);

					TranceiverTask task;
					task.Clear();
					task.id = TX_ACK;
					task.listener = this;
					task.priority = GetTrxTaskPriority(TX_ACK);

					EnableTrigger(LifiGtsCoordHandler::AllocNotification);
					ReplaceTriggerCallback (m_allocNotification,
											LifiGtsCoordHandler::onAllocNotification1);
					m_trxHandler->ServiceRequest(task);

//					m_user->MlmeGtsIndication(m_allocGtsDevAddr, m_gtsDeviceCharacter);
				}
			}
			else{
				if(m_gtsDesInfo.size() <= MAX_GTS_DESCRIPTOR){
					GTSDesInfo tempGtsDesInfo;
					tempGtsDesInfo.gtsDescriptor.gtsStartSlot = 0;
					m_allocGtsDevAddr.CopyTo((uint8_t*)&tempGtsDesInfo.gtsDescriptor.deviceShortAddr);
					tempGtsDesInfo.gtsDescriptor.gtsLenth = 16;
					m_gtsDesInfo.push_back(tempGtsDesInfo);
				}
			}

	}else if(m_gtsDeviceCharacter.charType == DEALLOCATION){
		GTSDesInfo tempGtsDesInfo;
		m_allocGtsDevAddr.CopyTo((uint8_t*)&tempGtsDesInfo.gtsDescriptor.deviceShortAddr);
		tempGtsDesInfo.gtsDescriptor.gtsLenth = m_gtsDeviceCharacter.gtsLength;
		tempGtsDesInfo.gtsDirection = (GTSDirection)gtsRequest.GetDirection();

		GtsDesInfos::iterator it = std::find_if(m_gtsDesInfo.begin(), m_gtsDesInfo.end(), tempGtsDesInfo);
			if(it != m_gtsDesInfo.end()){
				m_gtsDesInfo.erase(it);
				m_curStartSlot = 15;
				for(GtsDesInfos::iterator it = m_gtsDesInfo.begin(); it != m_gtsDesInfo.end(); it ++){
					(*it).gtsDescriptor.gtsStartSlot = m_curStartSlot - (*it).gtsDescriptor.gtsLenth;
					m_curStartSlot = m_curStartSlot - (*it).gtsDescriptor.gtsLenth;
					Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
					coordImpl->SetCFPLenth(15 - m_curStartSlot);
				}
				TranceiverTask task;
				task.Clear();
				task.id = TX_ACK;
				task.listener = this;
				task.priority = GetTrxTaskPriority(TX_ACK);

				EnableTrigger(LifiGtsCoordHandler::AllocNotification);
				ReplaceTriggerCallback (m_allocNotification,
										LifiGtsCoordHandler::onAllocNotification1);
				m_trxHandler->ServiceRequest(task);
			}
	}
}

void LifiGtsCoordHandler::AllocNotification (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	if (CheckTrigger(LifiGtsCoordHandler::AllocNotification))
	{
		if (!m_allocNotification.IsNull())
				m_allocNotification (service);
		else
			NS_LOG_ERROR ("LifiGtsCoordHandler is null.");
	}else {
		NS_LOG_ERROR("Ignore LifiGtsCoordHandler::AllocNotification");
	}
}

void LifiGtsCoordHandler::onAllocNotification1 (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiGtsCoordHandler::AllocNotification);

	SendAck();
}

void LifiGtsCoordHandler::onAllocNotification2 (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiGtsCoordHandler::AllocNotification);

	GtsTransactions::iterator gts_beg = m_gtsTransactions.lower_bound(m_attributes->macShortAddress);
	GtsTransactions::iterator gts_end = m_gtsTransactions.upper_bound(m_attributes->macShortAddress);
	if(gts_beg != gts_end){
		m_curGtsTransmit = gts_beg->second;
		m_curGtsTransIterator = gts_beg;
		m_dataService->Transmit(m_curGtsTransmit.m_packetInfo);

		EnableTrigger(LifiGtsCoordHandler::TxResultNotification);
		ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsCoordHandler::onTxResultNotification2);
	}else{
		NS_LOG_ERROR("there is no packet for this device.");
	}
}


void LifiGtsCoordHandler::onAllocNotification3 (Ptr<DataService> service){
	NS_LOG_FUNCTION (this << service);
	NS_ASSERT (service != 0);
	m_dataService = service;
	DisableTrigger(LifiGtsCoordHandler::AllocNotification);

	SendAck2();
}

void LifiGtsCoordHandler::SendAck(){
	NS_LOG_FUNCTION(this);

	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
//	ExtendedAddress or ShortAddress?

	header.SetDstAddress(m_allocGtsDevAddr);
	header.SetFramePending(false);
	p->AddHeader(header);


	PacketInfo info;
	info.m_band = m_curChannel;
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
	EnableTrigger(LifiGtsCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsCoordHandler::onTxResultNotification1);
}

void LifiGtsCoordHandler::SendAck2(){
	NS_LOG_FUNCTION(this);

	LifiMacAck ack;
	Ptr<Packet> p = ack.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_ACK);
	header.SetSrcAddress(m_impl->GetLifiMac()->GetDevice()->GetAddress());
//	ExtendedAddress or ShortAddress?

	header.SetDstAddress(m_dataAddr);
	header.SetFramePending(false);
	p->AddHeader(header);

	PacketInfo info;
	info.m_band = m_curChannel;
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
	EnableTrigger(LifiGtsCoordHandler::TxResultNotification);
	ReplaceTriggerCallback (m_txRstNotification,
							LifiGtsCoordHandler::onTxResultNotification3);
}

void LifiGtsCoordHandler::TxResultNotification (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	if (CheckTrigger(LifiGtsCoordHandler::TxResultNotification))
	{
		NS_LOG_FUNCTION (this << (uint32_t) status);
		if (!m_txRstNotification.IsNull())
			m_txRstNotification (status, info, ack);
		else
			NS_LOG_ERROR ("m_txRstNotification is null.");
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsCoordHandler::onTxRuesult");
	}
}

void LifiGtsCoordHandler::SendGtsDatas() {
	NS_LOG_FUNCTION(this);
	if(m_gtsTransmitState == true){//may change  suddenly, so not use "assert".
		TranceiverTask task;
		task.Clear();
		task.id = TX_DATA;
		task.listener = this;
		task.priority = GetTrxTaskPriority(TX_DATA);
		m_trxHandler->ServiceRequest(task);
		EnableTrigger(LifiGtsCoordHandler::AllocNotification);
		ReplaceTriggerCallback (m_allocNotification,
								LifiGtsCoordHandler::onAllocNotification2);
	}
}

void LifiGtsCoordHandler::EndGtsTransmit() {
	NS_LOG_FUNCTION(this);
	m_gtsTransmitState = 0;
}

void LifiGtsCoordHandler::OpenGtsDataReceive(uint16_t devAddr) {
	NS_LOG_FUNCTION(this);
	EnableTrigger(LifiGtsCoordHandler::ReceiveData);
}

void LifiGtsCoordHandler::CloseGtsDataReceive() {
	NS_LOG_FUNCTION(this);
	DisableTrigger(LifiGtsCoordHandler::ReceiveData);
}

void LifiGtsCoordHandler::ReceiveData(uint32_t timestamp, Ptr<Packet> p) {
	if (CheckTrigger(LifiGtsCoordHandler::ReceiveData))
	{
		NS_LOG_FUNCTION (this << timestamp << p);
		onReceiveData(timestamp, p);
	}
	else{
		NS_LOG_ERROR("Ignore LifiGtsCoordHandler::ReceiveData");
	}
}

void LifiGtsCoordHandler::onReceiveData(uint32_t timestamp, Ptr<Packet> p) {
	NS_LOG_FUNCTION(this);

	LifiMacHeader header;
	p->PeekHeader(header);
	m_dataAddr = header.GetSrcAddress();
	NS_ASSERT (header.GetFrameType() == LIFI_DATA);
	//pass this packet to the upper layer.

	TranceiverTask task;
	task.Clear();
	task.id = TX_ACK;
	task.listener = this;
	task.priority = GetTrxTaskPriority(TX_ACK);
	m_trxHandler->ServiceRequest(task);
	EnableTrigger(LifiGtsCoordHandler::AllocNotification);
	ReplaceTriggerCallback (m_allocNotification,
							LifiGtsCoordHandler::onAllocNotification3);
}

void LifiGtsCoordHandler::onTxResultNotification1 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(status == MAC_SUCCESS
			||status == CHANNEL_ACCESS_FAILURE);
	m_dataService->Release();
	m_dataService = 0;
	if(status == MAC_SUCCESS){
//		m_user->MlmeGtsIndication(m_allocGtsDevAddr, m_gtsDeviceCharacter);
	}
}

void LifiGtsCoordHandler::onTxResultNotification2 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
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

void LifiGtsCoordHandler::onTxResultNotification3 (MacOpStatus status, PacketInfo info, Ptr<Packet> ack){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(status == MAC_SUCCESS
			||status == CHANNEL_ACCESS_FAILURE);
	if(status == MAC_SUCCESS){
//		m_user->MlmeGtsIndication(m_dataAddr, m_gtsDeviceCharacter);
	}
}


GtsList LifiGtsCoordHandler::GetGtsDescList(){
	NS_LOG_FUNCTION(this);
	m_gtsList.clear();
	GtsDesInfos::iterator it = m_gtsDesInfo.begin();
	while (it != m_gtsDesInfo.end()){
		std::cout << m_gtsDesInfo.size() << std::endl;
		m_gtsList.push_back((*it).gtsDescriptor);
		NS_ASSERT((*it).counter <= 3);
		(*it).counter ++;
		if((*it).counter >= LifiMac::aGTSDescPersistenceTime){
			it = m_gtsDesInfo.erase(it);

			std::cout << m_gtsDesInfo.size() << std::endl;
			m_curStartSlot = 15;
			for(GtsDesInfos::iterator it = m_gtsDesInfo.begin(); it != m_gtsDesInfo.end(); it ++){
				(*it).gtsDescriptor.gtsStartSlot = m_curStartSlot - (*it).gtsDescriptor.gtsLenth;
				m_curStartSlot = m_curStartSlot - (*it).gtsDescriptor.gtsLenth;
//				Ptr<LifiMacCoordImpl> coordImpl = DynamicCast<LifiMacCoordImpl> (m_impl);
//				coordImpl->SetCFPLenth(16 - m_curStartSlot);

//				we do not need those codes for this beacon.
			}
		}else {
			it ++;
		}
//		if ((it = m_gtsDesInfo.erase(it)) != m_gtsDesInfo.end())
//			it++;
	}
	return m_gtsList;
}

GtsList LifiGtsCoordHandler::GetBeaconGtsDescList(){
	NS_LOG_FUNCTION(this);
	return m_gtsList;
}

uint8_t LifiGtsCoordHandler::GetGtsDesCount(){
	NS_LOG_FUNCTION(this);
	return m_gtsDesInfo.size();
}

uint8_t LifiGtsCoordHandler::GetGtsDirectionsMark(){
	NS_LOG_FUNCTION(this);
	uint8_t tempInt = 7;
	uint8_t tempBool;
	uint8_t tempDirectionMark = 0;
	NS_ASSERT(m_gtsDesInfo.size() <= MAX_GTS_DESCRIPTOR);
	for(GtsDesInfos::iterator it = m_gtsDesInfo.begin(); it != m_gtsDesInfo.end(); it ++){
		tempBool = (uint8_t)(*it).gtsDirection;
		tempDirectionMark = tempDirectionMark || (tempBool << tempInt);
		tempInt --;
	}
	return tempDirectionMark;
}

void LifiGtsCoordHandler::SetGtsTransmitArgument(uint16_t shortAddr, bool transmitState){
	NS_LOG_FUNCTION(this);
	m_deviceAddress = shortAddr;
	m_gtsTransmitState = transmitState;
	if(m_gtsTransmitState == true)
		SendGtsDatas();
}

void LifiGtsCoordHandler::SetTrxHandler(Ptr<LifiTrxHandler> trxHandler) {
	NS_LOG_FUNCTION(this);
	m_trxHandler = trxHandler;
}

}

