/*
 * lifi-spectrum-phy.h
 *
 *  Created on: 2014\u5e744\u67084\u65e5
 *      Author: will
 */

#ifndef LIFI_SPECTRUM_PHY_H_
#define LIFI_SPECTRUM_PHY_H_

#include "ns3/spectrum-phy.h"
#include "ns3/antenna-model.h"
//#include "ns3/spectrum-channel.h"
#include "ns3/net-device.h"
#include "ns3/mobility-model.h"
#include "ns3/spectrum-model.h"
#include "lifi-interference.h"
#include "lifi-spectrum-channel.h"
#include "lifi-spectrum-signal-parameters.h"
//#include "lifi-phy.h"

namespace ns3 {

//class AntennaModel;
//class SpectrumChannel;
//class NetDevice;
//class MobilityModel;
//class SpecturmModel;
//struct SpectrumSignalParameters;
//class LifiSpectrumChannel;
class LifiPhy;


class LifiSpectrumPhy : public SpectrumPhy
{


public:
	LifiSpectrumPhy(Ptr<NetDevice> device);

	LifiSpectrumPhy();

	virtual ~LifiSpectrumPhy();

	static TypeId GetTypeId ();

	void SetNodeSpectrum(uint8_t channel);

	void  Send(Ptr<Packet>pb,uint32_t size,uint8_t band,  bool isCellMode,  uint8_t cellId,  uint8_t trxid,double txPower,Time duration,
			    Ptr<SpectrumValue> psd,Time txTime);

	bool CarrierSense();

	bool CarrierSense(uint8_t band,double edTh);

	virtual void SetDevice (Ptr<NetDevice> d);

	virtual Ptr<NetDevice> GetDevice ();

	virtual void SetMobility (Ptr<MobilityModel> m);

	virtual Ptr<MobilityModel> GetMobility ();

	virtual void SetChannel (Ptr<SpectrumChannel> c);

	virtual Ptr<const SpectrumModel> GetRxSpectrumModel () const;

	virtual Ptr<AntennaModel> GetRxAntenna ();

	virtual void StartRx (Ptr<SpectrumSignalParameters> params);

	Ptr<AntennaModel> GetAntennaModel ();

	void SetAntennaModel (Ptr<AntennaModel> antenna);

	void SetCellId(uint16_t id);

	uint16_t GetCellId();

	void SetErrorModel(Ptr<LifiSpectrumErrorModel> e);

	void SetSpectrumSignalParameters(Ptr<LifiSpectrumSignalParameters> param);

	Ptr<LifiSpectrumSignalParameters> GetSpectrumSignalParameters();

	Ptr<SpectrumChannel> GetChannel();

	void SetRxPowerTh(double th);

	double GetmRxPowerTh(void);

	void EndRx(Ptr<LifiSpectrumSignalParameters> params);

	void SetInterference(Ptr<LifiInterference> interference);

	Ptr<LifiInterference> GetInterference(void);

	void SetBerTh(double ber);

	double GetberTh(void);

//	void SetLifiPhy(Ptr<LifiPhy> phy);
//
//	Ptr<LifiPhy> GetLifiPhy(void);

//	uint8_t GetBand();

private:
//	void EndRx (Ptr<SpectrumSignalParameters> params);

	/**
	 * pp.214, Table 76&mdash;Visible light wavelength band plan.
	 * .
	 */
	uint8_t CalculateWqi(double sinr);

	double CalculateBer(double sinr);//ber = 1/2(erfc(sprt(sinr/4)))

	double CalculateErf(double x,uint8_t n);//x:upper limit of integral n:number of check

	Ptr<AntennaModel> m_antenna;
	Ptr<NetDevice> m_device;
	Ptr<MobilityModel> m_mobility;
	Ptr<LifiInterference> m_interference;
	Ptr<LifiSpectrumSignalParameters> m_SignalParameters;
	Ptr<SpectrumChannel> m_channel;
//	Ptr<LifiPhy> m_phy;
	double m_rxPowerTh;
	double m_berTh;
	uint8_t m_rxNumCount;
//	uint8_t m_WQI;
//	spectrum

};

} /* namespace ns3 */

#endif /* LIFI_SPECTRUM_PHY_H_ */
