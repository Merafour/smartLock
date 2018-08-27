// -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : safety.cpp
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the safety library.
* Description        : 安全设备接口.
******************************************************************************/

#include "safety.h"

uint16_t safety::event=0;


uint16_t safety::revent(void)
{
	return event;
}
void safety::wevent(const uint16_t mask)
{
	event |= mask;
}
void safety::cevent(const uint16_t mask)
{
	event &= (~mask);
}



