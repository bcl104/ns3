/*
 * utils.h
 *
 *  Created on: 2014年4月24日
 *      Author: will
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "ns3/core-module.h"

template <class T>
void* mem_cast(T ptr)
{
	T _ptr = ptr;
	T *_pptr = &_ptr;
	void* r = (void*) *reinterpret_cast<long*> (_pptr);
	return r;
}


#endif /* UTILS_H_ */
