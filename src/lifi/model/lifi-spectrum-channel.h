/*
 * lifi-spectrum-channel.h
 *
 *  Created on: 2014年4月15日
 *      Author: pp
 */

#ifndef LIFI_SPECTRUM_CHANNEL_H_
#define LIFI_SPECTRUM_CHANNEL_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-model.h"
//#include "lifi-indoor-propagation-loss-model.h"
//#include "lifi-spectrum-phy.h"
#include <map>
#include <vector>

namespace ns3 {

struct LifiSpectrumSignalParameters;
class LifiSpectrumPhy;

//typedef std::multimap<uint8_t ,std::pair<Ptr<LifiSpectrumPhy>,uint32_t> > PhyList;

typedef std::multimap<uint8_t ,Ptr<LifiSpectrumPhy> > PhyList;
//typedef std::multimap<uint8_t ,Ptr<SpectrumPhy> > PhyList;

class LifiSpectrumChannel: public SpectrumChannel {

public:
	LifiSpectrumChannel();
	virtual ~LifiSpectrumChannel();
	static TypeId GetTypeId (void);

	// inherited from SpectrumChannel
	virtual void AddPropagationLossModel(Ptr<PropagationLossModel> loss);

	virtual void AddSpectrumPropagationLossModel(Ptr<SpectrumPropagationLossModel> loss);

	virtual void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);

	virtual void StartTx(Ptr<SpectrumSignalParameters> params);

	std::pair<PhyList::iterator,PhyList::iterator> SearchRxList(uint8_t band);

	std::pair<PhyList::iterator,PhyList::iterator> SearchTxList(uint8_t band);

//	virtual void AddRx(uint8_t band ,Ptr<LifiSpectrumPhy> phy);
//	void AddRx(uint8_t band ,Ptr<LifiSpectrumPhy> phy);

	virtual void AddRx (Ptr<SpectrumPhy> phy) ;

	// inherited from Channel
  /**
   * \returns the number of NetDevices connected to this Channel.
   *
   * This method must be implemented by subclasses.
   */
	virtual Ptr<NetDevice> GetDevice (uint32_t i) const ;

	virtual uint32_t GetNDevices (void) const ;

  /**
   * \param i index of NetDevice to retrieve
   * \returns one of the NetDevices connected to this channel.
   *
   * This method must be implemented by subclasses.
   */
	virtual Ptr<NetDevice> GetRxDevice (uint32_t i) const;

	virtual Ptr<NetDevice> GetTxDevice (uint32_t i) const;

		//
	virtual Ptr<SpectrumPropagationLossModel> GetSpectrumPropagationLossModel (void);

	Ptr<PropagationLossModel> GetPropagationModel(void);//added to get propagation loss model

    void SetSpectrumMap(std::map<int,int> spectrum);

	void AddTx(Ptr<LifiSpectrumPhy> phy);

	bool DeleteRx(Ptr<LifiSpectrumPhy> phy);

	bool DeleteTx(Ptr<LifiSpectrumPhy> phy);

	void AddRxInterference(Ptr<LifiSpectrumPhy> phy);

	void SubtraRxInterference(Ptr<LifiSpectrumPhy> phy);

	double CalcMyCcaPower(Ptr<MobilityModel> myMobilityModel,uint8_t bandId);

//    void AddTx(uint8_t band,Ptr<SpectrumPhy> phy);
//
//	bool DeleteRx(uint8_t band,Ptr<SpectrumPhy> phy);
//
//	bool DeleteTx(uint8_t band,Ptr<SpectrumPhy> phy);

//	void SetRxPowerTh(double th);
//
//	double GetmRxPowerTh(void);

//	void SetDelay(Time dalay);
//
//	Time GetDelay(void);

protected:
	std::map<int,int> m_channelMap;

private:
  /**
   * used internally to reschedule transmission after the propagation delay
   *
   * @param params
   * @param receiver
   */
	void StartRx (Ptr<LifiSpectrumSignalParameters> params, Ptr<LifiSpectrumPhy> receiver);
//	void StartRx (Ptr<SpectrumSignalParameters> params, Ptr<SpectrumPhy> receiver);

  /**
   * SpectrumModel that this channel instance
   * is supporting
   */
	Ptr<const SpectrumModel> m_spectrumModel;


  /**
   * propagation delay model to be used with this channel
   *
   */
	Ptr<PropagationDelayModel> m_propagationDelay;


  /**
	* single-frequency propagation loss model to be used with this channel
	*
	*/


  /**
   * frequency-dependent propagation loss model to be used with this channel
   *
   */
	Ptr<SpectrumPropagationLossModel> m_spectrumPropagationLoss;

	Ptr<PropagationLossModel>  m_propagationLossModel;

	uint32_t m_numDevices;

	uint32_t m_rxNumDevices;

	uint32_t m_txNumDevices;

//	double m_rxPowerTh;

//	Time m_delay ;

	PhyList m_rxPhyList;

	PhyList m_txPhyList;

};

} /* namespace ns3 */

#endif /* LIFI_SPECTRUM_CHANNEL_H_ */
