/*
 * lifi-coord-dev-phy-mac-test.cc

 *
 *  Created on: 2014年5月12日
 *      Author: st125475466
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-delay-model.h"
NS_LOG_COMPONENT_DEFINE("LifiCoordDevPhyMacTest");
using namespace ns3;
class SinglePhyTest {
public:
	SinglePhyTest();
	~SinglePhyTest(){

	}
	Ptr<PlmeSapProvider> GetPlmeSapProviderTx();
	Ptr<PlmeSapProvider> GetPlmeSapProviderRx();
	Ptr<PlmeSapProvider> GetPlmeSapProviderCca();
	Ptr<PdSapProvider> GetPdSapProviderTx();
	Ptr<PdSapProvider> GetPdSapProviderRx();
	Ptr<PdSapProvider> GetPdSapProviderCca();
	Ptr<Node> GetNodeTx();
	Ptr<Node> GetNodeRx();
	Ptr<Node> nodeCca();
	Ptr<LifiNetDevice> GetLifiNetDeviceTx();
	Ptr<LifiNetDevice> GetLifiNetDeviceRx();
	Ptr<LifiNetDevice> GetLifiNetDeviceCca();
	Ptr<LifiSpectrumPhy> GetLifiSpectrumPhyTx();
	Ptr<LifiSpectrumPhy> GetLifiSpectrumPhyRx();
	Ptr<LifiSpectrumPhy> GetLifiSpectrumPhyCca();
	Ptr<LifiPhy> GetLifiPhyTx();
	Ptr<LifiPhy> GetLifiPhyRx();
	Ptr<LifiPhy> GetLifiPhyCca();
	Ptr<LifiSpectrumChannel> GetLifiSpectrumChannel();
	Ptr<RandomPropagationDelayModel> GetRandomPropagationDelayModel();
	Ptr<LifiSpectrumPropagationLossModel> GetLifiSpectrumPropagationLossModel();\
	Ptr<PropagationLossModel> GetPropagationLossModel();
	Ptr<LifiSpectrumErrorModel> GetLifiSpectrumErrorModel();
	Ptr<PlmeSapUser> GetPlmeSapUserTx();
	Ptr<PlmeSapUser> GetPlmeSapUserRx();
	Ptr<PlmeSapUser> GetPlmeSapUserCca();
	Ptr<PdSapUser> GetPdSapUserTx();
	Ptr<PdSapUser> GetPdSapUserRx();
	Ptr<PdSapUser> GetPdSapUserCca();
	Ptr<LifiSpectrumSignalParameters> GetLifiSpectrumSignalParametersTx();
	Ptr<LifiSpectrumSignalParameters> GetLifiSpectrumSignalParametersRx();
	Ptr<LifiSpectrumSignalParameters> GetLifiSpectrumSignalParametersCca();
	Ptr<LifiInterference> GetLifiInterferenceTx();
	Ptr<LifiInterference> GetLifiInterferenceRx();
	Ptr<LifiInterference> GetLifiInterferenceCca();
	Ptr<ConstantPositionMobilityModel> GetConstantPositionMobilityModelTx();
	Ptr<ConstantPositionMobilityModel> GetConstantPositionMobilityModelRx();
	Ptr<ConstantPositionMobilityModel> GetConstantPositionMobilityModelCca();
	Vector3D GetTxPosition();
	Vector3D GetRxPosition();
	Vector3D GetCcaPosition();
	Ptr<LifiMac> GetLifiMacTx();
	Ptr<LifiMac> GetLifiMacRx();
	Ptr<LifiMac> GetLifiMacCca();
	LifiMac *GetPlifiMacTx();
	LifiMac *GetPlifiMacRx();
	LifiMac *GetPlifiMacCca();
	/*
	 * initialization:txPosition:(0,0,0)
	 *            rxPosition(1,1,1)
	 *            ccaPosition(3,3,3)	 *
	*/
	void SetTxPosition(double x , double y , double z);
	void SetRxPosition(double x , double y , double z);
	void SetCcaPosition(double x , double y , double z);
	/*
	 * initialization:TxState:TX_ON
	 * 				  RxState:RX_ON
	*/
	void SetTxState(PhyOpStatus state);
	void SetRxState(PhyOpStatus state);
	/*
	 * initialization:TxPower 30dbm
	*/
	void SetTxPower(double txPower);
	/*
	 * initialization: SettMcsId 1
	*/
	void SettMcsId(uint8_t mcsid);


