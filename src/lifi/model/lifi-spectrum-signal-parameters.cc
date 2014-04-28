/*
 * lifi-spectrum-signal-parameters.cc
 *
 *  Created on: 2014年4月21日
 *      Author: st125475466
 */

#include "lifi-spectrum-signal-parameters.h"
#include "ns3/ptr.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LifiSpectrumSignalParameters");

namespace ns3 {

LifiSpectrumSignalParameters::LifiSpectrumSignalParameters (){
	NS_LOG_FUNCTION (this);
	pb = 0;
	cellId = -1;
	trxId = -1;
	cellMode = false;
	band = 0x01;
	trxPower = 1.0;
	time = Seconds(0);
//	PsduSize = 0;
//	burstMode = false;
//	mcsId = 0;
//	ookDim = false;
//	reservedFields = 0;
}

LifiSpectrumSignalParameters::LifiSpectrumSignalParameters (const LifiSpectrumSignalParameters& p)
:SpectrumSignalParameters (p)
{
	NS_LOG_FUNCTION (this);
	pb = p.pb->Copy();
	cellId = p.cellId;
	trxId = p.trxId;
	cellMode = p.cellMode;
	band = p.band;
	trxPower = p.trxPower;
	time = p.time;
//	PsduSize = 0;
//	burstMode = false;
//	mcsId = 0;
//	ookDim = false;
//	reservedFields = 0;
}

Ptr<SpectrumSignalParameters> LifiSpectrumSignalParameters::Copy (){
	NS_LOG_FUNCTION (this);
	return 0;
}

}


