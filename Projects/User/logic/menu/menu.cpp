/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#include "menu.h"
//#include "delay.h"
#include <string.h>
#include <stdio.h>
//#include "data_flash.h"
#include "data.h"
#include "dhcp_check.h"
//#include "wtn6.h"

uint16_t MENU::check_uid=0;
uint16_t MENU::back=15;
uint16_t MENU::verify=0;
uint8_t MENU::passwd_len=0;
uint8_t MENU::error_count=0;
uint8_t MENU::error_delay=0;
uint8_t MENU::door_count=0;
uint8_t MENU::door_delay=0;
uint32_t MENU::_uid = 0;
uint8_t MENU::open_count=0;

//static uint16_t check_uid=0;
//uint16_t MENU::uid(uint16_t _uid)
//{
//	if(_uid)
//	{
//		check_uid=_uid;
//	}
//	return check_uid;
//}
//void MENU::show_menu(const uint8_t Choice, const uint8_t row, const uint8_t font[], const uint8_t len, const uint8_t ascii)
//{
//	uint32_t i=0;
//	if(Choice)
//	{
//		_display.setCursor(8, 16*row);
//		_display.printf("*");
//	}
//	if(ascii) _display.drawStr(16, 16*row, font, WHITE, BLACK, 2);
//	for(i=0; i<len; i++) 
//	{
//		_display.drawCharCN(16*i+16, 16*row, i, WHITE, BLACK, 1, font);
//	}
//}
uint32_t MENU::start_date(void)
{
	uint16_t year=0, month=0, day=0;
	uint16_t hour=0, minute=0, second=0;
	uint32_t _date;
	get_time(year, month, day, hour, minute, second);
	// start_date; // 录入系统时间, 0x Y:M:D:H
	_date = ((year&0xFF)<<24) | ((month&0xFF)<<16) | ((day&0xFF)<<8) | (hour&0xFF);
	return _date;
}

