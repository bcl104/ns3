/*
 * LifiSpectrumChannel.cc
 *
 *  Created on: 2014年4月15日
 *      Author: pp
 */

#include "ns3/log.h"
//#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/lifi-spectrum-loss-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/antenna-model.h"
#include "lifi-spectrum-channel.h"
#include "lifi-spectrum-phy.h"
#include "lifi-phy-pib-attributes.h" //attributes are used in function addTx deleteTx and deleteRx
#include "lifi-net-device.h"//calculate cca

NS_LOG_COMPONENT_DEFINE("LifiSpectrumChannel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiSpectrumChannel);

//struct LifiSpectrumSignalParameters;
//class LifiSpectrumPhy;

LifiSpectrumChannel::LifiSpectrumChannel() {
	// TODO Auto-generated constructor stub
	NS_LOG_FUNCTION(this);
	m_numDevices = 0;
	m_rxNumDevices = 0;
	m_txNumDevices = 0;
//	m_delay = MicroSeconds (0);
//	m_rxPowerTh = 1;
}

LifiSpectrumChannel::~LifiSpectrumChannel() {
	// TODO Auto-generated destructor stub
	NS_LOG_FUNCTION(this);
}

TypeId LifiSpectrumChannel::GetTypeId(void) {
	static TypeId tid = TypeId ("ns3::LifiSpectrumChannel")
						.SetParent<SpectrumChannel>()
						.AddConstructor<LifiSpectrumChannel>();
	return tid;
}

void LifiSpectrumChannel::AddPropagationLossModel(Ptr<PropagationLossModel> loss) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(m_propagationLossModel==0);
	m_propagationLossModel = loss;
}

void LifiSpectrumChannel::AddSpectrumPropagationLossModel(Ptr<SpectrumPropagationLossModel> loss) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(m_spectrumPropagationLoss==0);
	m_spectrumPropagationLoss = loss;
}

void LifiSpectrumChannel::SetPropagationDelayModel(Ptr<PropagationDelayModel> delay) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT (m_propagationDelay == 0);
	m_propagationDelay = delay;
}

std::pair<PhyList ::iterator,PhyList::iterator> LifiSpectrumChannel::SearchRxList(uint8_t band){
	NS_LOG_FUNCTION(this);
//	PhyList::iterator it;
//	PhyList::iterator end ;
	//full-band while band==7
	if(band == 7){
		std::pair<PhyList ::iterator,PhyList::iterator> pos;
		pos.first = m_rxPhyList.begin();
		pos.second = m_rxPhyList.end();
		return pos;
	}
	else
	return m_rxPhyList.equal_range(band);
}

std::pair<PhyList::iterator,PhyList::iterator> LifiSpectrumChannel::SearchTxList(uint8_t band){
	NS_LOG_FUNCTION(this);
	return m_txPhyList.equal_range(band);
}

Ptr<PropagationLossModel> LifiSpectrumChannel::GetPropagationModel(void){
	NS_LOG_FUNCTION(this);
	return m_propagationLossModel;
}
//void LifiSpectrumChannel::SetDelay(Time delay){
//	NS_LOG_FUNCTION(this);
//	m_delay = delay;
//}
//
//Time LifiSpectrumChannel::GetDelay(){
//	NS_LOG_FUNCTION(this);
//	return m_delay;
//}

