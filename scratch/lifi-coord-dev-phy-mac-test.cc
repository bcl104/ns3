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
	Ptr<LifiSpectrumPropagationLossModel> GetLifiSpectrumPropagationLossModel();
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

	Ptr<LifiNodeCoord> GetLifiNodeCoord();
	Ptr<LifiNodeDev> GetLifiNodeDev();
	Ptr<LifiNodeDev> GetLifiNodeDev1();

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
	void SetCcaState(PhyOpStatus state);
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
	Ptr<MlmeSapUser> m_mlmeSapUserTx;
	Ptr<MlmeSapUser> m_mlmeSapUserRx;
	Ptr<MlmeSapUser> m_mlmeSapUserCca;
	Ptr<McpsSapUser> m_mcpsSapUserTx;
	Ptr<McpsSapUser> m_mcpsSapUserRx;
	Ptr<McpsSapUser> m_mcpsSapUserCca;
	Ptr<MlmeSapProvider> m_mlmeSapProviderTx;
	Ptr<MlmeSapProvider> m_mlmeSapProviderRx;
	Ptr<MlmeSapProvider> m_mlmeSapProviderCca;
	Ptr<McpsSapProvider> m_mcpsSapProviderTx;
	Ptr<McpsSapProvider> m_mcpsSapProviderRx;
	Ptr<McpsSapProvider> m_mcpsSapProviderCca;
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

	Ptr<LifiNodeCoord> m_lifiNodeCoord;
	Ptr<LifiNodeDev> m_lifiNodeDev;
	Ptr<LifiNodeDev> m_lifiNodeDev1;
	LifiNodeCoord *m_MlifiNodeCoord;
	LifiNodeDev *m_MlifiNodeDev;
	LifiNodeDev *m_MlifiNodeDev1;
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

	m_lifiNodeCoord=CreateObject<LifiNodeCoord>();
	m_lifiNodeDev=CreateObject<LifiNodeDev>();
	m_lifiNodeDev1=CreateObject<LifiNodeDev>();
	m_MlifiNodeCoord=GetPointer(m_lifiNodeCoord);
	m_MlifiNodeDev=GetPointer(m_lifiNodeDev);
	m_MlifiNodeDev1=GetPointer(m_lifiNodeDev1);

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

	m_mcpsSapProviderTx=Create<McpsSpecificSapProvider<LifiMac> >(m_PlifiMacTx);
	m_mcpsSapProviderRx=Create<McpsSpecificSapProvider<LifiMac> >(m_PlifiMacRx);
	m_mcpsSapProviderCca=Create<McpsSpecificSapProvider<LifiMac> >(m_PlifiMacCca);

	m_lifiSpectrumSignalParametersTx=Create<LifiSpectrumSignalParameters>();
	m_lifiSpectrumSignalParametersRx=Create<LifiSpectrumSignalParameters>();
	m_lifiSpectrumSignalParametersCca=Create<LifiSpectrumSignalParameters>();

	m_lifiInterferenceTx=CreateObject<LifiInterference>();
	m_lifiInterferenceRx=CreateObject<LifiInterference>();
	m_lifiInterferenceCca=CreateObject<LifiInterference>();
	m_lifiSpectrumPropagationLossModel=CreateObject<LifiSpectrumPropagationLossModel>();
	m_lifiPropagationLossModel = CreateObject<LifiIndoorPropagationLossModel>();

	m_lifiPhyTx->SetPlmeSapUser(m_plmeSapUserTx);
	m_lifiPhyRx->SetPlmeSapUser(m_plmeSapUserRx);
	m_lifiPhyCca->SetPlmeSapUser(m_plmeSapUserCca);


	m_lifiPhyTx->SetPdSapUser(m_pdSapUserTx);
	m_lifiPhyRx->SetPdSapUser(m_pdSapUserRx);
	m_lifiPhyCca->SetPdSapUser(m_pdSapUserCca);

	m_lifiMacTx->SetMlmeSapUser(m_lifiNodeCoord->GetMlmeSapUser());
	m_lifiMacRx->SetMlmeSapUser(m_lifiNodeDev->GetMlmeSapUser());
	m_lifiMacCca->SetMlmeSapUser(m_lifiNodeDev1->GetMlmeSapUser());

	m_lifiMacTx->SetMcpsSapUser(m_lifiNodeCoord->GetMcpsSapUser());
	m_lifiMacRx->SetMcpsSapUser(m_lifiNodeDev->GetMcpsSapUser());
	m_lifiMacCca->SetMcpsSapUser(m_lifiNodeDev1->GetMcpsSapUser());


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

	m_lifiSpectrumSignalParametersTx->txPhy=m_lifiSpectrumPhyTx;
	m_lifiSpectrumSignalParametersRx->txPhy=m_lifiSpectrumPhyRx;
	m_lifiSpectrumSignalParametersCca->txPhy=m_lifiSpectrumPhyCca;

	m_lifiSpectrumErrroModel=CreateObject<LifiSpectrumErrorModel>();