private:
	Ptr<Node> m_nodeTx;
	Ptr<Node> m_nodeRx;
	Ptr<Node> m_nodeCca;
	Ptr<LifiNetDevice> m_lifiNetDeviceTx;
	Ptr<LifiNetDevice> m_lifiNetDeviceRx;
	Ptr<LifiNetDevice> m_lifiNetDeviceCca;
	Ptr<LifiSpectrumPhy> m_lifiSpectrumPhyTx;
	Ptr<LifiSpectrumPhy> m_lifiSpectrumPhyRx;
	Ptr<LifiSpectrumPhy> m_lifiSpectrumPhyCca;
	Ptr<LifiPhy> m_lifiPhyTx;
	Ptr<LifiPhy> m_lifiPhyRx;
	Ptr<LifiPhy> m_lifiPhyCca;
	Ptr<LifiSpectrumChannel> m_lifiSpectrumChannel;
	Ptr<RandomPropagationDelayModel> m_randomPropagationDelayModel;
	Ptr<LifiSpectrumPropagationLossModel> m_lifiSpectrumPropagationLossModel;
	Ptr<PropagationLossModel> m_lifiPropagationLossModel;
	Ptr<LifiSpectrumErrorModel> m_lifiSpectrumErrroModel;
	Ptr<PlmeSapUser> m_plmeSapUserTx;
	Ptr<PlmeSapUser> m_plmeSapUserRx;
	Ptr<PlmeSapUser> m_plmeSapUserCca;
	Ptr<PlmeSapProvider> m_plmeSapProviderTx;
	Ptr<PlmeSapProvider> m_plmeSapProviderRx;
	Ptr<PlmeSapProvider> m_plmeSapProviderCca;
	Ptr<PdSapUser> m_pdSapUserTx;
	Ptr<PdSapUser> m_pdSapUserRx;
	Ptr<PdSapUser> m_pdSapUserCca;
	Ptr<PdSapProvider> m_pdSapProviderTx;
	Ptr<PdSapProvider> m_pdSapProviderRx;
	Ptr<PdSapProvider> m_pdSapProviderCca;
	Ptr<LifiSpectrumSignalParameters> m_lifiSpectrumSignalParametersTx;
	Ptr<LifiSpectrumSignalParameters> m_lifiSpectrumSignalParametersRx;
	Ptr<LifiSpectrumSignalParameters> m_lifiSpectrumSignalParametersCca;
	Ptr<LifiInterference> m_lifiInterferenceTx;
	Ptr<LifiInterference> m_lifiInterferenceRx;
	Ptr<LifiInterference> m_lifiInterferenceCca;
	Ptr<ConstantPositionMobilityModel> m_txMobility;
	Ptr<ConstantPositionMobilityModel> m_rxMobility;
	Ptr<ConstantPositionMobilityModel> m_ccaMobility;
	Vector3D m_txPosition;
	Vector3D m_rxPosition;
	Vector3D m_ccaPosition;
	Ptr<LifiMac> m_lifiMacTx;
	Ptr<LifiMac> m_lifiMacRx;
	Ptr<LifiMac> m_lifiMacCca;
	LifiMac *m_PlifiMacTx;
	LifiMac *m_PlifiMacRx;
	LifiMac *m_PlifiMacCca;
};
SinglePhyTest::SinglePhyTest(){

	m_nodeTx=CreateObject<Node>();
	m_nodeRx=CreateObject<Node>();
	m_nodeCca=CreateObject<Node>();

	m_lifiNetDeviceTx=CreateObject<LifiNetDevice>();
	m_lifiNetDeviceRx=CreateObject<LifiNetDevice>();
	m_lifiNetDeviceCca=CreateObject<LifiNetDevice>();

	m_lifiSpectrumPhyTx=CreateObject<LifiSpectrumPhy>(m_lifiNetDeviceTx);
	m_lifiSpectrumPhyRx=CreateObject<LifiSpectrumPhy>(m_lifiNetDeviceRx);
	m_lifiSpectrumPhyCca=CreateObject<LifiSpectrumPhy>(m_lifiNetDeviceCca);

	m_lifiPhyTx=CreateObject<LifiPhy>(m_lifiSpectrumPhyTx);
	m_lifiPhyRx=CreateObject<LifiPhy>(m_lifiSpectrumPhyRx);
	m_lifiPhyCca=CreateObject<LifiPhy>(m_lifiSpectrumPhyCca);

	m_lifiSpectrumChannel=CreateObject<LifiSpectrumChannel>();

	m_lifiMacTx=CreateObject<LifiMac>();
	m_PlifiMacTx=GetPointer(m_lifiMacTx);
	m_lifiMacRx=CreateObject<LifiMac>();
	m_PlifiMacRx=GetPointer(m_lifiMacRx);
	m_lifiMacCca=CreateObject<LifiMac>();
	m_PlifiMacCca=GetPointer(m_lifiMacCca);

	m_randomPropagationDelayModel=CreateObject<RandomPropagationDelayModel>();

	m_plmeSapUserTx=Create<PlmeSpecificSapUser<LifiMac> >(m_PlifiMacTx);
	m_plmeSapUserRx=Create<PlmeSpecificSapUser<LifiMac> >(m_PlifiMacRx);
	m_plmeSapUserCca=Create<PlmeSpecificSapUser<LifiMac> >(m_PlifiMacCca);
	LifiPhy *m_PlifiPhyTx=GetPointer(m_lifiPhyTx);
	LifiPhy *m_PlifiPhyRx=GetPointer(m_lifiPhyRx);
	LifiPhy *m_PlifiPhyCca=GetPointer(m_lifiPhyCca);
	m_plmeSapProviderTx=Create<PlmeSpecificSapProvider<LifiPhy> >(m_PlifiPhyTx);
	m_plmeSapProviderRx=Create<PlmeSpecificSapProvider<LifiPhy> >(m_PlifiPhyRx);
	m_plmeSapProviderCca=Create<PlmeSpecificSapProvider<LifiPhy> >(m_PlifiPhyCca);

	m_pdSapUserTx=Create<PdSpecificSapUser<LifiMac> >(m_PlifiMacTx);
	m_pdSapUserRx=Create<PdSpecificSapUser<LifiMac> >(m_PlifiMacRx);
	m_pdSapUserCca=Create<PdSpecificSapUser<LifiMac> >(m_PlifiMacCca);

	m_pdSapProviderTx=Create<PdSpecificSapProvider<LifiPhy> >(m_PlifiPhyTx);
	m_pdSapProviderRx=Create<PdSpecificSapProvider<LifiPhy> >(m_PlifiPhyRx);
	m_pdSapProviderCca=Create<PdSpecificSapProvider<LifiPhy> >(m_PlifiPhyCca);

//
//	double centerFreq[]={500,1000,1500,2000,2500};
//	int count=sizeof(centerFreq)/sizeof(double);
//	std::vector<double> centerFreqs(centerFreq,centerFreq+count);
//	Ptr<SpectrumModel> spectrumModel=Create<SpectrumModel> (centerFreqs);
//	Ptr<SpectrumValue> spectrumValueTx=Create<SpectrumValue> (spectrumModel);
//	Ptr<SpectrumValue> spectrumValueRx=Create<SpectrumValue> (spectrumModel);

	m_lifiSpectrumSignalParametersTx=Create<LifiSpectrumSignalParameters>();
	m_lifiSpectrumSignalParametersRx=Create<LifiSpectrumSignalParameters>();
	m_lifiSpectrumSignalParametersCca=Create<LifiSpectrumSignalParameters>();

	m_lifiInterferenceTx=CreateObject<LifiInterference>();
	m_lifiInterferenceRx=CreateObject<LifiInterference>();
	m_lifiInterferenceCca=CreateObject<LifiInterference>();
	m_lifiSpectrumPropagationLossModel=CreateObject<LifiSpectrumPropagationLossModel>();
	m_lifiPropagationLossModel = CreateObject<LifiIndoorPropagationLossModel>();

//	m_lifiPhyTx->SetSpectrumValue(spectrumValueTx);
//	m_lifiPhyRx->SetSpectrumValue(spectrumValueRx);
	m_lifiPhyTx->SetPlmeSapUser(m_plmeSapUserTx);
	m_lifiPhyRx->SetPlmeSapUser(m_plmeSapUserRx);
	m_lifiPhyCca->SetPlmeSapUser(m_plmeSapUserCca);


	m_lifiPhyTx->SetPdSapUser(m_pdSapUserTx);
	m_lifiPhyRx->SetPdSapUser(m_pdSapUserRx);
	m_lifiPhyCca->SetPdSapUser(m_pdSapUserCca);

	//ConstantPositionMobilityModel
	m_txMobility=CreateObject<ConstantPositionMobilityModel> ();
	m_rxMobility=CreateObject<ConstantPositionMobilityModel> ();
	m_ccaMobility=CreateObject<ConstantPositionMobilityModel> ();

	m_txPosition.x = 0;
	m_txPosition.y = 0;
	m_txPosition.z = 0;
	m_rxPosition.x = 1;
	m_rxPosition.y = 1;
	m_rxPosition.z = 1;
	m_ccaPosition.x = 3;
	m_ccaPosition.y = 3;
	m_ccaPosition.z = 3;

	m_txMobility->SetPosition(m_txPosition);
	m_rxMobility->SetPosition(m_rxPosition);
	m_ccaMobility->SetPosition(m_ccaPosition);

	m_lifiSpectrumPhyTx->SetMobility(m_txMobility);
	m_lifiSpectrumPhyRx->SetMobility(m_rxMobility);
	m_lifiSpectrumPhyCca->SetMobility(m_ccaMobility);

	m_lifiSpectrumPhyTx->SetInterference(m_lifiInterferenceTx);
	m_lifiSpectrumPhyRx->SetInterference(m_lifiInterferenceRx);
	m_lifiSpectrumPhyCca->SetInterference(m_lifiInterferenceCca);

	m_lifiSpectrumPhyTx->SetSpectrumSignalParameters(m_lifiSpectrumSignalParametersTx);
	m_lifiSpectrumPhyRx->SetSpectrumSignalParameters(m_lifiSpectrumSignalParametersRx);
	m_lifiSpectrumPhyCca->SetSpectrumSignalParameters(m_lifiSpectrumSignalParametersCca);

	m_lifiSpectrumPhyTx->SetChannel(m_lifiSpectrumChannel);
	m_lifiSpectrumPhyRx->SetChannel(m_lifiSpectrumChannel);
	m_lifiSpectrumPhyCca->SetChannel(m_lifiSpectrumChannel);

	m_lifiSpectrumChannel->SetPropagationDelayModel(m_randomPropagationDelayModel);
	m_lifiSpectrumChannel->AddSpectrumPropagationLossModel(m_lifiSpectrumPropagationLossModel);
	m_lifiSpectrumChannel->AddPropagationLossModel(m_lifiPropagationLossModel);

	m_lifiNetDeviceTx->SetPhy(m_lifiPhyTx);
	m_lifiNetDeviceRx->SetPhy(m_lifiPhyRx);
	m_lifiNetDeviceCca->SetPhy(m_lifiPhyCca);

	m_lifiNetDeviceTx->SetMac(m_lifiMacTx);
	m_lifiNetDeviceRx->SetMac(m_lifiMacRx);
	m_lifiNetDeviceCca->SetMac(m_lifiMacCca);

	m_lifiNetDeviceTx->SetNode(m_nodeTx);
	m_lifiNetDeviceRx->SetNode(m_nodeRx);
	m_lifiNetDeviceCca->SetNode(m_nodeCca);

	m_nodeTx->AddDevice(m_lifiNetDeviceTx);
	m_nodeRx->AddDevice(m_lifiNetDeviceRx);
	m_nodeCca->AddDevice(m_lifiNetDeviceCca);

//	m_lifiNetlifiSpectrumm_SignalParametersTxDeviceCca->psd=spectrumValueTx;
//	lifiSpectrumSignalParametersRx->psd=spectrumValueRx;
	m_lifiSpectrumSignalParametersTx->txPhy=m_lifiSpectrumPhyTx;
	m_lifiSpectrumSignalParametersRx->txPhy=m_lifiSpectrumPhyRx;
	m_lifiSpectrumSignalParametersCca->txPhy=m_lifiSpectrumPhyCca;

	m_lifiSpectrumErrroModel=CreateObject<LifiSpectrumErrorModel>();
//	lifiInterferenceTx->SetErrorModel(lifiSpectrumErrroModel);

	m_lifiMacTx->SetPdSapProvider(m_lifiPhyTx->GetPdSapProvider());
	m_lifiMacTx->SetPlmeSapProvider(m_lifiPhyTx->GetPlmeSapProvider());
	m_lifiMacTx->SetDevice(m_lifiNetDeviceTx);
	m_lifiMacRx->SetPdSapProvider(m_lifiPhyRx->GetPdSapProvider());
	m_lifiMacRx->SetPlmeSapProvider(m_lifiPhyRx->GetPlmeSapProvider());
	m_lifiMacRx->SetDevice(m_lifiNetDeviceRx);
	m_lifiPhyTx->SetPdSapUser(m_lifiMacTx->GetPdSapUser());
	m_lifiPhyTx->SetPlmeSapUser(m_lifiMacTx->GetPlmeSapUser());
	m_lifiPhyRx->SetPdSapUser(m_lifiMacRx->GetPdSapUser());
	m_lifiPhyRx->SetPlmeSapUser(m_lifiMacRx->GetPlmeSapUser());
	m_lifiMacTx->SetOpticalPeriod(m_lifiPhyTx->GetOpticClock());
	m_lifiMacRx->SetOpticalPeriod(m_lifiPhyRx->GetOpticClock());


	m_lifiPhyTx->SetTRxState(TX_ON);
	m_lifiPhyRx->SetTRxState(RX_ON);
	m_lifiPhyTx->SetTxPower(30);
	m_lifiPhyRx->SetTxPower(30);
//	m_lifiPhyTx->SetMcsId(1);


//	uint8_t *buffer=new uint8_t;
//	*buffer = 0x18;
//	Ptr<Packet> packet=Create<Packet>(buffer,sizeof(buffer));
//	uint8_t *buffers = new uint8_t;
//	packet->CopyData(buffers,100);
//	std::cout<<"send buffers:"<<(int)*buffers<<std::endl;
//	std::cout<<"send packet size:"<<packet->GetSize()<<std::endl;
//	m_lifiPhyTx->Transmit(packet->GetSize(),packet,3);
//	m_lifiPhyCca->DoCca(3);
//	Simulator::Run ();
//	Simulator::Stop(Seconds(50));
//	Simulator::Destroy();
}
	void SinglePhyTest::SetTxPosition(double x , double y , double z){
		NS_LOG_FUNCTION(this);
		m_txPosition.x=x;
		m_txPosition.y=y;
		m_txPosition.z=z;
		m_txMobility->SetPosition(m_txPosition);
	}

	void SinglePhyTest::SetRxPosition(double x , double y , double z){
		NS_LOG_FUNCTION(this);
		m_txPosition.x=x;
		m_txPosition.y=y;
		m_txPosition.z=z;
		m_rxMobility->SetPosition(m_rxPosition);
	}

	void SinglePhyTest::SetCcaPosition(double x , double y , double z){
		NS_LOG_FUNCTION(this);
		m_txPosition.x=x;
		m_txPosition.y=y;
		m_txPosition.z=z;
		m_ccaMobility->SetPosition(m_ccaPosition);
	}

	void SinglePhyTest::SetTxState(PhyOpStatus state){
		NS_LOG_FUNCTION(this);
		m_lifiPhyTx->SetTRxState(state);
	}

	void SinglePhyTest::SetRxState(PhyOpStatus state){
		NS_LOG_FUNCTION(this);
		m_lifiPhyRx->SetTRxState(state);
	}

	void SinglePhyTest::SetTxPower(double txPower){
		NS_LOG_FUNCTION(this);
		m_lifiPhyTx->SetTxPower(30);
	}

	void SinglePhyTest::SettMcsId(uint8_t mcsid){
		NS_LOG_FUNCTION(this);
		m_lifiPhyTx->SetMcsId(mcsid);
	}
	Ptr<PlmeSapProvider> SinglePhyTest::GetPlmeSapProviderTx(){
		NS_LOG_FUNCTION(this);
		return m_plmeSapProviderTx;
	}
	Ptr<PlmeSapProvider> SinglePhyTest::GetPlmeSapProviderRx(){
		NS_LOG_FUNCTION(this);
		return m_plmeSapProviderRx;
	}

	Ptr<PlmeSapProvider> SinglePhyTest::GetPlmeSapProviderCca(){
		NS_LOG_FUNCTION(this);
		return m_plmeSapProviderCca;
	}
	Ptr<PdSapProvider> SinglePhyTest::GetPdSapProviderTx(){
		NS_LOG_FUNCTION(this);
		return m_pdSapProviderTx;
	}
	Ptr<PdSapProvider> SinglePhyTest::GetPdSapProviderRx(){
		NS_LOG_FUNCTION(this);
		return m_pdSapProviderRx;
	}
	Ptr<PdSapProvider> SinglePhyTest::GetPdSapProviderCca(){
		NS_LOG_FUNCTION(this);
		return m_pdSapProviderCca;
	}

	Ptr<LifiMac> SinglePhyTest::GetLifiMacTx(){
		NS_LOG_FUNCTION(this);
		return m_lifiMacTx;
	}
	Ptr<LifiMac> SinglePhyTest::GetLifiMacRx(){
		NS_LOG_FUNCTION(this);
		return m_lifiMacRx;
	}
	Ptr<LifiMac> SinglePhyTest::GetLifiMacCca(){
		NS_LOG_FUNCTION(this);
		return m_lifiMacCca;
	}

