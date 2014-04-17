/*
 * LifiIndoorSpectrumErrorModel.cc
 *
 *  Created on: 2014年4月10日
 *      Author: pp
 */

#include "lifi-spectrum-error-model.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE("LifiSpectrumErrorModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiSpectrumErrorModel);

LifiSpectrumErrorModel::LifiSpectrumErrorModel() {
	// TODO Auto-generated constructor stub
	NS_LOG_FUNCTION (this);
	m_bytes = 0;
	m_deliverableBytes = 0;
}

LifiSpectrumErrorModel::~LifiSpectrumErrorModel() {
	// TODO Auto-generated destructor stub
	NS_LOG_FUNCTION (this);
}

TypeId LifiSpectrumErrorModel::GetTypeId(){
	static TypeId tid = TypeId ("ns3::LifiSpectrumErrorModel")
							.SetParent<Object>()
							.AddConstructor<LifiSpectrumErrorModel>();
	return tid;
}

void LifiSpectrumErrorModel::StartRx (Ptr<const PacketBurst> p){
	NS_LOG_FUNCTION (this);
}

void LifiSpectrumErrorModel::EvaluateChunk (const SpectrumValue& sinr, Time duration){
	NS_LOG_FUNCTION (this);
}

bool LifiSpectrumErrorModel::IsRxCorrect (){
	  NS_LOG_FUNCTION (this);
	  return (m_deliverableBytes > m_bytes);
}

} /* namespace ns3 */

