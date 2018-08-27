/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : subfunc.cpp
* Author             : Merafour
* Last Modified Date : 04/19/2018
* Description        : This file provides the dhcp check library.

******************************************************************************/
#include "dhcp_check.h"
//#include "RTC.h"
//#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#define SUB_CYCLE   5
const uint8_t Subfunc::cycle = 5;
const uint32_t Subfunc::b1 = 24 * 3600 / cycle;

//const key_dhcp6_admin = '1113335559';
//const key_dhcp8_admin = '1113335558';
//const key_scener_admin = '1113335562';
//const key_scener_super_admin = '1113335552';
//const key_devNumber = '1113335557';
#define  order_value1   12
#define  order_value2   13
#define  order_value3   14
#define  order_value4   15
#define  order_value5   16
#define  order_value6   17
#define  delay_value1   11 // [11, 17, 23, 29];
#define  delay_value2   17
#define  delay_value3   23
#define  delay_value4   29
//const uint16_t delay1 = [11, 17, 23, 29];// 生成一个新的非空数组，数组元素可以是任何类型
//const uint16_t delay2 = [1 * 60, 2 * 60, 5 * 60, 10 * 60];
//var order = [12, 13, 14, 15];
//const uint16_t order = [order_value1, order_value2, order_value3, order_value4, order_value5, order_value6];
const uint16_t Subfunc::delay1[] = {11, 17, 23, 29};
const uint16_t Subfunc::delay2[] = {1 * 60, 2 * 60, 5 * 60, 10 * 60};
const uint16_t Subfunc::order[] = {order_value1, order_value2, order_value3, order_value4, order_value5, order_value6};
const uint16_t Subfunc::order_length = sizeof(order)/sizeof(order[0]);
const uint16_t Subfunc::order_sync=order_value6;

uint16_t Subfunc::get_delay(const uint8_t index) {
  if (index >= sizeof(delay1)/sizeof(delay1[0])) return 100;
  return delay1[index];
}
uint16_t Subfunc::get_order(const uint8_t index) {
  if (index >= sizeof(order)/sizeof(order[0])) return 100;
  return order[index];
}
#if 0
uint16_t Subfunc::get_time(check_date_t* _date) {
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	check_date_t date;
	uint16_t Y,M,D;
	uint32_t ss=0;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &Time);
	RTC_GetDate(RTC_Format_BIN, &Date);
	date.Y = Date.RTC_Year+2000;
	date.M = Date.RTC_Month;
	date.D = Date.RTC_Date;
	date. h = Time.RTC_Hours;
	date.m = Time.RTC_Minutes;
	date.s = Time.RTC_Seconds;
	Y = date.Y;
	M = date.M;
	D = date.D;
	ss = date.s + date.m * 60 + date.h * 3600;// + D * 24 * 3600;// + M * 31 * 24 * 3600;// + (Y % 10)*12*31*24*3600;
	date.base = (ss - ss % cycle) / cycle;
	date.base2 = Y * 12 + M * 31 + D * 24 * 3600;
	date.base3 = (Y % 100) * 10000 + (M % 100) * 100 + (D % 100);
	memcpy(_date, &date, sizeof(check_date_t));
	return 0;
#if 0	
  //var date = getDate(); //返回当前时间对象
  var timestamp = Date.parse(new Date());
  timestamp = timestamp / 1000;
  //获取当前时间  
  var n = timestamp * 1000;
  var date = new Date(n);
  var Y = date.getFullYear();
  var M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1);
  var D = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
  var h = date.getHours();
  var m = date.getMinutes();
  var s = date.getSeconds();

  var ss = s + m * 60 + h * 3600;// + D * 24 * 3600;// + M * 31 * 24 * 3600;// + (Y % 10)*12*31*24*3600;
  var base = (ss - ss % cycle) / cycle;
  //var base2 = Y + M * 70 + D * 24 * 3600;// + this.data.delay;
  var base2 = Y * 12 + M * 31 + D * 24 * 3600;
  var base3 = (Y % 100) * 10000 + (M % 100) * 100 + (D % 100);
  return { ss, base, base2, base3, Y, M, D, h, m, s };