void LifiSpectrumChannel::StartTx(Ptr<SpectrumSignalParameters> param) {
	NS_LOG_FUNCTION(this);
	Ptr<LifiSpectrumSignalParameters> params = DynamicCast<LifiSpectrumSignalParameters>(param);
	PhyList::iterator beg;
	PhyList::iterator end;
//	std::vector<Ptr<LifiSpectrumPhy> > rxPoint;
	std::pair<PhyList::iterator,PhyList::iterator> pos;
	uint8_t rxBand = 7;
//	rxBand = params->band;full band receive
	pos = SearchRxList(rxBand);
	beg = pos.first;
	end = pos.second;
	uint8_t bandid = params->band;
	Ptr<NetDevice> device = params->txPhy->GetDevice();
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(device);
	Ptr<LifiPhy> lifi_phy = lifi_device->GetPhy();
	uint8_t subBand = lifi_phy->GetSunBandsNum();
	Ptr<LifiSpectrumPropagationLossModel> lifi_loss = DynamicCast<LifiSpectrumPropagationLossModel>(m_spectrumPropagationLoss);
	lifi_loss->SetBand(bandid);
	lifi_loss->SetSubBand(subBand);
	lifi_loss->SetBandWidth(params->psd,bandid,subBand);
	while(beg != end){
//		double Pr = m_propagationLossModel->CalcRxPower(params->trxPower,params->txPhy->GetMobility(),beg->second->GetMobility());///the first param is not dbm
//		Ptr<LifiSpectrumPropagationLossModel> lifipropagation = DynamicCast<LifiSpectrumPropagationLossModel>(m_spectrumPropagationLoss);
//		Ptr<SpectrumValue> tempPsd = lifipropagation->GetBandPsd(params->psd,params->band,)
		Ptr<SpectrumValue> rxPsd = m_spectrumPropagationLoss->CalcRxPowerSpectralDensity(params->psd,params->txPhy->GetMobility(),beg->second->GetMobility());
		double Pr = Integral(*rxPsd);//transform Psd into power unit w.
//		std::cout<< "Pr:"<<Pr<<std::endl;
		double PrDbm = 10*log10(1000.0*Pr);
		if(PrDbm > beg->second->GetmRxPowerTh()){
//			rxPoint.push_back(beg->second);
//			Time delay = m_propagationDelay->GetDelay(params->txPhy->GetMobility(),beg->second->GetMobility());//too much to close the real situation
			Ptr<LifiSpectrumSignalParameters> rxParams = Create<LifiSpectrumSignalParameters>(*params);//????
//			Ptr<LifiSpectrumSignalParameters> rxParams = params;
			uint32_t detNode = beg->second->GetDevice()->GetNode()->GetId();
			rxParams->time = Simulator::Now();
			rxParams->trxPower = Pr;
			rxParams->psd = rxPsd->Copy();
//			std::cout<<"the delay is "<<delay<<std::endl;
//			std::cout<<"the duration is "<<params->duration<<std::endl;
			Time delay=NanoSeconds(30);//10m/c
			Simulator::ScheduleWithContext (detNode,delay, &LifiSpectrumChannel::StartRx, this, rxParams,beg->second);
		}//by comparing the rxPower ,we can choose to compare the rxPsd;
		++beg;
	}
//	std::vector<Ptr<LifiSpectrumPhy> >::iterator it = rxPoint.begin();
//	while(it != rxPoint.end()){
//		Time delay = m_propagationDelay->GetDelay(params->txPhy->GetMobility(),(*it)->GetMobility());
//		Ptr<LifiSpectrumSignalParameters> rxParams = params;//????
//		Simulator::Schedule (delay, &LifiSpectrumChannel::StartRx, this, rxParams,*it);
//		++it;
//	}
}

//
//void LifiSpectrumChannel::SetRxPowerTh(double th){
//	NS_LOG_FUNCTION(this);
//	m_rxPowerTh = th ;
//}
//
//double LifiSpectrumChannel::GetmRxPowerTh(void){
//	NS_LOG_FUNCTION(this);
//	return m_rxPowerTh;
//}

void LifiSpectrumChannel::AddRx (Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(phy!=0);
	Ptr<LifiSpectrumPhy> lifi_phy = DynamicCast<LifiSpectrumPhy>(phy);
	Ptr<NetDevice> pD = lifi_phy->GetDevice();
	NS_ASSERT(pD!=0);
//	std::pair<Ptr<SpectrumPhy>,uint32_t> temp;
	PhyList::iterator it = m_rxPhyList.begin();
	bool flag = true;
	while(it!=m_rxPhyList.end()){
		if(it->second->GetDevice() == pD){
			flag = false;
			NS_LOG_WARN("this device has already been added to RxList");
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_rxNumDevices++;
		uint8_t band = (lifi_phy->GetSpectrumSignalParameters()->band);
		std::pair<uint8_t,Ptr<LifiSpectrumPhy> > insertElement;
		insertElement.first = band;
		insertElement.second = lifi_phy;
		m_rxPhyList.insert(insertElement);//modification at 10:57 04 25 by st125475466
	}
//	m_rxPhyList.insert(std::make_pair(band,std::make_pair(phy,m_rxNumDevices)));

}

//void LifiSpectrumChannel::AddRx(uint8_t band,Ptr<LifiSpectrumPhy> phy) {
//	NS_ASSERT(phy!=0);
//	Ptr<NetDevice> pD = phy->GetDevice();
//	NS_ASSERT(pD!=0);
//	std::pair<Ptr<SpectrumPhy>,uint32_t> temp;
//	PhyList::iterator it = m_rxPhyList.begin();
//	bool flag = true;
//	while(it!=m_rxPhyList.end()){
//		if(it->second->GetDevice() == pD){
//			flag = false;
//			break;
//		}
//		++it;
//	}
//	if(flag){
//		m_numDevices++;
//		m_txNumDevices++;
//	}
////	m_rxPhyList.insert(std::make_pair(band,std::make_pair(phy,m_rxNumDevices)));
//	m_rxPhyList.insert(std::make_pair(band,phy));
//}

Ptr<NetDevice> LifiSpectrumChannel::GetDevice (uint32_t i) const {
	NS_LOG_FUNCTION(this);
//	Ptr<NetDevice> device;
	return 0;
}


uint32_t LifiSpectrumChannel::GetNDevices(void) const {
	NS_LOG_FUNCTION(this);
	return m_numDevices;
}

Ptr<NetDevice> LifiSpectrumChannel::GetRxDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(i<=m_rxNumDevices);
	Ptr<NetDevice> pDevice = 0;
	PhyList ::const_iterator it = m_rxPhyList.begin();
	uint32_t count = 0;
	while(it != m_rxPhyList.end()){
		++count;
	if(count == i){
			pDevice = it->second->GetDevice();
			return pDevice;
			break;
			}
		++it;
		}
		NS_LOG_WARN ("ID number "<<i<<"out of range ,total " << m_rxNumDevices << "RX Devices");
		return 0;
}

