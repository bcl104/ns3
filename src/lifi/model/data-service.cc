/*
 * data-service.cc
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#include "data-service.h"

namespace ns3 {

DataService::DataService() {
	m_connected = false;
	m_base = 0;
}

DataService::~DataService() {
	if (m_base != 0)
		free (m_base);
}

} /* namespace ns3 */
