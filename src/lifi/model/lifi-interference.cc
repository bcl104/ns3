/*
 * LifiInterference.cc
 *
 *  Created on: 2014年4月10日
 *      Author: pp
 */

#include "lifi-interference.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("LifiInterference");

namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED(LifiInterference);

LifiInterference::LifiInterference() {
	// TODO Auto-generatedSpectrumInterference constructor stub
	NS_LOG_FUNCTION (this);
	m_receiving = false;
	m_rxSignal = 0;
	m_allSignal = 0;
	m_noise = 0;
	m_errorModel = 0;
//	m_allSignalSpd = 0;
//	m_allSignalDbm = 0;
//	m_rxSignalDbm = 0;
}

LifiInterference::~LifiInterference() {
	// TODO Auto-generated destructor stub
	NS_LOG_FUNCTION (this);
}

TypeId LifiInterference::GetTypeId (void){
	static TypeId tid = TypeId ("ns3::LifiInterference")
						.SetParent<Object>()
						.AddConstructor<LifiInterference>();
	return tid;
}

void LifiInterference::AddSignal(Ptr<const SpectrumValue> psd, const Time duration){
	NS_LOG_FUNCTION (this);
	DoAddSignal (psd->Copy());
	Simulator::Schedule (duration, &LifiInterference::DoSubtractSignal, this, psd->Copy());
}

void LifiInterference::LifiAddSignal(Ptr<SpectrumValue> psd,Time rxTime){
	NS_LOG_FUNCTION (this);
	if(m_receiving == true){
	DoLifiAddSignal (psd->Copy(),rxTime);
	}
//	Simulator::Schedule (duration, &LifiInterference::DoSubtractSignalDbm, this, rxPower);
}

void LifiInterference::LifiSubtractSignal(Ptr<SpectrumValue> psd,Time rxTime){
	NS_LOG_FUNCTION (this);
	if(m_receiving == true){
	DoLifiSubtractSignal(psd->Copy(),rxTime);
	}
}

void LifiInterference::StartRx(Ptr<const Packet> p , Ptr<const SpectrumValue> rxPsd){
	  NS_LOG_FUNCTION (this);
	  m_rxSignal = rxPsd->Copy();
	  m_lastChangeTime = Now ();
	  m_receiving = true;
//	  m_errorModel->StartRx (p);      /*not added yet*/
}

//double LifiInterference::GetAllSignalDbm(void){
//	NS_LOG_FUNCTION (this);
//	return m_allSignalDbm;
//}

bool LifiInterference::EndRx(){
	NS_LOG_FUNCTION (this);
	return 1;
}

void LifiInterference::SetNoisePowerSpectralDensity(Ptr<const SpectrumValue> noisePsd){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::DoLifiAddSignal(Ptr<SpectrumValue> psd,Time rxTime){
	NS_LOG_FUNCTION (this);
	*m_allSignal +=*psd;
	std::pair<std::map<Time,Ptr<SpectrumValue> >::iterator ,bool > it =m_allSignalPsd.insert(std::make_pair(rxTime,psd));
	if(!it.second){
		it.first->second = m_allSignal;
	}
}

void LifiInterference::DoLifiSubtractSignal(Ptr<SpectrumValue> psd,Time rxTime){
	NS_LOG_FUNCTION (this);
	*m_allSignal -=*psd;
	std::pair<std::map<Time,Ptr<SpectrumValue> >::iterator ,bool > it =m_allSignalPsd.insert(std::make_pair(rxTime,psd));
	if(!it.second){
		it.first->second = m_allSignal;
	}
}
//calulate time average
Ptr<SpectrumValue> LifiInterference::CalcuAveInterference(Time duration){
	NS_LOG_FUNCTION (this);
	Ptr<SpectrumValue> average = 0;
	std::map<Time,Ptr<SpectrumValue> >::iterator beg =  m_allSignalPsd.begin();
	std::map<Time,Ptr<SpectrumValue> >::iterator end =  m_allSignalPsd.end();
	while(beg != end){
		std::map<Time,Ptr<SpectrumValue> >::iterator temp = beg++;
		*average = ((beg->first.GetSeconds() - temp->first.GetSeconds()) / duration.GetSeconds()) *  (*(temp->second));
	}
return average;
}

Ptr<SpectrumValue> LifiInterference::CalSinr(Ptr<SpectrumValue> rxSignal , Ptr<SpectrumValue> AveAllSignal ){
	NS_LOG_FUNCTION (this);
	m_rxSignal = rxSignal;
	SpectrumValue sinr = *m_rxSignal / (*AveAllSignal+*m_noise);
	return &sinr;
}

void LifiInterference::DoSubtractSignal(Ptr<SpectrumValue> psd){
	NS_LOG_FUNCTION (this);
	}

void LifiInterference::DoAddSignal(Ptr<const SpectrumValue> psd){
	NS_LOG_FUNCTION (this);

}

void LifiInterference::SetReceiveState(bool state){
	NS_LOG_FUNCTION (this);
	m_receiving = state;
}
bool LifiInterference::GetReceiveState(void){
	NS_LOG_FUNCTION (this);
	return m_receiving;
}

//void LifiInterference::DoSubtractSignalDbm(double rxPower){
//
//}
void LifiInterference::ConditionallyEvaluateChunk(){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::SetErrorModel( Ptr<LifiSpectrumErrorModel> error){
	NS_LOG_FUNCTION (this);
	m_errorModel = error;
}

void LifiInterference::SetAllsignal(Ptr<SpectrumValue> allsignal){
	NS_LOG_FUNCTION (this);
	m_allSignal = allsignal;
}

Ptr<SpectrumValue>LifiInterference::GetAllSignal(){
	NS_LOG_FUNCTION (this);
	return m_allSignal;
}
} /* namespace ns3 */

