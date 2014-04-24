/*
 * lifi-indoor-propagation-loss-model.h
 *
 *  Created on: 2014年4月9日
 *      Author: pp
 */

#ifndef LIFI_INDOOR_PROPAGATION_LOSS_MODEL_H_
#define LIFI_INDOOR_PROPAGATION_LOSS_MODEL_H_

#include "ns3/core-module.h"
#include "cmath"
#include "ns3/mobility-model.h"
#include "ns3/propagation-loss-model.h"

namespace ns3 {

struct LifiAntennaParameters: public Object
{
	double DetectorArea;
	double FilterGain;
	double ConcentratorGain;
	double HalfPowerDegree;
	double FOV;
	double TransmitterDegree;
	double ReceiverDegree;
};

class LifiIndoorPropagationLossModel: public PropagationLossModel {
public:
	LifiIndoorPropagationLossModel();
	virtual ~LifiIndoorPropagationLossModel();
	void SetLifiAntennaParameters(Ptr<LifiAntennaParameters> parameters);
	Ptr<LifiAntennaParameters> GetLifiAntennaParameters();
//	double CalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b);
private:
	virtual int64_t DoAssignStreams(int64_t stream);
	//input unit is dbm out unit id dbm.
	virtual double DoCalcRxPower(double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b);
	Ptr<LifiAntennaParameters> m_antennaParameters;
};

} /* namespace ns3 */

#endif /* LIFI_INDOOR_PROPAGATION_LOSS_MODEL_H_ */
