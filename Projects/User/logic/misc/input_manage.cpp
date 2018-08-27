/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : input_manage.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the data flash library.

******************************************************************************/
#include "delay.h"
#include "input_manage.h"
//#include "rgbw.h"
#include "data.h"
//#include "wtn6.h"

#include "bs8116a-3.h"
#include "stm32_i2c.h"
uint16_t input_manage::_operation=OPERATION_NULL;

void input_manage::Init(void)
{
	STM32_I2Cx_Init(I2C1);
	STM32_I2Cx_Init(I2C2);
	Bs8116a_3_init();
}
void input_manage::update(void)
{
}
uint8_t input_manage::get_key(void)
{
	uint16_t key_tmp=0;
	uint8_t key=0;
	key_tmp=Key_value();
	if(key_last!=key_tmp)
	{
		key_last = key_tmp;
		if(key_tmp&KEY_NUM0)       key = '0';
		else if(key_tmp&KEY_NUM1)  key = '1';
		else if(key_tmp&KEY_NUM2)  key = '2';
		else if(key_tmp&KEY_NUM3)  key = '3';
		else if(key_tmp&KEY_NUM4)  key = '4';
		else if(key_tmp&KEY_NUM5)  key = '5';
		else if(key_tmp&KEY_NUM6)  key = '6';
		else if(key_tmp&KEY_NUM7)  key = '7';
		else if(key_tmp&KEY_NUM8)  key = '8';
		else if(key_tmp&KEY_NUM9)  key = '9';
		else if(key_tmp&KEY_LOCK)  key = '*';
		else if(key_tmp&KEY_BELL)  key = '#';
		
		return key;
	}
	return 0;
}
uint8_t input_manage::Infr_Scan(void)
{
	uint8_t key=0;
	key=get_key();
	if(key)
	{
#if 0
		if('*'==key) rgbw.show(9);
		else if('#'==key) rgbw.show(11);
		else if('0'==key) rgbw.show(10);
		else
		{
			rgbw.show(key-'1');
		}
#else
		key_led(key);
		//while(voice.Read_Busy());
		//voice.Play_Num(10,1,"0");
		//_voice.play_voice(DATA::get_volume(),1,0); // µÎ
		_voice.play_info(DATA::get_volume() , 1, "µÎ"); // µÎ
#endif
	}
	return key;
}
void input_manage::key_led(const uint8_t key)
{
	switch(_operation)
	{
		case OPERATION_MENU:
			_rgbw.multi(RGBW_MENU);
			break;
		case OPERATION_RFID:
		case OPERATION_FING:
			_rgbw.multi(RGBW_RFID);
			break;
		case OPERATION_BEEP:
			_rgbw.multi(RGBW_BEEP);
			break;
		case OPERATION_NULL:
			_rgbw.flowing(1);
			break;
		case OPERATION_NUM:
			if('*'==key) 
			{
				_rgbw.show(9);
			}
			else if('#'==key) 
			{
				_rgbw.show(11);
			}
			else if('0'==key) 
			{
				_rgbw.show(10);
			}
			else
			{
				_rgbw.show(key-'1');
			}
			break;
		default:
			break;
	}
}
void input_manage::operation(uint16_t mask, const uint8_t key)
{
	_recover = _operation;
	_operation = mask;
	key_led(key);
}

uint32_t input_manage::rfid(void)
{
	return 0;
}
uint32_t input_manage::finger(void)
{
	return 0;
}

//uint8_t input_manage::read(const uint16_t Addr, uint16_t *buf, const uint16_t len)
//{
//	uint16_t index=0;
//	return index;
//}

//uint8_t input_manage::write(const uint16_t Addr, const uint16_t *buf, const uint16_t len)
//{
//	uint16_t index=0;
//	return index;
//}


void input_manage::key_test(void)
{
	//uint16_t key=0;
	while(1)
	{
//		uint16_t key_tmp=0;
//		key_tmp=Key_value();
//		if(key!=key_tmp)
//		{
//			key = key_tmp;
//			if(key&KEY_NUM0)  rgbw.show(10);
//			else if(key&KEY_NUM1)  rgbw.show(0);
//			else if(key&KEY_NUM2)  rgbw.show(1);
//			else if(key&KEY_NUM3)  rgbw.show(2);
//			else if(key&KEY_NUM4)  rgbw.show(3);
//			else if(key&KEY_NUM5)  rgbw.show(4);
//			else if(key&KEY_NUM6)  rgbw.show(5);
//			else if(key&KEY_NUM7)  rgbw.show(6);
//			else if(key&KEY_NUM8)  rgbw.show(7);
//			else if(key&KEY_NUM9)  rgbw.show(8);
//			else if(key&KEY_LOCK)  rgbw.show(9);
//			else if(key&KEY_BELL)  rgbw.show(11);
//		}
		uint8_t key=0;
		key=get_key();
		if(key)
		{
			if('*'==key) _rgbw.show(10);
			else if('#'==key) _rgbw.show(11);
			else
			{
				_rgbw.show(key-'0');
			}
		}
	}
}

