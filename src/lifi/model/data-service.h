/*
 * data-service.h
 *
 *  Created on: 2014年3月30日
 *      Author: will
 */

#ifndef DATA_SERVICE_H_
#define DATA_SERVICE_H_

#include "ns3/core-module.h"
#include "ns3/network-module.h"

namespace ns3 {

class DataService {
public:
	DataService();
	virtual ~DataService();
	virtual void Send(uint32_t size, Ptr<Packet> p, uint8_t band, bool contention) = 0;
};

} /* namespace ns3 */

#endif /* DATA_SERVICE_H_ */