void MENU::showToast(uint16_t status, const uint16_t _verify, const uint32_t uid)
{
	sys_data_t _sys;
	_data.get_sys(&_sys);
	if (1 == status) 
	{
		if(_sys.verify)
		{
			if(0==verify)
			{
				_display.vformat(false, 0, 0, "已确认", 500, "请校验第二密钥"); // 已确认:10
				verify = _verify;
				_uid = uid;
			}
			//else if((0!=verify) && (0!=_verify) && (_verify!=verify))
			else if((0!=verify) && (0!=_verify) && (_verify!=verify) && ((uid!=_uid) || (VERIFY_PASSWD==verify) || (VERIFY_PASSWD==_verify)))
			{
				//_voice.play_voice(DATA::get_volume(),1,10); // 已确认
				//_display.format(false, 0, 0, 2, 2000, "已确认");
				_display.vformat(false, 0, 0, "已确认", 500, "已确认"); // 已确认:10
				//show_title(0, 32, "已确认!");
				open_count++;
				_motor.open_door();
				verify=0;
			}
			else
			{
				verify=0;
			}
		}
		else
		{
			//_voice.play_voice(DATA::get_volume(),1,10); // 已确认
			//_display.format(false, 0, 0, 2, 2000, "已确认");
			_display.vformat(false, 0, 0, "已确认", 500, "已确认"); // 已确认:10
			//show_title(0, 32, "已确认!");
			open_count++;
			_motor.open_door();
		}
		if(open_count>3)
		{
			open_count=0;
			door_count=0;
			door_delay=0;
		}
	}
	else if (2 == status) 
	{
		show_title(0, 32, "已失效!");
	}
	else 
	{
		//show_title(0, 32, "权限拒绝!");
		//_voice.play_voice(DATA::get_volume(),1,6); // 操作拒绝
		//_display.format(false, 0, 0, 2, 2000, "操作拒绝");
		_display.vformat(false, 0, 0, "操作拒绝", 2000, "操作拒绝"); // 操作拒绝:6
		door_count++;
		if(door_count>30) door_delay=180;
		if(door_count>20) door_delay=90;
		if(door_count>10) door_delay=60;
		if(door_count> 5) door_delay=30;
		//verify=0;
	}
}
void MENU::show_title(const int16_t x, const int16_t y, const char* title)
{
	sys_data_t _sys;
	_data.get_sys(&_sys);
	_display.clear(0);
	_display.setCursor(x, y);
	_display.printf(title);
	_display.update();
	waitting(2000); 
	while(waitting(0)>0);
	waitting(_sys.back); 
}
//void MENU::show_title(const int16_t x, const int16_t y, const char* title, const uint16_t number)
//{
//	char buf[64];
//	memset(buf, 0, sizeof(buf));
//	snprintf(buf, sizeof(buf)-1, "%d %s", number, title);
//	show_title(x, y, buf);
//}
/*
Navigation:
        2
    4       6
        8
*/
uint16_t MENU::browse(data_user_t *_u)
{
	uint8_t key=0;
	uint8_t user_index = 0;
	uint8_t user_start_row = 0;
	uint8_t choice = 0;
	uint8_t max_row=7;
	data_user_t _user;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	const uint16_t len = _data.get_total();
	if(max_row>len) max_row=len;
	memset(&_user, 0, sizeof(data_user_t));
	memset(_u, 0, sizeof(data_user_t));
	_display.setTextSize(1);
	while(waitting(0)>0)
	{
		uint8_t i=0;
		uint8_t index = user_index;
		key=_input.Infr_Scan();//key_temp=Infr_Scan();
		if(key)//if(key_temp)
		{
			waitting(_sys.back); 
			if('2'==key || '^'==key)
			{
				if(0==user_index) user_index=len;
				user_index--;
				if(0==choice)
				{
					if(0==user_start_row) user_start_row=len;
					user_start_row--;
				}
				if(choice>0) choice--;
			}
			if('8'==key || '!'==key)
			{
				for(i=0; i<7; i++)
				{
					user_index++;
					if(len==user_index) user_index=0;
					if((max_row-1)==choice)
					{
						user_start_row++;
						if(len==user_start_row) user_start_row=0;
					}
					if(choice<max_row-1) choice++;
				}
			}
			if('4'==key || '<'==key)
			{
				break;
			}
			if('6'==key || '>'==key)
			{
				//_menu[menu_index].func(1);
				//return &user_data[index];
				index = (user_start_row+choice)%len;
				//get_user_index(index, &_user);
				_data.get_user_index(&_user, index);
				_display.setTextSize(2);
				memcpy(_u, &_user, sizeof(data_user_t));
				_u->uid += user_number_min;
				return 0;
			}
		}
		//index = menu_index;
		_display.clear(0);
		for(i=0; i<max_row; i++)
		{
			index = (user_start_row+i)%len;
			_display.setCursor(0,8*i);
			//get_user_index(index, &_user);
			_data.get_user_index(&_user, index);
			//show_menu(i==choice, i, _menu[index].font, _menu[index].len);
			//printf("%cUID:%5d RF:%c FGR:%c", (i==choice)?'*':' ', (0==user_data[index].uid)?0:(user_data[index].uid-10000), (user_data[index].rfid)?'*':' ', (user_data[index].finger)?'*':' ');
			//_display.printf("%cUID:%5d RF:%c FGR:%c", (i==choice)?'*':' ', (0==_user->uid)?0:(_user->uid), (_user->rfid)?'*':' ', (_user->finger)?'*':' ');
			//_display.printf("%cUID:%4d RF:%c FGR:%c", (i==choice)?16:' ', _user.uid, (_user.rfid)?'*':18, (_user.finger)?'*':' ');
			if((0+user_number_min)==check_uid)
			{
				_display.printf("%cUID:%4d %6d", (i==choice)?16:' ', _user.uid, _user.devnumber);
			}
			else
			{
				_display.printf("%cUID:%4d RF:%c FGR:%c", (i==choice)?16:' ', _user.uid, (_user.rfid)?'*':' ', (_user.finger)?'*':' ');
			}
		}
		_display.setCursor(0,56);
		//printf("Menu:%d %c %d", menu_index, key, get_timeout()/1000);
		//printf("total: %d [%d %d] %d %d", max_row, len, get_user_total(), choice, user_start_row);
		_display.printf("total[%d]: %d [%d]", _data.get_size(), len, (user_start_row+choice)%len+1);
		//show_menu(1, 1, Menu[0].font, Menu[0].len);
		_display.update();
	}
	_display.setTextSize(2);
	return 1;
}
uint16_t MENU::browse_user(data_user_t *_u)
{
	uint8_t key=0;
	//uint8_t user_index = 0;
	uint8_t user_start_row = 0;
	uint8_t choice = 0;
	uint8_t max_row=7;
	data_user_t _user;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	const uint16_t len = _data.get_size()-lock_admin_max;
	if(max_row>len) max_row=len;
	memset(&_user, 0, sizeof(data_user_t));
	memset(_u, 0, sizeof(data_user_t));
	_display.setTextSize(1);
	while(waitting(0)>0)
	{
		uint8_t i=0;
		uint8_t index = 0;//user_index;
		key=_input.Infr_Scan();//key_temp=Infr_Scan();
		if(key)//if(key_temp)
		{
			waitting(_sys.back); 
			if('2'==key || '^'==key)
			{
				//if(0==user_index) user_index=len;
				//user_index--;
				if(0==choice)
				{
					if(0==user_start_row) user_start_row=len;
					user_start_row--;
				}
				if(choice>0) choice--;
			}
			if('8'==key || '!'==key)
			{
				//user_index++;
				//if(len==user_index) user_index=0;
				for(i=0; i<7; i++)
				{
					if((max_row-1)==choice)
					{
						user_start_row++;
						if(len==user_start_row) user_start_row=0;
					}
					if(choice<max_row-1) choice++;
				}
			}
			if('4'==key || '<'==key)
			{
				break;
			}
			if('6'==key || '>'==key)
			{
				index = (user_start_row+choice)%len;
				_data.get_data(&_user, index+lock_admin_max);
				//_display.setTextSize(2);
				//memcpy(_u, &_user, sizeof(data_user_t));
				if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
				{
					_display.setTextSize(2);
					memset(_u, 0, sizeof(data_user_t));
					_u->uid = index+lock_admin_max+user_number_min;
					return 0;
				}
			}
		}
		//index = menu_index;
		_display.clear(0);
		for(i=0; i<max_row; i++)
		{
			index = (user_start_row+i)%len;
			_display.setCursor(0,8*i);
			//get_user_index(index, &_user);
			_data.get_data(&_user, index+lock_admin_max);
			//_user.uid = index+lock_admin_max+user_number_min;
			//if((0+user_number_min)==check_uid)
			//if(0==ret)
			if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
			{
				//_display.printf("%cUID:%4d %6d", (i==choice)?16:' ', _user.uid, _user.devnumber);
				_user.uid = index+lock_admin_max;
				_display.printf("%cUID:%4d ", (i==choice)?16:' ', _user.uid);
			}
			else
			{
				_display.printf("%cUID:%4d RF:%c FGR:%c", (i==choice)?16:' ', _user.uid-user_number_min, (_user.rfid)?'*':' ', (_user.finger)?'*':' ');
			}
		}
		_display.setCursor(0,56);
		//printf("Menu:%d %c %d", menu_index, key, get_timeout()/1000);
		//printf("total: %d [%d %d] %d %d", max_row, len, get_user_total(), choice, user_start_row);
		_display.printf("total[%d]: %d [%d]", _data.get_size(), len, (user_start_row+choice)%len+1);
		//show_menu(1, 1, Menu[0].font, Menu[0].len);
		_display.update();
	}
	_display.setTextSize(2);
	return 1;
}
uint16_t MENU::browse_admin(data_user_t *_u)
{
	uint8_t key=0;
	//uint8_t user_index = 0;
	uint8_t user_start_row = 0;
	uint8_t choice = 0;
	uint8_t max_row=7;
	data_user_t _user;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	const uint16_t len = lock_admin_max;
	if(max_row>len) max_row=len;
	memset(&_user, 0, sizeof(data_user_t));
	memset(_u, 0, sizeof(data_user_t));
	_display.setTextSize(1);
	while(waitting(0)>0)
	{
		uint8_t i=0;
		uint8_t index = 0;//user_index;
		key=_input.Infr_Scan();//key_temp=Infr_Scan();
		if(key)//if(key_temp)
		{
			waitting(_sys.back); 
			if('2'==key || '^'==key)
			{
				//if(0==user_index) user_index=len;
				//user_index--;
				if(0==choice)
				{
					if(0==user_start_row) user_start_row=len;
					user_start_row--;
				}
				if(choice>0) choice--;
			}
			if('8'==key || '!'==key)
			{
				//user_index++;
				//if(len==user_index) user_index=0;
				if((max_row-1)==choice)
				{
					user_start_row++;
					if(len==user_start_row) user_start_row=0;
				}
				if(choice<max_row-1) choice++;
			}
			if('4'==key || '<'==key)
			{
				break;
			}
			if('6'==key || '>'==key)
			{
				index = (user_start_row+choice)%len;
				_data.get_data(&_user, index);
				//_display.setTextSize(2);
				//memcpy(_u, &_user, sizeof(data_user_t));
				if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
				{
					_display.setTextSize(2);
					memset(_u, 0, sizeof(data_user_t));
					_u->uid = index+user_number_min;
					return 0;
				}
			}
		}
		//index = menu_index;
		_display.clear(0);
		for(i=0; i<max_row; i++)
		{
			index = (user_start_row+i)%len;
			_display.setCursor(0,8*i);
			//get_user_index(index, &_user);
			_data.get_data(&_user, index);
			//_user.uid = index+lock_admin_max+user_number_min;
			//if((0+user_number_min)==check_uid)
			//if(0==ret)
			if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
			{
				//_display.printf("%cUID:%4d %6d", (i==choice)?16:' ', _user.uid, _user.devnumber);
				_user.uid = index;
				_display.printf("%cUID:%4d ", (i==choice)?16:' ', _user.uid);
			}
			else
			{
				_display.printf("%cUID:%4d RF:%c FGR:%c", (i==choice)?16:' ', _user.uid-user_number_min, (_user.rfid)?'*':' ', (_user.finger)?'*':' ');
			}
		}
		_display.setCursor(0,56);
		//printf("Menu:%d %c %d", menu_index, key, get_timeout()/1000);
		//printf("total: %d [%d %d] %d %d", max_row, len, get_user_total(), choice, user_start_row);
		_display.printf("total[%d]: %d [%d]", _data.get_size(), len, (user_start_row+choice)%len+1);
		//show_menu(1, 1, Menu[0].font, Menu[0].len);
		_display.update();
	}
	_display.setTextSize(2);
	return 1;
}
uint8_t MENU::get_text_input(const uint8_t _key, uint8_t passwd[32], const uint8_t len, const uint8_t fixation, const char* title)
{
	uint8_t key=0;
	uint8_t i=0;
    //uint32_t input;
	//uint8_t passwd[16]={0};
    uint8_t pass_index=0;
	uint8_t _index=0;
	uint32_t ms;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	
	memset(passwd, 0, len+2);
	waitting(_sys.back); 
	key=_key;
	_input.operation(OPERATION_NUM, key);
	while(waitting(0)>0) // 5s
	{
		ms = tick();
		if(key)
		{
			waitting(_sys.back); 
			//_input.operation(OPERATION_NUM, key);
			if('*'==key) // back
			{
//				if(pass_index>0) 
//				{
//					pass_index--;
//					//if(pass_index>0) ch=passwd[pass_index-1];
//				}
				if(pass_index<len) 
				{
					passwd[pass_index]=key;
					pass_index++;
				}
			}
			else if('#'==key) // OK
			{
				if(fixation)
				{
					if(pass_index<len)
					{
						_display.clear(0);
						_display.setCursor(0,32);
						//_display.printf("len must %d [%d]", len, pass_index);
						_display.printf("输入小于%d位 [%d]", len, pass_index);
						_display.update();
						waitting(2000); // 2s
						while(waitting(0)>0);
						waitting(_sys.back); 
						key=0;//Infr_Scan();
						continue;
					}
				}
				if((pass_index>0) && ('*'==passwd[pass_index-1]))
				{
					pass_index=0;
					key=0;
					continue;
				}
				if(pass_index<len) 
				{
					passwd[pass_index]=key;
					pass_index++;
				}
				break;
			}
			else if(('0'<=key) && ('9'>=key)) // passwd
			{
				//if(pass_index<sizeof(passwd)) 
				if(pass_index<len) 
				{
					passwd[pass_index]=key;
					pass_index++;
					//ch=key;
				}
			}
		}
		_display.clear(0);
		_display.setCursor(0,16);
		_index = pass_index>0?pass_index-1:0;
		//for(i=0; i<sizeof(passwd); i++)
		for(i=0; i<(len+1); i++)
		{
			ms = tick();
			if(i<_index) _display.printf("%c", 1);
			if(pass_index>_index)
			{
				if(i==_index) _display.printf("%c", passwd[i]);
				if(i==pass_index)
				{
					ms = ms/500; // ms/512;
					_display.printf("%c", (ms&0x1)?' ':'_');
				}
				//if(i>pass_index) _display.printf("-");
				if(i>pass_index) _display.printf(" ");
			}
			else
			{
				if(i==_index)
				{
					ms = ms/500; // ms/512;
					_display.printf("%c", (ms&0x1)?' ':'_');
				}
				//if(i>_index) _display.printf("-");
				if(i>_index) _display.printf(" ");
			}
		}
		_display.setCursor(0,0);
		//_display.printf("%s:%d %c %d", title, pass_index, key, waitting_time(0)/1000);
		_display.printf("%s %d", title, waitting(0)/1000);
		//_display.setCursor(0,48);
		//_display.printf("%d", waitting_time(0)/1000);
		_display.update();
		//key=Infr_Scan();
		key=_input.Infr_Scan();
	}
	_input.recover(0);
	return pass_index;
}
uint32_t MENU::get_user_input(const uint8_t _key, const uint8_t len, const uint8_t fixation, const char* title)
{
	uint8_t key=0;
	uint8_t i=0;
	//uint8_t ch=' ';
    uint32_t input;
	uint8_t passwd[16]={0};
    uint8_t pass_index=0;
	uint8_t _index=0;
	uint32_t ms;
//	sys_data_t _sys;
//	_data.get_sys(&_sys);
	
	memset(passwd, 0, sizeof(passwd));
	waitting(back); 
	key=_key;
	_input.operation(OPERATION_NUM, key);
	while(waitting(0)>0) // 5s
	//while(1)
	{
		ms = tick();
		//key=_input.Infr_Scan();
		if(key)
		{
			waitting(back);
			//key = Infr_deco(key_temp);
			if('*'==key) // back
			{
				if(pass_index>0) 
				{
					pass_index--;
					//if(pass_index>0) ch=passwd[pass_index-1];
				}
			}
			else if('#'==key) // OK
			{
				input=0;
				if(fixation)
				{
					if(pass_index<len) //if(pass_index<6)
					{
#if 1
						_display.clear(0);
//						_display.setCursor(0,32);
//						//_display.printf("len must %d [%d]", len, pass_index);
//						_display.printf("输入小于%d位 [%d]", len, pass_index);
//						_display.update();
//						waitting(2000); // 2s
//						while(waitting(0)>0);
						_display.format(0, 32, 2, 2000, "输入小于%d位 [%d]", len, pass_index);
						waitting(back); 
#else
						show_title(0, 32, const char* title)
#endif
						key=0;//Infr_Scan();
						continue;
					}
				}
				for(i=0; i<pass_index; i++)
				{
					input = input*10 + (passwd[i]-'0');
				}
				break;
			}
			else if(('0'<=key) && ('9'>=key)) // passwd
			{
				//if(pass_index<sizeof(passwd)) 
				if(pass_index<len) 
				{
					passwd[pass_index]=key;
					pass_index++;
					//ch=key;
				}
			}
		}
		_display.clear(0);
		_display.setTextSize(2);
		_display.setCursor(0,32);
		_index = pass_index>0?pass_index-1:0;
		//for(i=0; i<sizeof(passwd); i++)
		for(i=0; i<(len+1); i++)
		{
			ms = tick();
			if(i<_index) _display.printf("*");
			if(pass_index>_index)
			{
				if(i==_index) _display.printf("%c", passwd[i]);
				if(i==pass_index)
				{
					ms = ms/500; // ms/512;
					_display.printf("%c", (ms&0x1)?' ':'_');
				}
				//if(i>pass_index) _display.printf("-");
				if(i>pass_index) _display.printf(" ");
			}
			else
			{
				if(i==_index)
				{
					ms = ms/500; // ms/512;
					_display.printf("%c", (ms&0x1)?' ':'_');
				}
				//if(i>_index) _display.printf("-");
				if(i>_index) _display.printf(" ");
			}
		}
		_display.setCursor(0,0);
		//_display.printf("%s:%d %c %d", title, pass_index, key, waitting_time(0)/1000);
		_display.printf("%s", title);
		_display.setCursor(0,48);
		_display.printf("%d", waitting(0)/1000);
		_display.update();
		//key=Infr_Scan();
		key=_input.Infr_Scan();
	}
	_input.recover(0);
	return input;
}

