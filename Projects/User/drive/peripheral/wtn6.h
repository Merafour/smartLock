/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WTN6_H
#define __WTN6_H

#include "main.h"
#include "stdint.h"
#include <string.h>
#include "object.h"


#define WTN_CLK_GPIO_CLK      RCC_AHBPeriph_GPIOB
#define WTN_DATA_GPIO_CLK     RCC_AHBPeriph_GPIOB
#define WTN_POWER_GPIO_CLK    RCC_AHBPeriph_GPIOA
#define WTN_BUSY_GPIO_CLK     RCC_AHBPeriph_GPIOB

#define WTN_CLK_PORT   				GPIOB
#define WTN_CLK_BIT    				GPIO_Pin_1

#define WTN_DATA_PORT  				GPIOB
#define WTN_DATA_BIT   				GPIO_Pin_0

#define WTN_POWER_PORT  			GPIOA
#define WTN_POWER_BIT   			GPIO_Pin_5

#define WTN_BUSY_PORT  				GPIOB
#define WTN_BUSY_BIT   				GPIO_Pin_2

#define WTN_CLK_0    					WTN_CLK_PORT->BRR=WTN_CLK_BIT
#define WTN_CLK_1   					WTN_CLK_PORT->BSRR=WTN_CLK_BIT

#define WTN_DATA_0   					WTN_DATA_PORT->BRR=WTN_DATA_BIT
#define WTN_DATA_1   					WTN_DATA_PORT->BSRR=WTN_DATA_BIT

#define WTN_POWER_ON          WTN_POWER_PORT->BSRR=WTN_POWER_BIT
#define WTN_POWER_OFF         WTN_POWER_PORT->BRR=WTN_POWER_BIT

#define WTN_BUSY_READ         (WTN_BUSY_PORT->IDR & WTN_BUSY_BIT)

#define WTN_TIMx              TIM16

#define WTN_DELAY_PWR         5000
#define WTN_DELAY_PRE         500
#define WTN_DELAY_DATA        30
#define WTN_DELAY_BUSY        100

#define WTN_MAX_DATA_LEN      20

class Voice : public voice_obj {
public:
	virtual void init(void){Init();}
	virtual int8_t play_num(uint8_t volume,uint8_t speed,char *str_num){return Play_Num(volume, speed, str_num);}
	virtual int8_t play_voice(uint8_t volume,uint8_t speed,uint8_t num)
	{
		while(Read_Busy()) __nop();
		{
			return Play_voice(volume, speed, num);
		}
		//return -1;
	}
	virtual int8_t busy(void){return Read_Busy();}
	void Init(void);
	/*
		函数名:Play_Num
		功能:播放一串数据
		参数volume->音量大小 0-15
		参数speed->播放的间隔1-255ms (播放多段语音是该参数才有效)
		参数:num->数字
		返回值:0->正常播放 -1->忙碌 -2 数据长度超出预期 -3速度参数错误
	*/
	int8_t Play_Num(uint8_t volume,uint8_t speed,char *str_num);
	/*
		函数名:Play_voice
		功能:播放一串语音
	  参数volume->音量大小 0-15
		参数speed->播放的间隔1-255ms (播放多段语音是该参数才有效)
		参数:num->语音编号
		返回值:0->正常播放 -1->忙碌 -2 数据长度超出预期 -3速度参数错误
	*/
	int8_t Play_voice(uint8_t volume,uint8_t speed,uint8_t num);
	/*
		函数名:Read_Busy
		功能:读取语音状态机忙碌状态
		返回值:0->空闲 -1->忙碌
	*/
	int8_t Read_Busy(void);
	
private:
	
};


//extern Voice voice;

#endif



