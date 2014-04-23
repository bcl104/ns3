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

NS_LOG_COMPONENT_DEFINE ("LifiAssocHandler");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiAssocHandler);

LifiAssocHandler::LifiAssocHandler() {
	m_state = IDLE;
	flag = false ;
	m_dataService = 0;
	m_user = 0;
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

void LifiAssocHandler::TxResultNotification(MacOpStatus status){
	NS_LOG_FUNCTION (this);
	onTxRuesult(status);
}

void Send(uint32_t size, Ptr<Packet> p, uint8_t band, bool contention){
	return ;
}

void LifiAssocHandler::ReceiveBeacon (uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION (this);
	onReceiveBeacon(timestamp,msdu);
}

void LifiAssocHandler::ReceiveAck (uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION (this << timestamp << msdu);
	onReceiveAck(timestamp,msdu);
}

void LifiAssocHandler::ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> msdu){
	NS_LOG_FUNCTION (this);
	onReceiveAssocResponse(timestamp,msdu);
}

void LifiAssocHandler::onTxRuesult(MacOpStatus status) {
	NS_LOG_FUNCTION (this);
	if(status == CHANNEL_ACCESS_FAILURE)
	{
		std::cout<<"aaa"<<std::endl;
		sendPacket();
//		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"),
//				CHANNEL_ACCESS_FAILURE, NO_COLOR_STABI);
		EndAssoc();
		return ;
	}

	if(status == MAC_SUCCESS)
	{
//		if(m_state == IDLE)
//			m_state = WAIT_FOR_ACK1;
//		if(m_state == WAIT_FOR_ACK1)
//				m_state = WAIT_FOR_RESPO_WITH_TRACK;
//		if(m_state == WAIT_FOR_RESPO_WITH_TRACK)
//				m_state = WAIT_FOR_ACK2;
//		if(m_state == WAIT_FOR_RESPO_WITHOUT_TRACK)
//				m_state = WAIT_FOR_ACK2;
//		if(m_state == WAIT_FOR_ACK2)
//				m_state = WAIT_FOR_ASSOC_RESPO;
//		if(m_state == WAIT_FOR_ASSOC_RESPO)
//				m_state = (State)0;
		switch (m_state)
		{
		case IDLE:
			m_state = WAIT_FOR_ACK1;break;
		case WAIT_FOR_ACK1:
			m_state = WAIT_FOR_RESPO_WITH_TRACK;break;
		case WAIT_FOR_RESPO_WITH_TRACK:
			m_state = WAIT_FOR_ACK2;break;
		case WAIT_FOR_RESPO_WITHOUT_TRACK:
			m_state = WAIT_FOR_ACK2;break;
		case WAIT_FOR_ACK2:
			m_state = WAIT_FOR_ASSOC_RESPO;break;
		case WAIT_FOR_ASSOC_RESPO:
			m_state = END;break;
		case END:
			m_state = IDLE;break;
		}
		DoRun();
		return ;
	}
}

void LifiAssocHandler::AssociationStart(VPANDescriptor &vpandescri, bool trackbeacon){
	m_curChannel = vpandescri.logicChannel;
	m_VPANId = vpandescri.coordVPANId;
	m_CoordAddr = vpandescri.coordAddr;
	m_trackBeacon = trackbeacon;

	SendAssocRequest();
}

void LifiAssocHandler::DoRun(){
	if(m_state == WAIT_FOR_ACK1)
	{
		m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
		m_timer.Schedule(Seconds(10));
		return ;
	}

	if(m_state == WAIT_FOR_RESPO_WITH_TRACK)
	{
		m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
		m_timer.Schedule(Seconds(11));
		return ;
	}

	if(m_state == WAIT_FOR_RESPO_WITHOUT_TRACK)
	{
		m_timer.SetFunction(&LifiAssocHandler::SendDataRequest, this);
		m_timer.Schedule(Seconds(12));
		return ;
	}

	if(m_state == WAIT_FOR_ACK2)
	{
		m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
		m_timer.Schedule(Seconds(13));
		return ;
	}

	if(m_state == WAIT_FOR_ASSOC_RESPO)
	{
		m_timer.SetFunction(&LifiAssocHandler::EndAssoc, this);
		m_timer.Schedule(Seconds(14));
		return ;
	}
}

void LifiAssocHandler::EndAssoc(){
	NS_LOG_FUNCTION(this);
//	if(m_state == WAIT_FOR_ACK1 || m_state == WAIT_FOR_ACK2)
//		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"),
//			NO_ACK, NO_COLOR_STABI);
//	if(m_state == WAIT_FOR_RESPO_WITH_TRACK)
//		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"),
//			NO_BEACON, NO_COLOR_STABI);
//	if(m_state == WAIT_FOR_ASSOC_RESPO)
////		how to define the returning status??
//		m_user->MlmeAssociateConfirm(Mac16Address("ff:ff"),
//			PAST_TIME, NO_COLOR_STABI);

}

