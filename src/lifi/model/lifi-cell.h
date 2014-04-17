/*
 * lifi-cell.h
 *
 *  Created on: 2014-4-10
 *      Author: root
 */

#ifndef LIFI_CELL_H_
#define LIFI_CELL_H_

#include "ns3/core-module.h"
#include "ns3/object.h"
#include "lifi-spectrum-phy.h"
#include <vector>

namespace ns3 {

struct LifiTransmitter : public Object
{
	uint8_t m_id;
};

class LifiCell: public Object {
public:
	LifiCell();
	LifiCell(Ptr<LifiSpectrumPhy> spectrumPhy,uint16_t id);
	virtual ~LifiCell();
	static TypeId GetTypeId();
	void BindSpectrumPhy();
	void UnbindSpectrumPhy();
	void UpdateCell(std::vector< Ptr<LifiTransmitter> > trxList);
	Ptr<LifiSpectrumPhy> GetLifiSpectrumPhy();
	void SetCellId (uint8_t id);
	uint8_t GetCellId ();

private:
	Ptr<LifiSpectrumPhy> m_spectrumPhy;
	std::vector< Ptr<LifiTransmitter> >  m_lifiTrxList;
	uint16_t m_cellId;
	uint32_t a;


};

} /* namespace ns3 */
#endif /* LIFI_CELL_H_ */
