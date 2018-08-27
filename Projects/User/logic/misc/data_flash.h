/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : data_flash.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#ifndef _DATA_FLASH_H_
#define _DATA_FLASH_H_

#include "data.h"


class DATA_FLASH: public DATA {
public:
	DATA_FLASH(){}
	void Init(void);
	//uint16_t check_admin(const uint16_t uid);
protected:
	virtual uint8_t read(const uint16_t Addr, uint16_t *buf, const uint16_t len);
	virtual uint8_t write(const uint16_t Addr, const uint16_t *buf, const uint16_t len);
	virtual uint8_t erase_all(void);
	virtual uint16_t lenght(void);
private:
};

//extern DATA_FLASH data;
//extern DATA_RAM data;

#if 1
#define DATA_TYPE  DATA_FLASH
#else
#define DATA_TYPE  DATA_RAM
#endif

//extern DATA_TYPE data;

#endif















