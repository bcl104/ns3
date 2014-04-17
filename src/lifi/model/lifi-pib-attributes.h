/*
 * lifi-pib-attribute.h
 *
 *  Created on: 2014年4月3日
 *      Author: will
 */

#ifndef LIFI_PIB_ATTRIBUTE_H_
#define LIFI_PIB_ATTRIBUTE_H_
#include "ns3/core-module.h"
#include <map>

#define LOAD_PIB_ATTRIBUTE(id, attribute, value) \
attribute = (value);\
std::pair<uint8_t, void*> p_##attribute ((uint8_t)(id),(void*)&(attribute));\
m_attributes.insert (p_##attribute)

#define TRY_SET_PIB_ATTRIBUTES(_id,type)\
if (id == _id)\
	*(type*)(it->second) = *(type*)value;\
return

namespace ns3 {

class LifiPIBAttribute : public SimpleRefCount<LifiPIBAttribute>
{
public:
	LifiPIBAttribute();

	virtual ~LifiPIBAttribute();

protected:

	std::map<uint8_t, void*> m_attributes;

};



} /* namespace ns3 */

#endif /* LIFI_PIB_ATTRIBUTE_H_ */