int main(){
//	char huanjing[] ="NS_LOG=*=prefix_time";
//	std::cout << putenv(huanjing) << std::endl;
//	std::cout<<5 <<std::endl;
	SinglePhyTest _test;
	LogComponentEnable("LifiMac", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiMac", LOG_PREFIX_TIME);
	LogComponentEnable("LifiMacCoordImpl", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiMacCoordImpl", LOG_PREFIX_TIME);
	LogComponentEnable("LifiMacDevImpl", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiMacDevImpl", LOG_PREFIX_TIME);
	LogComponentEnable("LifiTrxHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiTrxHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiCoordTrxHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiCoordTrxHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDevTrxHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDevTrxHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDevAssocHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDevAssocHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiCoordAssocHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiCoordAssocHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiTransactionHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiTransactionHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDataDevHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDataDevHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDataCoordHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDataCoordHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDisassocDevHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDisassocDevHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiDisassocCoordHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiDisassocCoordHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiGtsDevHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiGtsDevHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiGtsCoordHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiGtsCoordHandler", LOG_PREFIX_TIME);
	LogComponentEnable("LifiGtsHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiGtsHandler", LOG_PREFIX_TIME);
//	LogComponentEnable("Timer", LOG_LEVEL_FUNCTION);
//	LogComponentEnable ("LifiPhy", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiSpectrumPhy", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiSpectrumChannel", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiInterference",LOG_LEVEL_ALL);

	_test.GetLifiMacTx()->StartVPAN(0x01, CHANNEL1, 0, 8, 8, true);
	_test.GetLifiMacRx()->Reset();
//	TxOption op;
//	op.ackTx = false;
//	op.indirectTx = false;
//	op.gtsTx = false;
//
//	_test.GetLifiMacRx()->Send(LifiMac::GetTypeId(),
//							   LifiMac::GetTypeId(),
//							   0x01,
//							   Address(Mac16Address("ff:ff")),
//							   1,
//							   Create<Packet> (1),
//							   2,
//							   op,
//							   PHY_III_24_00_MBPS,
//							   false);

//	CapabilityInfo info;
//	info.CoordCapabi = true;
//	info.SecCapabi = false;
//	info.TrafficSup = false;
//	info.batteryInfo = 0x02;
//	info.powerSource = true;
//	_test.GetLifiMacRx()->Associate(CHANNEL1, EXTENDED, 0x01,
//									Mac64Address ("00:00:00:00:00:00:00:01"), info);

//	TxOption op;
//	op.ackTx = true;
//	op.indirectTx = false;
//	op.gtsTx = false;
//	_test.GetLifiMacTx()->Send(EXTENDED, EXTENDED, 0x01,
//							   Mac64Address ("00:00:00:00:00:00:00:02"), 10, Create<Packet> (10),
//							   45, op, PHY_III_24_00_MBPS, false);

//	_test.GetLifiMacRx()->Disassociate(EXTENDED, 0x01,
//									   Mac64Address ("00:00:00:00:00:00:00:01"), DEVICE, false);

//	_test.GetLifiMacTx()->Disassociate(EXTENDED, 0x01,
//									   Mac64Address ("00:00:00:00:00:00:00:02"), COORD, true);

//	GTSCharacteristics character;
//	character.gtsLength = 3;
//	_test.GetLifiMacRx()->GtsRequest(character, Mac64Address ("00:00:00:00:00:00:00:01"));

	GTSCharacteristics character;
	character.gtsLength = 3;
	_test.GetLifiMacTx()->GtsRequest(character, Mac16Address ("55:55"));

	Simulator::Stop(Seconds(8));
	Simulator::Run ();
	Simulator::Destroy();

	return 0;
}
