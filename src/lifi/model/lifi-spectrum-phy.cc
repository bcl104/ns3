 /* lifi-spectrum-phy.cc*/

#include "lifi-spectrum-phy.h"
#include "ns3/spectrum-channel.h"
#include "ns3/log.h"
#include "ns3/propagation-loss-model.h"//calculate the rxPower from txList to receiver
#include "ns3/spectrum-propagation-loss-model.h"
#include "lifi-net-device.h"
#include "lifi-phy.h"
NS_LOG_COMPONENT_DEFINE ("LifiSpectrumPhy");
namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED (LifiSpectrumPhy);
LifiSpectrumPhy::LifiSpectrumPhy() {
	NS_LOG_FUNCTION(this);
	m_rxPowerTh = 1;

}


void LifiSpectrumPhy::SetRxPowerTh(double th){
	NS_LOG_FUNCTION(this);
	m_rxPowerTh = th ;
}

double LifiSpectrumPhy::GetmRxPowerTh(void){
	NS_LOG_FUNCTION(this);
	return m_rxPowerTh;
}

LifiSpectrumPhy::LifiSpectrumPhy(Ptr<NetDevice> device) {
	NS_LOG_FUNCTION(this);
//	LifiSpectrumPhy();//?????????????
	m_device = device;
	m_rxPowerTh = 1;
//	m_cellId = 0;
//	m_band = 0;
}

LifiSpectrumPhy::~LifiSpectrumPhy() {
	NS_LOG_FUNCTION(this);
}

TypeId LifiSpectrumPhy::GetTypeId() {
	static TypeId tid = TypeId ("ns3::LifiSpectrumPhy")
						.SetParent<SpectrumPhy>()
						.AddConstructor<LifiSpectrumPhy>();
	return tid;
}

void LifiSpectrumPhy::SetNodeSpectrum(uint8_t channel) {
	NS_LOG_FUNCTION(this);
}

void LifiSpectrumPhy::Send(Ptr<Packet>pb,uint32_t size,uint8_t band,  bool isCellMode,  uint8_t cellId,  uint8_t trxid , double trxPower,
							Time duration,Ptr<SpectrumValue> apsd,Time txTime,uint8_t mcsId,uint8_t lifipsduSize,uint8_t lifireservedFields,
							bool lifiookDim,bool lifiburstMode) {
	NS_LOG_FUNCTION(this);
	m_SignalParameters->band = band;
	m_SignalParameters->cellId = cellId;
	m_SignalParameters->cellMode = isCellMode;
	m_SignalParameters->duration = duration;
	m_SignalParameters->pb = pb;
	m_SignalParameters->trxId = trxid;
	m_SignalParameters->trxPower = trxPower;
	m_SignalParameters->txPhy = this;
	m_SignalParameters->psd = apsd;
	m_SignalParameters->time = txTime;
	m_SignalParameters->mcsId = mcsId;
	m_SignalParameters->PsduSize = lifipsduSize;
	m_SignalParameters->reservedFields = lifireservedFields;
	m_SignalParameters->ookDim = lifiookDim;
	m_SignalParameters->burstMode = lifiburstMode;
	m_channel->StartTx(m_SignalParameters);//subclasses pointer can be transfered to the upper class formal parameters
//	m_channel->StartTx(DynamicCast<SpectrumSignalParameters>(m_SignalParameters));//turn LifiSpectrumSignalParamters to SpectrumSignalParameters
}

bool LifiSpectrumPhy::CarrierSense() {
	NS_LOG_FUNCTION(this);
	return false;
}

