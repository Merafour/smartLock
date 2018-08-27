/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : rgbw.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the MY9221 library.

******************************************************************************/
#ifndef _RGBW_H_
#define _RGBW_H_


#include "main.h"
#include "object.h"


class RGBW : public rgbw_obj {
public:
    RGBW() {}
	void Init(void);
	void Send(uint8_t *data);
	virtual void flowing(uint8_t _flow) { start=_flow;}
	virtual void init(void) { Init();}
	virtual void update(void);
	virtual void flowing(void);
	virtual void show(uint8_t key);
	virtual void multi(uint16_t mask);
	uint8_t data[12];
private:
	void flow(void);
	uint64_t ms;

};

//extern RGBW rgbw;

#endif















