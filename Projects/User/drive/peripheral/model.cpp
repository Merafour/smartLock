/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : model.cpp
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the model library.
* Description        : 电机马达接口,包含与小板的通信，获取电量.
******************************************************************************/
#include "model.h"
#include "delay.h"
#include "ssd1306syp.h"
#include "uart.h"

extern ssd1306syp display;

#define MODEL_USE_ACK  1
#undef  MODEL_USE_ACK

void motor_obj::init(void)
{
	uart_init();
//	while(1)
//	{
//		//USARTx_str("hello\r\n");
//		_delay_ms(100);
//	}
	//uart_test();
	send_cmd(MOTOR_CMD_SYNC);
	tmp_door = _door;
}
void motor_obj::sleep(void)
{
	motor_t _buf;
	packet(&_buf);
	_buf.u.cmd = MOTOR_CMD_SLEEP;
	_buf.u.len = sizeof(motor_t);
	_buf.u.crc = crc_calc(_buf.buf, _buf.u.len);
	if(send(_buf.buf, _buf.u.len) == _buf.u.len) 
	{
#ifndef MODEL_USE_ACK
#else
		_delay_ms(5);
		if(recv(_buf.buf, sizeof(motor_t)))
		{
			crc_check(&_buf, _buf.u.crc);
		}
#endif
	}
}
uint8_t motor_obj::open_door(void)
{
	motor_t _buf;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	open_tick = tick() + _sys.door*1000; // 10s
	//open_tick = tick() + 10000; // 10s
	_door = 0;//DOOR_OPEN|0x1; // open
	tmp_door = _door;
	sync_invalid = 5;
	//display.vformat(false, 0, 0, "门已开", 2000, "门已开!"); 
	packet(&_buf);
	_buf.u.cmd = MOTOR_CMD_OPEN;
	_buf.u.len = sizeof(motor_t);
	_buf.u.arg = _sys.speed;
	_buf.u.crc = crc_calc(_buf.buf, _buf.u.len);
	if(send(_buf.buf, _buf.u.len) == _buf.u.len) 
	{
#ifndef MODEL_USE_ACK
		return 0;
#else
		_delay_ms(5);
		if(recv(_buf.buf, sizeof(motor_t)))
		{
			//check crc
			if(0==crc_check(&_buf, _buf.u.crc))
			{
				if(MOTOR_CMD_ACK==_buf.u.cmd) return 0;
			}
		}
#endif
	}
	return 1;
}
uint8_t motor_obj::close_door(void)
{
	motor_t _buf;
	sys_data_t _sys;
	_data.get_sys(&_sys);
	_door = 0;//DOOR_CLOSE|0x1; // close
	tmp_door = _door;
	sync_invalid = 5;
	packet(&_buf);
	_buf.u.cmd = MOTOR_CMD_CLOSE;
	_buf.u.len = sizeof(motor_t);
	_buf.u.arg = _sys.speed;
	_buf.u.crc = crc_calc(_buf.buf, _buf.u.len);
	if(send(_buf.buf, _buf.u.len) == _buf.u.len) 
	{
#ifndef MODEL_USE_ACK
		return 0;
#else
		_delay_ms(5);
		if(recv(_buf.buf, sizeof(motor_t)))
		{
			//check crc
			if(0==crc_check(&_buf, _buf.u.crc))
			{
				if(MOTOR_CMD_ACK==_buf.u.cmd) return 0;
			}
		}
#endif
	}
	return 1;
}
uint16_t motor_obj::get_voltage(void)
{
	motor_t _buf;
	//return 3300; // 3.3V
	packet(&_buf);
	_buf.u.cmd = MOTOR_CMD_VOL;
	_buf.u.len = sizeof(motor_t);
	_buf.u.crc = crc_calc(_buf.buf, _buf.u.len);
	if(send(_buf.buf, _buf.u.len) == _buf.u.len) return _buf.u.len;
	return 0;
}
int16_t motor_obj::poll(void)
{
//	motor_t _motor;
	//return MODEL_NULL;
	if(_tick<tick())        // 获取电量
	{
		_tick = tick()+500; // 50 ms
		//get_voltage();
	}
	if((open_tick>0) && (open_tick<tick())) // 自动关门
	{
		// close
		close_door();
		//send_cmd(MOTOR_CMD_STOP);
		open_tick = 0;
	}
	if(recv(_temp.buf, sizeof(motor_t)))
	{
		//check crc
		if(0==crc_check(&_temp, _temp.u.crc))
		{
			switch(_temp.u.cmd)
			{
				case MOTOR_CMD_OPEN:
					break;
				case MOTOR_CMD_CLOSE:
					break;
//				case MOTOR_CMD_VOL:
//#if 1
//				{
//					uint16_t _vol = _buf.u.argL;
//					_voltage -= _voltage>>5;
//					_voltage += _vol>>5;
//				}
//#else
//				_voltage = _buf.u.argL;
//#endif
//					break;
				case MOTOR_CMD_SYNC:
					if(sync_invalid>0) sync_invalid--;
					if(sync_invalid>0) break;
					_voltage = _temp.us.volL | (_temp.us.volH<<8);
					//_door &= 0xF0;
					//_door |= _temp.us.door&0x0F;
					_door = _temp.us.door;
					break;
				default:
					break;
			}
		}
		memset(&_temp, 0, sizeof(motor_t));
	}
	if(tmp_door != _door)
	{
		tmp_door = _door;
		if((DOOR_OPEN|0x1) == tmp_door) return MODEL_OPEN_IN;
		if((DOOR_CLOSE|0x1) == tmp_door) return MODEL_CLOSE_IN;
		if(DOOR_OPEN == tmp_door) 
		{
			sys_data_t _sys;
			_data.get_sys(&_sys);
			open_tick = tick() + _sys.door*1000; // 10s
			return MODEL_OPEN;
		}
		if(DOOR_CLOSE == tmp_door) return MODEL_CLOSE;
	}
	return MODEL_NULL;
}