#endif
}
#endif
uint16_t Subfunc::get_time(check_date_t* _date) 
{
	uint32_t ss=0;
	uint16_t year=0, month=0, day=0;
	uint16_t hour=0, minute=0, second=0;
	check_date_t date;
	/* Get the current Time */
	get_time(year, month, day, hour, minute, second);

	date.Y = year+2000;
	date.M = month;
	date.D = day;
	date. h = hour;
	date.m = minute;
	date.s = second;
	ss = date.s + date.m * 60 + date.h * 3600;// + D * 24 * 3600;// + M * 31 * 24 * 3600;// + (Y % 10)*12*31*24*3600;
	date.base = (ss - ss % cycle) / cycle;
	date.base2 = year * 12 + month * 31 + day * 24 * 3600;
	date.base3 = (year % 100) * 10000 + (month % 100) * 100 + (day % 100);
	memcpy(_date, &date, sizeof(check_date_t));
	return 0;
}
//function get_date(x) {
//  //var date = getDate(); //返回当前时间对象
//  var timestamp = Date.parse(new Date());
//  timestamp = timestamp / 1000;
//  //获取当前时间  
//  var n = timestamp * 1000;
//  var date = new Date(n);
//  var Y = date.getFullYear();
//  var M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1);
//  var D = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
//  return { Y, M, D };
//}
//function create_number(x) {
//  var data = get_time();
//  var number = ((data.base2 * 1096 * 64 + data.ss * 2048) % 89409 + data.base * 2048) % 89409;
//  var text = '' + (data.ss % 9) + number % 10;            // 1,2
//  number = (number - number % 10) / 10; // 2
//  text = text + number % 10;            // 2
//  number = (number - number % 10) / 10; // 3
//  text = text + number % 10;            // 3
//  number = (number - number % 10) / 10; // 4
//  text = text + number % 10;            // 4
//  number = (number - number % 10) / 10; // 5
//  text = text + number % 10;            // 5
//  //console.log('userInfo:', app.globalData.userInfo); 
//  return text;
//}
uint16_t Subfunc::admin_length(uint32_t admin) 
{
  uint8_t tmp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t i = 0;
  for (i = 0; i < 8; i++) {
    tmp[i] = admin % 10;
    admin = (admin - tmp[i]) / 10;
  }
  for (i = 8; i > 0; i--) {
    if (tmp[i - 1] > 0) break;
  }
  return i;
}
//function OrderParse(text) 
uint16_t Subfunc::OrderParse(const uint8_t text[32], check_order_t* _check)
{
	/**
	 var text = OrderFormat(passwd4, crc, '' + len + _order+ devNumber, indate);
	 // text : p4:crc:*:L:O:number:indate:#
	 // text : 4+2+1+1+1+8+3+1
	*/
	check_order_t Parse;
	uint8_t code[32];
	memset(code, 0, sizeof(code));
	memset(_check, 0, sizeof(check_order_t));
	if ('*' == text[6]) // '*'
	{
		//passwd4 = parseInt(text, 10); // p4:crc
		memset(code, 0, sizeof(code)); memcpy(code, text, 4);
		uint32_t passwd4 = atol((const char*)code); // parseInt(text.slice(0, 4), 10);         // p4
		memset(code, 0, sizeof(code)); memcpy(code, &text[4], 2);
		Parse.crc = atol((const char*)code); // parseInt(text.slice(4, 6), 10);             // crc
		Parse.len = text[7]-'0'; // parseInt(text.slice(7, 8), 10);             // L
		Parse._order = text[8]-'0'; // parseInt(text.slice(8, 9), 10);          // O
		memset(code, 0, sizeof(code)); memcpy(code, &text[9], Parse.len);
		Parse.devNumber = atol((const char*)code); // parseInt(text.slice(9, 9 + len), 10); // number
		memset(code, 0, sizeof(code)); memcpy(code, &text[9 + Parse.len], 5);
		Parse.indate = atol((const char*)code); // parseInt(text.slice(9 + len, 32), 10);   // indate
		//console.log("passwd4：" + passwd4);
		uint32_t tmp = passwd4 % 100;
		//console.log("tmp：" + tmp);
		passwd4 = (passwd4 - tmp) / 100;
		//console.log("passwd4：" + passwd4);
		passwd4 = passwd4 + tmp * 100;
		Parse.passwd4 = passwd4;
		memcpy(_check, &Parse, sizeof(check_order_t));
	}
	//return { passwd4, crc, len, _order, devNumber, indate };
	return 0;
}
#if 0
function OrderFormat(passwd4, crc, devNumber, indate) {
  // text : p4,p6
  var passwd3 = passwd4 % 100;
  var passwd6 = crc;
  passwd4 = (passwd4 - passwd3) / 100;
  var tmp = (passwd3 % 10);
  var text = "" + ((passwd3 - tmp) / 10);
  text = text + tmp;
  tmp = (passwd4 % 10);
  text = text + ((passwd4 - tmp) / 10);
  text = text + tmp;
  tmp = (passwd6 % 10);
  text = text + ((passwd6 - tmp) / 10);
  text = text + tmp;
  text = text + '*' + devNumber + indate + '#';
  console.log("Order text：" + text + " dev：" + devNumber + " indate：" + indate);
  // text : p4:crc:*:number:indate:#
  return text;
}
#endif
#if 0
function getNumber(x) {
  var dev_write = function (dev) {
    var tmp = 0;
    try {
      wx.setStorageSync(key_devNumber, dev);
      tmp = 1;
    } catch (e) {
      tmp = 0;
    }
    return tmp;
  }
  var dev_read = function (x) {
    var dev = '';
    try {
      dev = wx.getStorageSync(key_devNumber)
    }
    catch (e) {
      dev = '';
    }
    return dev;
  }
  //wx.removeStorageSync(key_devNumber);
  var num = create_number();
  var dev = dev_read();
  if ('' == dev) {
    dev_write(num);
    dev = dev_read();
    if ('' == dev) {
      // wx.showToast({
      //   title: '数据保存出错，请重试！',
      //   icon: 'none',
      //   duration: 2000
      // })
      return dev;
    }
  }
  console.log('dev:', dev)
  console.log('num:', num)
  return dev;
}
#endif
const uint8_t* Subfunc::_dhcp8_build(const uint16_t delay, const uint32_t devNumber, const uint32_t base, const uint32_t base2)
{
	//uint32_t index=0;
	uint32_t passwd3 = 0;
	uint32_t passwd6 = 0;
	uint32_t passwd4 = 0;
    uint32_t sum1 = (((base* 37)% 86400) * 1024 ) % 86400; 
    uint32_t sum2 = (((base2 * 67) % 86400) * 1024) % 86400; 
	uint32_t sum3 = 0;
	uint32_t sum4 = 0;
	
	//passwd4 = (base * 1024 * 37 + base2 * 1024 * 67) % 86400; // 24*3600/10 = 8640
	passwd4 = (sum1 + sum2) % 86400;
	passwd4 = (passwd4 - passwd4 % 9) / 9 + delay;
	passwd4 = passwd4 % 10000;
	//passwd6 = (passwd4 * 32 + base * 64 + base2 * 1024 + devNumber * 1024 * 64) % 66400;
	sum1 = ((passwd4* 32)% 66400); 
	sum2 = ((base* 64)% 66400); 
	sum3 = (((base2)% 66400) * 1024 ) % 66400; 
	sum4 = (((devNumber* 64)% 66400) * 1024 ) % 66400; 
	passwd6 = (sum1 + sum2 + sum3 + sum4) % 66400;
	passwd6 = (passwd6 - passwd6 % 7) / 7;

	// text : p4,p6
	memset(text, 0, sizeof(text));
	passwd3 = passwd4 % 100;
	passwd4 = (passwd4 - passwd3) / 100 + passwd3*100;
	passwd3 = passwd6 % 100;
	passwd6 = (passwd6 - passwd3) / 100 + passwd3*100;
	sprintf((char*)text, "%04d%04d#", passwd4, passwd6);
#if 0
	index=0;
	memset(text, 0, sizeof(text));
	passwd3 = passwd4 % 100;
	passwd4 = (passwd4 - passwd3) / 100;
	uint32_t tmp = (passwd3 % 10);
	text[index++] = '0' + ((passwd3 - tmp) / 10);
	text[index++] = '0' + tmp;
	tmp = (passwd4 % 10);
	text[index++] = '0' + ((passwd4 - tmp) / 10);
	text[index++] = '0' + tmp;
	passwd3 = passwd6 % 100;
	passwd6 = (passwd6 - passwd3) / 100;
	tmp = (passwd3 % 10);
	text[index++] = '0' + ((passwd3 - tmp) / 10);
	text[index++] = '0' + tmp;
	tmp = (passwd6 % 10);
	text[index++] = '0' + ((passwd6 - tmp) / 10);
	text[index++] = '0' + tmp;
	text[index++] = '#';
#endif
	return text;
}
//function dhcp8_build(delay, devNumber) 
const uint8_t* Subfunc::dhcp8_build(uint16_t delay, const uint32_t devNumber)
{
	check_date_t _date;
	get_time(&_date);
//	//  uint32_t ss = _date.ss;
//	uint32_t base = _date.base;
//	uint32_t base2 = _date.base2;
	delay = get_delay(delay);
#if 1
	return _dhcp8_build(delay, devNumber, _date.base, _date.base2);
#else
	// 39603437 ==> delay:23 devNumber:225674 base：10298 base2:1752340
	// sum1:74624 sum2:65920
	// sum1:60448 sum2:61472 sum3:2560 sum4:34464
	return _dhcp8_build(23, 225674, 10298, 1752340);
#endif
}
const uint8_t* Subfunc::_dhcp6_build(const uint16_t delay, const uint32_t admin, const uint32_t base, const uint32_t base2)
{
	//uint32_t index=0;
	uint32_t passwd3 = 0;
	uint32_t passwd6 = 0;
	uint32_t passwd4 = 0;
    uint32_t sum1 = (((base* 32)% 86400) * 1024 ) % 86400; 
    uint32_t sum2 = ((base2% 86400) * 1024) % 86400;
    uint32_t sum3 = (((admin* 64) % 86400) * 1024 ) % 86400; //(admin * 1024 * 64) % 86400;
    //var passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
    //console.log("sum1:" + sum1 + " sum2:" + sum2 + " sum3:" + sum3);
    passwd4 = (sum1 + sum2 + sum3) % 86400;
	passwd4 = (passwd4 - passwd4 % 9) / 9;
	passwd3 = 0;
	passwd6 = passwd4 % 100;
	passwd6 = passwd6 + (passwd4 - passwd6) / 100;
	passwd6 = passwd6 + delay + base % 100;
	passwd6 = passwd6 % 100;
	
#if 0
    var passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
    passwd4 = (passwd4 - passwd4 % 9) / 9;
    var passwd3 = 0;
    var passwd6 = passwd4 % 100;
    passwd6 = passwd6 + (passwd4 - passwd6) / 100;
    passwd6 = passwd6 + delay + base % 100;
    passwd6 = passwd6 % 100;
#endif
	// text : p3,p4,[p6]
	memset(text, 0, sizeof(text));
	passwd3 = passwd4 % 100;
	passwd4 = (passwd4 - passwd3) / 100 + passwd3*100;
	sprintf((char*)text, "%04d%02d#", passwd4, passwd6);
#if 0
	index=0;
	memset(text, 0, sizeof(text));
	passwd3 = passwd4 % 100;
	passwd4 = (passwd4 - passwd3) / 100;
	uint32_t tmp = (passwd3 % 10);
	text[index++] = '0' + ((passwd3 - tmp) / 10);
	text[index++] = '0' + tmp;
	tmp = (passwd4 % 10);
	text[index++] = '0' + ((passwd4 - tmp) / 10);
	text[index++] = '0' + tmp;
	tmp = (passwd6 % 10);
	text[index++] = '0' + ((passwd6 - tmp) / 10);
	text[index++] = '0' + tmp;
	text[index++] = '#';
#endif
	return text;
}
//function dhcp6_build(delay, admin) 
const uint8_t* Subfunc::dhcp6_build(uint16_t delay, const uint32_t admin)
{
	delay = get_delay(delay);
	check_date_t _date;
	get_time(&_date);
	//var data = get_time();
	//  uint32_t ss = _date.ss;
	uint32_t base = _date.base;
	uint32_t base2 = _date.base2;
#if 1
	return _dhcp6_build(delay, admin, base, base2);
#else
	// 599042 ==> delay:23 admin:123456 base：9170 base2:1752340
	// sum1:69760 sum2:40960 sum3:57216
	return _dhcp6_build(23, 123456, 9170, 1752340);
#endif
}

