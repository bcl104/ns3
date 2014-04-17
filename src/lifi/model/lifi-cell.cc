/*
 * lifi-cell.cc
 *
 *  Created on: 2014-4-10
 *      Author: root
 */

#include "lifi-cell.h"
#include <ns3/log.h>
#include <iostream>

NS_LOG_COMPONENT_DEFINE("LifiCell");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(LifiCell);

LifiCell::LifiCell(){
	NS_LOG_FUNCTION(this);
	NS_FATAL_ERROR ("This constructor should not be called");
}

LifiCell::LifiCell(Ptr<LifiSpectrumPhy> spectrumPhy,uint16_t id) {
	// TODO Auto-generated constructor stub
	NS_LOG_FUNCTION (this);
	m_spectrumPhy = spectrumPhy;
	m_cellId = id;
}

LifiCell::~LifiCell() {
	// TODO Auto-generated destructor stub
	NS_LOG_FUNCTION (this);
}
TypeId LifiCell::GetTypeId(){
	static TypeId tid = TypeId ("ns3::LifiCell")
						.SetParent<Object>()
						.AddConstructor<LifiCell>();
	return tid;
}


void LifiCell::BindSpectrumPhy(){
	NS_LOG_FUNCTION (this);
	m_spectrumPhy->SetCellId(m_cellId);
}


void LifiCell::UnbindSpectrumPhy(){
	NS_LOG_FUNCTION (this);
}


void LifiCell::UpdateCell(std::vector< Ptr<LifiTransmitter> > trxList){
	NS_LOG_FUNCTION (this);
	m_lifiTrxList = trxList;
}

Ptr<LifiSpectrumPhy> LifiCell::GetLifiSpectrumPhy(){
	NS_LOG_FUNCTION (this);
	return m_spectrumPhy;
}

void LifiCell::SetCellId (uint8_t id){
	NS_LOG_FUNCTION (this<<id);
	m_cellId = id;
	if(m_spectrumPhy){
		m_spectrumPhy->SetCellId(m_cellId);
	}else{
		std::cout<<"Warning:not bind a lifiSpectrumPhy!"<<std::endl;
	}
}
uint8_t LifiCell::GetCellId (){
	NS_LOG_FUNCTION (this);
	return m_cellId;
}

} /* namespace ns3 */
