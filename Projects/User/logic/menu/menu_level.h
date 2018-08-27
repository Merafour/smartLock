/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : menu.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the menu library.

******************************************************************************/
#ifndef _MENU_LEVEL_H_
#define _MENU_LEVEL_H_

#include "menu.h"

class MENU_LEVEL : public MENU {
public:
    //MENU(ssd1306syp &display):_display(display),uid(0){}
	MENU_LEVEL(Draw &display, input_manage &input, DATA &data, safety &finger, safety &rf, voice_obj &voice, model_obj &motor):MENU(display, input, data, finger, rf, voice, motor){}
	//virtual void user_option(void)=0;
	virtual void option(void);
	void func_about(void);
	void func_finger_verify(void);
	void func_finger_del(void);
	void func_finger(void);
	void func_passwd(void);
	void func_admin(void);
	uint16_t user_check_admin(void);
	/************** menu admin *******************/
	void menu_admin_option(void);
	void func_del(void);
	void func_add_admin(void);
	void func_add(void);
	void func_time(void);
	
	/************** menu level3 *******************/
	void level3_finger_del(void);
	void level3_finger(void);
	void level3_passwd(void);
	void level3_default(void);
	void level3_verify(void);
	void level3_backlight(void);
	void level3_delay(void);
	void level3_volume(void);
	void func_see(void);
	void func_speed(void);
	void func_door(void);
protected:
private:
//	static menu_t  _menu[];
//	static uint8_t _menu_size;// = sizeof(user_menu)/sizeof(user_menu[0]);
};

//class MENU_ADMIN : public MENU {
//public:
//    //MENU(ssd1306syp &display):_display(display),uid(0){}
//	MENU_ADMIN(Draw &display, input_manage &input, DATA &data):MENU(display, input, data){}
//	//virtual void user_option(void)=0;
//	virtual void option(void);
//	void func_del(void);
//	void func_add_admin(void);
//	void func_add(void);
//	void func_time(void);
//protected:
//private:
////	static menu_t  _menu[];
////	static uint8_t _menu_size;// = sizeof(user_menu)/sizeof(user_menu[0]);
//};


//extern uint16_t check_uid;
extern MENU_LEVEL menu;
//extern MENU_ADMIN menu_admin;
//extern void menu_level_option(void);
//extern void menu_admin_option(void);


#endif















