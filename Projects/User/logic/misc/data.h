/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>
typedef struct {
//	uint8_t back;       // backlight [5-30]
//	uint8_t delay;      // [5-30]
	uint16_t back;      // backlight [5s-30s] ==> [5000ms-30000ms]
	uint8_t volume;     // [0-100]
	uint8_t verify;     // 0 or 1
	uint8_t speed;      // 电机转速
	uint8_t door;       // 关门延迟
	uint8_t recv2;
	uint8_t recv3;
}sys_data_t;

#define HEAD_FORMAT   0xAEEDF985
#define HEAD_VERSION  0x0101
#define ADMIN_DHCP    345678
/*
0: spuer admin
1: static passwd
[2-9]: admin
[10-19]: dhcp4
[20-29]: dhcp6
*/
//#define ADMIN_SIZE        30
//#define ADMIN_SUPER       0
//#define PASSWD_STATIC     1
//#define PASSWD_DHCP4_MAX  20
//#define PASSWD_DHCP6_MAX  30
/**
 * 0 super admin
 * 1 static passwd
 * [2-9] admin
 * [10-19] dhcp4
 * [20-29] dhcp6
 * [30---] dhcp8
*/
#define lock_admin_max    10  // [0-9]
#define lock_dhcp4_max    20 // [10-19]
#define lock_dhcp6_max    30 // [20-29]
#define user_number_min   100    // uid 16 bit
#define user_number_max   30000  // uid 16 bit
#define devNumber_min     1000 
#define devNumber_max     100000000  // 8wei
typedef struct {
	uint16_t version; // 8:4:4
	uint16_t len;     // key
	uint32_t format;  // flag = 0xadebf985
	//uint32_t admin[ADMIN_SIZE];
//	uint8_t back;       // backlight [5-30] s
//	uint8_t delay;      // [5-30] s
//	uint8_t volume;     // [0-100]
//	uint8_t verify;     // 0 or 1
	sys_data_t _sys;
	}data_head_t;  // size:16
typedef struct{
	uint16_t uid;        // ID号,用户身份识别
	uint16_t days;        // 有效天数
	uint32_t start_date; // 录入系统时间, 0x Y:M:D:H
	uint32_t devnumber;  // 设备号或密码
	uint32_t rfid;       // RFID标识
	uint32_t finger;     // 指纹标识
}data_user_t; // size:20

class DATA {
public:
	DATA(){}
	//virtual void Init(void)=0;
	void Init(void);
	void factory(void)
	{
		reset(head);
	}
	uint16_t Test(void);
	static uint16_t get_sys(sys_data_t *_sys);
	static uint16_t get_back(void)  { return head._sys.back;}
	static uint8_t get_volume(void){ return head._sys.volume;}
	static uint8_t get_verify(void){ return head._sys.verify;}
	uint16_t save_sys(const sys_data_t *_sys);
	uint16_t get_data(data_user_t *_user, const uint16_t index);
	uint16_t get_user_index(data_user_t *_u, const uint16_t index);
	uint16_t get_total(void);
	uint16_t get_user_empty(data_user_t *_u, const uint16_t lock_region);
	uint16_t remove_data(const uint16_t index);
	uint16_t user_del(const uint16_t uid);
	uint16_t save_data(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger);
	uint16_t update_data(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger);
	int16_t  update_passwd(const uint16_t uid, const uint32_t passwd);
	int16_t  update_finger(const uint16_t uid, const uint32_t finger);
	uint16_t update_devnumber(uint16_t uid, uint32_t devnumber, uint32_t start_date, uint16_t days, uint32_t rfid, uint32_t finger);
	uint16_t del_data(uint16_t uid, uint32_t devnumber);
	uint16_t search_uid(const uint16_t uid);
	uint16_t check_admin(const uint16_t uid, const uint32_t passwd);
	uint16_t get_size(void){ return lock_size;}
protected:
	virtual uint8_t read(const uint16_t Addr, uint16_t *buf, const uint16_t len)=0;
	virtual uint8_t write(const uint16_t Addr, const uint16_t *buf, const uint16_t len)=0;
	virtual uint8_t erase_all(void)=0;
	virtual uint16_t lenght(void)=0;
private:
	uint8_t user_read(const uint16_t Addr, uint16_t *buf, const uint16_t len)
	{
		//return read(Addr+(sizeof(data_head_t)>>1), buf, len);
		return read(Addr+(sizeof(data_head_t)), buf, len);
	}
	uint8_t user_write(const uint16_t Addr, const uint16_t *buf, const uint16_t len)
	{
		//return write(Addr+(sizeof(data_head_t)>>1), buf, len);
		return write(Addr+(sizeof(data_head_t)), buf, len);
	}
	void reset(data_head_t &_head);
	static data_head_t head; //={1, sizeof(struct user_head), HEAD_FORMAT, ADMIN_DHCP};
	static uint16_t lock_size;
	static uint16_t _lenght;
};

class DATA_RAM: public DATA {
public:
	DATA_RAM(){}
	void Init(void);
	//uint16_t check_admin(const uint16_t uid);
protected:
	virtual uint8_t read(const uint16_t Addr, uint16_t *buf, const uint16_t len)
	{
		uint16_t i=0;
		if(len>(sizeof(data)>>1)) return 0;
		for(i=0; i<len; i++)
		{
			buf[i] = data[Addr+i];
		}
		return i;
	}
	virtual uint8_t write(const uint16_t Addr, const uint16_t *buf, const uint16_t len)
	{
		uint16_t i=0;
		if(len>(sizeof(data)>>1)) return 0;
		for(i=0; i<len; i++)
		{
			data[Addr+i]=buf[i];
		}
		return i;
	}
	virtual uint8_t erase_all(void);
	virtual uint16_t lenght(void){ return sizeof(data);}
private:
	uint16_t data[356];
};


#endif















