/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : data.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the data library.

******************************************************************************/
#include "data.h"
//#include "delay.h"
#include <string.h>
#include "ssd1306syp.h"

uint16_t DATA::lock_size=0;
uint16_t DATA::_lenght=0;
data_head_t DATA::head={0,0,0,{15000, 50, 0}};


void DATA::Init(void)
{
#if 0
	//data_user_t _user;
	//data_head_t _head;
	memset(&head, 0, sizeof(data_head_t));
	lock_size=(lenght()-sizeof(data_head_t))/sizeof(data_user_t);
#else
	uint16_t len  = lenght();
	uint16_t len2 = sizeof(data_head_t);
	uint16_t len3 = sizeof(data_user_t);
	memset(&head, 0, sizeof(data_head_t));
	lock_size=(len-len2)/len3;
#endif
	read(0, (uint16_t *)(&head), sizeof(data_head_t)>>1);
	if(HEAD_FORMAT!=head.format)
	{
		// default
		reset(head);
	}
	
	//_user.uid=12345;
//	write(head.len, (uint16_t *)(&_user), sizeof(data_user_t)>>1);
//	_user.uid=0;
//	get_data(&_user, 0);
//	_user.uid=0;
//	read(head.len, (uint16_t *)(&_user), sizeof(data_user_t)>>1);
//	get_data(&_user, 1);
//	get_data(&_user, 2);
//	get_data(&_user, 0 + lock_admin_max);
//	update_data(0 + user_number_min, 52345678, 0, 0, 0, 0);
//	update_data(1 + user_number_min, 5456, 0, 0, 0, 0);      // static passwd
//	update_data(0 + lock_dhcp6_max+user_number_min, 545678, 1, 1, 0, 0);    // admin 
	//reset(head);
#if 0
	display.clear(0);
	display.setCursor(0,8);
	data.get_data(&_user, 0);
	display.printf("UID:%5d %d", _user.uid, _user.devnumber);
	display.setCursor(0,16);
	data.get_data(&_user, 1);
	display.printf("UID:%5d %d", _user.uid, _user.devnumber);
	display.setCursor(0,24);
	data.get_data(&_user, lock_dhcp6_max);
	display.printf("UID:%5d %d", _user.uid, _user.devnumber);
	//printf("Menu:%d %c %d", menu_index, key, get_timeout()/1000);
	//printf("total: %d [%d %d] %d %d", max_row, len, get_user_total(), choice, user_start_row);
	//display.printf("total[%d]: %d [%d]", data.get_size(), len, (user_start_row+choice)%len+1);
	//show_menu(1, 1, Menu[0].font, Menu[0].len);
	display.update();
	while(1);
#endif
}
void DATA_RAM::Init(void)
{
	memset(data, 0, sizeof(data));
	DATA::Init();
}
uint8_t DATA_RAM::erase_all(void)
{
	memset(data, 0, sizeof(data));
	return 0;
}
uint16_t DATA::Test(void)
{
	uint16_t test = 1010;
//	write(100, &test, 1);
//	test = 0;
//	read(100, &test, 1);
	read(100, &test, 1);
	test += 10;
	write(100, &test, 1);
	read(100, &test, 1);
	display.clear(0);
	display.setCursor(0,8);
	display.printf("FLASH:%5d ", test);
	display.update();
#if 0
	while(1);
#else
	return test;
#endif
}
void DATA::reset(data_head_t &_head)
{
	memset(&_head, 0, sizeof(data_head_t));
	erase_all();
	_head.version = HEAD_VERSION;
	_head.len = sizeof(data_head_t)>>1;
	_head.format = HEAD_FORMAT;
	_head._sys.back = 15000; // 15s
	//_head._sys.delay = 15000; // 15s
	_head._sys.volume = 15;
	_head._sys.verify = 0;
	_head._sys.speed = 1;
	_head._sys.door = 10;
//	_head.admin[0] = 12345678;  // spuer admin
//	//_head.admin[1] = 0;       // static passwd
//	_head.admin[2] = 345678;    // admin
//	_head.admin[10] = 345678;   // dhcp4
//	_head.admin[20] = 345678;   // dhcp6
	write(0, (uint16_t *)(&_head), sizeof(data_head_t)>>1);
	update_data(0 + user_number_min, 12345678, 0, 0, 0, 0);  // super admin
#if 0 // debug
	update_data(1 + user_number_min, 34567, 0, 0, 0, 0);      // static passwd
	update_data(2 + user_number_min, 345678, 1, 1, 0, 0);    // admin 
	update_data(0 + lock_admin_max + user_number_min, 345678, 0, 0, 0, 0); // DHCP4
	update_data(0 + lock_dhcp4_max + user_number_min, 345678, 0, 0, 0, 0); // DHCP6
	update_devnumber(0 + lock_dhcp6_max + user_number_min, 360480, 0, 0, 0, 0); // DHCP8
#else
	update_data(1 + user_number_min, 0, 0, 0, 0, 0);      // static passwd
//	update_data(2 + user_number_min, 0, 1, 1, 0, 0);    // admin 
//	update_data(0 + lock_admin_max + user_number_min, 0, 0, 0, 0, 0); // DHCP4
//	update_data(0 + lock_dhcp4_max + user_number_min, 0, 0, 0, 0, 0); // DHCP6
//	update_devnumber(0 + lock_dhcp6_max + user_number_min, 0, 0, 0, 0, 0); // DHCP8
#endif
}