const uint8_t* Subfunc::_dhcp4_build(const uint16_t delay, const uint32_t admin, const uint32_t base, 
	const uint32_t base3, const uint32_t random, const uint32_t data_base)
{
	//uint32_t index=0;
	uint8_t u0=0, u1=0;
	//var passwd4 = (base * 1024 * 32 + base3 * 32 + random * 64) % 1000; // 24*3600/10 = 8640
	uint32_t passwd4 = (base * 129 + base3 * 37 + random * 127) % 1000;
	uint32_t passwd3 = passwd4 % 9 + admin % 9 + data_base % 9 + delay;
	passwd3 = passwd3 % 10;
	// text : p3,p4[0],p4[1],p4[2]
	memset(text, 0, sizeof(text));
	u0 = passwd4 % 10;
	passwd4 = (passwd4 - u0) / 10;
	u1 = passwd4 % 10;
	passwd4 = (passwd4 - u1) / 10;
	sprintf((char*)text, "%d%d%d%d#", passwd3, u0, u1, passwd4);
#if 0
	index=0;
	memset(text, 0, sizeof(text));
	text[index++] = '0' + passwd3;
	uint32_t tmp = passwd4 % 10;
	passwd4 = (passwd4 - tmp) / 10;
	text[index++] = '0' + tmp;
	tmp = (passwd4 % 10);
	passwd4 = (passwd4 - tmp) / 10;
	text[index++] = '0' + tmp;
	text[index++] = '0' + passwd4;
	text[index++] = '#';
#endif
	return text;
}
//function dhcp4_build(delay, admin) 
const uint8_t* Subfunc::dhcp4_build(uint16_t delay, const uint32_t admin)
{
	delay = get_delay(delay);
	check_date_t _date;
	get_time(&_date);
	//uint32_t data = get_time();
	uint32_t base = _date.base % 1000;
	uint32_t base3 = _date.base3 + admin % 1000000;
	//base3 = base3 + admin % 1000000;
	uint32_t random = (_date.base - base) / 1000;
	//console.log("base：" + base + " | " + data.base + " base3：" + base3 + " delay:" + delay + " admin:" + admin);
	//---console.log("base：" + base + " | " + data.base + " base3：" + base3 + " admin:" + admin + " random:" + random + " delay:" + delay);
#if 1
	return _dhcp4_build(delay, admin, base, base3, random, _date.base);
#else
	return _dhcp4_build(23, 123456, 628, 303876, 8, 8628);
#endif
}


