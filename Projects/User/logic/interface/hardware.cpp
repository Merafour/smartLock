/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : hardware.cpp
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the hardware library.
* Description        : Ó²¼þ²Ù×÷.
******************************************************************************/
#include <stdint.h>
#include "object.h"
#include "safety.h"
#include "delay.h"
#include "menu.h"
#include "dhcp_check.h"

#include "main.h"
#include "RTC.h"


void MENU::get_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second)
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	RTC_GetTime(RTC_Format_BIN, &Time);
	RTC_GetDate(RTC_Format_BIN, &Date);
	year = Date.RTC_Year;//+2000;
	month = Date.RTC_Month;
	day = Date.RTC_Date;
	hour = Time.RTC_Hours;
	minute = Time.RTC_Minutes;
	second = Time.RTC_Seconds;
}
void MENU::set_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second)
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	Date.RTC_Year = year;
	Date.RTC_Month = month;
	Date.RTC_Date = day;
	Time.RTC_Hours = hour;
	Time.RTC_Minutes = minute;
	Time.RTC_Seconds = second;
	RTC_SetTime(RTC_Format_BIN, &Time);
	RTC_SetDate(RTC_Format_BIN, &Date);
}
uint32_t MENU::waitting(const uint32_t ms)
{
	return waitting_time(ms);
}
uint64_t MENU::tick(void)
{
	return GetTick();
}

void Subfunc::get_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second)
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	RTC_GetTime(RTC_Format_BIN, &Time);
	RTC_GetDate(RTC_Format_BIN, &Date);
	year = Date.RTC_Year;//+2000;
	month = Date.RTC_Month;
	day = Date.RTC_Date;
	hour = Time.RTC_Hours;
	minute = Time.RTC_Minutes;
	second = Time.RTC_Seconds;
}
void Subfunc::set_time(uint16_t &year, uint16_t &month, uint16_t &day, uint16_t &hour, uint16_t &minute, uint16_t &second)
{
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	Date.RTC_Year = year;
	Date.RTC_Month = month;
	Date.RTC_Date = day;
	Time.RTC_Hours = hour;
	Time.RTC_Minutes = minute;
	Time.RTC_Seconds = second;
	RTC_SetTime(RTC_Format_BIN, &Time);
	RTC_SetDate(RTC_Format_BIN, &Date);
}


