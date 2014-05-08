/*
 * packet-test.cc
 *
 *  Created on: 2014年4月9日
 *      Author: will
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-delay-model.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("PhyTest");

int main ()
{
	LogComponentEnable ("LifiPhy", LOG_LEVEL_FUNCTION);
	LogComponentEnable ("LifiSpectrumPhy", LOG_LEVEL_FUNCTION);
	LogComponentEnable ("LifiSpectrumChannel", LOG_LEVEL_FUNCTION);
	LogComponentEnable ("LifiInterference",LOG_LEVEL_FUNCTION);

	Ptr<Node> nodeTx=CreateObject<Node>();
	Ptr<Node> nodeRx=CreateObject<Node>();

	Ptr<LifiNetDevice> lifiNetDeviceTx=CreateObject<LifiNetDevice>();
	Ptr<LifiNetDevice> lifiNetDeviceRx=CreateObject<LifiNetDevice>();
	Ptr<LifiSpectrumPhy> lifiSpectrumPhyTx=CreateObject<LifiSpectrumPhy>(lifiNetDeviceTx);
	Ptr<LifiSpectrumPhy> lifiSpectrumPhyRx=CreateObject<LifiSpectrumPhy>(lifiNetDeviceRx);
	Ptr<LifiPhy> lifiPhyTx=CreateObject<LifiPhy>(lifiSpectrumPhyTx);
	Ptr<LifiPhy> lifiPhyRx=CreateObject<LifiPhy>(lifiSpectrumPhyRx);
	Ptr<LifiSpectrumChannel> lifiSpectrumChannel=CreateObject<LifiSpectrumChannel>();
	Ptr<LifiMac> lifiMacTx=CreateObject<LifiMac>();
	LifiMac *PlifiMacTx=GetPointer(lifiMacTx);
	Ptr<LifiMac> lifiMacRx=CreateObject<LifiMac>();
	LifiMac *PlifiMacRx=GetPointer(lifiMacRx);
	Ptr<RandomPropagationDelayModel> randomPropagationDelayModel=CreateObject<RandomPropagationDelayModel>();

	Ptr<PlmeSapUser> plmeSapUserTx=Create<PlmeSpecificSapUser<LifiMac> >(PlifiMacTx);
	Ptr<PlmeSapUser> plmeSapUserRx=Create<PlmeSpecificSapUser<LifiMac> >(PlifiMacRx);
	Ptr<PdSapUser> pdSapUserTx=Create<PdSpecificSapUser<LifiMac> >(PlifiMacTx);
	Ptr<PdSapUser> pdSapUserRx=Create<PdSpecificSapUser<LifiMac> >(PlifiMacRx);

	Ptr<LifiSpectrumSignalParameters> lifiSpectrumSignalParametersTx=Create<LifiSpectrumSignalParameters>();
	Ptr<LifiSpectrumSignalParameters> lifiSpectrumSignalParametersRx=Create<LifiSpectrumSignalParameters>();
	Ptr<LifiInterference> lifiInterference=CreateObject<LifiInterference>();
	Ptr<LifiSpectrumPropagationLossModel> lifiSpectrumPropagationLossModel=CreateObject<LifiSpectrumPropagationLossModel>();

	lifiPhyTx->SetPlmeSapUser(plmeSapUserTx);
	lifiPhyRx->SetPlmeSapUser(plmeSapUserRx);
	lifiPhyTx->SetPdSapUser(pdSapUserTx);
	lifiPhyRx->SetPdSapUser(pdSapUserRx);

	//ConstantPositionMobilityModel
	Ptr<ConstantPositionMobilityModel> txMobility=CreateObject<ConstantPositionMobilityModel> ();
	Ptr<ConstantPositionMobilityModel> rxMobility=CreateObject<ConstantPositionMobilityModel> ();
	Vector3D txPosition(0,0,0);
	Vector3D rxPosition(2,2,2);
	txMobility->SetPosition(txPosition);
	txMobility->SetPosition(rxPosition);
	lifiSpectrumPhyTx->SetMobility(txMobility);
	lifiSpectrumPhyRx->SetMobility(rxMobility);
	lifiSpectrumPhyTx->SetInterference(lifiInterference);
	lifiSpectrumPhyRx->SetInterference(lifiInterference);
	lifiSpectrumPhyTx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersTx);
	lifiSpectrumPhyRx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersRx);
	lifiSpectrumPhyTx->SetChannel(lifiSpectrumChannel);
	lifiSpectrumPhyRx->SetChannel(lifiSpectrumChannel);

	lifiSpectrumChannel->SetPropagationDelayModel(randomPropagationDelayModel);
	lifiSpectrumChannel->AddSpectrumPropagationLossModel(lifiSpectrumPropagationLossModel);

	lifiNetDeviceTx->SetPhy(lifiPhyTx);
	lifiNetDeviceRx->SetPhy(lifiPhyRx);
	lifiNetDeviceTx->SetMac(lifiMacTx);
	lifiNetDeviceRx->SetMac(lifiMacRx);
	lifiNetDeviceTx->SetNode(nodeTx);
	lifiNetDeviceRx->SetNode(nodeRx);

	nodeTx->AddDevice(lifiNetDeviceTx);
	nodeRx->AddDevice(lifiNetDeviceRx);

//	lifiSpectrumSignalParametersTx->psd=spectrumValueTx;
//	lifiSpectrumSignalParametersRx->psd=spectrumValueRx;
	lifiSpectrumSignalParametersTx->txPhy=lifiSpectrumPhyTx;
	lifiSpectrumSignalParametersRx->txPhy=lifiSpectrumPhyRx;

	Ptr<LifiSpectrumErrorModel> lifiSpectrumErrroModel=CreateObject<LifiSpectrumErrorModel>();
	lifiInterference->SetErrorModel(lifiSpectrumErrroModel);

	lifiPhyTx->SetTRxState(TX_ON);
	lifiPhyRx->SetTRxState(RX_ON);
	lifiPhyTx->SetTxPower(30);
	lifiPhyTx->SetMcsId(1);
//	randomPropagationDelayModel->SetSpeed(1);
	Ptr<Packet> packet=Create<Packet>(100);
	lifiPhyTx->Transmit(100,packet,3);



	Simulator::Run ();
	Simulator::Stop(Seconds(50));
	Simulator::Destroy();
	return 0;
}