Ptr<NetDevice> LifiSpectrumChannel::GetTxDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(i<=m_txNumDevices);
	Ptr<NetDevice> pDevice = 0;
	PhyList ::const_iterator it = m_txPhyList.begin();
	uint32_t count = 0;
	while(it != m_txPhyList.end()){
		++count;
	if(count == i){
			pDevice = it->second->GetDevice();
			return pDevice;
			break;
			}
		++it;
		}
		NS_LOG_WARN ("ID number "<<i<<"out of range ,total " << m_txNumDevices << "RX Devices");
		return 0;
}

Ptr<SpectrumPropagationLossModel> LifiSpectrumChannel::GetSpectrumPropagationLossModel(void) {
	NS_LOG_FUNCTION(this);
	return m_spectrumPropagationLoss;
}

void LifiSpectrumChannel::SetSpectrumMap(std::map<int, int> spectrum) {
	NS_LOG_FUNCTION(this);
	m_channelMap = spectrum;
}



void LifiSpectrumChannel::AddTx(Ptr<LifiSpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(phy!=0);
	Ptr<NetDevice> pD = phy->GetDevice();
	NS_ASSERT(pD!=0);
	std::pair<Ptr<SpectrumPhy>,uint32_t> temp;
	PhyList::iterator it = m_txPhyList.begin();
	bool flag = true;
	while(it!=m_txPhyList.end()){
		if(it->second->GetDevice() == pD){
			flag = false;
			NS_LOG_WARN("this device has already been added to TxList.");
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_txNumDevices++;
		uint8_t band = phy->GetSpectrumSignalParameters()->band;
		std::pair<uint8_t,Ptr<LifiSpectrumPhy> > insertElement;
		insertElement.first = band;
		insertElement.second = phy;
		m_txPhyList.insert(insertElement);
		AddRxInterference(phy);
	}
}

bool LifiSpectrumChannel::DeleteRx(Ptr<LifiSpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it;
	PhyList::iterator end ;
//	it = m_rxPhyList.lower_bound(band);
//	end = m_rxPhyList.upper_bound(band);
	uint8_t band = phy->GetSpectrumSignalParameters()->band;
//	std::make_pair(it,end) = SearchTxList(band);
	std::pair<PhyList::iterator,PhyList::iterator> pos;
	pos = SearchRxList(band);
	it = pos.first;
	end = pos.second;
	while(it != end){
		if(it->second == phy){
			m_rxNumDevices--;
			m_numDevices--;
			m_rxPhyList.erase(it);
			flag = true;
			break;
		}
		++it;
	}
	if(flag == false){
		NS_LOG_WARN("can not find this device in RxList");
	}
	return flag;
}

bool LifiSpectrumChannel::DeleteTx(Ptr<LifiSpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it;
	PhyList::iterator end ;
	uint8_t band = phy->GetSpectrumSignalParameters()->band;
//	std::make_pair(it,end) = SearchTxList(band);
	std::pair<PhyList::iterator,PhyList::iterator> pos;
	pos = SearchTxList(band);
	it = pos.first;
	end = pos.second;
//	PhyList::iterator it = m_txPhyList.equal_range(band);
//	PhyList::iterator end = m_txPhyList.equal_range(band);
	while(it != end){
		if(it->second == phy){
			m_txNumDevices--;
			m_numDevices--;
			m_txPhyList.erase(it);
			flag = true;
			break;
		}
		++it;
	}
	if(flag == false){
			NS_LOG_WARN("can not find this device in RxList");
		}
	return flag;

}

void LifiSpectrumChannel::AddRxInterference(Ptr<LifiSpectrumPhy> phy){
	NS_LOG_FUNCTION(this);
	PhyList::iterator it;
	PhyList::iterator end ;
//	uint8_t band = phy->GetSpectrumSignalParameters()->band;

	Ptr<NetDevice> device = phy->GetDevice();
	Ptr<LifiNetDevice> lifi_device = DynamicCast<LifiNetDevice>(device);
	Ptr<LifiPhy> lifi_phy = DynamicCast<LifiPhy>(lifi_device->GetPhy());

	uint8_t bandid = phy->GetSpectrumSignalParameters()->band;
	uint8_t subBand = lifi_phy->GetSunBandsNum();
	Ptr<LifiSpectrumPropagationLossModel> lifi_loss = DynamicCast<LifiSpectrumPropagationLossModel>(m_spectrumPropagationLoss);
	lifi_loss->SetBand(bandid);
	lifi_loss->SetSubBand(subBand);
	lifi_loss->SetBandWidth(phy->GetSpectrumSignalParameters()->psd,bandid,subBand);
	Ptr<SpectrumValue> txPsd = phy->GetSpectrumSignalParameters()->psd;
	Time lifi_duration = phy->GetSpectrumSignalParameters()->duration;
	std::pair<PhyList::iterator,PhyList::iterator> pos;
//	std::make_pair(it,end) = SearchRxList(band);
	uint8_t band = 7;
	pos = SearchRxList(band);
	it = pos.first;
	end = pos.second;
	Time rxTime = Simulator::Now();
	while(it != end){
		Ptr<SpectrumValue> rxPsd = m_spectrumPropagationLoss->CalcRxPowerSpectralDensity(txPsd,phy->GetMobility(),it->second->GetMobility());
		it->second->GetInterference()->LifiAddSignal(rxPsd,lifi_duration ,rxTime);//transform now() into duration 10:34 04.28
		++it;
	}
}

void LifiSpectrumChannel::SubtraRxInterference(Ptr<LifiSpectrumPhy> phy){
	NS_LOG_FUNCTION(this);
	PhyList::iterator it;
	PhyList::iterator end ;
//	uint8_t band = phy->GetSpectrumSignalParameters()->band;
	uint8_t band = 7;
	Ptr<SpectrumValue> txPsd = phy->GetSpectrumSignalParameters()->psd;
	std::pair<PhyList::iterator,PhyList::iterator> pos;
	pos = SearchRxList(band);
//	std::make_pair(it,end) = SearchRxList(band);
	it = pos.first;
	end = pos.second;
	Time rxTime = Simulator::Now();
	while(it != end){
		Ptr<SpectrumValue> rxPsd = m_spectrumPropagationLoss->CalcRxPowerSpectralDensity(txPsd,phy->GetMobility(),it->second->GetMobility());
		it->second->GetInterference()->LifiSubtractSignal(rxPsd,rxTime);
		++it;
	}
}

void LifiSpectrumChannel::StartRx(Ptr<LifiSpectrumSignalParameters> params,Ptr<LifiSpectrumPhy> receiver) {
	NS_LOG_FUNCTION(this);
//	PhyList::iterator beg;
//	PhyList::iterator  end;
//	std::vector<Ptr<LifiSpectrumPhy> > txPoint;
//	std::pair<PhyList::iterator,PhyList::iterator> list_pair = std::make_pair(beg,end);
//	list_pair = SearchTxList(params->band);
//	while(beg != end){
	receiver->StartRx(params);
//	}
}

double LifiSpectrumChannel::CalcMyCcaPower(Ptr<MobilityModel> myMobilityModel,uint8_t bandId)
{
	NS_ASSERT(myMobilityModel!=0);
	double powerSum=0;
	PhyList::iterator it;
	it=m_txPhyList.find(bandId);
	if(it==m_txPhyList.end())
	{
//		NS_LOG_WARN("the bandId:"<<bandId<<"is not using");
		return 0;
	}
	uint16_t k=0;
	for(k=0;k!=m_txPhyList.count(bandId);k++,it++)
	{
		Ptr<LifiSpectrumPhy> lifiSpectrumPhy = it->second;
//		lifiSpectrumPhy=DynamicCast<LifiSpectrumPhy> ((*it).second);
		Ptr<MobilityModel> txMobilityModel=lifiSpectrumPhy->GetMobility();
		NS_ASSERT(txMobilityModel!=0);
		Ptr<NetDevice> netDevice;
		netDevice=(*it).second->GetDevice();
		Ptr<LifiNetDevice> lifiNetDevice;
		lifiNetDevice=DynamicCast<LifiNetDevice> (netDevice);
		NS_ASSERT(lifiNetDevice!=0);
		double txPowerDbm=lifiNetDevice->GetPhy()->GetTxPower();
		powerSum+=m_propagationLossModel->CalcRxPower(txPowerDbm,txMobilityModel,myMobilityModel);
	}
    return powerSum;

}

} /* namespace ns3 */
