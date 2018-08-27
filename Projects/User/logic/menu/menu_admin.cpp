/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#include "menu_level.h"
//#include "delay.h"
//#include "user.h"
//#include "main.h"
//#include "RTC.h"
#include "data.h"
#include <string.h>

//extern ssd1306syp display;


void func_browse(void)
{
	data_user_t _user;
	menu.browse(&_user);
}
void func_time(void)
{
	menu.func_time();
}
void MENU_LEVEL::func_time(void)
{
//	RTC_TimeTypeDef Time;
//	RTC_DateTypeDef Date;
	
	uint8_t key=0;
	//uint8_t key_temp;
	uint8_t buf[14]; // Y:4, M:2, D:2, H:2, M:2, S:2
	uint8_t tmp[14]; // Y:4, M:2, D:2, H:2, M:2, S:2
	uint16_t year=0, month=0, day=0;
	uint16_t hour=0, minute=0, second=0;
	//uint16_t num=0;
	uint32_t ms;
	uint8_t index=0;
	uint8_t i=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	waitting(_sys.back);
	
	init:
	get_time(year, month, day, hour, minute, second);
#if 0
	RTC_GetTime(RTC_Format_BIN, &Time);
	RTC_GetDate(RTC_Format_BIN, &Date);
	year = Date.RTC_Year;//+2000;
//	tmp[0] = year/1000;
//	data = year%1000;
//	tmp[1] = year/100;
//	data = year%100;
	tmp[0] = 2;
	tmp[1] = 0;
	tmp[2] = year/10;
	tmp[3] = year%10;
	tmp[4] = Date.RTC_Month/10;
	tmp[5] = Date.RTC_Month%10;
	tmp[6] = Date.RTC_Date/10;
	tmp[7] = Date.RTC_Date%10;
	tmp[8] = Time.RTC_Hours/10;
	tmp[9] = Time.RTC_Hours%10;
	tmp[10] = Time.RTC_Minutes/10;
	tmp[11] = Time.RTC_Minutes%10;
	tmp[12] = Time.RTC_Seconds/10;
	tmp[13] = Time.RTC_Seconds%10;
#endif
	tmp[0]  = 2;
	tmp[1]  = 0;
	tmp[2]  = year/10;
	tmp[3]  = year%10;
	tmp[4]  = month/10;
	tmp[5]  = month%10;
	tmp[6]  = day/10;
	tmp[7]  = day%10;
	tmp[8]  = hour/10;
	tmp[9]  = hour%10;
	tmp[10] = minute/10;
	tmp[11] = minute%10;
	tmp[12] = second/10;
	tmp[13] = second%10;
	index=2;
	while(waitting(0)>0)
	{
		ms = tick();
		ms = ms/500; // ms/512;
		for(i=0; i<14; i++)
		{
			buf[i] = tmp[i]+'0';
		}
		if(ms&0x1) buf[index] = ' ';
		_display.clear(0);
#if 1
		_display.setCursor(0,0);
		_display.printf("时间同步");
		_display.setCursor(0,16);
		_display.printf("%c%c%c%c.%c%c.%c%c", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		_display.setCursor(0,32);
		_display.printf("%c%c:%c%c:%c%c  %c", buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], (14==index)?'#':' ');
#else
		_display.format(0,  0, 2, 0, "时间同步");
		_display.format(0, 16, 2, 0, "%c%c%c%c.%c%c.%c%c", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		_display.format(0, 32, 2, 0, "%c%c:%c%c:%c%c  %c", buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], (14==index)?'#':' ');
#endif
//			setCursor(0,48);
//			printf("%d:%d:%d", sys_data.hour, sys_data.minute, sys_data.second);
		_display.update();
		key=_input.Infr_Scan();
		if(key)
		{
			waitting(_sys.back);
			//key = Infr_deco(key_temp);
			if('*'==key) // back
			{
				if(index>2) index--;
			}
			else if('#'==key) // OK
			{
				if(index<14)
				{
					index++;
					continue;
				}
				year = 0;
//				for(i=0; i<4; i++)
//				{
//					year = year*10 + tmp[i];
//				}
#if 0
				sys_data.year = data;
				sys_data.month = tmp[4]*10+tmp[5];
				sys_data.day = tmp[6]*10+tmp[7];
				sys_data.hour = tmp[8]*10+tmp[9];
				sys_data.minute = tmp[10]*10+tmp[11];
				sys_data.second = tmp[12]*10+tmp[13];
#else
				// check
				year = tmp[2]*10+tmp[3];
				if(year<18) 
				{
					menu.show_title(0, 32, "Year ERROR!");
					index=0;
					goto init;
				}
				month = tmp[4]*10+tmp[5];
				if(month>12) 
				{
					menu.show_title(0, 32, "Month ERROR!");
					index=4;
					goto init;
				}
				day = tmp[6]*10+tmp[7];
				if(day>31) 
				{
					menu.show_title(0, 32, "Day ERROR!");
					index=6;
					goto init;
				}
				hour = tmp[8]*10+tmp[9];
				if(hour>23) 
				{
					menu.show_title(0, 32, "Hour ERROR!");
					index=8;
					goto init;
				}
				minute = tmp[10]*10+tmp[11];
				if(minute>59) 
				{
					menu.show_title(0, 32, "Minute ERROR!");
					index=10;
					goto init;
				}
				second = tmp[12]*10+tmp[13];
				if(second>59) 
				{
					menu.show_title(0, 32, "Second ERROR!");
					index=12;
					goto init;
				}
#endif
				// save
#if 0
				//SetTime(0);
				Date.RTC_Year = year;
				Date.RTC_Month = month;
				Date.RTC_Date = day;
				Time.RTC_Hours = hour;
				Time.RTC_Minutes = minute;
				Time.RTC_Seconds = second;
				RTC_SetTime(RTC_Format_BIN, &Time);
				RTC_SetDate(RTC_Format_BIN, &Date);
#endif
				set_time(year, month, day, hour, minute, second);
				break;
			}
			else if(('0'<=key) && ('9'>=key)) // passwd
			{
				//if(pass_index<sizeof(passwd)) 
				if(index<14) 
				{
					tmp[index]=key-'0';
					index++;
				}
			}
		}
	}
}
void func_add(void)
{
	menu.func_add();
}
void MENU_LEVEL::func_add(void)
{
	data_user_t _user;
	uint32_t passwd=0;
	uint32_t passwd_tmp=0;
	uint16_t count=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	waitting(_sys.back);
	while(waitting(0)>0)
	{
#if 0
		_data.get_user_empty(&_user, lock_dhcp4_max);
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			menu.show_title(0, 32, "存储已满!");
			break;
		}
#else
		browse_user(&_user);
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			break;
		}
