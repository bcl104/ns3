/*
 * plme-sap-user.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef PLME_SAP_USER_H_
#define PLME_SAP_USER_H_

#include "ns3/core-module.h"
#include "lifi-mac-general.h"
#include "lifi-phy-general.h"

namespace ns3 {

class PlmeSapUser : public SimpleRefCount<PlmeSapUser>
{

public:
	PlmeSapUser();
	virtual ~PlmeSapUser();

//	virtual void PlmeCcaConfirm(PhyOpStatus status) = 0;
//	virtual void PlmeGetConfirm(PhyOpStatus status, PhyPIBAttributeId id, uint8_t value) = 0;
//	virtual void PlmeSetConfirm(PhyOpStatus status, PhyPIBAttributeId id) = 0;
//	virtual void PlmeSetTRXStateConfirm(PhyOpStatus status) = 0;
	virtual void PlmeSwitchConfirm(PhyOpStatus status) = 0;

};

template <class C>
class PlmeSpecificSapUser : public PlmeSapUser
{
public:
	PlmeSpecificSapUser (C* mac)
	{
		m_mac = mac;
	}

	virtual ~PlmeSpecificSapUser ()
	{
	}

	virtual void PlmeSwitchConfirm(PhyOpStatus status)
	{
		m_mac->SwitchConfirm (status);
	}

private:
	PlmeSpecificSapUser ()
	{
	}

	C* m_mac;
};

} /* namespace ns3 */

#endif /* PLME_SAP_USER_H_ */
