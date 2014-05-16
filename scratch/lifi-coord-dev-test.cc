/*
 * lifi-coord-dev-test.cc
 *
 *  Created on: 2014年5月11日
 *      Author: will
 */
#include "ns3/core-module.h"
#include "ns3/lifi-module.h"

using namespace ns3;
int main ()
{
	Ptr<LifiMac> coordMac = CreateObject<LifiMac> ();
	Ptr<LifiMac> devMac = CreateObject<LifiMac> ();
	Ptr<LifiPhy> coordPhy = CreateObject<LifiPhy> ();
	Ptr<LifiPhy> devPhy = CreateObject<LifiPhy> ();
	coordMac->SetPdSapProvider(coordPhy->GetPdSapProvider());
	coordMac->SetPlmeSapProvider(coordPhy->GetPlmeSapProvider());
	devMac->SetPdSapProvider(devPhy->GetPdSapProvider());
	devMac->SetPlmeSapProvider(devPhy->GetPlmeSapProvider());
	coordPhy->SetPdSapUser(coordMac->GetPdSapUser());
	coordPhy->SetPlmeSapUser(coordMac->GetPlmeSapUser());
	devPhy->SetPdSapUser(devMac->GetPdSapUser());
	devPhy->SetPlmeSapUser(devMac->GetPlmeSapUser());
	coordMac->SetOpticalPeriod(coordPhy->GetOpticClock());
	devMac->SetOpticalPeriod(devPhy->GetOpticClock());



}



