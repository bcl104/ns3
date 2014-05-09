/*
 * lifi-mac-handler.h
 *
 *  Created on: 2014年4月24日
 *      Author: will
 */

#ifndef LIFI_MAC_HANDLER_H_
#define LIFI_MAC_HANDLER_H_
#include "ns3/object.h"
#include "src/lifi/utils/utils.h"

#include <map>

namespace ns3 {

class LifiMacImpl;
class PlmeSapProvider;
class PdSapProvider;
class LifiMacPibAttribute;

typedef std::map<void*, bool> TrigMap;

class LifiMacHandler: public Object
{
public:
	LifiMacHandler();

	virtual ~LifiMacHandler();

	static TypeId GetTypeId ();

	void SetMacPibAttributes (LifiMacPibAttribute* attrubutes);


protected:

	template <class T> void AddTrigger (T trigger, bool enable)
	{
		void* key = mem_cast(trigger);
		NS_ASSERT (m_trigger.find(key) == m_trigger.end());
		m_trigger.insert(std::pair<void*, bool> (key, enable));
	}

	template <class T> void DelTrigger (T trigger)
	{
		void* key = mem_cast(trigger);
		NS_ASSERT (m_trigger.find(key) != m_trigger.end());
		m_trigger.erase(key);
	}

	template <class T> void EnableTrigger (T trigger)
	{
		void* key = mem_cast(trigger);
		TrigMap::iterator it = m_trigger.find(key);
		NS_ASSERT (it != m_trigger.end());
		it->second = true;
	}

	template <class T> void DisableTrigger (T trigger)
	{
		void* key = mem_cast(trigger);
		TrigMap::iterator it = m_trigger.find(key);
		NS_ASSERT (it != m_trigger.end());
		it->second = false;
	}

	void EnableAllTrigger ()
	{
		TrigMap::iterator it = m_trigger.begin();
		while (it != m_trigger.end())
		{
			it->second = true;
			it++;
		}
	}

	void DisableAllTrigger ()
	{
		TrigMap::iterator it = m_trigger.begin();
		while (it != m_trigger.end())
		{
			it->second = false;
			it++;
		}
	}

	template <class T> bool CheckTrigger (T trigger)
	{
		void* key = mem_cast(trigger);
		TrigMap::iterator it = m_trigger.find(key);
		NS_ASSERT (it != m_trigger.end());
		return it->second;
	}

	template <class T1, class T2, class T3> void ReplaceTriggerCallback (T1 &callback, T2 replace, T3 obj)
	{
		callback.Nullify();
		callback = MakeCallback(replace, obj);
	}

	template <class T1, class T2> void SwitchState (T1 newState, T2 type)
	{

		(type->*newState) ();
	}


	TrigMap m_trigger;

	LifiMacImpl* m_impl;
	PlmeSapProvider* m_plmeProvider;
	PdSapProvider* m_pdProvider;
	LifiMacPibAttribute* m_attributes;
	const Time* m_opticalPeriod;
};

#define AddTrigger(trigger, enable) AddTrigger(&trigger,enable)
#define DelTrigger(trigger) DelTrigger(&trigger)
#define EnableTrigger(trigger) EnableTrigger(&trigger)
#define DisableTrigger(trigger) DisableTrigger(&trigger)
#define CheckTrigger(trigger) CheckTrigger(&trigger)

#define SwitchState(state) SwitchState(&state, this)
#define ReplaceTriggerCallback(callback,replace) ReplaceTriggerCallback(callback,&replace,this)

} /* namespace ns3 */


#endif /* LIFI_MAC_HANDLER_H_ */
