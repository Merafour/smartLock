/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.h
* Author             : Merafour
* Last Modified Date : 03/09/2018
* Description        : This file provides the LTM022A69B LCD library.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "menu_level.h"
//#include "Adafruit_ssd1306syp.h"
//#include "Infr.h"
//#include "delay.h"
//#include "user.h"
#include <stdio.h>
//#include "data_flash.h"
//#include "ssd1306syp.h"
//#include "fingerprint.h"

static void func_see(void)
{
	menu.func_see();
}
void MENU_LEVEL::func_see(void)
{
	char _speed[][3]= {"高", "中", "低", "低"};
	uint8_t key=1;
	sys_data_t _sys;
	uint8_t user_index=0;
	uint8_t i=0;
	_data.get_sys(&_sys);
	while(waitting(0)>0)
	{
		//key=_input.Infr_Scan();
		if(key)
		{
			waitting(_sys.back); 
			if('2'==key || '^'==key)
			{
				if(0==user_index) user_index=2;
				user_index--;
			}
			if('8'==key || '!'==key)
			{
				user_index++;
				if(2==user_index) user_index=0;
			}
			if('4'==key || '<'==key)
			{
				break;
			}
			if('6'==key || '>'==key)
			{
			}
			_display.clear(false);
			i=0;
			switch(user_index)
			{
				case 0:
					_display.format(0, i*16, 2, 0, "背光:%d s", _sys.back/1000);
					i++;
					if(i>=4) break;
				case 1:
					_display.format(0, i*16, 2, 0, "声音:%d", _sys.volume); 
					i++;
					if(i>=4) break;
				case 2:
					_display.format(0, i*16, 2, 0, "多重校验:%s", (_sys.verify)?"开":"关"); 
					i++;
					if(i>=4) break;
				case 3:
					_display.format(0, i*16, 2, 0, "电机转速:%s档", _speed[_sys.speed&0x3]);
					i++;
					if(i>=4) break;
				case 4:
					_display.format(0, i*16, 2, 0, "关门延时:%d s", _sys.door);
					i++;
					if(i>=4) break;
				default:
					break;
			}
		}
		key=_input.Infr_Scan();
//		_display.format(0, 0, 2, 0, "背光: %d s", _sys.back/1000); 
//		_display.format(0, 16, 2, 0, "声音: %d", _sys.volume); 
//		_display.format(0, 32, 2, 0, "多重校验: %s", (_sys.verify)?"开":"关"); 
//		_display.format(0, 48, 2, 0, "电机转速: %s档", _speed[_sys.speed&0x3]); 
	}
}

static void func_door(void)
{
	menu.func_door();
}
void MENU_LEVEL::func_door(void)
{
	uint16_t delay = 0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	{
		delay = menu.get_user_input(0, 3, 0, "关门延时:\n[5-200]s");
		if(delay>200 || delay<5) 
		{
			_display.vformat(false, 0, 16, "操作拒绝", 2000, "超范围:\n[5-200]s");
		}
		else
		{
			_sys.door = delay;
			_data.save_sys(&_sys);
		}
	}
}

static void func_speed(void)
{
	menu.func_speed();
}
void MENU_LEVEL::func_speed(void)
{
	uint8_t key=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	_display.format(false, 0, 16, 2, 0, "1. 高档");
	_display.format(0, 32, 2, 0, "2. 中档");
	_display.format(0, 48, 2, 0, "3. 低档");
	while(waitting(0)>0)
	{
		key=_input.Infr_Scan();
		if(key)
		{
			waitting(_sys.verify);
			if('1'==key) // back
			{
				_sys.speed=0;
				_data.save_sys(&_sys);
				break;
			}
			else if('2'==key) // OK
			{
				_sys.speed=1;
				_data.save_sys(&_sys);
				break;
			}
			else if('3'==key) // OK
			{
				_sys.speed=2;
				_data.save_sys(&_sys);
				break;
			}
			else 
			{
				break;
			}
		}
	}
}

