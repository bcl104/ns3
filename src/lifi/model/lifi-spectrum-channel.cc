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

void LifiSpectrumChannel::AddRx(Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(phy!=0);
	Ptr<NetDevice> pD = phy->GetDevice();
	NS_ASSERT(pD!=0);

	std::vector<Ptr<SpectrumPhy> >::const_iterator it = m_rxPhyList.begin();
	bool flag = true;
	while(it!=m_rxPhyList.end()){
		if(pD==((*it)->GetDevice())){
			flag = false;
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_rxNumDevices++;
	}
	m_rxPhyList.push_back(phy);
}

uint32_t LifiSpectrumChannel::GetNDevices(void) const {
	NS_LOG_FUNCTION(this);
	return m_numDevices;
}

Ptr<NetDevice> LifiSpectrumChannel::GetDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(i<=m_numDevices);
	Ptr<NetDevice> pDevice = 0;
	if(i<=m_rxNumDevices){
		pDevice = m_rxPhyList.at(i)->GetDevice();
	}else{
		pDevice = m_txPhyList.at(i-m_rxNumDevices)->GetDevice();
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

void LifiSpectrumChannel::AddTx(Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	NS_ASSERT(phy!=0);
	Ptr<NetDevice> pD = phy->GetDevice();
	NS_ASSERT(pD!=0);

	std::vector<Ptr<SpectrumPhy> >::const_iterator it = m_txPhyList.begin();
	bool flag = true;
	while(it!=m_txPhyList.end()){
		if(pD==(*it)->GetDevice()){
			flag = false;
			break;
		}
		++it;
	}
	if(flag){
		m_numDevices++;
		m_txNumDevices++;
	}
	m_txPhyList.push_back(phy);
}

void LifiSpectrumChannel::DeleteRx(Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = true;
	std::vector<Ptr<SpectrumPhy> >::iterator it = m_rxPhyList.begin();
	while(it!=m_rxPhyList.end()){
		if((*it)->GetDevice()==phy->GetDevice()&&(*it)!=phy){
			flag = false;
		}
		if((*it)==phy){
			it = m_rxPhyList.erase(it);
		}else{
			it++;
		}
	}
	if(flag){
		m_rxNumDevices--;
		m_numDevices--;
	}
}

void LifiSpectrumChannel::DeleteTx(Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this);
	bool flag = true;
	std::vector<Ptr<SpectrumPhy> >::iterator it = m_txPhyList.begin();
	while(it!=m_txPhyList.end()){
		if(((*it)->GetDevice())==(phy->GetDevice())&&(*it)!=phy){
			flag = false;
		}
		if((*it)==phy){
			it = m_txPhyList.erase(it);
		}else{
			it++;
		}
	}
	if(flag){
		m_txNumDevices--;
		m_numDevices--;
	}
}

void LifiSpectrumChannel::StartRx(Ptr<SpectrumSignalParameters> params,Ptr<SpectrumPhy> receiver) {
	NS_LOG_FUNCTION(this);
}

} /* namespace ns3 */