////	lifiInterferenceTx->SetErrorModel(lifiSpectrumErrroModel);
	m_lifiNodeCoord->SetMcpsSapProvider(m_lifiMacTx->GetMcpsSapProvider());
	m_lifiNodeCoord->SetMlmeSapProvider(m_lifiMacTx->GetMlmeSapProvider());
	m_lifiNodeDev->SetMcpsSapProvider(m_lifiMacRx->GetMcpsSapProvider());
	m_lifiNodeDev->SetMlmeSapProvider(m_lifiMacRx->GetMlmeSapProvider());
	m_lifiNodeDev1->SetMcpsSapProvider(m_lifiMacCca->GetMcpsSapProvider());
	m_lifiNodeDev1->SetMlmeSapProvider(m_lifiMacCca->GetMlmeSapProvider());
////	m_lifiNodeCoord->SetLifiMac(m_lifiMacTx);
	m_lifiNodeDev->SetLifiMac(m_lifiMacRx);
	m_lifiNodeDev1->SetLifiMac(m_lifiMacCca);
	m_lifiMacTx->SetMlmeSapUser(m_lifiNodeCoord->GetMlmeSapUser());
	m_lifiMacTx->SetMcpsSapUser(m_lifiNodeCoord->GetMcpsSapUser());
	m_lifiMacRx->SetMlmeSapUser(m_lifiNodeDev->GetMlmeSapUser());
	m_lifiMacRx->SetMcpsSapUser(m_lifiNodeDev->GetMcpsSapUser());
	m_lifiMacCca->SetMlmeSapUser(m_lifiNodeDev1->GetMlmeSapUser());
	m_lifiMacCca->SetMcpsSapUser(m_lifiNodeDev1->GetMcpsSapUser());
	m_lifiMacTx->SetLifiNode(m_lifiNodeCoord);
	m_lifiMacRx->SetLifiNode(m_lifiNodeDev);
	m_lifiMacCca->SetLifiNode(m_lifiNodeDev1);

	m_lifiMacTx->SetPdSapProvider(m_lifiPhyTx->GetPdSapProvider());
	m_lifiMacTx->SetPlmeSapProvider(m_lifiPhyTx->GetPlmeSapProvider());
	m_lifiMacTx->SetDevice(m_lifiNetDeviceTx);
	m_lifiMacRx->SetPdSapProvider(m_lifiPhyRx->GetPdSapProvider());
	m_lifiMacRx->SetPlmeSapProvider(m_lifiPhyRx->GetPlmeSapProvider());
	m_lifiMacRx->SetDevice(m_lifiNetDeviceRx);
	m_lifiMacCca->SetPdSapProvider(m_lifiPhyCca->GetPdSapProvider());
	m_lifiMacCca->SetPlmeSapProvider(m_lifiPhyCca->GetPlmeSapProvider());
	m_lifiMacCca->SetDevice(m_lifiNetDeviceCca);
	m_lifiPhyTx->SetPdSapUser(m_lifiMacTx->GetPdSapUser());
	m_lifiPhyTx->SetPlmeSapUser(m_lifiMacTx->GetPlmeSapUser());
	m_lifiPhyRx->SetPdSapUser(m_lifiMacRx->GetPdSapUser());
	m_lifiPhyRx->SetPlmeSapUser(m_lifiMacRx->GetPlmeSapUser());
	m_lifiPhyCca->SetPdSapUser(m_lifiMacCca->GetPdSapUser());
	m_lifiPhyCca->SetPlmeSapUser(m_lifiMacCca->GetPlmeSapUser());
	m_lifiMacTx->SetOpticalPeriod(m_lifiPhyTx->GetOpticClock());
	m_lifiMacRx->SetOpticalPeriod(m_lifiPhyRx->GetOpticClock());
	m_lifiMacCca->SetOpticalPeriod(m_lifiPhyCca->GetOpticClock());

	m_lifiPhyTx->SetTRxState(TX_ON);
	m_lifiPhyRx->SetTRxState(RX_ON);
	m_lifiPhyCca->SetTRxState(RX_ON);
	m_lifiPhyTx->SetTxPower(30);
	m_lifiPhyRx->SetTxPower(30);
	m_lifiPhyCca->SetTxPower(30);

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

	void SinglePhyTest::SetCcaState(PhyOpStatus state){
		NS_LOG_FUNCTION(this);
		m_lifiPhyCca->SetTRxState(state);
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

	Ptr<LifiNodeCoord> SinglePhyTest::GetLifiNodeCoord(){
		NS_LOG_FUNCTION(this);
		return m_lifiNodeCoord;
	}

	Ptr<LifiNodeDev> SinglePhyTest::GetLifiNodeDev(){
		NS_LOG_FUNCTION(this);
		return m_lifiNodeDev;
	}

	Ptr<LifiNodeDev> SinglePhyTest::GetLifiNodeDev1(){
		NS_LOG_FUNCTION(this);
		return m_lifiNodeDev1;
	}

int main(){
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
	LogComponentEnable("LifiChannelScanHandler", LOG_LEVEL_FUNCTION);
	LogComponentEnable("LifiChannelScanHandler", LOG_PREFIX_TIME);
//	LogComponentEnable("Timer", LOG_LEVEL_FUNCTION);
//	LogComponentEnable ("LifiPhy", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiSpectrumPhy", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiSpectrumChannel", LOG_LEVEL_ALL);
//	LogComponentEnable ("LifiInterference",LOG_LEVEL_ALL);

//	_test.GetLifiNodeCoord()->Start(0x01, CHANNEL2, 0, 8, 8, true);
//	CapabilityInfo info;
//	info.CoordCapabi = true;
//	info.SecCapabi = false;
//	info.TrafficSup = false;
//	info.batteryInfo = 0x02;
//	info.powerSource = true;
//	_test.GetLifiNodeDev()->StartAssoc(CHANNEL2, EXTENDED, 0x01,
//										Mac64Address ("00:00:00:00:00:00:00:01"), info);
//	SendDataInfo dataInfo;
//	dataInfo.srcAddrMode = SHORT;
//	dataInfo.dstAddrMode = EXTENDED;
//	dataInfo.dstVPANId = 0x01;
//	dataInfo.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
//	dataInfo.msduLength = 10;
//	dataInfo.msdu = Create<Packet> (10);
//	dataInfo.msduHandle = 45;
//	dataInfo.txOption.ackTx = true;
//	dataInfo.txOption.indirectTx = false;
//	dataInfo.txOption.gtsTx = false;
//	dataInfo.rate = PHY_III_24_00_MBPS;
//	dataInfo.burstMode = false;
//
//	SendDataInfo dataInfo1;
//	dataInfo1.srcAddrMode = EXTENDED;
//	dataInfo1.dstAddrMode = SHORT;
//	dataInfo1.dstVPANId = 0x01;
//	dataInfo1.dstAddr = Mac16Address ("12:34");
//	dataInfo1.msduLength = 10;
//	dataInfo1.msdu = Create<Packet> (10);
//	dataInfo1.msduHandle = 46;
//	dataInfo1.txOption.ackTx = true;
//	dataInfo1.txOption.indirectTx = false;
//	dataInfo1.txOption.gtsTx = false;
//	dataInfo1.rate = PHY_III_24_00_MBPS;
//	dataInfo1.burstMode = false;
//
//	SendDataInfo dataInfo2;
//	dataInfo2.srcAddrMode = SHORT;
//	dataInfo2.dstAddrMode = EXTENDED;
//	dataInfo2.dstVPANId = 0x01;
//	dataInfo2.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
//	dataInfo2.msduLength = 10;
//	dataInfo2.msdu = Create<Packet> (10);
//	dataInfo2.msduHandle = 47;
//	dataInfo2.txOption.ackTx = true;
//	dataInfo2.txOption.indirectTx = false;
//	dataInfo2.txOption.gtsTx = true;
//	dataInfo2.rate = PHY_III_24_00_MBPS;
//	dataInfo2.burstMode = false;
//
//	SendDataInfo dataInfo3;
//	dataInfo3.srcAddrMode = EXTENDED;
//	dataInfo3.dstAddrMode = SHORT;
//	dataInfo3.dstVPANId = 0x01;
//	dataInfo3.dstAddr = Mac16Address ("12:34");
//	dataInfo3.msduLength = 10;
//	dataInfo3.msdu = Create<Packet> (10);
//	dataInfo3.msduHandle = 48;
//	dataInfo3.txOption.ackTx = true;
//	dataInfo3.txOption.indirectTx = false;
//	dataInfo3.txOption.gtsTx = true;
//	dataInfo3.rate = PHY_III_24_00_MBPS;
//	dataInfo3.burstMode = false;
//
//	GTSCharacteristics character;
//	character.gtsLength = 3;
//
//	GTSCharacteristics character1;
//	character1.gtsLength = 3;
//	character1.charType = DEALLOCATION;
//
////	_test.GetLifiNodeCoord().Start(0x01, CHANNEL1, 0, 8, 8, true);
//
////	_test.GetLifiMacTx()->StartVPAN(0x01, CHANNEL1, 0, 8, 8, true);
////	_test.GetLifiMacRx()->Reset();
////	TxOption op;
////	op.ackTx = false;
////	op.indirectTx = false;
////	op.gtsTx = false;
////
////	_test.GetLifiMacRx()->Send(LifiMac::GetTypeId(),
////							   LifiMac::GetTypeId(),
////							   0x01,
////							   Address(Mac16Address("ff:ff")),
////							   1,
////							   Create<Packet> (1),
////							   2,
////							   op,
////							   PHY_III_24_00_MBPS,
////							   false);
//
////	CapabilityInfo info;
////	info.CoordCapabi = true;
////	info.SecCapabi = false;
////	info.TrafficSup = false;
////	info.batteryInfo = 0x02;
////	info.powerSource = true;
////	_test.GetLifiMacRx()->Associate(CHANNEL1, EXTENDED, 0x01,
////									Mac64Address ("00:00:00:00:00:00:00:01"), info);
//
////	TxOption op;
////	op.ackTx = true;
////	op.indirectTx = false;
////	op.gtsTx = false;
////	_test.GetLifiMacTx()->Send(EXTENDED, EXTENDED, 0x01,
////							   Mac64Address ("00:00:00:00:00:00:00:02"), 10, Create<Packet> (10),
////							   45, op, PHY_III_24_00_MBPS, false);
//
////	_test.GetLifiMacRx()->Disassociate(EXTENDED, 0x01,
////									   Mac64Address ("00:00:00:00:00:00:00:01"), DEVICE, false);
//
////	_test.GetLifiMacTx()->Disassociate(EXTENDED, 0x01,
////									   Mac64Address ("00:00:00:00:00:00:00:02"), COORD, true);
//
////	GTSCharacteristics character;
////	character.gtsLength = 3;
////	_test.GetLifiMacRx()->GtsRequest(character, Mac64Address ("00:00:00:00:00:00:00:01"));
//
////	GTSCharacteristics character;
////	character.gtsLength = 3;
////	_test.GetLifiMacTx()->GtsRequest(character, Mac16Address ("55:55"));
//
////	_test.GetLifiMacRx()->ScanChannel(ACTIVE_SCAN, 10);
//	Simulator::Schedule(Seconds(0.2), &LifiNodeDev::SendData, _test.GetLifiNodeDev(), dataInfo);
//	Simulator::Schedule(Seconds(0.3), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo1);
//	Simulator::Schedule(Seconds(0.4), &LifiNodeDev::SendData, _test.GetLifiNodeDev(), dataInfo2);
//	Simulator::Schedule(Seconds(0.5), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo3);
//	Simulator::Schedule(Seconds(0.6), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev(), character, Mac64Address ("00:00:00:00:00:00:00:01"));
////	Simulator::Schedule(Seconds(0.7), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev(), character1, Mac64Address ("00:00:00:00:00:00:00:01"));
//	Simulator::Schedule(Seconds(0.8), &LifiNodeCoord::GtsRequest, _test.GetLifiNodeCoord(), character1, Mac16Address("12:34"));
////		Simulator::Schedule(Seconds(2.6), &LifiNodeDev::DisassocRequst, _test.GetLifiNodeDev(), EXTENDED, 0x01,
////												   Mac64Address ("00:00:00:00:00:00:00:01"), DEVICE, false);
//
////	Simulator::Schedule(Seconds(2.7), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), SHORT, 0x01,
////													   Mac16Address ("12:34"), COORD, false);
//	Simulator::Schedule(Seconds(7.0), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), EXTENDED, 0x01,
//														Mac64Address ("00:00:00:00:00:00:00:02"), COORD, true);
	_test.GetLifiNodeCoord()->Start(0x01, CHANNEL2, 0, 8, 8, true);
	CapabilityInfo info;
	info.CoordCapabi = true;
	info.SecCapabi = false;
	info.TrafficSup = false;
	info.batteryInfo = 0x02;
	info.powerSource = true;
	_test.GetLifiNodeDev()->StartAssoc(CHANNEL2, EXTENDED, 0x01,
										Mac64Address ("00:00:00:00:00:00:00:01"), info);
	SendDataInfo dataInfo;
	dataInfo.srcAddrMode = SHORT;
	dataInfo.dstAddrMode = EXTENDED;
	dataInfo.dstVPANId = 0x01;
	dataInfo.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
	dataInfo.msduLength = 10;
	dataInfo.msdu = Create<Packet> (10);
	dataInfo.msduHandle = 45;
	dataInfo.txOption.ackTx = true;
	dataInfo.txOption.indirectTx = false;
	dataInfo.txOption.gtsTx = false;
	dataInfo.rate = PHY_III_24_00_MBPS;
	dataInfo.burstMode = false;

	SendDataInfo dataInfo1;
	dataInfo1.srcAddrMode = EXTENDED;
	dataInfo1.dstAddrMode = SHORT;
	dataInfo1.dstVPANId = 0x01;
	dataInfo1.dstAddr = Mac16Address ("12:34");
	dataInfo1.msduLength = 10;
	dataInfo1.msdu = Create<Packet> (10);
	dataInfo1.msduHandle = 46;
	dataInfo1.txOption.ackTx = true;
	dataInfo1.txOption.indirectTx = false;
	dataInfo1.txOption.gtsTx = false;
	dataInfo1.rate = PHY_III_24_00_MBPS;
	dataInfo1.burstMode = false;

	SendDataInfo dataInfo2;
	dataInfo2.srcAddrMode = SHORT;
	dataInfo2.dstAddrMode = EXTENDED;
	dataInfo2.dstVPANId = 0x01;
	dataInfo2.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
	dataInfo2.msduLength = 10;
	dataInfo2.msdu = Create<Packet> (10);
	dataInfo2.msduHandle = 47;
	dataInfo2.txOption.ackTx = true;
	dataInfo2.txOption.indirectTx = false;
	dataInfo2.txOption.gtsTx = true;
	dataInfo2.rate = PHY_III_24_00_MBPS;
	dataInfo2.burstMode = false;

	SendDataInfo dataInfo3;
	dataInfo3.srcAddrMode = EXTENDED;
	dataInfo3.dstAddrMode = SHORT;
	dataInfo3.dstVPANId = 0x01;
	dataInfo3.dstAddr = Mac16Address ("12:34");
	dataInfo3.msduLength = 10;
	dataInfo3.msdu = Create<Packet> (10);
	dataInfo3.msduHandle = 48;
	dataInfo3.txOption.ackTx = true;
	dataInfo3.txOption.indirectTx = false;
	dataInfo3.txOption.gtsTx = true;
	dataInfo3.rate = PHY_III_24_00_MBPS;
	dataInfo3.burstMode = false;

	SendDataInfo dataInfo4;
	dataInfo4.srcAddrMode = SHORT;
	dataInfo4.dstAddrMode = EXTENDED;
	dataInfo4.dstVPANId = 0x01;
	dataInfo4.dstAddr = Mac64Address ("00:00:00:00:00:00:00:01");
	dataInfo4.msduLength = 11;
	dataInfo4.msdu = Create<Packet> (11);
	dataInfo4.msduHandle = 47;
	dataInfo4.txOption.ackTx = true;
	dataInfo4.txOption.indirectTx = false;
	dataInfo4.txOption.gtsTx = true;
	dataInfo4.rate = PHY_III_24_00_MBPS;
	dataInfo4.burstMode = false;

	GTSCharacteristics character;
	character.gtsLength = 3;

	GTSCharacteristics character1;
	character1.gtsLength = 3;
	character1.charType = DEALLOCATION;

	Simulator::Schedule(Seconds(0.3), &LifiNodeDev::SendData, _test.GetLifiNodeDev(), dataInfo);
	Simulator::Schedule(Seconds(0.4), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo1);
	Simulator::Schedule(Seconds(0.5), &LifiNodeDev::SendData, _test.GetLifiNodeDev(), dataInfo2);
	Simulator::Schedule(Seconds(0.6), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo3);
	Simulator::Schedule(Seconds(1.7), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev(), character, Mac64Address ("00:00:00:00:00:00:00:01"));
//	Simulator::Schedule(Seconds(0.7), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev1(), character1, Mac64Address ("00:00:00:00:00:00:00:01"));
//	Simulator::Schedule(Seconds(0.8), &LifiNodeCoord::GtsRequest, _test.GetLifiNodeCoord(), character1, Mac16Address("12:34"));
	Simulator::Schedule(Seconds(6.6), &LifiNodeDev::DisassocRequst, _test.GetLifiNodeDev(), EXTENDED, 0x01,
												   Mac64Address ("00:00:00:00:00:00:00:01"), DEVICE, false);

//	Simulator::Schedule(Seconds(2.7), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), SHORT, 0x01,
//													   Mac16Address ("12:34"), COORD, false);
//	Simulator::Schedule(Seconds(7.0), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), EXTENDED, 0x01,
//														Mac64Address ("00:00:00:00:00:00:00:02"), COORD, true);


	Simulator::Schedule(Seconds(0.2), &LifiNodeDev::StartAssoc, _test.GetLifiNodeDev1(), CHANNEL2, EXTENDED, 0x01,
			                                                      Mac64Address ("00:00:00:00:00:00:00:01"), info);
	Simulator::Schedule(Seconds(1.75), &LifiNodeDev::SendData, _test.GetLifiNodeDev1(), dataInfo);
	Simulator::Schedule(Seconds(1.85), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo1);
	Simulator::Schedule(Seconds(1.95), &LifiNodeDev::SendData, _test.GetLifiNodeDev1(), dataInfo4);
	Simulator::Schedule(Seconds(2.05), &LifiNodeCoord::SendData, _test.GetLifiNodeCoord(), dataInfo3);
	Simulator::Schedule(Seconds(2.15), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev1(), character, Mac64Address ("00:00:00:00:00:00:00:01"));
//	Simulator::Schedule(Seconds(0.7), &LifiNodeDev::GtsRequest, _test.GetLifiNodeDev1(), character1, Mac64Address ("00:00:00:00:00:00:00:01"));
//	Simulator::Schedule(Seconds(2.2), &LifiNodeCoord::GtsRequest, _test.GetLifiNodeCoord(), character1, Mac16Address("56:78"));
	Simulator::Schedule(Seconds(6.7), &LifiNodeDev::DisassocRequst, _test.GetLifiNodeDev1(), EXTENDED, 0x01,
											   Mac64Address ("00:00:00:00:00:00:00:01"), DEVICE, false);

//	Simulator::Schedule(Seconds(2.7), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), SHORT, 0x01,
//													   Mac16Address ("12:34"), COORD, false);
	Simulator::Schedule(Seconds(7.5), &LifiNodeCoord::DisassocRequst, _test.GetLifiNodeCoord(), EXTENDED, 0x01,
														Mac64Address ("00:00:00:00:00:00:00:03"), COORD, true);

	Simulator::Stop(Seconds(10));
	Simulator::Run ();

	Simulator::Destroy();

//	TxOption op;
	//		op.ackTx = true;
	//		op.indirectTx = false;
	//		op.gtsTx = false;
	//		m_mcpsSapProvider->McpsDataRequest(SHORT, EXTENDED, 0x01,
	//										   Mac64Address ("00:00:00:00:00:00:00:01"), 10, Create<Packet> (10),
	//										   45, op, PHY_III_24_00_MBPS, false);
	return 0;
}
