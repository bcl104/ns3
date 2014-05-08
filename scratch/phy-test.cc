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
/*
 * single transmit single receive single cca
 */
int main ()
{
	LogComponentEnable ("LifiPhy", LOG_LEVEL_ALL);
	LogComponentEnable ("LifiSpectrumPhy", LOG_LEVEL_ALL);
	LogComponentEnable ("LifiSpectrumChannel", LOG_LEVEL_ALL);
	LogComponentEnable ("LifiInterference",LOG_LEVEL_ALL);

	Ptr<Node> nodeTx=CreateObject<Node>();
	Ptr<Node> nodeRx=CreateObject<Node>();
	Ptr<Node> nodeCca=CreateObject<Node>();

	Ptr<LifiNetDevice> lifiNetDeviceTx=CreateObject<LifiNetDevice>();
	Ptr<LifiNetDevice> lifiNetDeviceRx=CreateObject<LifiNetDevice>();
	Ptr<LifiNetDevice>  lifiNetDeviceCca=CreateObject<LifiNetDevice>();

	Ptr<LifiSpectrumPhy> lifiSpectrumPhyTx=CreateObject<LifiSpectrumPhy>(lifiNetDeviceTx);
	Ptr<LifiSpectrumPhy> lifiSpectrumPhyRx=CreateObject<LifiSpectrumPhy>(lifiNetDeviceRx);
	Ptr<LifiSpectrumPhy> lifiSpectrumPhyCca=CreateObject<LifiSpectrumPhy>(lifiNetDeviceCca);

	Ptr<LifiPhy> lifiPhyTx=CreateObject<LifiPhy>(lifiSpectrumPhyTx);
	Ptr<LifiPhy> lifiPhyRx=CreateObject<LifiPhy>(lifiSpectrumPhyRx);
	Ptr<LifiPhy> lifiPhyCca=CreateObject<LifiPhy>(lifiSpectrumPhyCca);

	Ptr<LifiSpectrumChannel> lifiSpectrumChannel=CreateObject<LifiSpectrumChannel>();

	Ptr<LifiMac> lifiMacTx=CreateObject<LifiMac>();
	LifiMac *PlifiMacTx=GetPointer(lifiMacTx);
	Ptr<LifiMac> lifiMacRx=CreateObject<LifiMac>();
	LifiMac *PlifiMacRx=GetPointer(lifiMacRx);
	Ptr<LifiMac> lifiMacCca=CreateObject<LifiMac>();
	LifiMac *PlifiMacCca=GetPointer(lifiMacCca);

	Ptr<RandomPropagationDelayModel> randomPropagationDelayModel=CreateObject<RandomPropagationDelayModel>();

	Ptr<PlmeSapUser> plmeSapUserTx=Create<PlmeSpecificSapUser<LifiMac> >(PlifiMacTx);
	Ptr<PlmeSapUser> plmeSapUserRx=Create<PlmeSpecificSapUser<LifiMac> >(PlifiMacRx);
	Ptr<PlmeSapUser> plmeSapUserCca=Create<PlmeSpecificSapUser<LifiMac> >(PlifiMacCca);
	Ptr<PdSapUser> pdSapUserTx=Create<PdSpecificSapUser<LifiMac> >(PlifiMacTx);
	Ptr<PdSapUser> pdSapUserRx=Create<PdSpecificSapUser<LifiMac> >(PlifiMacRx);
	Ptr<PdSapUser> pdSapUserCca=Create<PdSpecificSapUser<LifiMac> >(PlifiMacCca);

//
//	double centerFreq[]={500,1000,1500,2000,2500};
//	int count=sizeof(centerFreq)/sizeof(double);
//	std::vector<double> centerFreqs(centerFreq,centerFreq+count);
//	Ptr<SpectrumModel> spectrumModel=Create<SpectrumModel> (centerFreqs);
//	Ptr<SpectrumValue> spectrumValueTx=Create<SpectrumValue> (spectrumModel);
//	Ptr<SpectrumValue> spectrumValueRx=Create<SpectrumValue> (spectrumModel);

	Ptr<LifiSpectrumSignalParameters> lifiSpectrumSignalParametersTx=Create<LifiSpectrumSignalParameters>();
	Ptr<LifiSpectrumSignalParameters> lifiSpectrumSignalParametersRx=Create<LifiSpectrumSignalParameters>();
	Ptr<LifiSpectrumSignalParameters> lifiSpectrumSignalParametersCca=Create<LifiSpectrumSignalParameters>();

	Ptr<LifiInterference> lifiInterferenceTx=CreateObject<LifiInterference>();
	Ptr<LifiInterference> lifiInterferenceRx=CreateObject<LifiInterference>();
	Ptr<LifiInterference> lifiInterferenceCca=CreateObject<LifiInterference>();
	Ptr<LifiSpectrumPropagationLossModel> lifiSpectrumPropagationLossModel=CreateObject<LifiSpectrumPropagationLossModel>();
	Ptr<PropagationLossModel> lifiPropagationLossModel = CreateObject<LifiIndoorPropagationLossModel>();

//	lifiPhyTx->SetSpectrumValue(spectrumValueTx);
//	lifiPhyRx->SetSpectrumValue(spectrumValueRx);
	lifiPhyTx->SetPlmeSapUser(plmeSapUserTx);
	lifiPhyRx->SetPlmeSapUser(plmeSapUserRx);
	lifiPhyCca->SetPlmeSapUser(plmeSapUserCca);

	lifiPhyTx->SetPdSapUser(pdSapUserTx);
	lifiPhyRx->SetPdSapUser(pdSapUserRx);
	lifiPhyCca->SetPdSapUser(pdSapUserCca);

	//ConstantPositionMobilityModel
	Ptr<ConstantPositionMobilityModel> txMobility=CreateObject<ConstantPositionMobilityModel> ();
	Ptr<ConstantPositionMobilityModel> rxMobility=CreateObject<ConstantPositionMobilityModel> ();
	Ptr<ConstantPositionMobilityModel> ccaMobility=CreateObject<ConstantPositionMobilityModel> ();

	Vector3D txPosition(0,0,0);
	Vector3D rxPosition(1,1,1);
	Vector3D ccaPosition(3,3,3);

	txMobility->SetPosition(txPosition);
	rxMobility->SetPosition(rxPosition);
	ccaMobility->SetPosition(ccaPosition);

	lifiSpectrumPhyTx->SetMobility(txMobility);
	lifiSpectrumPhyRx->SetMobility(rxMobility);
	lifiSpectrumPhyCca->SetMobility(ccaMobility);

	lifiSpectrumPhyTx->SetInterference(lifiInterferenceTx);
	lifiSpectrumPhyRx->SetInterference(lifiInterferenceRx);
	lifiSpectrumPhyCca->SetInterference(lifiInterferenceCca);

	lifiSpectrumPhyTx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersTx);
	lifiSpectrumPhyRx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersRx);
	lifiSpectrumPhyCca->SetSpectrumSignalParameters(lifiSpectrumSignalParametersCca);

	lifiSpectrumPhyTx->SetChannel(lifiSpectrumChannel);
	lifiSpectrumPhyRx->SetChannel(lifiSpectrumChannel);
	lifiSpectrumPhyCca->SetChannel(lifiSpectrumChannel);

	lifiSpectrumChannel->SetPropagationDelayModel(randomPropagationDelayModel);
	lifiSpectrumChannel->AddSpectrumPropagationLossModel(lifiSpectrumPropagationLossModel);
	lifiSpectrumChannel->AddPropagationLossModel(lifiPropagationLossModel);

	lifiNetDeviceTx->SetPhy(lifiPhyTx);
	lifiNetDeviceRx->SetPhy(lifiPhyRx);
	lifiNetDeviceCca->SetPhy(lifiPhyCca);

	lifiNetDeviceTx->SetMac(lifiMacTx);
	lifiNetDeviceRx->SetMac(lifiMacRx);
	lifiNetDeviceCca->SetMac(lifiMacCca);

	lifiNetDeviceTx->SetNode(nodeTx);
	lifiNetDeviceRx->SetNode(nodeRx);
	lifiNetDeviceCca->SetNode(nodeCca);

	nodeTx->AddDevice(lifiNetDeviceTx);
	nodeRx->AddDevice(lifiNetDeviceRx);
	nodeCca->AddDevice(lifiNetDeviceCca);

