/*
 * packet-test.cc
 *
 *  Created on: 2014年4月9日
 *      Author: will
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lte-module.h"
#include "ns3/lifi-module.h"
#include "src/lifi/utils/utils.h"

using namespace ns3;

class L : public TrxHandlerListener
{
public:
	static TypeId GetTypeId ()
	{
		TypeId tid = TypeId ("ns3::L");
		return tid;
	}
	virtual void AllocNotification (Ptr<DataService> service) {};
	virtual void TxResultNotification (MacOpStatus status, Ptr<Packet> ack) {};
	virtual void ResumeNotification (Ptr<DataService> service) {};

	virtual void ReceiveAssocRequest (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Associate Request" << std::endl;
	};
	virtual void ReceiveAssocResponse (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Associate Response" <<std::endl;
	}
	virtual void ReceiveBeacon (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Beacon" << std::endl;
	}
	virtual void ReceiveBeaconRequest (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Beacon Request" << std::endl;
	}
	virtual void ReceiveData (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Data" << std::endl;
	}
	virtual void ReceiveDataRequest (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Data Request" << std::endl;
	}

	virtual void ReceiveDisassocNotification (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Disassociate Notification" << std::endl;
	}
	virtual void ReceiveGtsRequest (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Gts Request" << std::endl;
	}
	virtual void ReceiveGtsResponse (uint32_t timestamp, Ptr<Packet> p)
	{
		std::cout << "Gts Response" << std::endl;
	}
};

int main ()
{
//	LifiTrxHandler trxHandler;
//	L listener;
//	trxHandler.AddListener(L::GetTypeId(), listener);
//	LifiMacHeader header;
//	header.SetSrcAddress(Mac16Address("ff:ff"));
//	header.SetDstAddress(Mac16Address("ff:fe"));
//
//	LifiMacAck ack;
//	Ptr<Packet> pAck = ack.GetPacket();
//	header.SetFrameType(LIFI_ACK);
//	pAck->AddHeader(header);
//	trxHandler.ReceivePacket(0, pAck);
//
//	LifiMacBeacon beacon;
//	Ptr<Packet> pBeacon = beacon.GetPacket();
//	header.SetFrameType(LIFI_BEACON);
//	pBeacon->AddHeader(header);
//	trxHandler.ReceivePacket(0, pBeacon);
//
//	AssocResponseComm assocResponse;
//	Ptr<Packet> pAssocResponse = assocResponse.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pAssocResponse->AddHeader(header);
//	trxHandler.ReceivePacket(0, pAssocResponse);
//
//	AssocRqstComm assocRequst;
//	Ptr<Packet> pAssocRequst = assocRequst.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pAssocRequst->AddHeader(header);
//	trxHandler.ReceivePacket(0, pAssocRequst);
//
//	BeaconRequestComm beaconRequest;
//	Ptr<Packet> pBeaconRequest = beaconRequest.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pBeaconRequest->AddHeader(header);
//	trxHandler.ReceivePacket(0, pBeaconRequest);
//
//	DataRequestComm dataRequest;
//	Ptr<Packet> pDataRequest = dataRequest.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pDataRequest->AddHeader(header);
//	trxHandler.ReceivePacket(0, pDataRequest);
//
//	DisassocNotificationComm disassocNotification;
//	Ptr<Packet> pDisassocNotification = disassocNotification.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pDisassocNotification->AddHeader(header);
//	trxHandler.ReceivePacket(0, pDisassocNotification);
//
//	GtsRequestComm gtsRequest;
//	Ptr<Packet> pGtsRequest = gtsRequest.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pGtsRequest->AddHeader(header);
//	trxHandler.ReceivePacket(0, pGtsRequest);
//
//	GtsResponseComm gtsResponse;
//	Ptr<Packet> pGtsResponse = gtsResponse.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pGtsResponse->AddHeader(header);
//	trxHandler.ReceivePacket(0, pGtsResponse);
//
//	Ptr<Packet> pData = Create<Packet> ();
//	header.SetFrameType(LIFI_DATA);
//	pData->AddHeader(header);
//	trxHandler.ReceivePacket(0, pData);

	LifiTrxHandler handler;
	LifiMacImpl macImpl;
	handler.SetLifiMacImpl(&macImpl);

	return 0;
}