//function _order_encode(base, base2, delay, admin, order, devNumber, indate)
const uint8_t* Subfunc::_order_encode(const uint32_t base, const uint32_t base2, uint16_t delay, const uint32_t admin, uint32_t order, const uint32_t devNumber, const uint32_t indate)
{
	//uint8_t buf[20];
	uint32_t passwd4 = 0;
	//uint32_t passwd3 = 0;
	//uint32_t passwd6 = 0;
	uint32_t crc = 0;
	uint32_t len = admin_length(devNumber);
	uint32_t _order = 0;
	uint32_t sum1 = 0;
	uint32_t sum2 = 0;
	uint32_t sum3 = 0;
	//uint32_t index=0;
	delay = get_delay(delay);
	order = get_order(order);

	//(base * 1024 * 32) % 86400;
	sum1 = ((((base % 86400) * 32) % 86400) * 1024) % 86400;
	sum2 = (base2 * 1024) % 86400;
	sum3 = ((((admin % 86400) * 64) % 86400) * 1024) % 86400;
	passwd4 = (sum1 + sum2 + sum3) % 86400;
	//uint32_t passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
	passwd4 = (passwd4 - passwd4 % 9) / 9 + delay + admin_length(admin) * 10;
	passwd4 = passwd4 % 10000;
	//len = (len + passwd4 % 8) % 10;
	_order = (order + (passwd4 * 7) % 8) % 10;

	sum1 = (passwd4 * 1024) % 86400;
	sum2 = ((((devNumber% 86400) * 32)% 86400) * 1024) % 86400;
	sum3 = (indate * 1024 * 64) % 99;
	crc = (sum1 + sum2) % 86400 + sum3;
	crc = crc % 99;
	crc = crc + order + (base * 10) + len;
	crc = crc % 99;
	//var text = OrderFormat(passwd4, crc, devNumber, indate);
	//const uint8_t* _text = OrderFormat(passwd4, crc, '' + len + _order + devNumber, indate);
	// text : p4:crc:*:L:O:number:indate:#
	sum1 = passwd4%100;
	passwd4 = (passwd4-sum1)/100 + sum1*100;
	memset(text, 0, sizeof(text));
	sprintf((char*)text, "%04d%02d*%d%d%06d%d#", passwd4, crc, len, _order, devNumber, indate);
#if 0
	memset(buf, 0, sizeof(buf)); passwd4 += 10000; itoa(passwd4, buf, 10);
	memcpy(&text[0], &buf[1], 4);
	memset(buf, 0, sizeof(buf)); crc += 100; itoa(crc, buf, 10);
	memcpy(&text[4], &buf[1], 2);
	text[6] = '*';
	text[7] = '0'+len;
	text[8] = '0'+_order;
	if(len<6)
	{
		devNumber += 1000000;
		memset(buf, 0, sizeof(buf)); itoa(devNumber, &buf[10], 10);
		memcpy(&buf[0], &buf[11], 6);
		memset(&buf[10], 0, 10);
	}
	else
	{
		memset(buf, 0, sizeof(buf)); itoa(devNumber, &buf[0], 10);
	}
	memcpy(&text[9], &buf[0], strlen(buf));
	index = 9+strlen(buf);
	memset(buf, 0, sizeof(buf)); itoa(indate, buf, 10);
	memcpy(&text[index], &buf[0], strlen(buf));
	index += strlen(buf);
	text[index] = '#';
#endif
	return text;
}
//function order_encode(delay, admin, order, devNumber, indate) 
const uint8_t* Subfunc::order_encode(uint16_t delay, const uint32_t admin, const uint32_t order, const uint32_t devNumber, const uint32_t indate)
{
	//var data = get_time();
	check_date_t _date;
	get_time(&_date);
//	uint32_t base = _date.base;
//	uint32_t base2 = _date.base2;
	//var text = _order_encode(base, base2, delay, admin, order, devNumber, indate)
	//return text;
#if 0
	return _order_encode(_date.base, _date.base2, delay, admin, order, devNumber, indate);
#else
	// base:10755 base2:1752340 delay:2 admin:345678 order:1 devNumber:225674 indate:1 admin:6
	// sum1:80640 sum2:40960 sum3:14208
	// sum1:3328 sum2:82432 sum3:97
	// Order text：725579*672256741# dev：67225674 indate：1
	//base:11674 base2:1752340 delay:2 admin:345678 order:1 devNumber:225674 indate:1 admin:6
	return _order_encode(11674, 1752340, 2, 345678, 1, 225674, 1);
#endif
}
//function order_sync_encode(delay, admin, order, devNumber, indate) 
const uint8_t* Subfunc::order_sync_encode(uint16_t delay, const uint32_t admin, const uint32_t order, uint32_t devNumber, uint32_t indate)
{
  //var data = get_time();
	check_date_t _date;
	get_time(&_date);
  uint32_t base = _date.base;
  uint32_t base2 = _date.base2;
  devNumber = _date.base3;//(data.Y % 100) * 10000 + (data.M % 100) * 100 + (data.D % 100);
  indate = (base - base % 216) / 216;
  //var text = _order_encode(base, base2, delay, admin, order, devNumber, indate)
  //return text;
	return _order_encode(base, base2, delay, admin, order, devNumber, indate);
}
#if 0
function order_build(delay, admin, order, devNumber, indate) {
  delay = get_delay(delay);
  order = get_order(order);
  //this.data.devNumber = create_number();
  var data = get_time();
  var ss = data.ss;
  var base = data.base;
  var base2 = data.base2;

  var passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
  passwd4 = (passwd4 - passwd4 % 9) / 9 + delay + admin_length(admin) * 10;
  passwd4 = passwd4 % 10000;

  //---console.log("当前时间(s)：" + ss);
  //---console.log("base：" + base + " base2：" + base2 + " order:" + order + " delay:" + delay + " admin:" + admin_length(admin));
  var crc = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400 + indate * 1024 * 64;
  crc = crc % 99 + order + (base * 10);
  crc = crc % 99;
  var text = OrderFormat(passwd4, crc, devNumber, indate);
  // text : p4:crc:*:number:indate:#
  return text;
}

