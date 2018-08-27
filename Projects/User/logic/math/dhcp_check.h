/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : dhcp_check.h
* Author             : Merafour
* Last Modified Date : 04/19/2018
* Description        : This file provides the dhcp check library.

******************************************************************************/
#ifndef _DHCP_CHECK_H_
#define _DHCP_CHECK_H_

#include <stdint.h>
//#ifdef __cplusplus
// extern "C" {
//#endif




//#ifdef __cplusplus
//}
//#endif
typedef struct { 
	uint16_t status; 
	uint16_t _order; 
	uint32_t devNumber;
	uint16_t indate;
	uint32_t ss;
}check_t;
typedef struct { 
	//uint32_t ss;
	uint32_t base;
	uint32_t base2; 
	uint32_t base3; 
	uint16_t Y, M, D, h, m, s;
}check_date_t;
typedef struct { 
	uint32_t passwd4;
	uint32_t crc;
	uint32_t len;
	uint32_t _order;
	uint32_t devNumber;
	uint32_t indate ;
}check_order_t;


class dhcp_check {
public:
	dhcp_check(){}
	uint16_t dhcp4_check(const uint8_t text_show[20], const uint32_t admin);
	uint16_t dhcp6_check(const uint8_t text_show[20], const uint32_t admin);
	uint16_t dhcp8_check(const uint8_t text_show[20], const uint32_t devNumber);
	uint16_t order_decode(const uint8_t text_show[32], const uint32_t admin, check_t *_check);
private:
	uint16_t _dhcp4_check(const uint32_t passwd3, const uint32_t passwd4, const uint32_t admin, const uint32_t base, const uint32_t base3);
	uint16_t _dhcp6_check(const uint32_t passwd4, const uint32_t passwd6, const uint32_t admin, const uint32_t base, const uint32_t base2);
	uint16_t _dhcp8_check(const uint32_t passwd4, const uint32_t passwd6, const uint32_t devNumber, const uint32_t base, const uint32_t base2);
	uint16_t _order_decode(const uint32_t admin, const uint32_t base, const uint32_t base2, 
		const check_order_t &Parse, check_t *_check);
	//check_date_t data;
	static uint32_t check_time;
	static uint32_t b3[];
	static const uint32_t b1;
};

class Subfunc {
public:
	Subfunc(){}
	uint16_t get_time(check_date_t* _date);
	void get_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second);
	void set_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second);
	uint16_t admin_length(uint32_t admin);
	uint16_t OrderParse(const uint8_t text[32], check_order_t* _check);
	const uint8_t* dhcp8_build(uint16_t delay, const uint32_t devNumber);
	const uint8_t* dhcp6_build(uint16_t delay, const uint32_t admin);
	const uint8_t* dhcp4_build(uint16_t delay, const uint32_t admin);
	
	const uint8_t* order_encode(uint16_t delay, const uint32_t admin, const uint32_t order, const uint32_t devNumber, const uint32_t indate);
	const uint8_t* order_sync_encode(uint16_t delay, const uint32_t admin, const uint32_t order, uint32_t devNumber, uint32_t indate);
	static const uint8_t cycle;
	static const uint32_t b1;
	static const uint16_t delay1[];
	static const uint16_t delay2[];
	static const uint16_t order[];
	static const uint16_t order_length;
	static const uint16_t order_sync;
private:
	const uint8_t* _dhcp8_build(const uint16_t delay, const uint32_t devNumber, const uint32_t base, const uint32_t base2);
	const uint8_t* _dhcp6_build(const uint16_t delay, const uint32_t admin, const uint32_t base, const uint32_t base2);
	const uint8_t* _dhcp4_build(const uint16_t delay, const uint32_t admin, const uint32_t base, const uint32_t base3, const uint32_t random, const uint32_t data_base);
	const uint8_t* _order_encode(const uint32_t base, const uint32_t base2, uint16_t delay, const uint32_t admin, uint32_t order, const uint32_t devNumber, const uint32_t indate);
	uint16_t get_delay(const uint8_t index);
	uint16_t get_order(const uint8_t index);
	uint8_t  text[32];
};

extern dhcp_check dhcp;
extern Subfunc sub;
#endif