uint16_t DATA::get_data(data_user_t *_user, const uint16_t index)
{
	memset(_user, 0, sizeof(data_user_t));
	if(index < lock_size) user_read(index*(sizeof(data_user_t)>>1), (uint16_t *)(_user), sizeof(data_user_t)>>1);
	return 0;
}
uint16_t DATA::get_user_index(data_user_t *_u, const uint16_t index)
{
	data_user_t _user;
	uint16_t total=0;
	uint16_t i=0;
	for(i=0; i<lock_size; i++)
	{
		get_data(&_user, i);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			if(index == total)
			{
				memcpy(_u, &_user, sizeof(_user));
				_u->uid -= user_number_min;
				return 1;
			}
			total++;
		}
	}
	return 0;
}
uint16_t DATA::get_total(void)
{
	uint16_t total=0;
	uint16_t i=0;
	data_user_t _user;
	for(i=0; i<lock_size; i++)
	{
		get_data(&_user, i);
		if((user_number_min<=_user.uid) && (user_number_max>_user.uid))
		{
			total++;
		}
	}
	return total;
}
uint16_t DATA::get_user_empty(data_user_t *_u, const uint16_t lock_region)
{
	uint16_t i=0;
	uint16_t start=0, end=0;
	data_user_t _user;
	memset(_u, 0, sizeof(data_user_t));
	if(lock_admin_max==lock_region) // [2-9] admin
	{
		start = 2;
		end = lock_admin_max;
	}
	else  // user 
	{
		start = lock_admin_max;
		end = lock_size;
	}
	for(i=start; i<end; i++) // 
	{
		get_data(&_user, i);
		if((user_number_min>_user.uid) || (user_number_max<=_user.uid))
		{
			_u->uid = i+user_number_min;
			return 0;
		}
	}
	return 1;
}
uint16_t DATA::get_sys(sys_data_t *_sys)
{
	//read((sizeof(data_head_t)-sizeof(sys_data_t))>>1, (uint16_t *)(_sys), sizeof(sys_data_t)>>1);
	memcpy(_sys, &head._sys, sizeof(sys_data_t));
	return 0;
}
uint16_t DATA::save_sys(const sys_data_t *_sys)
{
	write((sizeof(data_head_t)-sizeof(sys_data_t))>>1, (uint16_t *)(_sys), sizeof(sys_data_t)>>1);
	memcpy(&head._sys, _sys, sizeof(sys_data_t));
	return 0;
}
uint16_t DATA::remove_data(const uint16_t index)
{
	data_user_t _user;
	memset(&_user, 0, sizeof(data_user_t));
	if(index < lock_size) 
	{
		//write(head.len+index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		user_write(index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		return 0;
	}
	return 1;
}
uint16_t DATA::user_del(const uint16_t uid)
{
	uint16_t index = uid - user_number_min;
	data_user_t _user;
	memset(&_user, 0, sizeof(data_user_t));
	if(index < lock_size) 
	{
		//write(head.len+index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		user_write(index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		return 0;
	}
	return 1;
}
uint16_t DATA::save_data(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger)
{
	uint16_t index = uid - user_number_min;
	if ((uid >= user_number_min) && (index < lock_size)) // save
	{
		data_user_t _user;
		_user.uid = uid;
		_user.devnumber = devnumber;
		_user.start_date = start_date;
		_user.days = days;
		_user.rfid = rfid;
		_user.finger = finger;
		user_write(index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		return 0; // OK
	}
	return 1; // FAIL
}
uint16_t DATA::update_data(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger)
{
	uint16_t i=0;
	data_user_t data;
  if (user_number_min > uid)//if(0==uid)
  {
    for ( i = 0; i < lock_size; i++) 
    {
		get_data(&data, i);
      if (devnumber == data.devnumber) 
      {
        uid = i + user_number_min;
        return save_data(uid, devnumber, start_date, days, rfid, finger);
      }
    }
    //if (user_number_min >= uid) // new 
    {
      for ( i = 0; i < lock_size; i++) 
      {
        get_data(&data, i);
        if ((devNumber_max <= data.devnumber) || (devNumber_min >= data.devnumber)) 
        {
          uid = i + user_number_min;
          return save_data(uid, devnumber, start_date, days, rfid, finger);
        }
      }
    }
  }
  else
  {
    return save_data(uid, devnumber, start_date, days, rfid, finger);
  }
  return 1; // FAIL
}
int16_t DATA::update_passwd(const uint16_t uid, const uint32_t passwd)
{
	uint16_t index=0;
	data_user_t _user;
	index = uid - user_number_min;
	if ((uid >= user_number_min) && (index < lock_size)) // save
	{
		get_data(&_user, index);
		_user.devnumber = passwd;
		user_write(index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		return 0; // OK
	}
	return 1; // FAIL
}
int16_t DATA::update_finger(const uint16_t uid, const uint32_t finger)
{
	uint16_t index=0;
	data_user_t _user;
	index = uid - user_number_min;
	if ((uid >= user_number_min) && (index < lock_size)) // save
	{
		get_data(&_user, index);
		_user.finger = finger;
		user_write(index*(sizeof(data_user_t)>>1), (uint16_t *)(&_user), sizeof(data_user_t)>>1);
		return 0; // OK
	}
	return 1; // FAIL
}

uint16_t DATA::update_devnumber(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger)
{
	uint16_t i=0;
	data_user_t data;
  if ((user_number_min + lock_dhcp6_max) >= uid)//if(0==uid)
  {
    // [lock_dhcp6_max -- max]
    for (i = lock_dhcp6_max; i < lock_size; i++) {
      get_data(&data, i);
      if (devnumber == data.devnumber) {
        uid = i + user_number_min + lock_dhcp6_max;
        return save_data(uid, devnumber, start_date, days, rfid, finger);
      }
    }
    // new, [lock_dhcp6_max -- max]
    for (i = lock_dhcp6_max; i < lock_size; i++) {
      get_data(&data, i);
      if ((devNumber_max <= data.devnumber) || (devNumber_min >= data.devnumber)) {
        uid = i + user_number_min;
        return save_data(uid, devnumber, start_date, days, rfid, finger);
      }
    }
  }
  else {
    return save_data(uid, devnumber, start_date, days, rfid, finger);
  }
  return 1;
}
uint16_t DATA::del_data(uint16_t uid, uint32_t devnumber)
{
	uint16_t i=0;
	data_user_t data;
  if ((user_number_min) >= uid) //if (0 == uid) 
  {
    for (i = 0; i < lock_size; i++) {
      get_data(&data, i);
      if (devnumber == data.devnumber) {
        return remove_data(i);
      }
    }
    return 1;
  }
  else {
    remove_data(uid - user_number_min);
  }
  return 0;
}

uint16_t DATA::search_uid(const uint16_t uid)
{
	uint16_t i=0;
	data_user_t data;
	for(i=0; i<lock_size; i++)
	{
		get_data(&data, i);
		if(uid==data.uid) return uid;
	}
	return 0xFFFF;
}

uint16_t DATA::check_admin(const uint16_t uid, const uint32_t passwd)
{
	uint16_t index = uid - user_number_min;
	data_user_t data;
	//if ((uid > 0) && (index < lock_size)) 
	if ((uid > 0) && (index < lock_size) && (passwd>999) && (passwd<100000000u)) // 密码低于四位无效,密码超过八位无效 
	{
		get_data(&data, index);
		if((uid==data.uid) && (passwd==data.devnumber))
		{
			// match OK
			return (uid );
		}
	}
	return 0xFFFF;
}

