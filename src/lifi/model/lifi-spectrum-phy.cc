 /* lifi-spectrum-phy.cc*/

#include "lifi-spectrum-phy.h"
#include "ns3/spectrum-channel.h"
#include "ns3/log.h"
#include "ns3/propagation-loss-model.h"//calculate the rxPower from txList to receiver
#include "ns3/spectrum-propagation-loss-model.h"
#include "lifi-net-device.h"
#include "lifi-phy.h"
#include "cmath"
NS_LOG_COMPONENT_DEFINE ("LifiSpectrumPhy");
namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED (LifiSpectrumPhy);
LifiSpectrumPhy::LifiSpectrumPhy() {
	NS_LOG_FUNCTION(this);
	m_rxPowerTh = 1;
	m_berTh = 0.5;
	m_rxNumCount = 0;

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
	m_berTh = 0.5;
	m_rxNumCount = 0;
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
							Time duration,Ptr<SpectrumValue> apsd,Time txTime){
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
	DynamicCast<LifiSpectrumChannel>(GetChannel())->AddTx(this);//add to TxList  9:59 4.29 by st125475466
//	m_SignalParameters->mcsId = mcsId;
//	m_SignalParameters->PsduSize = lifipsduSize;
//	m_SignalParameters->reservedFields = lifireservedFields;
//	m_SignalParameters->ookDim = lifiookDim;
//	m_SignalParameters->burstMode = lifiburstMode;
	m_channel->StartTx(m_SignalParameters);//subclasses pointer can be transfered to the upper class formal parameters
//	m_channel->StartTx(DynamicCast<SpectrumSignalParameters>(m_SignalParameters));//turn LifiSpectrumSignalParamters to SpectrumSignalParameters
}

bool LifiSpectrumPhy::CarrierSense() {
	NS_LOG_FUNCTION(this);
	return false;
}

