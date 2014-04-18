/*
 * lifi-phy.h
 *
 *  Created on: 2014年4月4日
 *      Author: will
 */

#ifndef LIFI_PHY_H_
#define LIFI_PHY_H_

#define MAX_PHY_FRAME_SIZE 65535

#include "ns3/core-module.h"
#include "lifi-phy-pib-attributes.h"
#include "lifi-net-device.h"
#include "lifi-spectrum-phy.h"
#include "pd-sap-user.h"
#include "pd-sap-provider.h"
#include "plme-sap-user.h"
#include "plme-sap-provider.h"
#include "lifi-cell.h"
#include "lifi-phy-header.h"
#include <vector>

namespace ns3 {

class LifiNetDevice;
class LifiSpectrumPhy;
EventId m_endTxEvent;

class LifiPhy : public Object
{

public:

	static const uint32_t aMaxPHYFrameSize = MAX_PHY_FRAME_SIZE;

	/**
	 * RX-to-TX maximum turnaround time, PHY 1:<=240 optical clock cycles.
	 */
	static const uint32_t aTurnaroundTime_RX_TX = 240;

	/**
	 * TX-to-RX maximum turnaround time.
	 */
	static const uint32_t aTurnaroundTime_TX_RX = 240;

	LifiPhy();
	virtual ~LifiPhy();

	LifiPhy(Ptr<LifiSpectrumPhy> spectrum);

	LifiPhy(LifiPhyPibAttribute pib, Ptr<LifiSpectrumPhy> spectrum);

	LifiPhy(std::vector< Ptr<LifiCell> > cellList);

	static TypeId GetTypeId();

	Ptr<LifiPhyPibAttribute> GetPhyPibAttributes ();

	bool DoCca();

	uint8_t DoCca(uint8_t band);

	void SetDeviceAttribute(PhyOpStatus status);

	void Transmit(uint32_t size, Ptr<Packet> pb, uint8_t band);

	void Receive(Ptr<LifiSpectrumSignalParameters> param);

	void Switch (bool** sw_bit_map, bool dir);

	void SetTRxState(PhyOpStatus state);

	double GetTxPower ();

	double GetEdThreadhold ();

	double GetCsThreadhold ();

	void DoSetCellId(uint16_t cellId);

	Ptr<PdSapProvider> GetPdSapProvider ();

	Ptr<PlmeSapProvider> GetPlmeSapProvider ();

	void SetPdSapUser (Ptr<PdSapUser> u);

	void SetPlmeSapUser (Ptr<PlmeSapUser> u);

	void SetcellMode (bool cellmode);

	bool GetcellMode (void);

	Ptr<LifiNetDevice> GetLifiNetDevice ();

	void SetLifiNetDevice (Ptr<LifiNetDevice> device);

	void SetLifiPhyHeader (Ptr<LifiPhyHeader> lifiphyheader);

	Ptr<LifiPhyHeader> GetLifiPhyHeader(void);

	void SetcellId(uint8_t cellId);

	uint8_t GetcellId();

	void SettrxId(uint8_t trxid);

	uint8_t GettrxId();

//	std::vector< Ptr<LifiSpectrumPhy> > GetSpectrumPhyList ();

//	void AddLifiSpectrumPhy(Ptr<LifiSpectrumPhy> spectrum);
	Ptr<LifiSpectrumPhy>  GetSpectrumPhy ();



	void AddCellList(Ptr<LifiCell> cell);

	Ptr<PdSapProvider> m_pdSapProvider;
	Ptr<PdSapUser> m_pdSapUser;
	Ptr<PlmeSapProvider> m_plmeSapProvider;
	Ptr<PlmeSapUser> m_PlmeSapUser;
	Ptr<LifiPhyHeader> m_phyheader;
protected:
	void StartTx (Ptr<Packet> pb);
	void EndTx (PhyOpStatus m_trxStatus);
	double GetRate(uint8_t mcsId);

private:

	double m_txPower;
	double m_edTh;
	double m_csTh;
	bool m_cellMode;
	PhyOpStatus m_trxStatus;
	TRxMode m_trxMode;
	uint8_t m_cellId;
	uint8_t m_trxid;
	Time duration;
	LifiPhyPibAttribute m_attributes;
	Ptr<LifiNetDevice> m_device;
	Ptr<LifiSpectrumPhy> m_spectrumPhy;
	std::vector< Ptr<LifiCell> > m_cellList;
//	std::vector< Ptr<LifiSpectrumPhy> > m_spectrumPhyList;

};

} /* namespace ns3 */

#endif /* LIFI_PHY_H_ */
