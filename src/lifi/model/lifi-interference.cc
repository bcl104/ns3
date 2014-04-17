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

void LifiInterference::AddSignal(Ptr<const SpectrumValue> spd, const Time duration){
	NS_LOG_FUNCTION (this);
	DoAddSignal (spd->Copy());
	Simulator::Schedule (duration, &LifiInterference::DoSubtractSignal, this, spd->Copy());
}

void LifiInterference::StartRx(Ptr<const Packet> p , Ptr<const SpectrumValue> rxPsd){
	  NS_LOG_FUNCTION (this);
	  m_rxSignal = rxPsd->Copy();
	  m_lastChangeTime = Now ();
	  m_receiving = true;
//	  m_errorModel->StartRx (p);      /*not added yet*/
}

bool LifiInterference::EndRx(){
	NS_LOG_FUNCTION (this);
	return 1;
}

void LifiInterference::SetNoisePowerSpectralDensity(Ptr<const SpectrumValue> noisePsd){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::DoAddSignal(Ptr<SpectrumValue> spd){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::DoSubtractSignal(Ptr<SpectrumValue> spd){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::ConditionallyEvaluateChunk(){
	NS_LOG_FUNCTION (this);
}

void LifiInterference::SetErrorModel( Ptr<LifiSpectrumErrorModel> error){
	NS_LOG_FUNCTION (this);
	m_errorModel = error;
}

} /* namespace ns3 */

