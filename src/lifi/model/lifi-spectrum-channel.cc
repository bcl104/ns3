/*
 * LifiSpectrumChannel.cc
 *
 *  Created on: 2014年4月15日
 *      Author: pp
 */

#include "lifi-spectrum-channel.h"
#include "ns3/log.h"

#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/antenna-model.h"

NS_LOG_COMPONENT_DEFINE("LifiSpectrumChannel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LifiSpectrumChannel);

LifiSpectrumChannel::LifiSpectrumChannel() {
	// TODO Auto-generated constructor stub
	NS_LOG_FUNCTION(this);
	m_numDevices = 0;
	m_rxNumDevices = 0;
	m_txNumDevices = 0;
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
	NS_ASSERT(m_propagationLoss==0);
	m_propagationLoss = loss;
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

void LifiSpectrumChannel::StartTx(Ptr<SpectrumSignalParameters> params) {
	NS_LOG_FUNCTION(this);

}

void LifiSpectrumChannel::AddRx(uint8_t band,Ptr<SpectrumPhy> phy) {
	NS_ASSERT(phy!=0);
	Ptr<NetDevice> pD = phy->GetDevice();
	NS_ASSERT(pD!=0);
	std::pair<Ptr<SpectrumPhy>,uint32_t> temp;
	PhyList::iterator it = m_rxPhyList.begin();
	bool flag = true;
	while(it!=m_rxPhyList.end()){
		if(it->second.first->GetDevice() == pD){
			flag = false;
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_txNumDevices++;
	}
	m_rxPhyList.insert(std::make_pair(band,std::make_pair(phy,m_rxNumDevices)));
}

uint32_t LifiSpectrumChannel::GetNDevices(void) const {
	NS_LOG_FUNCTION(this);
	return m_numDevices;
}

Ptr<NetDevice> LifiSpectrumChannel::GetRxDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(i<=m_numDevices);
	Ptr<NetDevice> pDevice = 0;
	PhyList ::iterator it = m_rxPhyList.begin()+i;
	if(it != m_rxPhyList.end()){
		pDevice = it->second.first->GetDevice();
	}else{
		 NS_LOG_LOGIC ("ID number "<<i<<"out of range ,total " << m_rxNumDevices << "RX Devices");
		 return -1;
	}
	return pDevice;
}

Ptr<NetDevice> LifiSpectrumChannel::GetTxDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(i<=m_numDevices);
	Ptr<NetDevice> pDevice = 0;
	PhyList ::iterator it = m_txPhyList.begin()+i;
	if(it != m_txPhyList.end()){
		pDevice = it->second.first->GetDevice();
	}else{
		 NS_LOG_LOGIC ("ID number "<<i<<"out of range ,total " << m_txNumDevices << "RX Devices");
		 return -1;
	}
	return pDevice;
}

Ptr<SpectrumPropagationLossModel> LifiSpectrumChannel::GetSpectrumPropagationLossModel(void) {
	NS_LOG_FUNCTION(this);
	return m_spectrumPropagationLoss;
}

void LifiSpectrumChannel::SetSpectrumMap(std::map<int, int> spectrum) {
	NS_LOG_FUNCTION(this);
	m_channelMap = spectrum;
}

void LifiSpectrumChannel::AddTx(uint8_t band,Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(phy!=0);
	Ptr<NetDevice> pD = phy->GetDevice();
	NS_ASSERT(pD!=0);
	std::pair<Ptr<SpectrumPhy>,uint32_t> temp;
	PhyList::iterator it = m_txPhyList.begin();
	bool flag = true;
	while(it!=m_txPhyList.end()){
		if(it->second.first->GetDevice() == pD){
			flag = false;
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_txNumDevices++;
	}
	m_txPhyList.insert(std::make_pair(band,std::make_pair(phy,m_txNumDevices)));
}

bool LifiSpectrumChannel::DeleteRx(uint8_t band,Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it = m_rxPhyList.equal_range(band);
	PhyList::iterator end = m_rxPhyList.equal_range(band);
	while(it != end){
		if(it->second.first == phy){
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

bool LifiSpectrumChannel::DeleteTx(uint8_t band,Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = false;
	PhyList::iterator it = m_txPhyList.equal_range(band);
	PhyList::iterator end = m_txPhyList.equal_range(band);
	while(it != end){
		if(it->second.first == phy){
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

void LifiSpectrumChannel::StartRx(Ptr<SpectrumSignalParameters> params,Ptr<SpectrumPhy> receiver) {
	NS_LOG_FUNCTION(this);
}

} /* namespace ns3 */
