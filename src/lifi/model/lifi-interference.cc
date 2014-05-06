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
	m_receiving = RX_ON;
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

void LifiInterference::LifiAddSignal(Ptr<SpectrumValue> psd,Time duration){
	NS_LOG_FUNCTION (this);
	if(m_receiving == RX_BUSY){
	Time rxTime = Simulator::Now();
	DoLifiAddSignal (psd->Copy());
	EventId id = Simulator::Schedule (duration, &LifiInterference::DoLifiSubtractSignal, this, psd->Copy());
	m_eventId.push_back(id);
	}

}

void LifiInterference::CancelEvent(){
	NS_LOG_FUNCTION (this);
	std::vector<EventId>::iterator beg = m_eventId.begin();
	std::vector<EventId>::iterator end = m_eventId.end();
	while(beg != end){
		if(!(beg->IsExpired())){
			beg->Cancel();
		}
		++beg;
	}
	m_eventId.clear();
}

void LifiInterference::LifiSubtractSignal(Ptr<SpectrumValue> psd){
	NS_LOG_FUNCTION (this);
	if(m_receiving == RX_BUSY){
	DoLifiSubtractSignal(psd->Copy());
	}
}

void LifiInterference::StartRx(Ptr<const Packet> p , Ptr<const SpectrumValue> rxPsd){
	  NS_LOG_FUNCTION (this);
	  m_rxSignal = rxPsd->Copy();
	  m_lastChangeTime = Now ();
	  m_receiving = RX_ON;
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
	m_noise = noisePsd->Copy();
}

void LifiInterference::DoLifiAddSignal(Ptr<SpectrumValue> psd){
	NS_LOG_FUNCTION (this);
	Time rxTime = Simulator::Now();
	if(m_allSignal == 0){
		Ptr<const SpectrumModel> model = psd->GetSpectrumModel();
//		std::cout<<"m_uid:"<<model->GetUid()<<std::endl;
		m_allSignal = Create<SpectrumValue>(model);
//		m_allSignal = psd->Copy();
	}
	else{
		Ptr<const SpectrumModel> model = m_allSignal->GetSpectrumModel();
		Ptr<SpectrumValue> temp = Create<SpectrumValue>(model);
		Values::iterator tempbeg = temp->ValuesBegin();
		Values::iterator tempend = temp->ValuesEnd();
		Values::iterator allbeg = psd->ValuesBegin();
		while(tempbeg != tempend){
			*tempbeg = *allbeg;
			tempbeg++;
			allbeg++;
		}
		psd = temp->Copy();
	}
//	std::cout<<"m_uid:"<<m_allSignal->GetSpectrumModelUid()<<std::endl;
	*m_allSignal += *psd;
	std::pair<Time,Ptr<SpectrumValue> > insertElement;
	insertElement.first = rxTime;
	insertElement.second = psd->Copy();
	std::pair<std::map<Time,Ptr<SpectrumValue> >::iterator ,bool > it =m_allSignalPsd.insert(insertElement);
	if(!it.second){
		it.first->second = m_allSignal;
	}
}

