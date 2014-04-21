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

namespace ns3 {

//class AntennaModel;
//class SpectrumChannel;
//class NetDevice;
//class MobilityModel;
//class SpecturmModel;
//struct SpectrumSignalParameters;
//class LifiSpectrumChannel;


class LifiSpectrumPhy : public SpectrumPhy
{


public:
	LifiSpectrumPhy(Ptr<NetDevice> device);

	LifiSpectrumPhy();

	virtual ~LifiSpectrumPhy();

	static TypeId GetTypeId ();

	void SetNodeSpectrum(uint8_t channel);

	void  Send(Ptr<Packet>pb,uint32_t size,uint8_t band,  bool isCellMode,  uint8_t cellId,  uint8_t trxid,double txPower,Time duration);

	bool CarrierSense();

	uint8_t CarrierSense(uint8_t band);

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

//	uint8_t GetBand();

private:
//	void EndRx (Ptr<SpectrumSignalParameters> params);

	/**
	 * pp.214, Table 76&mdash;Visible light wavelength band plan.
	 * .
	 */
	Ptr<AntennaModel> m_antenna;
	Ptr<NetDevice> m_device;
	Ptr<MobilityModel> m_mobility;
	Ptr<LifiInterference> m_interference;
	Ptr<LifiSpectrumSignalParameters> m_SignalParameters;
	Ptr<SpectrumChannel> m_channel;
	double m_rxPowerTh;

};

} /* namespace ns3 */

#endif /* LIFI_SPECTRUM_PHY_H_ */