function order_st_build(delay, admin, order, devNumber, indate) {
  delay = get_delay(delay);
  order = get_order(order);
  var date = get_date();
  devNumber = (date.Y % 100) * 10000 + (date.M % 100) * 100 + (date.D % 100);
  //var b1 = 24 * 3600 / cycle;  // 17280
  //indate = b1/216;  // 80
  var data = get_time();
  var ss = data.ss;
  var base = data.base;
  var base2 = (date.Y % 100) * 12 + (date.M % 100) * 31 + (date.D % 100) * 24 * 3600;//data.base2;
  indate = (base - base % 216) / 216;

  var passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
  passwd4 = (passwd4 - passwd4 % 9) / 9 + delay + admin_length(admin) * 10;
  passwd4 = passwd4 % 10000;

  //---console.log("当前时间(s)：" + ss);
  //---console.log("base：" + base + " base2：" + base2 + " order:" + order + " delay:" + delay + " admin:" + admin_length(admin));
  var crc = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400 + indate * 1024 * 64;
  crc = crc % 99 + order + (base * 10);
  crc = crc % 99;
  var text = '*' + OrderFormat(passwd4, crc, devNumber, indate);
  // text : p4:crc:indate:*:number:#
  return text;
}


function check_showToast(status) {
  status = status % 10;
  if (0 == status) {
    wx.showToast({
      title: '密码错误',
      icon: 'none',
      duration: 1000
    })
  }
  if (1 == status) {
    wx.showToast({
      title: '密码验证通过',
      icon: 'none',
      duration: 1000
    })
  }
  if (2 == status) {
    wx.showToast({
      title: '密码已失效',
      icon: 'none',
      duration: 1000
    })
  }
}

