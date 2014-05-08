/*
 * LifiIndoorPropagationLossModel.cpp
 *
 *  Created on: 2014年4月9日
 *      Author: pp
 */

#include "lifi-indoor-propagation-loss-model.h"

namespace ns3 {

LifiIndoorPropagationLossModel::LifiIndoorPropagationLossModel() {
	// TODO Auto-generated constructor stub
	m_antennaParameters = CreateObject<LifiAntennaParameters>();
	m_antennaParameters->DetectorArea = 0.01;
	m_antennaParameters->FilterGain = 1.0;
	m_antennaParameters->ConcentratorGain = 1.0;
	m_antennaParameters->HalfPowerDegree = (double)15/180*M_PI;
	m_antennaParameters->FOV = (double)30/180*M_PI;
	m_antennaParameters->TransmitterDegree = (double)30/180*M_PI;
	m_antennaParameters->ReceiverDegree = (double)30/180*M_PI;

}

LifiIndoorPropagationLossModel::~LifiIndoorPropagationLossModel() {
	// TODO Auto-generated destructor stub
}

int64_t LifiIndoorPropagationLossModel::DoAssignStreams(int64_t stream){
	return 0;
}
/*
   *  we assume directed-line-of-sight (LOS) propagation path
   * LifiIndoorPropagationLossModel:
   * m is the mode number of the radiation lobe:
   *               ln(2)
   * m = ---------------------------
   *      ln(cos(halfpowerdegree))
   * halfpowerdegree:   half power angle(radian)
   *
   * R0 is the transmitter radiant intensity:
   * R0 = ((m+1)/(2*Pi))*cos(TransmitterDegree)^m
   * TransmitterDegree:Transmitter angle(radian)
   *
   *H is the DC gain of subchannel:
   *H = (A/d^2)*R0*Ts*Gs*cos(ReceiverDegree);
   *
   * A: is the receiver area of the detector.(m^2)
   * d: distance (m)
   * Ts: the optical filter gain
   * Gs: the concentrator gain
   * ReceiverDegree:Receive angle(radian)
   *
   *
   * if 0 < receive angle < FOV
   * rxPower = 10*log(H*1000)+txPowerDbm
   * or
   * rxPower = 0
   * txPowerDbm:tx power(dbm)
   * FOV:the concentrator field-of-view (radian)
   */
double LifiIndoorPropagationLossModel::DoCalcRxPower(double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const{
	double distance = a->GetDistanceFrom (b);
	double m_temp1 = log(2);
	double m_temp2 = log(cosl(m_antennaParameters->HalfPowerDegree));
	double m = -m_temp1/m_temp2;
	double R0 = ((m+1)/(2*M_PI))*pow(cosl(m_antennaParameters->TransmitterDegree),m);
			if(!(m_antennaParameters->ReceiverDegree < m_antennaParameters->FOV))
			{
				double A_D_temp = m_antennaParameters->DetectorArea/pow(distance,2);
				double H = A_D_temp*R0*m_antennaParameters->FilterGain*m_antennaParameters->ConcentratorGain*cosl(m_antennaParameters->ReceiverDegree);
				double rx = 10*log10(H*1000);
				return rx+txPowerDbm;
			}
			else
				return 0;
}

//double LifiIndoorPropagationLossModel::CalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b){
//	double self;
//	self = DoCalcRxPower(txPowerDbm,a,b);
//	return self;
//}
void LifiIndoorPropagationLossModel::SetLifiAntennaParameters(Ptr<LifiAntennaParameters> parameters){
	m_antennaParameters = parameters;
}

Ptr<LifiAntennaParameters> LifiIndoorPropagationLossModel::GetLifiAntennaParameters(){
	return m_antennaParameters;
}

} /* namespace ns3 */
