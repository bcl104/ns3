/*
 * channel-scan-handler-test.cc
 *
 *  Created on: 2014年4月17日
 *      Author: will
 */
#include "ns3/core-module.h"
#include "ns3/lifi-module.h"

using namespace ns3;

int main ()
{
	LogComponentEnable("LifiChannelScanHandler", LOG_LEVEL_FUNCTION);
	LifiChannelScanHandler handler;
	LifiMacPibAttribute attribtes;
	handler.SetMacPibAttribtes(&attribtes);
	handler.Start(INACTIVE_SCAN, 5000);
	LifiMacBeacon beacon;
	Ptr<Packet> p = beacon.GetPacket();
	LifiMacHeader header;
	header.SetFrameType(LIFI_BEACON);
	header.SetSrcAddress(Address (Mac16Address ("11:12")));
	header.SetDstAddress(Address (Mac64Address ("21:22:23:24:25:26:27:28")));
	p->AddHeader(header);
	Simulator::Schedule(Seconds(30), &LifiChannelScanHandler::ReceiveBeacon, &handler, 30, p);
	Simulator::Run();
	Simulator::Stop(Seconds(100));

	return 0;
}



