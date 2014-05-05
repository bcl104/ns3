
/*
 * lifi-spectrum-loss-model.cc
 *
 *  Created on: 2014年4月24日
 *      Author: st125475466
 */

#include "lifi-spectrum-loss-model.h"
#include <ns3/log.h>
#include <ns3/lifi-indoor-propagation-loss-model.h>
#include <vector>
#include <cmath>
NS_LOG_COMPONENT_DEFINE ("LifiSpectrumPropagationLossModel");

namespace ns3 {
LifiSpectrumPropagationLossModel::LifiSpectrumPropagationLossModel() {
	NS_LOG_FUNCTION(this);
	m_antennaParameters = CreateObject<LifiAntennaParameters>();
	m_antennaParameters->DetectorArea = 0.01;
	m_antennaParameters->FilterGain = 1.0;
	m_antennaParameters->ConcentratorGain = 1.0;
	m_antennaParameters->HalfPowerDegree = (double)15/180*M_PI;
	m_antennaParameters->FOV = (double)30/180*M_PI;
	m_antennaParameters->TransmitterDegree = (double)30/180*M_PI;
	m_antennaParameters->ReceiverDegree = (double)30/180*M_PI;
	m_bandWith = 100;
	m_band = 0;
	m_subBand = 1;
}

ns3::LifiSpectrumPropagationLossModel::~LifiSpectrumPropagationLossModel() {
	NS_LOG_FUNCTION(this);
}

Ptr<SpectrumValue> LifiSpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity(
	Ptr<const SpectrumValue> txPsd, Ptr<const MobilityModel> a,
	Ptr<const MobilityModel> b) const{
	NS_LOG_FUNCTION(this);
	Ptr<SpectrumValue> rxPsd = txPsd->Copy();
	Values::iterator beg = rxPsd->ValuesBegin()+(6-m_band) * m_subBand;
	Values::iterator end = beg + m_subBand;
	double temp = Integral(*rxPsd);//unit W
	temp = 10.0*log10(1000.0*temp);//unit dbm
	double rxPower = DoCalcRxPower(temp,a,b);
	double rxPowerW = std::pow (10., (rxPower - 30) / 10);
	double rxPowerDendity = rxPowerW/m_bandWith;//MHZ
//	std::cout<<"rxPower:"<<rxPowerDendity<<std::endl;
	while(beg != end){

		(*beg) = rxPowerDendity;

		beg++;
	}
	return rxPsd->Copy();
}

/**
 * return txPower unit dbm
 */
//double  LifiSpectrumPropagationLossModel::BandIntegral(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand){
//	NS_LOG_FUNCTION(this);
//	NS_ASSERT(band < 7);
//	Values::iterator ValueBeg = psd->ValuesBegin() + (7 - band) * SubBand;
//	Values::iterator ValueEnd = psd->ValuesBegin() + (8 - band) * SubBand;
//	std::vector<BandInfo>::const_iterator BandBeg = psd->ConstBandsBegin()+ (7 - band) * SubBand;
//	double txPower = 0;
//	while(ValueBeg != ValueEnd){
//		txPower = *ValueBeg * (BandBeg->fh - BandBeg->fl);
//		++ValueBeg;
//		++BandBeg;
//	}
//	txPower = 10.0 * log10(txPower*1000.0);
//	return txPower;
//}

Ptr<SpectrumValue> LifiSpectrumPropagationLossModel::GetBandPsd(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand){
	NS_LOG_FUNCTION(this);
	NS_ASSERT(band < 7);
	Ptr<const SpectrumModel> model = psd->GetSpectrumModel();
	Ptr<SpectrumValue> bandPsd = Create<SpectrumValue>(model);
	Values::iterator psdValueBeg = psd->ValuesBegin() + (7 - band) * SubBand;
	Values::iterator psdValueEnd = psd->ValuesBegin() + (8 - band) * SubBand;
//	std::vector<BandInfo>::const_iterator psdBandBeg = psd->ConstBandsBegin()+ (7 - band) * SubBand;
	Values::iterator ValueBeg = bandPsd->ValuesBegin() + (7 - band) * SubBand;
//	Values::iterator ValueEnd = bandPsd->ValuesBegin() + (8 - band) * SubBand;
//	std::vector<BandInfo>::const_iterator BandBeg = bandPsd->ConstBandsBegin()+ (7 - band) * SubBand;
	while(psdValueBeg != psdValueEnd){
			*ValueBeg = *psdValueBeg;
			++psdValueBeg;
			++ValueBeg;
//			++BandBeg;
		}
	return bandPsd->Copy();
}

double LifiSpectrumPropagationLossModel::DoCalcRxPower(double txPowerDbm, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const{
	NS_LOG_FUNCTION(this);
	double distance = a->GetDistanceFrom (b);
	double m_temp1 = log(2);
	double m_temp2 = log(cosl(m_antennaParameters->HalfPowerDegree));
	double m = -m_temp1/m_temp2;
	double R0 = ((m+1)/(2*M_PI))*pow(cosl(m_antennaParameters->TransmitterDegree),m);
			if((!m_antennaParameters->ReceiverDegree < m_antennaParameters->FOV))
			{
				double A_D_temp = m_antennaParameters->DetectorArea/pow(distance,2);
				double H = A_D_temp*R0*m_antennaParameters->FilterGain*m_antennaParameters->ConcentratorGain*cosl(m_antennaParameters->ReceiverDegree);
				double rx = 10*log10(H*1000);
				return rx+txPowerDbm;
			}
			else
				return 0;
}

void LifiSpectrumPropagationLossModel::SetBandWidth(Ptr<SpectrumValue>psd , uint8_t band , uint8_t subBand){
	NS_LOG_FUNCTION(this);
	Bands::const_iterator psdBeg = psd->ConstBandsBegin() + (6 - band) * subBand;
	Bands::const_iterator psdEnd = psdBeg + subBand;
	double bandwidth = psdEnd->fl - psdBeg->fl;
	m_bandWith = bandwidth;
}

double LifiSpectrumPropagationLossModel::GetBandWidth(){
	NS_LOG_FUNCTION(this);
	return m_bandWith;
}

void LifiSpectrumPropagationLossModel::SetBand(uint8_t band){
	NS_LOG_FUNCTION(this);
	m_bandWith = band;
}

void LifiSpectrumPropagationLossModel::SetSubBand(uint8_t subband){
	NS_LOG_FUNCTION(this);
	m_subBand = subband;
}

void LifiSpectrumPropagationLossModel::SetLifiAntennaParameters(Ptr<LifiAntennaParameters> parameters){
	NS_LOG_FUNCTION(this);
	m_antennaParameters = parameters;
}

Ptr<LifiAntennaParameters> LifiSpectrumPropagationLossModel::GetLifiAntennaParameters(){
	NS_LOG_FUNCTION(this);
	return m_antennaParameters;
}
}
