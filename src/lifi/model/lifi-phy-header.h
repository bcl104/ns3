/*
 * lifi-phy-header.h
 *
 *  Created on: 2014年4月14日
 *      Author: pp
 */

#ifndef LIFI_PHY_HEADER_H_
#define LIFI_PHY_HEADER_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"

#define P1 111101011001000

namespace ns3 {

struct SHR{
	uint8_t* FLP;
	uint16_t TDP1;/*uint16_t*/
	uint16_t TDP2;/*uint16_t*/
	uint16_t TDP3;/*uint16_t*/
	uint16_t TDP4;/*uint16_t*/

};

class LifiPhyHeader: public Header {
public:
	LifiPhyHeader();
	virtual ~LifiPhyHeader();

	static TypeId GetTypeId (void);

	virtual TypeId GetInstanceTypeId (void) const;

	virtual void Print (std::ostream &os) const;

	virtual uint32_t GetSerializedSize (void) const;

	virtual void Serialize (Buffer::Iterator start) const;

	virtual uint32_t Deserialize (Buffer::Iterator start);

	void SetBurstMode(bool isBurstMode);

	bool GetBurstMode();

	void SetChannelNum(uint8_t channelNum);

	uint8_t GetChannelNum();

	void SetMcsId(uint8_t mcsId);

	uint8_t GetMcsId();

	void SetPsduLength(uint16_t length);

	uint16_t GetPsduLength();

	void SetOokDimmed(bool isOok);

	bool GetOokDimmed();

	void SetReservedFields(uint8_t field);

	uint8_t GetReservedFields();

	void SetPreamble(SHR s);

	SHR GetPreamble();

	void SetFlpSize(uint64_t size);/*64-16384*/

	uint64_t GetFlpSize();

	void SetHcs(uint16_t hcs);

	uint16_t GetHcs();

private:
	bool m_isBurstMode;
	uint8_t m_channelNum;
	uint8_t m_mcsId;
	uint16_t m_psduLength;
	bool m_ookDimmed;
	uint8_t m_reservedFields;
	uint64_t m_flpSize;
	uint16_t m_hcs;
	SHR m_shr;
};

} /* namespace ns3 */

#endif /* LIFI_PHY_HEADER_H_ */
