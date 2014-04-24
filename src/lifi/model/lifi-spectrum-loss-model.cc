
/*
 * lifi-spectrum-loss-model.cc
 *
 *  Created on: 2014年4月24日
 *      Author: st125475466
 */

#include "lifi-spectrum-loss-model.h"
#include <ns3/log.h>
#include <ns3/lifi-indoor-propagation-loss-model.h>

NS_LOG_COMPONENT_DEFINE ("SpectrumPropagationLossModel");

namespace ns3 {
LifiSpectrumPropagationLossModel::LifiSpectrumPropagationLossModel() {
	m_antennaParameters = Create<LifiAntennaParameters>();
	m_antennaParameters->DetectorArea = 0.01;
	m_antennaParameters->FilterGain = 1.0;
	m_antennaParameters->ConcentratorGain = 1.0;
	m_antennaParameters->HalfPowerDegree = (double)15/180*M_PI;
	m_antennaParameters->FOV = (double)120/180*M_PI;
	m_antennaParameters->TransmitterDegree = (double)30/180*M_PI;
	m_antennaParameters->ReceiverDegree = (double)30/180*M_PI;
}

ns3::LifiSpectrumPropagationLossModel::~LifiSpectrumPropagationLossModel() {
}

Ptr<SpectrumValue> LifiSpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity(
	Ptr<const SpectrumValue> txPsd, Ptr<const MobilityModel> a,
	Ptr<const MobilityModel> b) const{
	Ptr<SpectrumValue> rxPsd = txPsd->Copy();
	Values::iterator beg = rxPsd->ValuesBegin();
	Values::iterator end = rxPsd->ValuesEnd();
	while(beg != end){
		double temp = *beg;
		(*beg) = DoCalcRxPower(temp,a,b);
		beg++;
	}
	return rxPsd;
}

double LifiSpectrumPropagationLossModel::DoCalcRxPower(double txPowerDbm, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const{
	double distance = a->GetDistanceFrom (b);
	double m_temp1 = log(2);
	double m_temp2 = log(cosl(m_antennaParameters->HalfPowerDegree));
	double m = -m_temp1/m_temp2;
	double R0 = ((m+1)/(2*M_PI))*pow(cosl(m_antennaParameters->TransmitterDegree),m);
			if(m_antennaParameters->ReceiverDegree > m_antennaParameters->FOV)
			{
				double A_D_temp = m_antennaParameters->DetectorArea/pow(distance,2);
				double H = A_D_temp*R0*m_antennaParameters->FilterGain*m_antennaParameters->ConcentratorGain*cosl(m_antennaParameters->ReceiverDegree);
				double rx = 10*log(H*1000);
				return rx+txPowerDbm;
			}
			else
				return 0;
}

void LifiSpectrumPropagationLossModel::SetLifiAntennaParameters(Ptr<LifiAntennaParameters> parameters){
	m_antennaParameters = parameters;
}

Ptr<LifiAntennaParameters> LifiSpectrumPropagationLossModel::GetLifiAntennaParameters(){
	return m_antennaParameters;
}
}
