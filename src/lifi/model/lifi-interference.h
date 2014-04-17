/*
 * lifi-interference.h
 *
 *  Created on: 2014年4月10日
 *      Author: pp
 */

#ifndef LIFI_INTERFERENCE_H_
#define LIFI_INTERFERENCE_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "lifi-spectrum-error-model.h"

namespace ns3 {

class LifiInterference : public Object
{
public:
	LifiInterference();
	virtual ~LifiInterference();
	static TypeId GetTypeId (void);
	void AddSignal(Ptr<const SpectrumValue> spd, const Time duration);
	void StartRx(Ptr<const Packet> p , Ptr<const SpectrumValue> rxPsd);
	bool EndRx();
	void SetNoisePowerSpectralDensity(Ptr<const SpectrumValue> noisePsd);
	void SetErrorModel(Ptr<LifiSpectrumErrorModel> error);

private:
	bool m_receiving;
	Time m_lastChangeTime;
	Ptr<SpectrumValue> m_rxSignal;
	Ptr<SpectrumValue> m_allSignal;
	Ptr<const SpectrumValue> m_noise;
	Ptr<LifiSpectrumErrorModel> m_errorModel;

	void DoAddSignal(Ptr<SpectrumValue> spd);
	void DoSubtractSignal(Ptr<SpectrumValue> spd);
	void ConditionallyEvaluateChunk();
};

} /* namespace ns3 */

#endif /* LIFI_INTERFERENCE_H_ */
