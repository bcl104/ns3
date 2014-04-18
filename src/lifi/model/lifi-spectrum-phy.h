/*
 * lifi-spectrum-phy.h
 *
 *  Created on: 2014\u5e744\u67084\u65e5
 *      Author: will
 */

#ifndef LIFI_SPECTRUM_PHY_H_
#define LIFI_SPECTRUM_PHY_H_

#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-signal-parameters.h"
#include "ns3/antenna-model.h"
#include "ns3/spectrum-channel.h"
#include "ns3/net-device.h"
#include "ns3/mobility-model.h"
#include "ns3/spectrum-model.h"
#include "lifi-interference.h"
#include "lifi-spectrum-channel.h"

namespace ns3 {

class AntennaModel;
class SpectrumChannel;
class NetDevice;
class MobilityModel;
class SpecturmModel;

struct LifiSpectrumSignalParameters : public SpectrumSignalParameters
{
	LifiSpectrumSignalParameters ();
	LifiSpectrumSignalParameters (const LifiSpectrumSignalParameters& p);
	virtual Ptr<SpectrumSignalParameters> Copy ();
	Ptr<PacketBurst> pb;
	uint8_t cellId;
	uint8_t trxId;
	bool cellMode;
	uint8_t band;
	double txPower

};

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

	Ptr<LifiSpectrumChannel> GetChannel();
private:
//	void EndRx (Ptr<SpectrumSignalParameters> params);

	/**
	 * pp.214, Table 76&mdash;Visible light wavelength band plan.
	 * .
	 */
	Ptr<AntennaModel> m_antenna;
	c
	Ptr<NetDevice> m_device;
	Ptr<MobilityModel> m_mobility;
	Ptr<LifiInterference> m_interference;
	Ptr<LifiSpectrumSignalParameters> m_SignalParameters;

};

} /* namespace ns3 */

#endif /* LIFI_SPECTRUM_PHY_H_ */
