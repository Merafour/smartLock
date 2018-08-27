/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : dhcp_check.cpp
* Author             : Merafour
* Last Modified Date : 04/19/2018
* Description        : This file provides the dhcp check library.

******************************************************************************/
#include "dhcp_check.h"
#include <stdlib.h>
 
//var sub = require('../offline/Subfunc.js');
Subfunc sub;
uint32_t dhcp_check::check_time=0;
uint32_t dhcp_check::b3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//const uint32_t dhcp_check::b1 = 17280; //24 * 3600 / sub.cycle;
const uint32_t dhcp_check::b1 = 24 * 3600 / sub.cycle;

uint16_t dhcp_check::dhcp4_check(const uint8_t text_show[20], const uint32_t admin)
{
	uint32_t passwd4 = 0;
	uint32_t passwd3 = 0;
	uint32_t tmp;
//  var data = sub.get_time();
	check_date_t date;
	uint32_t base3 = 0;
	sub.get_time(&date);
	//uint32_t base = date.base % 1000;
	base3 = date.base3 + admin % 1000000;
	//uint32_t random = (data.base - base) / 1000;
	// text : p3,p4[0],p4[1],p4[2]
	passwd3 =  atol((const char*)text_show);
	tmp = passwd3 % 10; // p4[2]
	passwd4 = tmp;
	passwd3 = (passwd3 - tmp) / 10;
	tmp = passwd3 % 10; // p4[1]
	passwd4 = passwd4 * 10 + tmp;
	passwd3 = (passwd3 - tmp) / 10;
	tmp = passwd3 % 10; // p4[0]
	passwd4 = passwd4 * 10 + tmp;
	passwd3 = (passwd3 - tmp) / 10; // p3
	return _dhcp4_check(passwd3, passwd4, admin, date.base, base3);
}
uint16_t dhcp_check::_dhcp4_check(const uint32_t passwd3, const uint32_t passwd4, const uint32_t admin, const uint32_t base, const uint32_t base3)
{
	uint32_t random = 0;
	uint16_t status = 0;
	uint32_t b2 = 0;//b1 + base - (60 + 30);
	uint32_t index = 0;
	uint32_t p4 = 0;
	uint32_t _base = 0;
	uint32_t i=0, j=0;
	// p4
	//for (i = 0; i < 6900; ++i) 
	for (i = 0; i < 240; ++i) // +/- 600s
	{
		b2 = (b1 + base - i + 60) % b1; // + 300s
		_base = b2 % 1000;
		random = (b2 - _base) / 1000;
		p4 = (_base * 129 + base3 * 37 + random * 127) % 1000;
		if (p4 == passwd4) 
		{
			b3[index] = b2;
			index++;
			if (12 <= index) break;
		}
	}
	// p3
	for (i = 0; i < index; ++i) 
	{
		for (j = 0; j < 4; ++j) 
		{
			uint32_t p3 = passwd4 % 9 + admin % 9 + b3[i] % 9 + sub.delay1[j];
			p3 = p3 % 10;
			if (p3 == passwd3) 
			{
				int32_t d1 = base;//base - b3[i];
				int32_t d2 = (sub.delay2[j] + 300) / sub.cycle;
				d1 = d1 -b3[i];
				if (0 >= d1) d1 = -d1;
				if ((d1 < d2) && ( b3[i]>check_time)) // if (d1 < d2) 
				//if (d1 < d2) 
				{
					check_time =  b3[i];
					status = 1; // '密码验证通过'
					i = index + 1;
					break;
				}
				else 
				{
					status = 2; // '密码已失效'
				}
			}
		}
	}
  return status;
}
uint16_t dhcp_check::dhcp6_check(const uint8_t text_show[20], const uint32_t admin)
{
	uint32_t passwd4 = 0;
	uint32_t passwd3 = 0;
	uint32_t passwd6 = 0;
	check_date_t date;
	sub.get_time(&date);
	// text : p3,p4,[p6]
	passwd3 = atol((const char*)text_show); // parseInt(text_show, 10);
	passwd6 = passwd3 % 100;
	passwd3 = (passwd3 - passwd6) / 100;
	passwd4 = passwd3 % 100;
	passwd3 = (passwd3 - passwd4) / 100;
	passwd4 = passwd4 * 100 + passwd3;
	return _dhcp6_check(passwd4, passwd6, admin, date.base, date.base2);
}
uint16_t dhcp_check::_dhcp6_check(const uint32_t passwd4, const uint32_t passwd6, const uint32_t admin, const uint32_t base, const uint32_t base2)
{
	uint32_t sum1 = 0;
	uint32_t sum2 = 0;
	uint32_t sum3 = 0;
	uint32_t passwd3=0;
	uint16_t status = 0;
	uint32_t b2 = b1 + base - (60 + 30);
	uint32_t index = 0;
	uint32_t i=0, j=0;

	// p4
	//for (i = 0; i < 900; ++i) 
	for (i = 0; i < 240; ++i) // +/- 600s
	{
		b2 = (b1 + base - i + 60) % b1; // + 300s
		sum1 = (((b2* 32)% 86400) * 1024 ) % 86400; 
		sum2 = ((base2% 86400) * 1024) % 86400;
		sum3 = (((admin* 64) % 86400) * 1024 ) % 86400; //(admin * 1024 * 64) % 86400;
		passwd3 = (sum1 + sum2 + sum3) % 86400;
		//passwd3 = (b2 * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400; // 24*3600/10 = 8640
		passwd3 = (passwd3 - passwd3 % 9) / 9;
		if (passwd3 == passwd4) 
		{
			b3[index] = b2;
			index++;
			if (12 <= index) break;
		}
	}
	// p6
	for (i = 0; i < index; ++i) 
	{
		for (j = 0; j < 4; ++j) 
		{
			uint32_t p6 = passwd4 % 100;
			p6 = p6 + (passwd4 - p6) / 100;
			p6 = p6 + sub.delay1[j] + b3[i] % 100;
			p6 = p6 % 100;
			if (p6 == passwd6) 
			{
				int32_t d1 = base - b3[i];
				int32_t d2 = (sub.delay2[j] + 300) / sub.cycle;
				if (0 >= d1) d1 = -d1;
				if ((d1 < d2) && ( b3[i]>check_time)) // if (d1 < d2) 
				//if (d1 < d2) 
				{
					check_time =  b3[i];
					status = 1; // '密码验证通过'
					i = index + 1;
					break;
				}
				else 
				{
					status = 2; // '密码已失效'
				}
			}
		}
	}
  return status;
}
uint16_t dhcp_check::dhcp8_check(const uint8_t text_show[20], const uint32_t devNumber)
{
	uint32_t passwd4 = 0;
	uint32_t passwd3 = 0;
	uint32_t passwd6 = 0;
	check_date_t date;
	sub.get_time(&date);
	// text : p4,p6
	passwd3 = atol((const char*)text_show); // parseInt(text_show, 10);
	passwd6 = passwd3 % 10000;             // p6
	passwd4 = (passwd3 - passwd6) / 10000;   // p4
	passwd3 = passwd4 % 100;
	passwd4 = (passwd4 - passwd3) / 100;
	passwd4 = passwd3 * 100 + passwd4;
	passwd3 = passwd6 % 100;
	passwd6 = (passwd6 - passwd3) / 100;
	passwd6 = passwd3 * 100 + passwd6;
	return _dhcp8_check(passwd4, passwd6, devNumber, date.base, date.base2);
}
uint16_t dhcp_check::_dhcp8_check(const uint32_t passwd4, const uint32_t passwd6, const uint32_t devNumber, const uint32_t base, const uint32_t base2)
{
	uint32_t passwd3 = 0;
	uint16_t status = 0;
	uint32_t b2 = b1 + base - (60 + 30);
	uint32_t index = 0;
	uint32_t delay = 60; // default
	uint32_t i=0, j=0;
	// p4
	//for (i = 0; i < 900; ++i) 
	for (i = 0; i < 240; ++i) // +/- 600s
	{
		b2 = (b1 + base - i + 60) % b1; // + 300s
		for (j = 0; j < 4; j++) 
		{
			//passwd3 = (b2 * 1024 * 37 + base2 * 1024 * 67) % 86400;
			uint32_t sum = (((base2 * 67) % 86400) * 1024) % 86400;
			passwd3 = sum;
			sum = (((b2 * 37) % 86400) * 1024) % 86400;
			passwd3 = passwd3 + sum;
			passwd3 = passwd3 % 86400;
			passwd3 = (passwd3 - passwd3 % 9) / 9 + sub.delay1[j];
			passwd3 = passwd3 % 10000;
			if (passwd3 == passwd4) 
			{
				b3[index] = b2;
				index++;
				delay = sub.delay2[j];
				if (12 <= index) break;
			}
		}
	}
	// p6
	//for (var i = 0; i < index; ++i) 
	for (i = 0; i < index; ++i) 
	{
		//for (j = 0; j < num_size; ++j) 
		{
			//long sum = (((Number[j] * 64) % 66400) * 1024) % 66400;
			uint32_t sum = (((devNumber * 64) % 66400) * 1024) % 66400;
			uint32_t p6 = sum;
			sum = (base2 * 1024) % 66400;
			p6 = p6 + sum;
			sum = (b3[i] * 64) % 66400;
			p6 = p6 + sum;
			sum = (passwd4 * 32) % 66400;
			p6 = p6 + sum;
			p6 = p6 % 66400;
			p6 = (p6 - p6 % 7) / 7;
			if (p6 == passwd6) 
			{
				int32_t d1 = base - b3[i];
				int32_t d2 = (delay + 120) / sub.cycle; // delay，+/- 600s
				if (0 >= d1) d1 = -d1;
				if ((d1 < d2) && ( b3[i]>check_time)) // if (d1 < d2)
				//if (d1 < d2) 
				{
					check_time =  b3[i];
					status = 1; // '密码验证通过'
					i = index + 1;
					break;
				}
				else 
				{
					status = 2; // '密码已失效'
				}
			}
		}
	}
	return status;
}
uint16_t dhcp_check::_order_decode(const uint32_t admin, const uint32_t base, const uint32_t base2, const check_order_t &Parse, check_t *_check)
{
	//var Number = [];      //生成一个新的空数组
	//uint32_t ss = 0;
	//uint32_t b3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	//uint32_t b1 = 24 * 3600 / sub.cycle;
	uint32_t b2 = b1 + base - (60 + 30);
	uint32_t index = 0;
	uint32_t status = 0;
	uint32_t _delay[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t i=0, j=0;
	// p4
	for ( i = 0; i < 900; ++i) 
	{
		b2 = (b1 + base - i + 120) % b1; // +/- 600s
		//b2 = (b1 + base - i + 2) % b1; // +/- 600s
		for ( j = 0; j < 4; j++) 
		{
			//passwd4 = (base * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400;
			long sum = (((admin * 64) % 86400) * 1024) % 86400;
			uint32_t passwd3 = sum;
			sum = (base2 * 1024) % 86400;
			passwd3 = passwd3 + sum;
			sum = (((b2 * 32) % 86400) * 1024) % 86400;
			passwd3 = passwd3 + sum;
			passwd3 = passwd3 % 86400;
			//passwd4 = (passwd4 - passwd4 % 9) / 9 + delay + admin_length(admin) * 10;
			//passwd4 = passwd4 % 10000;
			passwd3 = (passwd3 - passwd3 % 9) / 9 + sub.delay1[j] + sub.admin_length(admin) * 10;
			passwd3 = passwd3 % 10000;
			if (passwd3 == Parse.passwd4) 
			{
				b3[index] = b2;
				index++;
				_delay[index] = sub.delay2[j];
				if (12 <= index) break;
			}
		}
	}
	// p6
	//for (var i = 0; i < index; ++i) 
	for (i = 0; i < index; ++i) 
	{
		//for (var j = 0; j < 2; ++j) {
		for (j = 0; j < sub.order_length; ++j) 
		{
			//var p6 = (passwd4 * 32 + b3[i] * 64 + base2 * 1024 + Number[j] * 1024 * 64) % 66400;
			//p6 = (p6 - p6 % 7) / 7;
			//var p6 = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400 + indate * 1024 * 64;
			//var sum = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400;
			long sum = (Parse.passwd4 * 1024) % 86400;
			uint32_t p6 = sum;
			sum = (((Parse.devNumber * 32) % 86400) * 1024) % 86400;
			//sum = (devNumber * 1024 * 32) % 86400;
			p6 = p6 + sum;
			p6 = p6 % 86400;
			sum = (((Parse.indate * 64) % 99) * 1024) % 99;
			p6 = p6 + sum;
			p6 = p6 % 99;

			p6 = p6 % 99 + sub.order[j] + b3[i] * 10 + Parse.len;
			p6 = p6 % 99;
			if (p6 == Parse.crc) 
			{
				int32_t d1 = base - b3[i];
				//var d2 = (delay + 120) / cycle; // delay，+/- 600s
				int32_t d2 = (_delay[i] + 120) / sub.cycle; // delay，+/- 600s
				if (0 >= d1) d1 = -d1;
				if (d1 < d2) 
				{
					//status = 1;
					//_order = (order + (passwd4 * 7) % 8) % 10;
					if (Parse._order == (sub.order[j] + (Parse.passwd4 * 7) % 8) % 10) 
					{
						status = 1;
						_check->_order = sub.order[j];
						_check->ss = b3[i]*5;
					}
					else 
					{
						status = 0;
					}
				}
				else 
				{
					status = 2;
				}
			}
		}
	}
	_check->devNumber = Parse.devNumber;
	_check->indate = Parse.indate;
	_check->status = status;
	//return { status, _order, devNumber, indate, ss};
	return 0;
}
//function order_decode(text_show, admin) 
uint16_t dhcp_check::order_decode(const uint8_t text_show[32], const uint32_t admin, check_t *_check)
{
	check_date_t date;
	check_order_t Parse;
	uint32_t base = 0;
	uint32_t base2 = 0;

	sub.get_time(&date);
	base = date.base;
	base2 = date.base2;
	sub.OrderParse(text_show, &Parse);
	//if (sub.order_value6 == Parse._order) // sync
	if (sub.order_sync == (Parse._order + (Parse.passwd4 * 7) % 8) % 10) 
	{
		//base = (Parse.indate + 1) * 216;//(ss - ss % cycle) / cycle;
		//var base2 = Y + M * 70 + D * 24 * 3600;// + this.data.delay;
		uint32_t D = Parse.devNumber % 100;
		uint32_t Y = (Parse.devNumber - D) / 100;
		uint32_t M = Y % 100;
		Y = (Y - M) / 100;
		base2 = Y * 12 + M * 31 + D * 24 * 3600;
		base = (Parse.indate + 1) * 216;
	}
	//return _order_decode(admin, base, base2, Parse.passwd4, Parse.crc, Parse.len, Parse._order, Parse.devNumber, Parse.indate, _check);
#if 1
	return _order_decode(admin, base, base2, Parse, _check);
#else
	// decode admin:345678 Order passwd：8360 crc：24 len：6 order：3 dev：225674 indate：1
	Parse.passwd4 = 8360;
	Parse.crc = 24;
	Parse.len = 6;
	Parse._order = 3;
	Parse.devNumber = 225674;
	Parse.indate = 1;
	return _order_decode(345678, base, base2, Parse, _check);
#endif
}
#if 0
function order_check(text_show, admin) {
  var data = sub.get_time();
  var ss = data.ss;
  var base = data.base;
  var base2 = data.base2;

  var passwd4 = 0;
  var passwd3 = 0;
  var passwd6 = 0;
  var crc = 0;
  var delay = 60; // default

  //sub.OrderParse(this.data.text);
  //passwd3 = parseInt(text_show, 10);
  passwd3 = parseInt(text_show.slice(0, 6), 10);
  var devNumber = parseInt(text_show.slice(7, 13), 10);
  var indate = parseInt(text_show.slice(13, 20), 10);
  var _order = 0;
  passwd6 = passwd3 % 100;             // p6
  passwd4 = (passwd3 - passwd6) / 100;   // p4
  passwd3 = passwd4 % 100;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  passwd4 = (passwd4 - passwd3) / 100;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  passwd4 = passwd3 * 100 + passwd4;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  crc = passwd6;
  console.log("passwd4：" + passwd4 + "  passwd6：" + passwd6 + " admin:" + admin);
  console.log("passwd：" + passwd4 + "  crc：" + crc + "  dev：" + devNumber + "  indate：" + indate);

  //var Number = [];      //生成一个新的空数组
  //var order = [];
  var b3 = [];
  var b1 = 24 * 3600 / sub.cycle;
  var b2 = b1 + base - (60 + 30);
  var index = 0;
  var status = 0;
  //Number = [123456, 234567, 345678, 456789, 123698, 225674];  //生成一个新的非空数组，数组元素可以是任何类型
  b3 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
  //order = [order_value1, order_value2, order_value3, order_value4, order_value5, order_value6];
  ///console.log("  b2：" + b2 + "  b1：" + b1);
  console.log("  b2：" + b2 + "  b1：" + b1 + "  base：" + base + "  base2：" + base2);
  // var delay1 = [];      //生成一个新的空数组
  // var delay2 = [];
  //var delay = 60; // default
  // delay1 = [delay_value1, delay_value2, delay_value3, delay_value4];  //生成一个新的非空数组，数组元素可以是任何类型
  // delay2 = [1 * 60, 2 * 60, 5 * 60, 10 * 60];
  // p4
  /**
  var passwd4 = (base * 1024 * 32 + base2 * 1024 + random * 1024 * 64) % 86400; // 24*3600/10 = 8640
  passwd4 = (passwd4 - passwd4 % 9) / 9 + this.data.delay;
  passwd4 = passwd4 % 10000;
  */
  for (var i = 0; i < 900; ++i) {
    b2 = (b1 + base - i + 120) % b1; // +/- 600s
    for (var j = 0; j < 4; j++) {
      //passwd3 = (b2 * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400;
      var sum = (((admin * 64) % 86400) * 1024) % 86400;
      passwd3 = sum;
      sum = (base2 * 1024) % 86400;
      passwd3 = passwd3 + sum;
      sum = (((b2 * 32) % 86400) * 1024) % 86400;
      passwd3 = passwd3 + sum;
      passwd3 = passwd3 % 86400;
      passwd3 = (passwd3 - passwd3 % 9) / 9 + sub.delay1[j] + sub.admin_length(admin) * 10;
      passwd3 = passwd3 % 10000;
      //console.log("passwd3：" + passwd3 + "  b2：" + b2 + "  b1：" + b1 + "  i：" + i);
      if (passwd3 === passwd4) {
        b3[index] = b2;
        index++;
        delay = sub.delay2[j];
        console.log("passwd3：" + passwd3 + "  b2：" + b2 + "  i：" + i + " delay" + delay);
        if (12 <= index) break;
      }
    }
  }
  // p6
  /**
  crc = (passwd4 * 1024 + this.data.devNumber * 1024 * 32) % 86400 + this.data.indate * 1024 * 64;
  crc = crc % 99 + this.data.order + (base * 10);
  crc = crc % 99;
  */
  //for (var i = 0; i < index; ++i) 
  for (var i = 0; i < index; ++i) {
    //for (var j = 0; j < 2; ++j) {
    for (var j = 0; j < sub.order.length; ++j) {
      //var p6 = (passwd4 * 32 + b3[i] * 64 + base2 * 1024 + Number[j] * 1024 * 64) % 66400;
      //p6 = (p6 - p6 % 7) / 7;
      //var p6 = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400 + indate * 1024 * 64;
      //var sum = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400;
      var sum = (passwd4 * 1024) % 86400;
      var p6 = sum;
      sum = (((devNumber * 32) % 86400) * 1024) % 86400;
      //sum = (devNumber * 1024 * 32) % 86400;
      p6 = p6 + sum;
      p6 = p6 % 86400;
      sum = (((indate * 64) % 99) * 1024) % 99;
      p6 = p6 + sum;
      p6 = p6 % 99;

      p6 = p6 % 99 + sub.order[j] + b3[i] * 10;
      p6 = p6 % 99;
      if (p6 === crc) {
        console.log("p6：" + p6 + "  base：" + b3[i] + "  order：" + sub.order[j]);
        var d1 = base - b3[i];
        var d2 = (delay + 120) / sub.cycle; // delay，+/- 600s
        if (0 >= d1) d1 = -d1;
        if (d1 < d2) {
          status = 1;
          _order = sub.order[j];
        }
        else {
          status = 2;
        }

        console.log("p6：" + p6 + "  base：" + b3[i] + "  order：" + sub.order[j] + "  d1：" + d1 + "  d2：" + d2);
      }
    }
  }
  return { status, _order, devNumber, indate };
}
function order_st_check(text_show, admin) {
  // var data = get_time();
  // var ss = data.ss;
  // var base = data.base;
  // var base2 = data.base2;

  var passwd4 = 0;
  var passwd3 = 0;
  var passwd6 = 0;
  var crc = 0;
  var delay = 60; // default

  //sub.OrderParse(this.data.text);
  passwd3 = parseInt(text_show, 10);
  var devNumber = parseInt(text_show.slice(7, 13), 10);
  var indate = parseInt(text_show.slice(13, 20), 10);
  var _order = 0;
  passwd6 = passwd3 % 100;             // p6
  passwd4 = (passwd3 - passwd6) / 100;   // p4
  passwd3 = passwd4 % 100;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  passwd4 = (passwd4 - passwd3) / 100;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  passwd4 = passwd3 * 100 + passwd4;
  //console.log("passwd3：" + passwd3 + "  passwd4：" + passwd4);
  crc = passwd6;
  console.log("passwd4：" + passwd4 + "  passwd6：" + passwd6 + " admin:" + admin);
  console.log("passwd：" + passwd4 + "  crc：" + crc + "  dev：" + devNumber + "  indate：" + indate);

  //var ss = s + m * 60 + h * 3600;// + D * 24 * 3600;// + M * 31 * 24 * 3600;// + (Y % 10)*12*31*24*3600;
  var base = (indate + 1) * 216;//(ss - ss % cycle) / cycle;
  //var base2 = Y + M * 70 + D * 24 * 3600;// + this.data.delay;
  var D = devNumber % 100;
  var Y = (devNumber - D) / 100;
  var M = Y % 100;
  Y = (Y - M) / 100;
  var base2 = Y * 12 + M * 31 + D * 24 * 3600;

  var Number = [];      //生成一个新的空数组
  //var order = [];
  var b3 = [];
  var b1 = 24 * 3600 / sub.cycle;
  var b2 = b1 + base - (60 + 30);
  var index = 0;
  var status = 0;
  Number = [123456, 234567, 345678, 456789, 567890, 225674];  //生成一个新的非空数组，数组元素可以是任何类型
  b3 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
  //order = [order_value1, order_value2, order_value3, order_value4, order_value5, order_value6];
  console.log("  b2：" + b2 + "  b1：" + b1 + "  base：" + base + "  base2：" + base2);
  //console.log("  Y：" + Y + "  M：" + M + "  D：" + D);
  // var delay1 = [];      //生成一个新的空数组
  // var delay2 = [];
  //var delay = 60; // default
  // delay1 = [delay_value1, delay_value2, delay_value3, delay_value4];  //生成一个新的非空数组，数组元素可以是任何类型
  // delay2 = [1 * 60, 2 * 60, 5 * 60, 10 * 60];
  // p4
  /**
  var passwd4 = (base * 1024 * 32 + base2 * 1024 + random * 1024 * 64) % 86400; // 24*3600/10 = 8640
  passwd4 = (passwd4 - passwd4 % 9) / 9 + this.data.delay;
  passwd4 = passwd4 % 10000;
  */
  for (var i = 0; i < 900; ++i) {
    b2 = (b1 + base - i + 120) % b1; // +/- 600s
    for (var j = 0; j < 4; j++) {
      //passwd3 = (b2 * 1024 * 32 + base2 * 1024 + admin * 1024 * 64) % 86400;
      var sum = (((admin * 64) % 86400) * 1024) % 86400;
      passwd3 = sum;
      sum = (base2 * 1024) % 86400;
      passwd3 = passwd3 + sum;
      sum = (((b2 * 32) % 86400) * 1024) % 86400;
      passwd3 = passwd3 + sum;
      passwd3 = passwd3 % 86400;
      passwd3 = (passwd3 - passwd3 % 9) / 9 + sub.delay1[j] + sub.admin_length(admin) * 10;
      passwd3 = passwd3 % 10000;
      //console.log("passwd3：" + passwd3 + "  b2：" + b2 + "  b1：" + b1 + "  i：" + i);
      if (passwd3 === passwd4) {
        b3[index] = b2;
        index++;
        delay = sub.delay2[j];
        console.log("passwd3：" + passwd3 + "  b2：" + b2 + "  i：" + i + " delay" + delay);
        if (12 <= index) break;
      }
    }
  }
  // p6
  /**
  crc = (passwd4 * 1024 + this.data.devNumber * 1024 * 32) % 86400 + this.data.indate * 1024 * 64;
  crc = crc % 99 + this.data.order + (base * 10);
  crc = crc % 99;
  */
  //for (var i = 0; i < index; ++i) 
  for (var i = 0; i < index; ++i) {
    //for (var j = 0; j < 2; ++j) {
    for (var j = 0; j < order.length; ++j) {
      //var p6 = (passwd4 * 32 + b3[i] * 64 + base2 * 1024 + Number[j] * 1024 * 64) % 66400;
      //p6 = (p6 - p6 % 7) / 7;
      //var p6 = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400 + indate * 1024 * 64;
      //var sum = (passwd4 * 1024 + devNumber * 1024 * 32) % 86400;
      var sum = (passwd4 * 1024) % 86400;
      var p6 = sum;
      sum = (((devNumber * 32) % 86400) * 1024) % 86400;
      //sum = (devNumber * 1024 * 32) % 86400;
      p6 = p6 + sum;
      p6 = p6 % 86400;
      sum = (((indate * 64) % 99) * 1024) % 99;
      p6 = p6 + sum;
      p6 = p6 % 99;

      p6 = p6 % 99 + order[j] + b3[i] * 10;
      p6 = p6 % 99;
      if (p6 === crc) {
        console.log("p6：" + p6 + "  base：" + b3[i] + "  order：" + sub.order[j]);
        var d1 = base - b3[i];
        var d2 = (delay + 1000) / sub.cycle; // delay，+/- 600s
        if (0 >= d1) d1 = -d1;
        if (d1 < d2) {
          status = 1;
          _order = sub.order[j];
        }
        else {
          status = 2;
        }

        console.log("p6：" + p6 + "  base：" + b3[i] + "  order：" + sub.order[j] + "  d1：" + d1 + "  d2：" + d2);
      }
    }
  }
  return { status, _order, devNumber, indate };
}


//module.exports = {
//  dhcp4_check: dhcp4_check,
//  dhcp6_check: dhcp6_check,
//  dhcp8_check: dhcp8_check,
//  // order_st_check: order_st_check,
//  // order_sync_encode: order_sync_encode,
//  order_decode: order_decode,
//}
#endif