void LifiInterference::DoLifiSubtractSignal(Ptr<SpectrumValue> psd){
	NS_LOG_FUNCTION (this);
	NS_ASSERT(m_allSignal != 0);
	Time rxTime = Simulator::Now();
	Ptr<const SpectrumModel> model = m_allSignal->GetSpectrumModel();
	Ptr<SpectrumValue> temp = Create<SpectrumValue>(model);
	Values::iterator tempbeg = temp->ValuesBegin();
	Values::iterator tempend = temp->ValuesEnd();
	Values::iterator allbeg = psd->ValuesBegin();
	while(tempbeg != tempend){
		*tempbeg = *allbeg;
		tempbeg++;
		allbeg++;
	}
	psd = temp->Copy();
	*m_allSignal -=*psd;
	std::pair<Time,Ptr<SpectrumValue> > insertElement;
	insertElement.first = rxTime;
	insertElement.second = psd->Copy();
	std::pair<std::map<Time,Ptr<SpectrumValue> >::iterator ,bool > it =m_allSignalPsd.insert(insertElement);
	if(!it.second){
		it.first->second = m_allSignal;
	}
}
//calulate time average
Ptr<SpectrumValue> LifiInterference::CalcuAveInterference(Time duration,Time startTime){
	NS_LOG_FUNCTION (this);
	NS_ASSERT(m_allSignal != 0);
	Ptr<const SpectrumModel> model = m_allSignal->GetSpectrumModel();
	Ptr<SpectrumValue> average = Create<SpectrumValue>(model);
	std::map<Time,Ptr<SpectrumValue> >::iterator beg =  m_allSignalPsd.lower_bound(startTime);
	std::map<Time,Ptr<SpectrumValue> >::iterator end =  m_allSignalPsd.lower_bound(startTime+duration);
	double last_duration = end->first.GetSeconds()-beg->first.GetSeconds();
	while(beg != end){
		std::map<Time,Ptr<SpectrumValue> >::iterator temp = beg++;
		*average = ((beg->first.GetSeconds() - temp->first.GetSeconds()) / last_duration) *  (*(temp->second));
//		std::cout<<"interal:"<<(Integral(*(temp->second)))<<std::endl;
	}
return average->Copy();
}

Ptr<SpectrumValue> LifiInterference::CalSinr(Ptr<SpectrumValue> rxSignal , Ptr<SpectrumValue> AveAllSignal ){
	NS_LOG_FUNCTION (this);
	NS_ASSERT(m_allSignal != 0);
//	NS_ASSERT(m_noise->GetSpectrumModel() == m_allSignal->GetSpectrumModel());
	Ptr<const SpectrumModel> model = AveAllSignal->GetSpectrumModel();
	Ptr<SpectrumValue> temp = Create<SpectrumValue>(model);
	Values::iterator tempbeg = temp->ValuesBegin();
	Values::iterator tempend = temp->ValuesEnd();
	Values::iterator allbeg = rxSignal->ValuesBegin();
	while(tempbeg != tempend){
		*tempbeg = *allbeg;
		tempbeg++;
		allbeg++;
	}
	rxSignal = temp->Copy();
//	Ptr<const SpectrumModel> model = m_allSignal->GetSpectrumModel();
	Ptr<SpectrumValue> sinr = Create<SpectrumValue>(model);
	Ptr<SpectrumValue> m_noise = Create<SpectrumValue>(model);//noise equal to 0
	Values::iterator noiseBeg = m_noise->ValuesBegin();
	Values::iterator noiseEnd = m_noise->ValuesEnd();
	while(noiseBeg != noiseEnd){
		*noiseBeg = 1.0e-3;//MHZ
		noiseBeg++;
	}
	m_rxSignal = rxSignal->Copy();
	*sinr = *m_rxSignal / (*AveAllSignal + *m_noise - *m_rxSignal);
	return sinr->Copy();
}

double  LifiInterference::BandIntegral(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(band < 7);
	Values::iterator ValueBeg = psd->ValuesBegin() + (6 - band) * SubBand;
	Values::iterator ValueEnd = psd->ValuesBegin() + (7 - band) * SubBand;
	std::vector<BandInfo>::const_iterator BandBeg = psd->ConstBandsBegin()+ (6 - band) * SubBand;
	double Power = 0;
	while(ValueBeg != ValueEnd){
		Power = *ValueBeg * (BandBeg->fh - BandBeg->fl);
		++ValueBeg;
		++BandBeg;
	}
	Power = 10.0 * log10(Power*1000.0);
	return Power;
}

void LifiInterference::DoSubtractSignal(Ptr<SpectrumValue> psd){
	NS_LOG_FUNCTION (this);
	}

void LifiInterference::DoAddSignal(Ptr<const SpectrumValue> psd){
	NS_LOG_FUNCTION (this);

}

void LifiInterference::SetReceiveState(PhyOpStatus state){
	NS_LOG_FUNCTION (this);
	m_receiving = state;
	m_allSignalPsd.clear();
	SetAllsignal(0);
}
PhyOpStatus LifiInterference::GetReceiveState(void){
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