static void func_backlight(void)
{
	menu.level3_backlight();
}
void MENU_LEVEL::level3_backlight(void)
{
	uint16_t delay = 0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	{
		delay = menu.get_user_input(0, 2, 0, "背光时间:\n[5-30]s");
		if(delay>30 || delay<5) _display.vformat(false, 0, 16, "操作拒绝", 2000, "超范围:\n[5-30]s");
		else
		{
			_sys.back = delay*1000;
			_data.save_sys(&_sys);
		}
	}
}
static void func_delay(void)
{
	menu.level3_delay();
}
void MENU_LEVEL::level3_delay(void)
{
	uint16_t delay = 0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	{
		delay = menu.get_user_input(0, 2, 0, "操作时间:\n[5-30]s");
		if(delay>30 || delay<5) menu.show_title(0, 32, "超范围:\n[5-30]s");
		else
		{
			_sys.back = delay*1000;
			_data.save_sys(&_sys);
		}
	}
}
static void func_volume(void)
{
	menu.level3_volume();
}
void MENU_LEVEL::level3_volume(void)
{
	uint16_t volume = 0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	{
		volume = menu.get_user_input(0, 2, 0, "声音 [0-100]");
		if(volume>100) menu.show_title(0, 32, "超范围:\n[0-100]s");
		else
		{
			_sys.volume = volume;
			_data.save_sys(&_sys);
		}
	}
}
static void func_verify(void)
{
	menu.level3_verify();
}
void MENU_LEVEL::level3_verify(void)
{
	uint8_t key=0;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	_display.format(false, 0, 16, 2, 0, "%c0: 多重校验开", (0==_sys.verify)?'*':' ');
	_display.format(0, 32, 2, 0, "%c1: 多重校验关", (1==_sys.verify)?'*':' ');
	while(waitting(0)>0)
	{
//		display.clear(0);
//		display.setCursor(0,16);
//		display.printf("%c0: 多重校验开", (0==_sys.verify)?'*':' ');
//		display.setCursor(0,32);
//		display.printf("%c1: 多重校验关", (1==_sys.verify)?'*':' ');
//		display.update();
//		_display.format(false, 0, 16, 2, 0, "%c0: 多重校验开", (0==_sys.verify)?'*':' ');
//		_display.format(0, 32, 2, 0, "%c1: 多重校验关", (1==_sys.verify)?'*':' ');
		key=_input.Infr_Scan();
		if(key)
		{
			waitting(_sys.verify);
			if('0'==key) // back
			{
				_sys.verify=0;
				_data.save_sys(&_sys);
				break;
			}
			else if('1'==key) // OK
			{
				_sys.verify=1;
				_data.save_sys(&_sys);
				break;
			}
			else 
			{
				break;
			}
		}
	}
}
static void func_default(void)
{
	menu.level3_default();
}
void MENU_LEVEL::level3_default(void)
{
	uint32_t passwd=0;
	passwd = menu.get_user_input(0, 6, 1, "请输入密码");
	if(-1!=_data.update_passwd(MENU::check_uid , passwd))
	{
		// OK
		_data.factory();
		menu.show_title(0, 32, "已恢复出厂设置!");
		waitting(1);
		//delay_ms(100);
	}
	else 
	{
		//menu.show_title(0, 32, "操作拒绝!");
		_display.vformat(false, 0, 0, "操作失败", 2000, "操作拒绝!"); 
		//set_timeout(10000); // 10s
	}
}
static void func_passwd(void)
{
	menu.level3_passwd();
}
void MENU_LEVEL::level3_passwd(void)
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
		browse(&_user);
		//if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			return ;
		}
//		if((MENU::check_uid>(1+user_number_min)) && (_user.uid<=(1+user_number_min)))
//		{
//			menu.show_title(0, 32, "操作拒绝!");
//		}
		// check
		//if((_user.uid>(1+user_number_min)) && (_user.uid<(lock_admin_max+user_number_min)) && (_user.uid!=check_uid))
		if((check_uid>(1+user_number_min)) && (check_uid<(lock_admin_max+user_number_min)) && (_user.uid!=check_uid))
		{
			show_title(0, 32, "操作拒绝!");
			//_display.format(false, 0, 0, 2, 2000, "操作拒绝"); 
			count++;
			if(count>=3) break;
			continue;
		}
		else if(check_uid>=(lock_admin_max+user_number_min)) break;
		_voice.play_info(DATA::get_volume(), 1, "请输入密码");
		passwd = get_user_input(0, 6, 0, "请输入新密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		_voice.play_info(DATA::get_volume(), 1, "请再次输入密码");
		passwd_tmp = get_user_input(0, 6, 0, "再次输入密码", _user.uid-user_number_min);
		if(0==waitting(0)) break;
		if(passwd==passwd_tmp)
		{
			if(-1!=_data.update_passwd(_user.uid , passwd))
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
			//if(count<3) waitting(back);
			//else break;
			if(count>=3) break;
		}
	}
}
static void func_rfid(void)
{
	menu.show_title(0, 32, "rfid");
}
static void func_finger(void)
{
	menu.level3_finger();
}
void MENU_LEVEL::level3_finger(void)
{
	//menu.show_title(0, 32, "finger");
	data_user_t _user;
	int8_t ret;
	uint16_t count=0;
	uint16_t tmp_num = 0;//
	waitting(MENU::back);
	while(waitting(0)>0)
	{
		menu.browse(&_user);
		//if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			return ;
		}
		// check
		//if((_user.uid>(1+user_number_min)) && (_user.uid<(lock_admin_max+user_number_min)) && (_user.uid!=MENU::check_uid))
		if((check_uid>(1+user_number_min)) && (check_uid<(lock_admin_max+user_number_min)) && (_user.uid!=check_uid))
		{
			//menu.show_title(0, 32, "操作拒绝!");
			_display.vformat(false, 0, 0, "操作拒绝", 2000, "操作拒绝!"); 
			count++;
			if(count>=3) break;
			continue;
		}
		else if(check_uid>=(lock_admin_max+user_number_min)) break;
		if(_user.finger>0)
		{
			tmp_num = _user.finger;
#if 0
			ret=_finger.Delete_Fingerprint(NULL,tmp_num,tmp_num);
			if(ret!=0) ret=_finger.Delete_Fingerprint(NULL,tmp_num,tmp_num);
#endif
			ret=_finger.remove(tmp_num,tmp_num);
			if(ret==0)
			{
				//_display.format(false, 0, 0, 2, 2000, "删除成功");
				_display.vformat(false, 0, 0, "操作完成", 2000, "删除成功!"); 
				_data.update_finger(_user.uid, 0); // del
			}
			else
			{
				//_display.format(false, 0, 0, 2, 2000, "删除失败");
				_display.vformat(false, 0, 0, "操作失败", 2000, "删除失败!"); 
			}
		}
		//else
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
				break;
			}
			else
			{
				//_display.format(false, 0, 0, 2, 2000, "写入失败 %d %d",(int8_t)0-ret, tmp_num);
				_display.vformat(false, 0, 0, "操作失败", 2000, "写入失败 %d %d",(int8_t)0-ret, tmp_num); 
			}
		}
