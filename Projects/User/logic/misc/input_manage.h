/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : input_manage.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the input_manage library.

******************************************************************************/
#ifndef _INPUT_MANAGE_H_
#define _INPUT_MANAGE_H_

#include <stdint.h>
#include "object.h"

#define OPERATION_NULL   0x0000
#define OPERATION_NUM    0x0001
#define OPERATION_MENU   0x0002
#define OPERATION_RFID   0x0004
#define OPERATION_FING   0x0008
#define OPERATION_BEEP   0x0010


class input_manage {
public:
	input_manage(voice_obj &voice, rgbw_obj &rgbw):_voice(voice), _rgbw(rgbw){}
	void Init(void);
	void update(void);
	uint32_t rfid(void);
	uint32_t finger(void);
	uint8_t get_key(void);
	uint8_t Infr_Scan(void);
	void operation(uint16_t mask, const uint8_t key);// {_operation=mask;}
	void recover(const uint8_t key) { operation(_recover, key);}
	
	void key_test(void);
protected:
//	uint8_t read(const uint16_t Addr, uint16_t *buf, const uint16_t len);
//	uint8_t write(const uint16_t Addr, const uint16_t *buf, const uint16_t len);
private:
	void key_led(const uint8_t key);
	uint16_t key_last;
	//uint8_t key;
	static uint16_t _operation;
	uint16_t _recover;
	voice_obj &_voice;
	rgbw_obj  &_rgbw;
};

//extern input_manage input;

#endif