function Unlock_showToast(status) {
  status = status % 10;
  if (1 === status) {
    wx.showModal({
      title: '提示',
      content: '解锁成功！',
      showCancel: false,
    })
  }
  else {
    wx.showModal({
      title: '提示',
      content: '解锁失败！',
      showCancel: false,
    })
  }
}
function Regular(str, reg) {
  if (reg.test(str))
    return true;
  return false;
}

//是否为中文
function IsChinese(str) {
  var reg = /^[\u0391-\uFFE5]+$/;
  return Regular(str, reg);
}

module.exports = {
  getRequestUrl: "http://localhost:59637",//获得接口地址
  cycle: cycle,
  order_value6: order_value6,
  delay1: delay1,
  delay2: delay2,
  order: order,
  admin_length: admin_length,
  OrderParse: OrderParse,
  key_dhcp6_admin: key_dhcp6_admin,
  key_dhcp8_admin: key_dhcp8_admin,
  IsChinese: IsChinese,
  get_time: get_time,
  //create_number: create_number,
  getNumber: getNumber,
  dhcp8_build: dhcp8_build,
  dhcp6_build: dhcp6_build,
  dhcp4_build: dhcp4_build,
  order_encode: order_encode,
  order_sync_encode: order_sync_encode,
  check_showToast: check_showToast,
  Unlock_showToast: Unlock_showToast,
  // get_delay: get_delay,
  get_order: get_order,
  get_date: get_date,
}

