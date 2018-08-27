/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#ifndef _MENU_H_
#define _MENU_H_

//#include "ssd1306syp.h"
#include "Draw.h"
#include "input_manage.h"
#include "data.h"
#include "safety.h"
#include "object.h"

typedef struct {
	void (*func)(void);
	//const uint8_t const *font;
	uint8_t const *font;
	//uint8_t len;
	uint8_t ascii:1;
	uint8_t len:7;
}menu_t;

#define  VERIFY_PASSWD       0x0001
#define  VERIFY_DHCP4        0x0002
#define  VERIFY_DHCP6        0x0004
#define  VERIFY_DHCP8        0x0008
#define  VERIFY_RFID         0x0010
#define  VERIFY_FING         0x0020


class MENU {
public:
    //MENU(ssd1306syp &display):_display(display),uid(0){}
	//MENU(ssd1306syp &display, input_manage &input):_display(display),_input(input){}
	MENU(Draw &display, input_manage &input, DATA &data, safety &finger, safety &rf, voice_obj &voice, model_obj &motor):_display(display), \
		_input(input), _data(data), _finger(finger), _rfid(rf), _voice(voice), _motor(motor){}
	//void show_menu(const uint8_t Choice, const uint8_t row, const uint8_t font[], const uint8_t len, const uint8_t ascii);
	void common(const menu_t* _menu, const uint16_t len, const uint8_t max_row);
	void showToast(uint16_t status, const uint16_t _verify, const uint32_t uid);
	void show_title(const int16_t x, const int16_t y, const char* title);
	//void show_title(const int16_t x, const int16_t y, const char* title, const uint16_t number);
	uint8_t get_text_input(const uint8_t _key, uint8_t passwd[32], const uint8_t len, const uint8_t fixation, const char* title);
	uint32_t get_user_input(const uint8_t _key, const uint8_t len, const uint8_t fixation, const char* title);
	uint32_t get_user_input(const uint8_t _key, const uint8_t len, const uint8_t fixation, const char* title, const uint16_t uid);
	uint16_t browse(data_user_t *_user);
	uint16_t browse_user(data_user_t *_u);
	uint16_t browse_admin(data_user_t *_u);
	void dhcp4(const uint8_t passwd[32]);
	void dhcp6(const uint8_t passwd[32]);
	void dhcp8(const uint8_t passwd[32]);
	int16_t match_passwd(const uint8_t uid, const uint32_t input);
	void rfid(const uint16_t _id);
	void finger(const uint16_t _id);
	void clear(void)
	{
		verify = 0;
		_uid=0;
	}
	//virtual void user_option(void)=0;
	//void user_option(void);
	virtual void option(void)=0;
	void get_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second);
	void set_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second);
	uint32_t start_date(void);
		
	static uint16_t check_uid;
	static uint16_t back;
	static uint8_t passwd_len;
	static uint8_t error_delay;
	static uint8_t door_delay;
	static uint8_t open_count;
protected:
	uint64_t tick(void);
	uint32_t waitting(const uint32_t ms);
	//uint16_t uid(uint16_t _uid);
	Draw &_display;
	input_manage &_input;
	DATA &_data;
	safety &_finger;
	safety &_rfid;
	voice_obj &_voice;
	model_obj &_motor;
	static uint16_t verify;     // 校验标志,用于多重校验
	static uint32_t _uid;     // 校验标志,用于多重校验
	static uint8_t error_count; // 密码错误计数
	static uint8_t door_count; // 密码错误计数
private:
	//ssd1306syp &_display;
	//uint16_t uid;
};


#endif