uint32_t MENU::get_user_input(const uint8_t _key, const uint8_t len, const uint8_t fixation, const char* title, const uint16_t uid)
{
	char buf[64];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf)-1, "%d %s", uid, title);
	return get_user_input(_key, len, fixation, buf);
}

void MENU::dhcp4(const uint8_t passwd[32])
{
	uint16_t index=0;
	uint16_t sign=0;
	data_user_t _user;
	for(index=lock_admin_max; index<lock_dhcp4_max; index++)
	{
		_data.get_data(&_user, index);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			sign = dhcp.dhcp4_check(passwd, _user.devnumber);
			//sign = dhcp.dhcp4_check(passwd, 345678);
			showToast(sign, VERIFY_DHCP4, _user.uid);
			if(1==sign) break;
		}
	}
}
void MENU::dhcp6(const uint8_t passwd[32])
{
	uint16_t index=0;
	uint16_t sign=0;
	data_user_t _user;
	for(index=lock_dhcp4_max; index<lock_dhcp6_max; index++)
	{
		_data.get_data(&_user, index);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			sign = dhcp.dhcp6_check(passwd, _user.devnumber);
			showToast(sign, VERIFY_DHCP6, _user.uid);
			if(1==sign) break;
		}
	}
}
void MENU::dhcp8(const uint8_t passwd[32])
{
	uint16_t index=0;
	uint16_t sign=0;
	data_user_t _user;
	for(index=lock_dhcp6_max; index<_data.get_size(); index++)
	{
		_data.get_data(&_user, index);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			sign = dhcp.dhcp8_check(passwd, _user.devnumber);
			showToast(sign, VERIFY_DHCP8, _user.uid);
			if(1==sign) break;
		}
	}
}
int16_t MENU::match_passwd(const uint8_t uid, uint32_t input)
{
	uint32_t sign=0;
	uint16_t uid2=0;
	// static passwd
	data_user_t _user;
	_data.get_data(&_user, uid);
	if((user_number_min<=_user.uid) && (user_number_max>_user.uid) && ((uid+user_number_min)==_user.uid))
	{
		sign = _user.devnumber/1000;
		uid2=_data.check_admin(uid+user_number_min, input); // static passwd
		//menu.show_title(0, 32, "权限拒绝!");
		if(sign>=10000)     passwd_len=8;
		else if(sign>=1000) passwd_len=7;
		else if(sign>=100)  passwd_len=6;
		else if(sign>=10)   passwd_len=5;
		else if(sign>=1)    passwd_len=4;
		else passwd_len=0;
		if(0xFFFF==uid2) 
		{
			return passwd_len;
		}
		else
		{
			//menu.show_title(0, 32, "已确认!");
			return 1;
		}
	}
	return -1;
}
void MENU::rfid(const uint16_t _id)
{
	uint16_t index=0;
	uint16_t sign=0;
	data_user_t _user;
	for(index=2; index<_data.get_size(); index++)
	{
		_data.get_data(&_user, index);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			if(_id == _user.rfid) sign=1;
			else sign=0;
			showToast(sign, VERIFY_RFID, _user.uid);
			if(1==sign) break;
		}
	}
}
void MENU::finger(const uint16_t _id)
{
	uint16_t index=0;
	uint16_t sign=0;
	data_user_t _user;
	for(index=2; index<_data.get_size(); index++)
	{
		_data.get_data(&_user, index);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			if(_id == _user.finger) sign=1;
			else sign=0;
			showToast(sign, VERIFY_FING, _user.uid);
			if(1==sign) break;
		}
	}
}