//	lifiSpectrumSignalParametersTx->psd=spectrumValueTx;
//	lifiSpectrumSignalParametersRx->psd=spectrumValueRx;
	lifiSpectrumSignalParametersTx->txPhy=lifiSpectrumPhyTx;
	lifiSpectrumSignalParametersRx->txPhy=lifiSpectrumPhyRx;
	lifiSpectrumSignalParametersCca->txPhy=lifiSpectrumPhyCca;

	Ptr<LifiSpectrumErrorModel> lifiSpectrumErrroModel=CreateObject<LifiSpectrumErrorModel>();
//	lifiInterferenceTx->SetErrorModel(lifiSpectrumErrroModel);

	lifiPhyTx->SetTRxState(TX_ON);
	lifiPhyRx->SetTRxState(RX_ON);
	lifiPhyTx->SetTxPower(30);
	lifiPhyTx->SetMcsId(1);
//	randomPropagationDelayModel->SetSpeed(1);
	uint8_t *buffer=new uint8_t;
//	uint8_t *buffer[]={0X86,0X12,0X32};
	*buffer = 0x18;
	Ptr<Packet> packet=Create<Packet>(buffer,sizeof(buffer));
	uint8_t *buffers = new uint8_t;
	packet->CopyData(buffers,100);
	std::cout<<"send buffers:"<<(int)*buffers<<std::endl;
	std::cout<<"send packet size:"<<packet->GetSize()<<std::endl;
