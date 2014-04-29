/*
 * lifi-spectrum-signal-parameters.h
 *
 *  Created on: 2014年4月21日
 *      Author: st125475466
 */

#ifndef LIFI_SPECTRUM_SIGNAL_PARAMETERS_H_
#define LIFI_SPECTRUM_SIGNAL_PARAMETERS_H_

#include "ns3/core-module.h"
#include "ns3/packet.h"
#include "ns3/spectrum-signal-parameters.h"

namespace ns3{
struct LifiSpectrumSignalParameters : public SpectrumSignalParameters
{
	LifiSpectrumSignalParameters ();
	LifiSpectrumSignalParameters (const LifiSpectrumSignalParameters& p);
	virtual Ptr<SpectrumSignalParameters> Copy ();
	Ptr<Packet> pb;
	uint8_t cellId;
	uint8_t trxId;
	bool cellMode;
	uint8_t band;
	double trxPower;
	Time time;
//	uint32_t PsduSize;
//	bool burstMode;
//	uint8_t mcsId;
//	bool ookDim;
//	uint8_t reservedFields;

};
}



#endif /* LIFI_SPECTRUM_SIGNAL_PARAMETERS_H_ */