// module.exports = {
//   getRequestUrl: "http://localhost:59637",//获得接口地址
//   cycle: cycle,
//   order_value6: order_value6,
//   delay1: delay1,
//   delay2: delay2,
//   order: order,
//   admin_length: admin_length,
//   OrderParse: OrderParse,
//   key_dhcp6_admin: key_dhcp6_admin,
//   key_dhcp8_admin: key_dhcp8_admin,
//   key_scener_admin: key_scener_admin,
//   key_scener_super_admin: key_scener_super_admin,
//   IsChinese: IsChinese,
//   get_time: get_time,
//   //create_number: create_number,
//   getNumber: getNumber,
//   dhcp8_build: dhcp8_build,
//   dhcp6_build: dhcp6_build,
//   dhcp4_build: dhcp4_build,
//   // dhcp8_check: dhcp8_check,
//   // dhcp6_check: dhcp6_check,
//   //dhcp4_check: dhcp4_check,
//   // order_build: order_build,
//   // order_check: order_check,
//   // order_st_build: order_st_build,
//   // order_st_check: order_st_check,
//   order_encode: order_encode,
//   order_sync_encode: order_sync_encode,
//   // order_decode: order_decode,
//   check_showToast: check_showToast,
//   // get_delay: get_delay,
//   // get_order: get_order,
//   get_date: get_date,
// }
#endif