uint8_t motor_obj::recv(uint8_t buf[], const uint8_t len)
{
	return uart_recv(buf, len);
}

uint8_t motor_obj::send(const uint8_t buf[], const uint8_t len)
{
	return uart_send(buf, len);
}

void motor_obj::packet(motor_t *_motor)
{
	memset(_motor->buf, 0xFF, sizeof(motor_t));
	_motor->u.head = 0x5A;
}
uint8_t motor_obj::crc_calc(const uint8_t buf[], const uint8_t len)
{
	uint8_t crc = 0;
	uint8_t i=0;
	crc = 0;
	for(i=0; i<len; i++)
	{
		crc += buf[i];
	}
	//_motor->u.crc = crc;
	return crc;
}

uint8_t motor_obj::crc_check(motor_t *_motor, const uint8_t _crc)
{
	motor_t _m;
	memcpy(&_m, _motor, sizeof(motor_t));
	_m.u.crc = 0xFF;
	_m.u.crc = crc_calc(_m.buf, _m.u.len);
	if(_crc == _m.u.crc)
	{
		return 0;
	}
	return 1;
}

void motor_obj::send_ack(void)
{
	motor_t _motor;
	packet(&_motor);
	_motor.u.cmd = MOTOR_CMD_ACK;
	_motor.u.len = sizeof(motor_t);
	_motor.u.crc = crc_calc(_motor.buf, _motor.u.len);
	send(_motor.buf, _motor.u.len);
}
void motor_obj::send_cmd(const uint8_t cmd)
{
	motor_t _motor;
	packet(&_motor);
	_motor.u.cmd = cmd;
	_motor.u.len = sizeof(motor_t);
	_motor.u.crc = crc_calc(_motor.buf, _motor.u.len);
	send(_motor.buf, _motor.u.len);
}






