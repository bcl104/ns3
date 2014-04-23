/*
 * LifiSpectrumChannel.cc
 *
 *  Created on: 2014年4月15日
 *      Author: pp
 */

#include "ns3/log.h"
//#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/antenna-model.h"
#include "lifi-spectrum-channel.h"
#include "lifi-spectrum-phy.h"
#include "lifi-phy-pib-attributes.h" //attributes are used in function addTx deleteTx and deleteRx

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
	std::vector<Ptr<LifiSpectrumPhy> > rxPoint;
	std::make_pair(beg,end) = SearchRxList(params->band);
	while(beg != end){
//		double Pr = 1;
		double Pr = m_propagationLossModel->CalcRxPower(params->trxPower,params->txPhy->GetMobility(),beg->second->GetMobility());///the first param is not dbm
		Ptr<SpectrumValue> rxPsd = m_spectrumPropagationLoss->CalcRxPowerSpectralDensity(params->psd,params->txPhy->GetMobility(),beg->second->GetMobility());
		if(Pr > beg->second->GetmRxPowerTh()){
//			rxPoint.push_back(beg->second);
			Time delay = m_propagationDelay->GetDelay(params->txPhy->GetMobility(),beg->second->GetMobility());
			Ptr<LifiSpectrumSignalParameters> rxParams = params;//????
			rxParams->time = Simulator::Now();
			rxParams->trxPower = Pr;
			rxParams->psd = rxPsd->Copy();
			Simulator::Schedule (delay, &LifiSpectrumChannel::StartRx, this, rxParams,beg->second);
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
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_rxNumDevices++;
	}
//	m_rxPhyList.insert(std::make_pair(band,std::make_pair(phy,m_rxNumDevices)));
	uint8_t band = (lifi_phy->GetSpectrumSignalParameters()->band);
	m_rxPhyList.insert(std::make_pair(band,lifi_phy));
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
		NS_LOG_LOGIC ("ID number "<<i<<"out of range ,total " << m_rxNumDevices << "RX Devices");
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
		NS_LOG_LOGIC ("ID number "<<i<<"out of range ,total " << m_txNumDevices << "RX Devices");
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
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_txNumDevices++;
	}
	uint8_t band = phy->GetSpectrumSignalParameters()->band;
	m_txPhyList.insert(std::make_pair(band,phy));
}

bool LifiSpectrumChannel::DeleteRx(Ptr<LifiSpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it;
	PhyList::iterator end ;
//	it = m_rxPhyList.lower_bound(band);
//	end = m_rxPhyList.upper_bound(band);
	uint8_t band = phy->GetSpectrumSignalParameters()->band;
	std::make_pair(it,end) = SearchTxList(band);

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
	return flag;
}

bool LifiSpectrumChannel::DeleteTx(Ptr<LifiSpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it;
	PhyList::iterator end ;
	uint8_t band = phy->GetSpectrumSignalParameters()->band;
	std::make_pair(it,end) = SearchTxList(band);
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
	return flag;

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

} /* namespace ns3 */
