/*
 * plme-sap-provider.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PLME_SAP_PROVIDER_H_
#define PLME_SAP_PROVIDER_H_

#include "ns3/core-module.h"
#include "lifi-phy-general.h"
#include "lifi-phy-pib-attributes.h"

namespace ns3 {

class PlmeSapProvider : public SimpleRefCount<PlmeSapProvider>
{
public:
	PlmeSapProvider();

	virtual ~PlmeSapProvider();

	virtual bool PlmeCcaRequset() = 0;

	virtual uint8_t PlmeCcaRequset(uint8_t band_plan) = 0;

	template <class T>
	T PlmeGetRequset(PhyPIBAttributeId id)
	{
		return *(T*)Get(id);
	}

	template <class T>
	void PlmeSetRequest(PhyPIBAttributeId id, T values)
	{
		T v = values;
		Set(id, (void*)(&v));
	}

	virtual PhyOpStatus PlmeSetTRXStateRequest(PhyOpStatus status) = 0;

	virtual void PlmeSwitchRequest(bool** sw_bit_map, bool dir) = 0;

protected:
	virtual void* Get (PhyPIBAttributeId id) = 0;

	virtual void Set (PhyPIBAttributeId id, void* values) = 0;
};

template <class C>
class PlmeSpecificSapProvider : public PlmeSapProvider
{
public:
	PlmeSpecificSapProvider (C* phy)
	{
		m_phy = phy;
	}

	virtual ~PlmeSpecificSapProvider ()
	{

	}

	virtual bool PlmeCcaRequset()
	{
		return m_phy->DoCca ();
	}

	virtual uint8_t PlmeCcaRequset(uint8_t band)
	{
		return m_phy->DoCca (band);
	}

	virtual PhyOpStatus PlmeSetTRXStateRequest(PhyOpStatus status)
	{
		return m_phy->SetTRxState (status);
	}

	virtual void PlmeSwitchRequest(bool** sw_bit_map, bool dir)
	{
		m_phy->Switch (sw_bit_map, dir);
	}

protected:

	virtual void* Get (PhyPIBAttributeId id)
	{
		return m_phy->GetPhyPibAttributes ()->GetAttributes (id);
	}

	virtual void Set (PhyPIBAttributeId id, void* values)
	{
		m_phy->GetPhyPibAttributes ()->SetAttributes (id, values);
	}

private:
	PlmeSpecificSapProvider ()
	{

	}

	C* m_phy;
};

} /* namespace ns3 */

#endif /* PLME_SAP_PROVIDER_H_ */