//	std::ofstream oso("abc.txt");
//	packet->Print(oso);
	lifiPhyTx->Transmit(packet->GetSize(),packet,3);
	lifiPhyCca->DoCca(3);




	Simulator::Run ();
	Simulator::Stop(Seconds(50));
	Simulator::Destroy();
	return 0;
}
	lifiSpectrumPhyTx->SetMobility(txMobility);
	lifiSpectrumPhyRx->SetMobility(rxMobility);
	lifiSpectrumPhyCca->SetMobility(ccaMobility);

	lifiSpectrumPhyTx->SetInterference(lifiInterferenceTx);
	lifiSpectrumPhyRx->SetInterference(lifiInterferenceRx);
	lifiSpectrumPhyCca->SetInterference(lifiInterferenceCca);

	lifiSpectrumPhyTx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersTx);
	lifiSpectrumPhyRx->SetSpectrumSignalParameters(lifiSpectrumSignalParametersRx);
	lifiSpectrumPhyCca->SetSpectrumSignalParameters(lifiSpectrumSignalParametersCca);

	lifiSpectrumPhyTx->SetChannel(lifiSpectrumChannel);
	lifiSpectrumPhyRx->SetChannel(lifiSpectrumChannel);
	lifiSpectrumPhyCca->SetChannel(lifiSpectrumChannel);

	lifiSpectrumChannel->SetPropagationDelayModel(randomPropagationDelayModel);
	lifiSpectrumChannel->AddSpectrumPropagationLossModel(lifiSpectrumPropagationLossModel);
	lifiSpectrumChannel->AddPropagationLossModel(lifiPropagationLossModel);

	lifiNetDeviceTx->SetPhy(lifiPhyTx);
	lifiNetDeviceRx->SetPhy(lifiPhyRx);
	lifiNetDeviceCca->SetPhy(lifiPhyCca);

	lifiNetDeviceTx->SetMac(lifiMacTx);
	lifiNetDeviceRx->SetMac(lifiMacRx);
	lifiNetDeviceCca->SetMac(lifiMacCca);

	lifiNetDeviceTx->SetNode(nodeTx);
	lifiNetDeviceRx->SetNode(nodeRx);
	lifiNetDeviceCca->SetNode(nodeCca);

	nodeTx->AddDevice(lifiNetDeviceTx);
	nodeRx->AddDevice(lifiNetDeviceRx);
	nodeCca->AddDevice(lifiNetDeviceCca);

//	lifiSpectrumSignalParametersTx->psd=spectrumValueTx;
//	lifiSpectrumSignalParametersRx->psd=spectrumValueRx;
	lifiSpectrumSignalParametersTx->txPhy=lifiSpectrumPhyTx;
	lifiSpectrumSignalParametersRx->txPhy=lifiSpectrumPhyRx;
	lifiSpectrumSignalParametersCca->txPhy=lifiSpectrumPhyCca;

	Ptr<LifiSpectrumErrorModel> lifiSpectrumErrroModel=CreateObject<LifiSpectrumErrorModel>();
//	lifiInterferenceTx->SetErrorModel(lifiSpectrumErrroModel);

	lifiPhyTx->SetTRxState(TX_ON);
	lifiPhyRx->SetTRxState(RX_ON);
	lifiPhyTx->SetTxPower(30);
	lifiPhyTx->SetMcsId(1);
//	randomPropagationDelayModel->SetSpeed(1);
	uint8_t *buffer=new uint8_t;
//	uint8_t *buffer[]={0X86,0X12,0X32};
	*buffer = 0x18;
	Ptr<Packet> packet=Create<Packet>(buffer,sizeof(buffer));
	uint8_t *buffers = new uint8_t;
	packet->CopyData(buffers,100);
	std::cout<<"send buffers:"<<(int)*buffers<<std::endl;
	std::cout<<"send packet size:"<<packet->GetSize()<<std::endl;
//	std::ofstream oso("abc.txt");
//	packet->Print(oso);
	lifiPhyTx->Transmit(packet->GetSize(),packet,3);
	lifiPhyCca->DoCca(3);




	Simulator::Run ();
	Simulator::Stop(Seconds(50));
	Simulator::Destroy();
	return 0;
}



