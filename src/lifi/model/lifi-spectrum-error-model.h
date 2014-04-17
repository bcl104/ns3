/*
 * lifi-spectrum-error-model.h
 *
 *  Created on: 2014年4月10日
 *      Author: pp
 */

#ifndef LIFI_SPECTRUM_ERROR_MODEL_H_
#define LIFI_SPECTRUM_ERROR_MODEL_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/spectrum-value.h"
#include "ns3/spectrum-signal-parameters.h"

namespace ns3 {

class LifiSpectrumErrorModel: public Object {
public:
	LifiSpectrumErrorModel();
	virtual ~LifiSpectrumErrorModel();

	static TypeId GetTypeId();

	void StartRx (Ptr<const PacketBurst> p);
	void EvaluateChunk (const SpectrumValue& sinr, Time duration);
	bool IsRxCorrect ();

private:
	uint32_t m_bytes;
	uint32_t m_deliverableBytes;
};

} /* namespace ns3 */

#endif /* LIFI_SPECTRUM_ERROR_MODEL_H_ */
