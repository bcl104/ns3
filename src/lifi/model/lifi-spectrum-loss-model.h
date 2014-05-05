/*
 * lifi-spectrum-loss-model.h
 *
 *  Created on: 2014年4月24日
 *      Author: st125475466
 */

#ifndef LIFI_SPECTRUM_LOSS_MODEL_H_
#define LIFI_SPECTRUM_LOSS_MODEL_H_
#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>
#include "ns3/spectrum-propagation-loss-model.h"

namespace ns3 {
struct LifiAntennaParameters;
/**
 * \ingroup spectrum
 *
 * \brief spectrum-aware propagation loss model
 *
 * Interface for propagation loss models to be adopted when
 * transmissions are modeled with a power spectral density by means of
 * the SpectrumValue class.
 *
 */
class LifiSpectrumPropagationLossModel: public SpectrumPropagationLossModel {
public:
	LifiSpectrumPropagationLossModel();
	virtual ~LifiSpectrumPropagationLossModel();
	void SetLifiAntennaParameters(Ptr<LifiAntennaParameters> parameters);
	Ptr<LifiAntennaParameters> GetLifiAntennaParameters();
//	double  BandIntegral(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand);
	Ptr<SpectrumValue> GetBandPsd(Ptr<SpectrumValue> psd , uint8_t band , uint8_t SubBand);
	double GetBandWidth(void);
	void SetBandWidth(Ptr<SpectrumValue>psd , uint8_t band , uint8_t subBand);
//  static TypeId GetTypeId ();
//
//
//  /**
//   * used to chain various instances of SpectrumPropagationLossModel
//   *
//   * @param next
//   */
//  void SetNext (Ptr<SpectrumPropagationLossModel> next);
//
//  /**
//   * This method is to be called to calculate
//   *
//   * @param txPsd the SpectrumValue representing the power spectral
//   * density of the transmission. Watt units are to be used for radio
//   * communications, and Pascal units for acoustic communications
//   * (e.g. underwater).
//   *
//   * @param a sender mobility
//   * @param b receiver mobility
//   *
//   * @return set of values vs frequency representing the received
//   * power in the same units used for the txPower parameter.
//   */
//  Ptr<SpectrumValue> CalcRxPowerSpectralDensity (Ptr<const SpectrumValue> txPsd,
//                                                 Ptr<const MobilityModel> a,
//                                                 Ptr<const MobilityModel> b) const;
//
//protected:
//  virtual void DoDispose ();

private:
	/**
	 *
	 *
	 * @param txPsd set of values vs frequency representing the
	 * transmission power. See SpectrumChannel for details.
	 * @param a sender mobility
	 * @param b receiver mobility
	 *
	 * @return set of values vs frequency representing the received
	 * power in the same units used for the txPower parameter.
	 */
	virtual Ptr<SpectrumValue> DoCalcRxPowerSpectralDensity(
			Ptr<const SpectrumValue> txPsd, Ptr<const MobilityModel> a,
			Ptr<const MobilityModel> b) const;

//  Ptr<SpectrumPropagationLossModel> m_next;
	double DoCalcRxPower(double txPowerDbm, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const;
	Ptr<LifiAntennaParameters> m_antennaParameters;
	double m_bandwith;
};

} // namespace ns3

#endif /* LIFI_SPECTRUM_LOSS_MODEL_H_ */
