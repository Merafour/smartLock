/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
//#include "user.h"
#include "menu_level.h"
//#include "delay.h"
#include <string.h>
//#include "data_flash.h"
#include "input_manage.h"
//#include "fingerprint.h"
//#include "power.h"

//extern ssd1306syp display;
//static uint16_t user_check_admin(void)
//{
//	menu.user_check_admin();
//}
uint16_t MENU_LEVEL::user_check_admin(void)
{
    uint16_t uid=0;
	uint16_t uid2=0;
	uint32_t passwd=0;
//	sys_data_t _sys;
//	data.get_sys(&_sys);
	while(waitting(0)>0)
	{
		uid = menu.get_user_input(0, 4, 0, "Admin Number");
		passwd = menu.get_user_input(0, 6, 1, "Admin Passwd");
		uid2=_data.check_admin(uid+user_number_min, passwd);
		if((0xFFFF==uid2) || (uid2!=uid)) 
		{
			menu.show_title(0, 32, "Passwd or Number ERROR!");
		}
	}
	return uid;
}
static void func_admin(void)
{
	menu.func_admin();
}
void MENU_LEVEL::func_admin(void)
{
#if 1
	//menu.show_title(0, 32, "admin");
	uint16_t uid=0;
	uint16_t uid2=0;
	uint32_t passwd=0;
	if((check_uid>=user_number_min) && (check_uid<user_number_max) && (65535!=_data.search_uid(check_uid))) // admin max number
	{
		//menu_common(Menu, Menu_size, 4);
		menu_admin_option();
	}
	else
	{
		//uid=user_check_admin();
//		uid = menu.get_user_input(4, 0, "Admin Number");
//		passwd = menu.get_user_input(6, 1, "Admin Passwd");
//		uid2=data.check_admin(uid+user_number_min, passwd);
		_voice.play_info(DATA::get_volume(), 1, "请输入用户编号");
		uid = menu.get_user_input(0, 4, 0, "请输入管理员编号");
		if(0==waitting(0)) return;
		_voice.play_info(DATA::get_volume(), 1, "请输入密码");
		passwd = menu.get_user_input(0, 8, 0, "请输入密码");
		uid2=_data.check_admin(uid+user_number_min, passwd);
		//if((0xFFFF==uid2) || (uid2!=uid)) 
		if(0xFFFF==uid2) 
		{
			//menu.show_title(0, 32, "Passwd or Number ERROR!");
			//menu.show_title(0, 0, "密码或编号错误!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "密码或编号错误!"); 
			return;
		}
		//if(uid) 
		{
			MENU::check_uid = uid+user_number_min;
			//menu_option();
			//menu_common(Menu, Menu_size, 4);
			menu_admin_option();
		}
	}
#else
	menu_admin_option();
#endif
}
static void func_passwd(void)
{
	menu.func_passwd();
}
void MENU_LEVEL::func_passwd(void)
{
	uint32_t passwd=0;
	uint32_t passwd_tmp=0;
	uint16_t count=0;
//	sys_data_t _sys;
//	data.get_sys(&_sys);
	waitting(MENU::back);
	while(waitting(0)>0)
	{
		_voice.play_info(DATA::get_volume(), 1, "请输入密码");
		passwd = menu.get_user_input(0, 6, 1, "请输入新密码");
		if(0==waitting(0)) break;
		_voice.play_info(DATA::get_volume(), 1, "请再次输入密码");
		passwd_tmp = menu.get_user_input(0, 6, 1, "再次输入密码");
		if(0==waitting(0)) break;
		if(passwd==passwd_tmp)
		{
			if(-1!=_data.update_passwd(MENU::check_uid , passwd))
			{
				// OK
				//menu.show_title(0, 32, "密码已更改!");
				_display.vformat(false, 0, 0, "操作完成", 2000, "密码已更改!"); 
				//printf("%d", uid);
				break;
			}
			else 
			{
				//menu.show_title(0, 32, "更改密码失败!");
				_display.vformat(false, 0, 0, "操作失败", 2000, "更改密码失败!"); 
				//set_timeout(10000); // 10s
			}
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
	//waitting_time(back);
}
static void func_rfid(void)
{
	menu.show_title(0, 32, "改RFID");
}
static void func_finger(void)
{
	menu.func_finger();
}
void MENU_LEVEL::func_finger(void)
{
	//menu.show_title(0, 32, "改指纹");
	data_user_t _user;
	int8_t ret;
	uint16_t tmp_num = 0;//
	_data.get_data(&_user, check_uid-user_number_min);
	if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
	{
		//_display.format(false, 0, 0, 2, 2000, "操作失败");
		_display.vformat(false, 0, 0, "操作失败", 2000, "操作失败!"); 
		return ;
	}
	if(_user.finger>0)
	{
		_display.format(false, 0, 0, 2, 2000, "校验指纹");
		//ret=fingerprint.Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
		//ret=fingerprint.Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
		ret=_finger.verify(tmp_num, 5000);
		if(0!=ret)
		{
			//_display.format(false, 0, 0, 2, 2000, "校验失败 %d", ret);
			_display.vformat(false, 0, 0, "操作失败", 2000, "校验失败 %d", ret);
			return ;
		}
		if(_user.finger != tmp_num)
		{
			//_display.format(false, 0, 0, 2, 2000, "指纹不匹配");
			_display.vformat(false, 0, 0, "操作失败", 2000, "指纹不匹配");
			return ;
		}
#if 0
		ret=fingerprint.Delete_Fingerprint(NULL,tmp_num,tmp_num);
		if(ret!=0) ret=fingerprint.Delete_Fingerprint(NULL,tmp_num,tmp_num);
#endif
		ret=_finger.remove(tmp_num,tmp_num);
		if(ret==0)
		{
			//_display.format(false, 0, 0, 2, 2000, "删除成功");
			_display.vformat(false, 0, 0, "操作完成", 2000, "删除成功");
			_data.update_finger(_user.uid, 0); // del
		}
		else
		{
			//_display.format(false, 0, 0, 2, 2000, "删除失败");
			_display.vformat(false, 0, 0, "操作失败", 2000, "删除失败");
		}
	}
	else
	{
		_display.format(false, 0, 0, 2, 2000, "校验指纹");
		//ret=fingerprint.Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
		ret=_finger.verify(tmp_num, 5000);
		if(0==ret)
		{
			//_display.format(false, 0, 0, 2, 2000, "指纹存在已关联 %d", tmp_num);
			_display.vformat(false, 0, 0, "操作完成", 2000, "指纹存在已关联 %d", tmp_num);
			_data.update_finger(_user.uid, tmp_num);
			return ;
		}
	}
	_display.format(false, 0, 0, 2, 200, "请按下手指 %d",tmp_num);
	{
		//ret=fingerprint.Fingerprint_Entry(menutask,(uint16_t *)&tmp_num);
		ret=_finger.add(tmp_num, 5000);
		if(ret==0)
		{
			//_display.format(false, 0, 0, 2, 2000, "写入成功 %d",tmp_num);
			_display.vformat(false, 0, 0, "操作完成", 2000, "写入成功 %d",tmp_num);
			_data.update_finger(_user.uid, tmp_num);
		}
		else
		{
			//_display.format(false, 0, 0, 2, 2000, "写入失败 %d %d",(int8_t)0-ret, tmp_num);
			_display.vformat(false, 0, 0, "操作失败", 2000, "写入失败 %d %d",(int8_t)0-ret, tmp_num);
		}
	}
}
static void func_finger_del(void)
{
	menu.func_finger_del();
}
void MENU_LEVEL::func_finger_del(void)
{
	//menu.show_title(0, 32, "改指纹");
	data_user_t _user;
	int8_t ret;
	uint16_t tmp_num = 0;//
	_data.get_data(&_user, MENU::check_uid-user_number_min);
	if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
	{
		//_display.format(false, 0, 0, 2, 2000, "操作失败");
		_display.vformat(false, 0, 0, "操作失败", 2000, "操作失败");
		return ;
	}
	if(_user.finger>0)
	{
		_display.format(false, 0, 0, 2, 2000, "校验指纹");
		//ret=fingerprint.Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
		ret=_finger.verify(tmp_num, 5000);
		if(0!=ret)
		{
			//_display.format(false, 0, 0, 2, 2000, "校验失败 %d", ret);
			_display.vformat(false, 0, 0, "操作失败", 2000, "校验失败 %d", ret);
			return ;
		}
		if(_user.finger != tmp_num)
		{
			//_display.format(false, 0, 0, 2, 2000, "指纹不匹配");
			_display.vformat(false, 0, 0, "操作失败", 2000, "指纹不匹配");
			return ;
		}
//		display.format(false, 0, 0, 2, 0, "Delete");
//		delay_ms(500);
#if 0
		ret=_finger.Delete_Fingerprint(NULL,1,100);
		if(ret!=0) ret=_finger.Delete_Fingerprint(NULL,1,100);
#else
		ret=_finger.remove(tmp_num,tmp_num);
#endif
		if(ret==0)
		{
			//_display.format(false, 0, 0, 2, 2000, "删除成功");
			_display.vformat(false, 0, 0, "操作完成", 2000, "删除成功");
			_data.update_finger(_user.uid, 0); // del
		}
		else
		{
			//_display.format(false, 0, 0, 2, 2000, "删除失败");
			_display.vformat(false, 0, 0, "操作失败", 2000, "删除失败");
			//NVIC_SystemReset();
		}
	}
	else
	{
		//_display.format(false, 0, 0, 2, 2000, "该用户无指纹信息");
		_display.vformat(false, 0, 0, "操作失败", 2000, "该用户无指纹信息");
	}
}
static void func_finger_verify(void)
{
	menu.func_finger_verify();
}
void MENU_LEVEL::func_finger_verify(void)
{
	int8_t ret;
	uint16_t tmp_num = 0;
	_display.format(false, 0, 0, 2, 2000, "开始校验");
	
	//ret=fingerprint.Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
	ret=_finger.verify(tmp_num, 5000);
	if(ret==0)
	{
		//_display.format(false, 0, 0, 2, 2000, "校验成功 %d", tmp_num);
		_display.vformat(false, 0, 0, "操作完成", 2000, "校验成功 %d", tmp_num);
	}
	else
	{
		//_display.format(false, 0, 0, 2, 2000, "校验失败 %d", ret);
		_display.vformat(false, 0, 0, "操作失败", 2000, "校验失败 %d", ret);
	}
}
void func_about(void)
{
	menu.func_about();
}
void MENU_LEVEL::func_about(void)
{
	//uint32_t i=0;
	//uint8_t key_temp;
	uint8_t key;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	_display.clear(0);
	//for(i=0; i<4; i++) _drawCharCN(16*i, 16, i, WHITE, BLACK, 1, CN_16x16[0]);
	//for(i=4; i<7; i++) _drawCharCN(16, 16, i-4, WHITE, BLACK, 1, CN_16x16);
	_display.setCursor(0,16);
	_display.printf("版本： V1.0");
	_display.setCursor(0,32);
	_display.printf("支持： merafour@163.com");
	_display.update();
	waitting(_sys.back);
	while(waitting(0)>0)
	{
		key=_input.Infr_Scan();
		//if(key_temp)
		{
			//key = Infr_deco(key_temp);
			if('*'==key) break;
		}
	}
	if(waitting(0)>0)
	{
		waitting(_sys.back);
	}
}
#if 1
static const menu_t _menu[] = 
{
	{func_admin,  "管理员"},
	{func_passwd, "改密码"},
	{func_rfid,   "改RFID"},
	{func_finger, "录入指纹"},
	{func_finger_del, "删除指纹"},
	{func_finger_verify, "校验指纹"},
	{func_about,  "About"}
};
static const uint8_t _menu_size = sizeof(_menu)/sizeof(_menu[0]);
#endif
void menu_level_option(void)
{
#if 0
	uint16_t uid=0;
	uint16_t uid2=0;
	uint32_t passwd=0;
	{
		//uid=user_check_admin();
		voice.play_info(DATA::get_volume(), 1, "请输入用户编号"); // 请输入用户编号:2
		uid = menu.get_user_input(0, 4, 0, "请输入用户编号");
		if(0==waitting_time(0)) return;
		voice.play_info(DATA::get_volume(), 1, "请输入密码"); // 请输入密码:3
		passwd = menu.get_user_input(0, 8, 1, "请输入密码");
		if(0==waitting_time(0)) return;
		uid2=_data.check_admin(uid+user_number_min, passwd);
		//if((0xFFFF==uid2) || (uid2!=uid)) 
		if(0xFFFF==uid2) 
		{
			//menu.show_title(0, 0, "密码或编号错误!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "密码或编号错误");
			return;
		}
		{
			MENU::check_uid = uid+user_number_min;
			menu.common(_menu, _menu_size, 4);
		}
	}
#else
	MENU::check_uid = 2+user_number_min;
	menu.common(_menu, _menu_size, 4);
#endif
}


//menu_t MENU_LEVEL::_menu[]=
//{
//	{func_admin,  "管理员"},
//	{func_passwd, "改密码"},
//	{func_rfid,   "改RFID"},
//	{func_finger, "改指纹"},
//	{func_finger_del, "删除指纹"},
//	{func_finger_verify, "校验指纹"},
//	{func_about,  "About"}
//};
//uint8_t MENU_LEVEL::_menu_size=sizeof(_menu)/sizeof(_menu[0]);

void MENU_LEVEL::option(void)
{
#if 1
	uint16_t uid=0;
	uint16_t uid2=0;
	uint32_t passwd=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	back = _sys.back;
	{
		//uid=user_check_admin();
		_voice.play_info(DATA::get_volume(), 1, "请输入用户编号"); // 请输入用户编号:2
		uid = menu.get_user_input(0, 4, 0, "请输入用户编号");
		if(0==waitting(0)) return;
		_voice.play_info(DATA::get_volume(), 1, "请输入密码"); // 请输入密码:3
		passwd = menu.get_user_input(0, 8, 0, "请输入密码");
		if(0==waitting(0)) return;
		uid2=_data.check_admin(uid+user_number_min, passwd);
		//if((0xFFFF==uid2) || (uid2!=uid)) 
		if(error_delay>0)
		{
			_display.vformat(false, 0, 0, "操作拒绝", 2000, "请%d秒后再试", error_delay);
			return;
		}
		if(0xFFFF==uid2) 
		{
			//menu.show_title(0, 0, "密码或编号错误!");
			_display.vformat(false, 0, 0, "操作失败", 2000, "密码或编号错误");
			error_count++;  // 错误计数
			if(error_count>=18) error_delay = 240;
			if(error_count>=9)  error_delay = 120;
			if(error_count>=6)  error_delay = 90;
			if(error_count>=3)  error_delay = 60;
			return;
		}
		{
			check_uid = uid+user_number_min;
			menu.common(_menu, _menu_size, 4);
			error_count=0;
			door_count=0;
			check_uid = 0;
		}
	}
#else
	sys_data_t _sys;
	_data.get_sys(&_sys);
	back = _sys.back;
	check_uid = 0+user_number_min;
	common(_menu, _menu_size, 4);
#endif
}