/*
Navigation:
        2
    4       6
        8
*/
void MENU::common(const menu_t* _menu, const uint16_t len, const uint8_t max_row)
{
	uint8_t key=0;
	//uint8_t key_tmp=0;
	uint8_t menu_index = 0;
	uint8_t menu_start_row = 0;
	uint8_t choice = 0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	back = _sys.back;
	waitting(back); 
	while(waitting(0))
	//while(1)
	{
		uint8_t i=0;
		uint8_t index = menu_index;
		key=_input.Infr_Scan();
		if(key)
		{
			waitting(back); 
			//key = Infr_deco(key_temp);
			//key_tmp = key;
			if('2'==key || '^'==key)
			{
				if(0==menu_index) menu_index=len;
				menu_index--;
				if(0==choice)
				{
					if(0==menu_start_row) menu_start_row=len;
					menu_start_row--;
				}
				if(choice>0) choice--;
			}
			if('8'==key || '!'==key)
			{
				menu_index++;
				if(len==menu_index) menu_index=0;
				if((max_row-1)==choice)
				{
					menu_start_row++;
					if(len==menu_start_row) menu_start_row=0;
				}
				if(choice<max_row-1) choice++;
			}
			if('4'==key || '<'==key)
			{
				break;
			}
			if('6'==key || '>'==key)
			{
				_menu[menu_index].func();
			}
		}
//		index = menu_index;
		_display.clear(0);
		for(i=0; i<max_row; i++)
		{
			//index = (menu_index+i)%len;
			//show_menu(index==menu_index, i, _menu[index].font, _menu[index].len);
#if 0
			index = (menu_start_row+i)%len;
			show_menu(i==choice, i, _menu[index].font, _menu[index].len, _menu[index].ascii);
#else
			index = (menu_start_row+i)%len;
			_display.setCursor(8, 16*i);
			//_display.printf("%c%s %c", (i==choice)?'*':' ', _menu[index].font, key_tmp);
			//_display.printf("%d %c%s", choice, (i==choice)?'*':' ', _menu[index].font);
			_display.printf("%c%s", (i==choice)?16:' ', _menu[index].font);
#endif
//			index++;
//			if(len==index) index=0;
		}
		//_display.setCursor(0,48);
		//printf("Menu:%d %c %d", menu_index, key, get_timeout()/1000);
		//show_menu(1, 1, Menu[0].font, Menu[0].len);
		_display.update();
	}
}