uint8_t LifiSpectrumPhy::CarrierSense(uint8_t band) {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::SetDevice(Ptr<NetDevice> device) {
	NS_LOG_FUNCTION(this);
	m_device = device;
}

Ptr<NetDevice> LifiSpectrumPhy::GetDevice() {
	NS_LOG_FUNCTION(this);
	return m_device;
}

void LifiSpectrumPhy::SetMobility(Ptr<MobilityModel> m) {
	NS_LOG_FUNCTION(this);
	m_mobility = m;
}

Ptr<MobilityModel> LifiSpectrumPhy::GetMobility() {
	NS_LOG_FUNCTION(this);
	return m_mobility;
}

void LifiSpectrumPhy::SetChannel(Ptr<SpectrumChannel> c) {
	NS_LOG_FUNCTION(this);
	m_channel = c;
}

Ptr<SpectrumChannel> LifiSpectrumPhy::GetChannel(){
	NS_LOG_FUNCTION(this);
	return m_channel;
}

Ptr<const SpectrumModel> LifiSpectrumPhy::GetRxSpectrumModel() const{
	NS_LOG_FUNCTION(this);
	return 0;
}

Ptr<AntennaModel> LifiSpectrumPhy::GetRxAntenna() {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::StartRx(Ptr<SpectrumSignalParameters> params) {
	NS_LOG_FUNCTION(this);
	Ptr<LifiSpectrumSignalParameters> lifi_params = DynamicCast<LifiSpectrumSignalParameters>(params);
	bool cellMode = lifi_params->cellMode;
	if(!cellMode){
		PhyList::iterator beg;
		PhyList::iterator  end;
//		std::vector<Ptr<LifiSpectrumPhy> > txPoint;
		std::pair<PhyList::iterator,PhyList::iterator> list_pair = std::make_pair(beg,end);
		Ptr<LifiSpectrumChannel> channel = DynamicCast<LifiSpectrumChannel>(m_channel);
		Ptr<LifiSpectrumSignalParameters> lifi_param = DynamicCast<LifiSpectrumSignalParameters>(params);
		list_pair = channel->SearchTxList(lifi_params->band);
		Ptr<SpectrumPropagationLossModel> propagationlossmodel = channel->GetSpectrumPropagationLossModel();
//		m_interference
		m_interference->SetReceiveState(true);
		m_interference->SetAllsignal(0);
		Time startTime = Simulator::Now();
		while(beg != end){
			Ptr<SpectrumValue> txPsd = beg->second->GetSpectrumSignalParameters()->psd;
			Ptr<SpectrumValue> rxPsd = propagationlossmodel->CalcRxPowerSpectralDensity(txPsd,beg->second->GetMobility(),lifi_params->txPhy->GetMobility());
			m_interference->LifiAddSignal(rxPsd,startTime);
			++beg;
		}
		Simulator::Schedule(lifi_params->duration,&LifiSpectrumPhy::EndRx,this, lifi_params);
	}
	else{

	}

}

uint8_t LifiSpectrumPhy::CalculateWqi(Ptr<SpectrumValue> sinr){
	NS_LOG_FUNCTION(this);
	return 255;
}

void LifiSpectrumPhy::EndRx(Ptr<LifiSpectrumSignalParameters> params){
	NS_LOG_FUNCTION(this);
	m_interference->SetReceiveState(false);
	Ptr<SpectrumValue> averageAllSignal = m_interference->CalcuAveInterference(params->duration);
	Ptr<SpectrumValue> sinr = m_interference->CalSinr(params->psd,averageAllSignal);
	m_interference->SetAllsignal(0);
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(m_device);
	Ptr<LifiPhy> lifiphy = lifi_device->GetPhy();
	//add a threshold detection statement
	uint8_t wqi = CalculateWqi(sinr);
	lifiphy->Receive(params,wqi);
}

//void LifiSpectrumPhy::SetLifiPhy(Ptr<LifiPhy> phy){
//	NS_LOG_FUNCTION(this);
//	m_phy = phy;
//}
//
//Ptr<LifiPhy> LifiSpectrumPhy::GetLifiPhy(void){
//	NS_LOG_FUNCTION(this);
//	return m_phy;
//}

Ptr<AntennaModel> LifiSpectrumPhy::GetAntennaModel() {
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::SetAntennaModel(Ptr<AntennaModel> antenna) {
	NS_LOG_FUNCTION(this);
}

void LifiSpectrumPhy::SetCellId(uint16_t id){
	NS_LOG_FUNCTION(this);
//	m_cellId = id;
}

uint16_t LifiSpectrumPhy::GetCellId(){
	NS_LOG_FUNCTION(this);
	return 0;
}

void LifiSpectrumPhy::SetSpectrumSignalParameters (Ptr<LifiSpectrumSignalParameters> param){
	NS_LOG_FUNCTION(this);
	m_SignalParameters = param;
}

Ptr<LifiSpectrumSignalParameters> LifiSpectrumPhy::GetSpectrumSignalParameters(){
	NS_LOG_FUNCTION(this);
	return m_SignalParameters;
}
//void LifiSpectrumPhy::EndRx(Ptr<SpectrumSignalParameters> params) {
//	NS_LOG_FUNCTION(this);
//}

void LifiSpectrumPhy::SetErrorModel(Ptr<LifiSpectrumErrorModel> e){
	NS_LOG_FUNCTION(this);
	if(m_interference){
		m_interference->SetErrorModel(e);
	}
}

//uint8_t LifiSpectrumPhy::GetBand(){
//	NS_LOG_FUNCTION(this);
//	return m_b
//}

} /* namespace ns3 */
