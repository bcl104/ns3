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
#include <map>
#include <vector>
#include "lifi-phy-general.h"

namespace ns3 {

class LifiInterference : public Object
{
public:
	LifiInterference();
	virtual ~LifiInterference();
	static TypeId GetTypeId (void);
	void AddSignal(Ptr<const SpectrumValue> psd, const Time duration);
//	void AddSignalDbm(double rxPower , const Time duration);//using Dbm to calculate the interference
	void LifiAddSignal(Ptr<SpectrumValue> psd,Time duration);
	void LifiSubtractSignal(Ptr<SpectrumValue> psd);
	void StartRx(Ptr<const Packet> p , Ptr<const SpectrumValue> rxPsd);
	bool EndRx();
	void SetNoisePowerSpectralDensity(Ptr<const SpectrumValue> noisePsd);
	void SetErrorModel(Ptr<LifiSpectrumErrorModel> error);
//	double GetAllSignalDbm(void);
//	void SetAllSignalSpd(SpectrumValue);
//	Ptr<SpectrumValue> GetAllSignalSpd(void);
	void SetReceiveState(PhyOpStatus state);//change receive state and clear m_allsignalPsd
	PhyOpStatus GetReceiveState(void);
	Ptr<SpectrumValue> GetAllSignal(void);
	void SetAllsignal(Ptr<SpectrumValue> allsignal);
	Ptr<SpectrumValue> CalcuAveInterference(Time duration,Time startTime);
	Ptr<SpectrumValue> CalSinr(Ptr<SpectrumValue> rxSignal,Ptr<SpectrumValue> AveAllSignal );
	void CancelEvent(void);//cancel unfinished event when finish receiving
	double  BandIntegral(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand);
private:
	PhyOpStatus m_receiving;
	Time m_lastChangeTime;
	Ptr<SpectrumValue> m_rxSignal;
	Ptr<SpectrumValue> m_allSignal;
	Ptr<const SpectrumValue> m_noise;
	Ptr<LifiSpectrumErrorModel> m_errorModel;
	std::map<Time,Ptr<SpectrumValue> > m_allSignalPsd;
	std::vector<EventId> m_eventId;
//	double m_allSignalDbm;
//	double m_rxSignalDbm;

	void DoLifiAddSignal(Ptr<SpectrumValue> psd);
	void DoLifiSubtractSignal(Ptr<SpectrumValue> psd);
	void DoSubtractSignal(Ptr<SpectrumValue> psd);
//	void DoAddSignalDbm(double rxPower,Time duration);
//	void DoSubtractSignalDbm(double rxPower);
	void DoAddSignal(Ptr<const SpectrumValue> psd);
	void ConditionallyEvaluateChunk();
};

} /* namespace ns3 */

#endif /* LIFI_INTERFERENCE_H_ */
