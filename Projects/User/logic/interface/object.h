/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : object.h
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the object library.
* Description        : 通用接口类.
******************************************************************************/
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdint.h>
#include "data.h"

/**
 语音接口
 */
class voice_obj {
private:
protected:
public:
    voice_obj() {}
	uint8_t search(const char* _voice);
	virtual void init(void) = 0;
	virtual int8_t play_num(uint8_t volume,uint8_t speed,char *str_num) = 0;
	virtual int8_t play_voice(uint8_t volume,uint8_t speed,uint8_t num) = 0;
	int8_t play_info(uint8_t volume, uint8_t speed, const char* info);
	virtual int8_t busy(void) = 0;
};

/** 2468 */
//#define RGBW_MENU    ((0x1<<2)|(0x1<<4)|(0x1<<6)|(0x1<<8))
#define RGBW_MENU    ((0x1<<1)|(0x1<<3)|(0x1<<5)|(0x1<<7))
#define RGBW_RFID    0xFFFF
#define RGBW_FING    0xFFFF
#define RGBW_BEEP    (0x1<<11)

class rgbw_obj {
private:
	
protected:
	uint64_t tick(void);
	uint8_t start;
	uint8_t _flowing;
public:
    rgbw_obj() { _flowing=1; bright=0x0F;}
	virtual void flowing(uint8_t _flow) = 0;// { start=_flow;}
	virtual void init(void) = 0;
	virtual void update(void) = 0;
	virtual void flowing(void) = 0;
	virtual void show(uint8_t key) = 0;
	virtual void multi(uint16_t mask) = 0;
	uint8_t bright;
};
/**
  电机马达接口,包含与小板的通信，获取电量
 */
#define MODEL_NULL       0x0001     // 无事件
#define MODEL_OPEN       0x0002     // 开门
#define MODEL_CLOSE      0x0004     // 关门
#define MODEL_ERROR      0x0008     // 错误
#define MODEL_OPEN_IN    0x0010     // 开门
#define MODEL_CLOSE_IN   0x0020     // 关门
//#define MODEL_SLEEP      0x0040     // 睡眠

#define DOOR_OPEN     0x10
#define DOOR_CLOSE    0x20

class model_obj {
private:
	
protected:
	uint64_t tick(void);
	void _delay_ms(uint16_t nms);
	volatile uint16_t _voltage;
	uint8_t _door;
	DATA &_data;
public:
    model_obj(DATA &data):_data(data) { _voltage=3700; _door=DOOR_CLOSE; }
	virtual void init(void) = 0;
	virtual void sleep(void) = 0;
	virtual uint8_t open_door(void) = 0;    // 开门
	virtual uint8_t close_door(void) = 0;   // 关门
	//virtual uint16_t voltage(void) = 0;  // 获取电量, 0.01V
	uint16_t voltage(void) { return _voltage; }
	uint8_t door(void) {return _door; }
	/** 非阻塞查询,检测门状态,并执行关门动作, 返回值: MODEL_* */
	virtual int16_t poll(void) = 0;
};



#endif // __OBJECT_H__