#endif
		_voice.play_info(DATA::get_volume(), 1, "请输入密码");
		passwd = menu.get_user_input(0, 6, 1, "请输入密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		_voice.play_info(DATA::get_volume(), 1, "请再次输入密码");
		passwd_tmp = menu.get_user_input(0, 6, 1, "再次输入密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		if(passwd==passwd_tmp)
		{
			_data.save_data(_user.uid, passwd, start_date(), 10000, 0, 0);
			// OK
			//menu.show_title(0, 32, "添加成功!");
			_display.vformat(false, 0, 0, "操作完成", 2000, "添加成功!"); 
			//printf("%d", uid);
			break;
		}
		else
		{
			//menu.show_title(0, 32, "密码不一致!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "密码不一致!"); 
			count++;
			//if(count<3) waitting_time(back);
			//else break;
			if(count>=3) break;
		}
	}
}
void func_add_admin(void)
{
	menu.func_add_admin();
}
void MENU_LEVEL::func_add_admin(void)
{
	data_user_t _user;
	uint32_t passwd=0;
	uint32_t passwd_tmp=0;
	uint16_t count=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	waitting(_sys.back);
	while(waitting(0)>0)
	{
#if 0
		_data.get_user_empty(&_user, lock_admin_max);
		//menu.get_user_input(6, 1, "管理员", _user.uid);
		//if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		if((user_number_min>_user.uid) || ((user_number_min+lock_admin_max)<=_user.uid))
		{
			menu.show_title(0, 32, "存储已满!");
			break;
		}
#else
		browse_admin(&_user);
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			break;
		}
#endif
		_voice.play_info(DATA::get_volume(), 1, "请输入密码");
		passwd = menu.get_user_input(0, 6, 0, "请输入密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		_voice.play_info(DATA::get_volume(), 1, "请再次输入密码");
		passwd_tmp = menu.get_user_input(0, 6, 0, "再次输入密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		if(passwd==passwd_tmp)
		{
			_data.save_data(_user.uid, passwd, start_date(), 10000, 0, 0);
			// OK
			//menu.show_title(0, 32, "添加成功!");
			_display.vformat(false, 0, 0, "操作完成", 2000, "添加成功!");
			//printf("%d", uid);
			break;
		}
		else
		{
			//menu.show_title(0, 32, "密码不一致!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "密码不一致!"); 
			count++;
			//if(count<3) waitting_time(back);
			//else break;
			if(count>=3) break;
		}
	}
}
void func_del(void)
{
	menu.func_del();
}
void MENU_LEVEL::func_del(void)
{
	data_user_t _user;
	menu.browse(&_user);
	if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
	{
		return ;
	}
	if(MENU::check_uid==_user.uid)
	{
		//menu.show_title(0, 32, "操作拒绝!");
		_display.vformat(false, 0, 0, "操作拒绝", 2000, "操作拒绝!"); 
	}
	else
	{
		// del
		if(0==_data.user_del(_user.uid))
		{
			//menu.show_title(0, 32, "已删除!");
			_display.vformat(false, 0, 0, "操作完成", 2000, "已删除!"); 
		}
		else
		{
			//menu.show_title(0, 32, "删除失败!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "删除失败!"); 
		}
	}
}
void func_net(void)
{
	menu.show_title(0, 32, "网络");
}
extern void func_info(void);
extern void func_set(void);
menu_t Menu[] = 
{
	//{func_admin, CN_16x16_admin[0], sizeof(CN_16x16_admin)/32},
	{func_add,       "添加用户"},
	{func_add_admin, "添加管理员"},
	{func_del,       "删除用户"},
	{func_info,      "信息"},
	{func_browse,    "浏览"},
	{func_net,       "网络"},
	{func_time,      "时间同步"}, 
	{func_set,       "系统设置"}, 
	//{func_about, CN_16x16_about[0], sizeof(CN_16x16_about)/32}
};

const unsigned int Menu_size = sizeof(Menu)/sizeof(Menu[0]);

void MENU_LEVEL::menu_admin_option(void)
{
	common(Menu, Menu_size, 4);
}
//void MENU_ADMIN::option(void)
//{
//	common(Menu, Menu_size, 4);
//}

