/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : safety.h
* Author             : Merafour
* Last Modified Date : 04/23/2018
* Description        : This file provides the safety library.
* Description        : 安全设备接口.
******************************************************************************/
#ifndef __SAFETY_H__
#define __SAFETY_H__

#include <stdint.h>



#define EVENT_RFID      0x01
#define EVENT_FINGER    0x02
#define EVENT_KEY       0x04
#define SAFETY_OK       0
#define SAFETY_TIMEOUT  -30

class safety {
private:
	static uint16_t event;
protected:
	/**
	  获取系统时间,单位:ms,在 main.cpp中实现
     */
	uint64_t tick(void);
	/**
	  延时函数,单位:ms,在 main.cpp中实现
     */
	void _delay_ms(uint16_t nms);
public:
    safety() {}
	/**
	  事件操作函数:读,写,清除
     */
	static uint16_t revent(void);
	static void wevent(const uint16_t mask);
	static void cevent(const uint16_t mask);
	
	/** 初始化接口 */
	virtual void init(void) = 0;
		/** 重启接口,用于出错时重新初始化设备,在初始化前需要做好资源的释放操作 */
	virtual void reset(void) = 0;
	/**
		添加函数,添加成功时通过 id返回标识
		返回值：
		0：成功, 负值:出错
     */
	virtual int8_t add(uint16_t &id, const uint16_t timeout) = 0;
	/**
		删除函数
		返回值：
		0：成功, 负值:出错
	 */
	virtual int8_t remove(const uint16_t id_start, const uint16_t id_end) = 0;
	/**
		校验函数,通过 id返回标识,该函数在达到 timeout(单位:ms)时间时必须返回
		返回值：
		0：成功, SAFETY_TIMEOUT:超时,  负值:出错
	 */
	virtual int8_t verify(uint16_t &id, const uint16_t timeout) = 0;
	/** 
		轮询函数,非阻塞方式轮询用户事件,有数据来时通过 id返回标识
		返回值：
		0：成功, 负值:出错
	 */
	virtual int8_t poll(uint16_t &id) = 0;
	/** 
		读函数,应用层通过事件操作查询数据到来,有数据来时通过 id读取标识
		返回值：
		0：成功, 负值:出错
	 */
	virtual int8_t read(uint16_t &id) = 0;
};

#endif // __SAFETY_H__