void LifiAssocHandler::sendPacket(){

}

void LifiAssocHandler::SendAssocRequest() {
	AssocRqstComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(COMMAND);
	header.SetSrcAddress(Address (Mac16Address ("ff:ff")));
	header.SetDstAddress(Address (Mac64Address ("21:22:23:24:25:26:27:28")));
	p->AddHeader(header);

	sendPacket();
//	here we use an empty function.
//	here we will recode in the future using the lower example
//	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);

	return;
}

void LifiAssocHandler::SendDataRequest(){
	DataRequestComm comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(COMMAND);
	header.SetSrcAddress(Address (Mac16Address ("ff:ff")));
	header.SetDstAddress(Address (Mac64Address ("21:22:23:24:25:26:27:28")));
	p->AddHeader(header);

	sendPacket();
//	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
}

void LifiAssocHandler::SendAck(){
	LifiMacAck comm;
	Ptr<Packet> p = comm.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(ACK);
	header.SetSrcAddress(Address (Mac16Address ("ff:ff")));
	header.SetDstAddress(Address (Mac64Address ("21:22:23:24:25:26:27:28")));
	p->AddHeader(header);

	sendPacket();
//	m_dataService->Send(p->GetSize(), p, (uint8_t)m_curChannel, true);
}

void LifiAssocHandler::onReceiveBeacon(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION(this << timestamp << msdu);
	NS_ASSERT (!m_timer.IsExpired());
	LifiMacHeader header;
	msdu->RemoveHeader(header);
	if(header.GetFrameType() == BEACON)
	{
		if(header.GetFramePending() == true)
		{
			m_timer.Cancel();
			if(header.GetSrcAddressMode() == SHORT)
			{
				flag = true ;
			}

			SendDataRequest();
		}
	}
}

void LifiAssocHandler::onReceiveAck(uint32_t timestamp, Ptr<Packet> msdu)
{
	NS_LOG_FUNCTION(this << timestamp << msdu);
//	std::cout<<Simulator::Now()<<std::endl;
//	std::cout<<m_timer.GetState()<<std::endl;
//	std::cout<<m_timer.IsRunning()<<std::endl;
	NS_ASSERT (!m_timer.IsExpired());
	LifiMacHeader header;
	msdu->RemoveHeader(header);
	m_type = header.GetFrameType();
	std::cout<<m_type<<std::endl;
	if(m_type == ACK)
	{
		m_timer.Cancel();
		if(m_state == WAIT_FOR_ACK1)
		{
			m_state = WAIT_FOR_RESPO_WITH_TRACK;
			if(m_trackBeacon == false)
				m_state = WAIT_FOR_RESPO_WITHOUT_TRACK;
		}
		if(m_state == WAIT_FOR_ACK2)
			m_state = WAIT_FOR_ASSOC_RESPO;
		DoRun();
		return ;
	}
}

void LifiAssocHandler::onReceiveAssocResponse(uint32_t timestamp, Ptr<Packet> msdu) {
	NS_LOG_FUNCTION(this << timestamp << msdu);
//	std::cout<<m_timer.IsExpired()<<std::endl;
	std::cout<<m_timer.GetState()<<std::endl;
	std::cout<<Simulator::Now()<<std::endl;
	NS_ASSERT (!m_timer.IsExpired());
	LifiMacHeader header;
	msdu->RemoveHeader(header);
	m_type = header.GetFrameType();
	if(m_type == COMMAND)
	{
		uint8_t* b = new uint8_t;
		uint16_t e;

		msdu->CopyData(b,1);
		if(*b == ASSOC_RESPONSE)
		{
			m_timer.Cancel();
			SendAck();
			m_state = IDLE;
			AssocResponseComm assocresponse = AssocResponseComm::Construct(msdu);
			if(assocresponse.GetAssocStatus() == ASSOC_SUCCESS)
			{
				e= assocresponse.GetShortAddr();
				void* p = &e;
				m_attributes->SetAttributes(MAC_SHORT_ADDRESS,p);
			}
			else
			{
				m_attributes->SetAttributes(MAC_VPAN_ID,(void*)(0xffff));
			}

			if(flag == true)
			{
				if(header.GetSrcAddressMode () == EXTENDED)
				{
					Address d = header.GetSrcAddress();
					void* ppp = &d;
//					we can set the PIB Attributes directly with its private parameter
					m_attributes->SetAttributes(MAC_COORD_EXTENDED_ADDRESS,ppp);
				}
			}
		}
		Mac16Address addr;
		addr.CopyFrom((uint8_t*) (&e));
		m_user->MlmeAssociateConfirm(addr, MAC_SUCCESS, NO_COLOR_STABI);
		delete(b);
	}
}

} /* namespace ns3 */
