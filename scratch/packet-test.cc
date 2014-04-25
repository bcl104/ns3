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

using namespace ns3;

int main ()
{
////	uint32_t a = 0x11111111;
////	uint32_t b;
////	Ptr<Packet> p = Create<Packet> ((uint8_t*)&a, 4);
////	LtePdcpHeader header;
////	p->AddHeader(header);
////	LtePdcpHeader h;
////	p->RemoveHeader(h);
////	p->CopyData((uint8_t*)&b, 4);
////	std::cout << "endl" << std::endl;
////
////	Buffer buffer = Buffer ();
////	buffer.AddAtStart(3);
////	Buffer::Iterator it = buffer.Begin();
////	it.WriteU8(1);
////	it.WriteU8(2);
////	it.WriteU8(3);
////	it.WriteU8(4);
////	uint32_t b;
////	it = buffer.Begin();
////	it.Read((uint8_t*)&b, 4);
////	b = 0;
////
////	Ptr<Packet> p = Create<Packet> ();
////	AssocRqstComm comm1 (p);
////	comm1.capability_info
////
////	GtsRequestComm gtsRequest;
////	gtsRequest.characteristic.gtsLength = 4;
////	gtsRequest.characteristic.gtsDirection = TX_ONLY;
////	gtsRequest.characteristic.charType = ALLOCATION;
////
////	Ptr<Packet> p = gtsRequest.GetPacket();
////
////	GtsRequestComm gtsRequestRx (p);
////
////	MultiChannelAssignComm multichannelAssign;
////	multichannelAssign.multi_channel = 0xfe;
////
////	Ptr<Packet> p = multichannelAssign.GetPacket();
////	uint32_t size = p->GetSize();
////	uint8_t* data = (uint8_t*) malloc (size);
////	p->CopyData(data, size);
////	uint8_t* newData = (uint8_t*) malloc (size + 4);
////	memset(newData, 0, size + 4);
////	memcpy(newData, data, size);
////	*(newData + 3) = 0xff;
////	Ptr<Packet> newP = Create <Packet> (newData, size+4);
////	uint8_t* bufData = (uint8_t*) malloc (size + 4);
////	memset(newData, 0, size + 4);
////	newP->CopyData(bufData, size + 4);
////
////	MultiChannelAssignComm multichannelAssignRx (p);
////	free (bufData);
////	free (data);
////	free (newData);
////
////	LifiMacHeader header1;
////	header1.SetAckRequest(true);
////	header1.SetFramePending(true);
////	header1.SetSrcAddress(Address (Mac64Address ("11:12:13:14:15:16:17:18")));
////	header1.SetDstAddress(Address (Mac16Address ("21:22")));
////	header1.SetFrameType(BEACON);
////	header1.SetSrcVPANId(0x01);
////	header1.SetDstVPANId(0x42);
////	header1.SetSequenceNumber(56);
////
////	Ptr<Packet> p = Create <Packet> ();
////	p->AddHeader(header1);
////
////	LifiMacHeader header2;
////	p->RemoveHeader(header2);
////
////	std::cout << header2.GetSrcAddress() << std::endl;
////	std::cout << header2.GetDstAddress() << std::endl;
////
////	return 0;
////
////	Ptr<Packet> p = Create<Packet> ();
////	LifiMacBeacon beacon1 = LifiMacBeacon::Construct(p);
////	LifiMacBeacon beacon2 = LifiMacBeacon::Construct(p);
////	return 0;
////
////	GtsDescriptor d1 (1, 2, 3);
////	GtsDescriptor d2 (4, 5, 6);
////	std::vector<GtsDescriptor> v;
////	v.push_back(d1);
////	v.push_back(d2);
////	std::vector<GtsDescriptor>::iterator it = std::find_if(v.begin(), v.end(), d1);
////	std::cout << it->deviceShortAddr << " " << (uint32_t)it->gtsLenth << " " << (uint32_t)it->gtsStartSlot << std::endl;
//
//	LifiMacBeacon beacon;
//	uint32_t payload = 12345;
//	beacon.SetAssocPermit(true);
//	beacon.SetBcnOrder(1);
//	beacon.SetCellSearchEn(true);
//	beacon.SetFinalCapSlot(2);
//	beacon.SetGtsDirMask(0xff);
//	beacon.SetPayload((uint8_t*)&payload, 4);
//	beacon.SetSupframeOrder(6);
//	beacon.SetVpanCoord(true);
//	beacon.SetGtsPermit(true);
//	beacon.SetCellSearchLenth(3);
//	beacon.AddGts(GtsDescriptor(0x1010, 15, 15));
//	beacon.AddGts(GtsDescriptor(0x1011, 14, 14));
//	beacon.AddPendingAddress(Address(Mac16Address("00:01")));
//	beacon.AddPendingAddress(Address (Mac64Address("11:12:13:14:15:16:17:18")));
//	beacon.AddPendingAddress(Address(Mac16Address("01:02")));
//	beacon.AddPendingAddress(Address (Mac64Address("21:22:23:24:25:26:27:28")));
//
//	Ptr<Packet> p = beacon.GetPacket();
//
//	LifiMacBeacon rec = LifiMacBeacon::Construct(p);
//
//	GtsDescriptor descriptor1 = *rec.FindAssignedGts(Mac16Address("10:10"));
//	GtsDescriptor descriptor2 = *rec.FindAssignedGts(Mac16Address("11:10"));
//	Address addr1 = *rec.CheckPendingAddress(Address(Mac16Address("00:01")));
//	Address addr2 = *rec.CheckPendingAddress(Address (Mac64Address("11:12:13:14:15:16:17:18")));
//	Address addr3 = *rec.CheckPendingAddress(Address(Mac16Address("01:02")));
//	Address addr4 = *rec.CheckPendingAddress(Address (Mac64Address("21:22:23:24:25:26:27:28")));
//
//	beacon.AddGts(descriptor1);
//	beacon.AddGts(descriptor2);
//	beacon.AddPendingAddress(addr1);
//	beacon.AddPendingAddress(addr2);
//	beacon.AddPendingAddress(addr3);
//	beacon.AddPendingAddress(addr4);

	return 0;
}