bool LifiSpectrumPhy::CarrierSense(uint8_t band,double edTh){
	NS_LOG_FUNCTION(this);
    double power=0;
    Ptr<LifiSpectrumChannel> lifiSpectrumChannel;
    lifiSpectrumChannel=DynamicCast<LifiSpectrumChannel> (m_channel);
	power=lifiSpectrumChannel->CalcMyCcaPower(m_mobility,band);
    return (power>edTh)? false:true;
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

void LifiSpectrumPhy::SetInterference(Ptr<LifiInterference> interference){
	NS_LOG_FUNCTION(this);
	m_interference = interference;
}

Ptr<LifiInterference> LifiSpectrumPhy::GetInterference(void){
	NS_LOG_FUNCTION(this);
	return m_interference;
}

void LifiSpectrumPhy::StartRx(Ptr<SpectrumSignalParameters> params) {
	NS_LOG_FUNCTION(this);
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(m_device);
//	Ptr<LifiPhy> lifiphy = lifi_device->GetPhy();
//	lifiphy->SetTRxState(RX_ON_BUSY);
//	lifiphy->GetPlmeSapUser()->PlemStateIndication(RX_ON_BUSY);
	Ptr<LifiSpectrumSignalParameters> lifi_params = DynamicCast<LifiSpectrumSignalParameters>(params);
	bool cellMode = lifi_params->cellMode;
	if(!cellMode){
		PhyList::iterator beg;
		PhyList::iterator  end;
//		std::vector<Ptr<LifiSpectrumPhy> > txPoint;
		std::pair<PhyList::iterator,PhyList::iterator> list_pair;
		Ptr<LifiSpectrumChannel> channel = DynamicCast<LifiSpectrumChannel>(m_channel);
//		Ptr<LifiSpectrumSignalParameters> lifi_param = DynamicCast<LifiSpectrumSignalParameters>(params);
		list_pair = channel->SearchTxList(lifi_params->band);
		beg = list_pair.first;
		end = list_pair.second;//last modified at 11.05 04.25 by st125475466
		Ptr<SpectrumPropagationLossModel> propagationlossmodel = channel->GetSpectrumPropagationLossModel();
//		m_interference
		if(m_rxNumCount == 0){
		NS_ASSERT(m_interference->GetReceiveState() == RX_ON);
		m_rxNumCount++;
		m_interference->SetReceiveState(RX_BUSY);
		Ptr<LifiPhy> lifiphy = lifi_device->GetPhy();
		lifiphy->SetTRxState(RX_BUSY);
		lifiphy->GetPlmeSapUser()->PlemStateIndication(RX_BUSY);
//		m_interference->SetAllsignal(0);
		}
		else{
			NS_ASSERT(m_interference->GetReceiveState() == RX_BUSY);
			m_rxNumCount++;
		}
		Time startTime = Simulator::Now();
		lifi_params->time = startTime;
		Simulator::Schedule(lifi_params->duration,&LifiSpectrumPhy::EndRx,this, lifi_params);
		while(beg != end){
			Ptr<SpectrumValue> txPsd = beg->second->GetSpectrumSignalParameters()->psd->Copy();
//			std::cout<<"aaa"<<Integral(*txPsd)<<std::endl;
			Ptr<SpectrumValue> rxPsd = propagationlossmodel->CalcRxPowerSpectralDensity(txPsd,beg->second->GetMobility(),m_mobility);
//			std::cout<<Integral(*rxPsd)<<std::endl;
			m_interference->LifiAddSignal(rxPsd,lifi_params->duration);
			++beg;
		}
//		std::cout<<"the duration is "<<lifi_params->duration<<std::endl;//?????????????????????????????????
//		Simulator::Schedule(lifi_params->duration,&LifiSpectrumPhy::EndRx,this, lifi_params);
	}
	else{

	}

}

uint8_t LifiSpectrumPhy::CalculateWqi(double sinr){
	NS_LOG_FUNCTION(this);
	return 255;
}

double LifiSpectrumPhy::CalculateErf(double x,uint8_t n){
	NS_LOG_FUNCTION(this);
	double resolution = x/n;
	double result = 0;
	for(uint8_t i = 0;i<n;i++){
		result +=resolution *exp(-pow((resolution*i+resolution/2),2));
	}
	result *= 2/sqrt(M_PI);
	return result;
}

void LifiSpectrumPhy::SetBerTh(double ber){
	NS_LOG_FUNCTION(this);
	m_berTh = ber;
}

double LifiSpectrumPhy::GetberTh(void){
	NS_LOG_FUNCTION(this);
	return m_berTh;
}

void LifiSpectrumPhy::EndRx(Ptr<LifiSpectrumSignalParameters> params){
	NS_LOG_FUNCTION(this);
	Ptr<SpectrumValue> averageAllSignal = m_interference->CalcuAveInterference(params->duration,params->time);
//	double allsignal = Integral(*averageAllSignal);
//	std::cout<<"aver signal:"<<allsignal<<std::endl;/////????????????????????????
	Ptr<SpectrumValue> sinr = m_interference->CalSinr(params->psd,averageAllSignal);
//	double TimeDomainSinr = Integral(*sinr);
	uint8_t band = params->band;
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(m_device);
//	Ptr<LifiPhy> lifi_phy = DynamicCast<LifiNetDevice>()
	Ptr<LifiPhy> lifi_phy = lifi_device->GetPhy();
	uint8_t subBand = lifi_phy->GetSunBandsNum();
	double TimeDomainSinr = m_interference->BandIntegral(sinr,band,subBand);
//	std::cout<<"TimeDomainSinr:"<<TimeDomainSinr<<std::endl;
//	TimeDomainSinr = Integral(*sinr);//??????????
//	std::cout<<"TimeDomainSinr:"<<TimeDomainSinr<<std::endl;
	double ber = CalculateBer(TimeDomainSinr);
//	std::cout<<"ber:"<<ber<<std::endl;
	NS_ASSERT(m_interference->GetReceiveState() == RX_BUSY);
	NS_ASSERT(m_rxNumCount > 0);
	if(m_rxNumCount == 1){
	m_rxNumCount--;
//	m_interference->SetAllsignal(0);
	m_interference->SetReceiveState(RX_ON);
	m_interference->CancelEvent();
	}
	else{
		m_rxNumCount--;
	}
	//add a threshold detection statement
//	ber = 1;//??????????????????????????????????????????????????
	if(ber < m_berTh){
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(m_device);
	Ptr<LifiPhy> lifiphy = lifi_device->GetPhy();
	uint8_t wqi = CalculateWqi(TimeDomainSinr);
	lifiphy->Receive(params,wqi);
	}
	else{
//		NS_LOG_WARN("the rxBer:"<<ber<<"is less than the berTh"<<m_berTh);
	}
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

double LifiSpectrumPhy::CalculateBer(double sinr){
	NS_LOG_FUNCTION(this);
	double ber = 0;
	if(sinr > 1){
		ber = 1.0/4.0*exp(-sinr/4);
	}
	else{
	ber = 1.0/2.0*(1.0-CalculateErf(sqrt(sinr/4.0),100));
	}
	return ber;
}

} /* namespace ns3 */
