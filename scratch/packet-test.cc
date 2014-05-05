<<<<<<< HEAD
///////*
////// * packet-test.cc
////// *
////// *  Created on: 2014年4月9日
////// *      Author: will
////// */
//////#include "ns3/core-module.h"
//////#include "ns3/network-module.h"
//////#include "ns3/lte-module.h"
//////#include "ns3/lifi-module.h"
//////#include "src/lifi/utils/utils.h"
//////
//////using namespace ns3;
//////
//////int main ()
//////{
////////	uint32_t a = 0x11111111;
////////	uint32_t b;
////////	Ptr<Packet> p = Create<Packet> ((uint8_t*)&a, 4);
////////	LtePdcpHeader header;
////////	p->AddHeader(header);
////////	LtePdcpHeader h;
////////	p->RemoveHeader(h);
////////	p->CopyData((uint8_t*)&b, 4);
////////	std::cout << "endl" << std::endl;
////////
////////	Buffer buffer = Buffer ();
////////	buffer.AddAtStart(3);
////////	Buffer::Iterator it = buffer.Begin();
////////	it.WriteU8(1);
////////	it.WriteU8(2);
////////	it.WriteU8(3);
////////	it.WriteU8(4);
////////	uint32_t b;
////////	it = buffer.Begin();
////////	it.Read((uint8_t*)&b, 4);
////////	b = 0;
////////
////////	Ptr<Packet> p = Create<Packet> ();
////////	AssocRqstComm comm1 (p);
////////	comm1.capability_info
////////
////////	GtsRequestComm gtsRequest;
////////	gtsRequest.characteristic.gtsLength = 4;
////////	gtsRequest.characteristic.gtsDirection = TX_ONLY;
////////	gtsRequest.characteristic.charType = ALLOCATION;
////////
////////	Ptr<Packet> p = gtsRequest.GetPacket();
////////
////////	GtsRequestComm gtsRequestRx (p);
////////
////////	MultiChannelAssignComm multichannelAssign;
////////	multichannelAssign.multi_channel = 0xfe;
////////
////////	Ptr<Packet> p = multichannelAssign.GetPacket();
////////	uint32_t size = p->GetSize();
////////	uint8_t* data = (uint8_t*) malloc (size);
////////	p->CopyData(data, size);
////////	uint8_t* newData = (uint8_t*) malloc (size + 4);
////////	memset(newData, 0, size + 4);
////////	memcpy(newData, data, size);
////////	*(newData + 3) = 0xff;
////////	Ptr<Packet> newP = Create <Packet> (newData, size+4);
////////	uint8_t* bufData = (uint8_t*) malloc (size + 4);
////////	memset(newData, 0, size + 4);
////////	newP->CopyData(bufData, size + 4);
////////
////////	MultiChannelAssignComm multichannelAssignRx (p);
////////	free (bufData);
////////	free (data);
////////	free (newData);
////////
////////	LifiMacHeader header1;
////////	header1.SetAckRequest(true);
////////	header1.SetFramePending(true);
////////	header1.SetSrcAddress(Address (Mac64Address ("11:12:13:14:15:16:17:18")));
////////	header1.SetDstAddress(Address (Mac16Address ("21:22")));
////////	header1.SetFrameType(BEACON);
////////	header1.SetSrcVPANId(0x01);
////////	header1.SetDstVPANId(0x42);
////////	header1.SetSequenceNumber(56);
////////
////////	Ptr<Packet> p = Create <Packet> ();
////////	p->AddHeader(header1);
////////
////////	LifiMacHeader header2;
////////	p->RemoveHeader(header2);
////////
////////	std::cout << header2.GetSrcAddress() << std::endl;
////////	std::cout << header2.GetDstAddress() << std::endl;
////////
////////	return 0;
////////
////////	Ptr<Packet> p = Create<Packet> ();
////////	LifiMacBeacon beacon1 = LifiMacBeacon::Construct(p);
////////	LifiMacBeacon beacon2 = LifiMacBeacon::Construct(p);
////////	return 0;
////////
////////	GtsDescriptor d1 (1, 2, 3);
////////	GtsDescriptor d2 (4, 5, 6);
////////	std::vector<GtsDescriptor> v;
////////	v.push_back(d1);
////////	v.push_back(d2);
////////	std::vector<GtsDescriptor>::iterator it = std::find_if(v.begin(), v.end(), d1);
////////	std::cout << it->deviceShortAddr << " " << (uint32_t)it->gtsLenth << " " << (uint32_t)it->gtsStartSlot << std::endl;
////////
////////	LifiMacBeacon beacon;
////////	uint32_t payload = 12345;
////////	beacon.SetAssocPermit(true);
////////	beacon.SetBcnOrder(1);
////////	beacon.SetCellSearchEn(true);
////////	beacon.SetFinalCapSlot(2);
////////	beacon.SetGtsDirMask(0xff);
////////	beacon.SetPayload((uint8_t*)&payload, 4);
////////	beacon.SetSupframeOrder(6);
////////	beacon.SetVpanCoord(true);
////////	beacon.SetGtsPermit(true);
////////	beacon.SetCellSearchLenth(3);
////////	beacon.AddGts(GtsDescriptor(0x1010, 15, 15));
////////	beacon.AddGts(GtsDescriptor(0x1011, 14, 14));
////////	beacon.AddPendingAddress(Address(Mac16Address("00:01")));
////////	beacon.AddPendingAddress(Address (Mac64Address("11:12:13:14:15:16:17:18")));
////////	beacon.AddPendingAddress(Address(Mac16Address("01:02")));
////////	beacon.AddPendingAddress(Address (Mac64Address("21:22:23:24:25:26:27:28")));
////////
////////	Ptr<Packet> p = beacon.GetPacket();
////////
////////	LifiMacBeacon rec = LifiMacBeacon::Construct(p);
////////
////////	GtsDescriptor descriptor1 = *rec.FindAssignedGts(Mac16Address("10:10"));
////////	GtsDescriptor descriptor2 = *rec.FindAssignedGts(Mac16Address("11:10"));
////////	Address addr1 = *rec.CheckPendingAddress(Address(Mac16Address("00:01")));
////////	Address addr2 = *rec.CheckPendingAddress(Address (Mac64Address("11:12:13:14:15:16:17:18")));
////////	Address addr3 = *rec.CheckPendingAddress(Address(Mac16Address("01:02")));
////////	Address addr4 = *rec.CheckPendingAddress(Address (Mac64Address("21:22:23:24:25:26:27:28")));
////////
////////	beacon.AddGts(descriptor1);
////////	beacon.AddGts(descriptor2);
////////	beacon.AddPendingAddress(addr1);
////////	beacon.AddPendingAddress(addr2);
////////	beacon.AddPendingAddress(addr3);
////////	beacon.AddPendingAddress(addr4);
//////
////////	LogComponentEnable("LifiDevAssocHandler", LOG_LEVEL_FUNCTION);
////////	LifiDevAssocHandler handler;
////////	TrigMap::iterator it = handler.m_trigger.find(mem_cast(&LifiDevAssocHandler::ReceiveBeacon));
////////	it->second = true;
////////	handler.ReceiveAck(0, 0);
////////	handler.ReceiveBeacon(0, 0);
//////
//////
//////	return 0;
//////}
//////
//////
//////
//#include <iostream>
////#include <cstring>
////
////using namespace std;
////template <class T, size_t N> void array_init(T (&para)[N]){
////	for(size_t i = 0; i != N; i++)
////		para[i] = 0;
////	cout<<N<<endl;
////}
////
////template <typename T> bool compare(const T &v1, const T &v2){
////	if (v1 < v2) return 1;
////	if (v2 < v1) return -1;
////	return 0;
////}
////
//////template<unsigned N, unsigned M>
//////bool compare (const char (&p1)[N], const char (&p2)[M])
//////{
//////    std::cout << "size : " << N << " " << M << std::endl;
//////    return strcmp(p1, p2);
//////}
////
//////指针
////template<const char* C>
////void pointerT(const char* str){
////    std::cout << C << " " << str << std::endl;
////}
////
//////引用
////template<char (&ra)[9]>
////void referenceT(const char* str){
////    std::cout << ra << " " << str << std::endl;
////}
////
////char ca[] = "Caroline"; //初始化指针
////char cr[9] = "Caroline"; //初始化引用, 包含一个结尾符号
////
////void f(const char* c) {std::cout << c << std::endl; }
////
//////函数指针
////template<void (*F)(const char*)>
////void fpointerT(const char* c) {
////    F(c);
////}
////
//template<class T> class Test{
//public:
//    Test():i(0){}
//    Test(T k);
//    ~Test(){}
//    void print(T m){
//    	n = m;
//    	std::cout << n ;
//    }
//    T operator+(T x);
//private:
//    T n;
//    const T i;
//    static T cnt;
//};
=======
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
>>>>>>> branch 'master' of https://github.com/bcl104/ns3.git
//
<<<<<<< HEAD
//int main (){
////int x[42];
////int y[34];
////array_init(x);
////array_init(y);
////
//////if(compare("Caroline", "Wendy"))
//////{
//////	std::cout << "Caroline is long." << std::endl;
//////}
//////else
//////{
//////	std::cout << "Wendy is long." << std::endl;
//////}
////
////    //无法使用局部变量或者动态变量作为模板参数
////    pointerT<ca>("Wendy"); //指针
////
////    referenceT<cr>("Wendy"); //引用
////
////    fpointerT<f>("Caroline Wendy"); //函数指针
////
////
////    cout << compare(1,0) << endl;
=======
//	LifiMacBeacon beacon;
//	Ptr<Packet> pBeacon = beacon.GetPacket();
//	header.SetFrameType(LIFI_BEACON);
//	pBeacon->AddHeader(header);
//	trxHandler.ReceivePacket(0, pBeacon);
>>>>>>> branch 'master' of https://github.com/bcl104/ns3.git
//
<<<<<<< HEAD
//	Test<int> qq;
//	qq.print(4);
=======
//	AssocResponseComm assocResponse;
//	Ptr<Packet> pAssocResponse = assocResponse.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pAssocResponse->AddHeader(header);
//	trxHandler.ReceivePacket(0, pAssocResponse);
>>>>>>> branch 'master' of https://github.com/bcl104/ns3.git
//
<<<<<<< HEAD
//return 0;
//}
=======
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
//	DataRequestComm dataRequest;
//	Ptr<Packet> pDataRequest = dataRequest.GetPacket();
//	header.SetFrameType(LIFI_COMMAND);
//	pDataRequest->AddHeader(header);
//	trxHandler.ReceivePacket(0, pDataRequest);
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
>>>>>>> branch 'master' of https://github.com/bcl104/ns3.git
//	Ptr<Packet> pData = Create<Packet> ();
//	header.SetFrameType(LIFI_DATA);
//	pData->AddHeader(header);
//	trxHandler.ReceivePacket(0, pData);


	LifiTrxHandler handler;
	LifiMacImpl macImpl;
	handler.SetLifiMacImpl(&macImpl);

using namespace std;
int main(){

	bool a = compare(1, 2);
	bool b = compare(1.1, 2.2);
	cout<< a <<endl<< b <<endl;
	return 0;
	short aa = 1;
	int bb = 2;
	compare(static_cast<int>(aa), bb);
}