//		count++;
//		if(count>=3) break;
	}
}
static void func_finger_del(void)
{
	menu.level3_finger_del();
}
void MENU_LEVEL::level3_finger_del(void)
{
	//menu.show_title(0, 32, "改指纹");
	data_user_t _user;
	int8_t ret;
	uint16_t count=0;
	uint16_t tmp_num = 0;//
	waitting(MENU::back);
	while(waitting(0)>0)
	{
		menu.browse(&_user);
		//if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			return ;
		}
		// check
		//if((_user.uid>(1+user_number_min)) && (_user.uid<(lock_admin_max+user_number_min)) && (_user.uid!=MENU::check_uid))
		if((check_uid>(1+user_number_min)) && (check_uid<(lock_admin_max+user_number_min)) && (_user.uid!=check_uid))
		{
			//menu.show_title(0, 32, "操作拒绝!");
			_display.vformat(false, 0, 0, "操作拒绝", 2000, "操作拒绝!"); 
			count++;
			if(count>=3) break;
			continue;
		}
		else if(check_uid>=(lock_admin_max+user_number_min)) break;
		if(_user.finger>0)
		{
			tmp_num = _user.finger;
#if 0
			ret=_finger.Delete_Fingerprint(NULL,tmp_num,tmp_num);
			if(ret!=0) ret=_finger.Delete_Fingerprint(NULL,tmp_num,tmp_num);
#endif
			ret=_finger.remove(tmp_num,tmp_num);
			if(ret==0)
			{
				//_display.format(false, 0, 0, 2, 2000, "删除成功");
				_display.vformat(false, 0, 0, "操作完成", 2000, "删除成功!"); 
				_data.update_finger(_user.uid, 0); // del
				break;
			}
			else
			{
				//_display.format(false, 0, 0, 2, 2000, "删除失败");
				_display.vformat(false, 0, 0, "操作失败", 2000, "删除失败!"); 
				//NVIC_SystemReset();
			}
		}
		else
		{
			//_display.format(false, 0, 0, 2, 2000, "该用户无指纹信息");
			_display.vformat(false, 0, 0, "操作失败", 2000, "该用户无指纹信息!"); 
			count++;
			if(count>=3) break;
		}
	}
}

//static void func_dhcp(void)
//{
//	uint32_t passwd_tmp=0;
//	passwd_tmp=menu.get_user_input(0, 6, 1, "DHCP PASSWD");
//	if(passwd_tmp)
//	{
//		//update_admin_dhcp(passwd_tmp);
//		menu.show_title(0, 32, "DHCP Update!");
//	}
//	else
//	{
//		menu.show_title(0, 32, "DHCP Update Fail!");
//	}
//}

menu_t info_menu[] = 
{
	{func_passwd, "改密码"},
	{func_rfid,   "改RFID"},
	{func_finger, "改指纹"},
	{func_finger_del, "删除指纹"},
	//{func_dhcp,   "改DHCP"},
};
const unsigned int info_menu_size = sizeof(info_menu)/sizeof(info_menu[0]);
menu_t sys_menu[] = 
{
	{func_see,       "查看"},
	{func_backlight, "操作延时"},
	{func_delay,     "背光延时"},
	{func_volume,    "声音"},
	{func_verify,    "多重校验"},
	{func_speed,     "电机转速"},
	{func_door,      "关门延时"},
	{func_default,   "恢复出厂设置"}
};
const unsigned int sys_menu_size = sizeof(sys_menu)/sizeof(sys_menu[0]);

void func_info(void)
{
	menu.common(info_menu, info_menu_size, 4);
}
void func_set(void)
{
	menu.common(sys_menu, sys_menu_size, 4);
}
