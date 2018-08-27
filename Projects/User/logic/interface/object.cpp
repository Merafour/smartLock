/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : object.cpp
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the object library.
* Description        : 通用接口类.
******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "object.h"
#include "safety.h"
#include "delay.h"
//#include "data.h"
typedef struct {
	const uint8_t num;  // 编号
	const char *title;  // 语音内容
}voice_t;

static voice_t voice_info[]={
	{0 , "滴"},
	{1 , "叮咚"},
	{2 , "请输入用户编号"},
	{3 , "请输入密码"},
	{4 , "请再次输入密码"},
	{5 , "操作失败"},
	{6 , "操作拒绝"},
	{7 , "操作完成"},
	{8 , "输入错误"},
	{9 , "没有权限"},
	{10, "已确认"},
	{11, "门已开"},
	{12, "进入菜单"},
	{13, "返回"},
	{14, "1"},
	{15, "2"},
	{16, "3"},
	{17, "4"},
	{18, "5"},
	{19, "6"},
	{20, "7"},
	{21, "8"},
	{22, "9"},
	{23, "0"},
	{24, "欢迎进入"},
	{25, "欢迎回来"},
	{26, "门未锁"},
	{27, "门已开"},
	{28, "请注意"},
};

uint8_t voice_obj::search(const char* _voice)
{
	uint8_t i=0;
	for(i=0; i<sizeof(voice_info)/sizeof(voice_info[0]); i++)
	{
		if(0==strcmp(voice_info[i].title, _voice))
		{
			return voice_info[i].num;
		}
	}
	return 0;
}
int8_t voice_obj::play_info(uint8_t volume, uint8_t speed, const char* info)
{
	return play_voice(volume, speed, search(info));
}


uint64_t rgbw_obj::tick(void)
{
	return GetTick();
}
uint64_t model_obj::tick(void)
{
	return GetTick();
}
void model_obj::_delay_ms(uint16_t nms)
{
	delay_ms(nms);
}

uint64_t safety::tick(void)
{
	return GetTick();
}
void safety::_delay_ms(uint16_t nms)
{
	delay_ms(nms);
}


