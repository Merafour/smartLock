/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : model.h
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the model library.
* Description        : 电机马达接口,包含与小板的通信，获取电量.
******************************************************************************/
#ifndef __MODEL_H__
#define __MODEL_H__

#include "stdint.h"
#include "object.h"


//typedef struct {
//	uint8_t head;
//	uint8_t len;
//}motor_t;

#define MOTOR_CMD_NULL   0x00
#define MOTOR_CMD_SYNC   0x01
#define MOTOR_CMD_ACK    0x02
#define MOTOR_CMD_OPEN   0x03
#define MOTOR_CMD_CLOSE  0x04
#define MOTOR_CMD_VOL    0x05
#define MOTOR_CMD_STOP   0x06
#define MOTOR_CMD_SLEEP  0x07     // 睡眠

typedef union{
	uint8_t buf[16];
	struct {
		uint8_t head;  // 0x5A
		uint8_t len;
		uint8_t crc;
		uint8_t cmd;
		uint8_t arg;
		uint8_t arg2;
		uint8_t arg3[10];
	}u;
	struct {    // sync
		uint8_t head;  // 0x5A
		uint8_t len;
		uint8_t crc;
		uint8_t cmd;
		uint8_t volL;
		uint8_t volH;
		uint8_t door;
		uint8_t recv;
		uint8_t arg2[8];
	}us;
}motor_t;



class motor_obj : public model_obj {
public:
	motor_obj(DATA &data):model_obj(data) { _tick=0; open_tick=0; sync_invalid=2; }
	virtual void init(void);
	virtual void sleep(void);
	virtual uint8_t open_door(void);    // 开门
	virtual uint8_t close_door(void);   // 关门
	//virtual uint16_t voltage(void);  // 获取电量, 0.01V
	virtual int16_t poll(void);
private:
	void packet(motor_t *_motor);
	uint8_t crc_calc(const uint8_t buf[], const uint8_t len);
	uint8_t crc_check(motor_t *_motor, const uint8_t crc);
	uint8_t recv(uint8_t buf[], const uint8_t len);
	uint8_t send(const uint8_t buf[], const uint8_t len);
	void send_ack(void);
	void send_cmd(const uint8_t cmd);
	uint16_t get_voltage(void); 
	

	//uint8_t _buf[64];
	//motor_t _buf;
	motor_t _temp;
	uint64_t _tick;
	uint64_t open_tick;
	uint8_t tmp_door;
	uint8_t sync_invalid;
};



#endif



