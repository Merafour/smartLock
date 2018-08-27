/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : rgbw.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the MY9221 library.

******************************************************************************/
#include "rgbw.h"
#include <string.h>
#include "stm32f0xx.h"

#define MY_CLK_GPIO_CLK      RCC_AHBPeriph_GPIOB
#define MY_DATA_GPIO_CLK     RCC_AHBPeriph_GPIOB
#define MY_CLK_PORT   GPIOB
#define MY_CLK_BIT    GPIO_Pin_11
#define MY_DATA_PORT  GPIOB
#define MY_DATA_BIT   GPIO_Pin_10
#define MY_CLK_0    MY_CLK_PORT->BRR=MY_CLK_BIT
#define MY_CLK_1    MY_CLK_PORT->BSRR=MY_CLK_BIT
#define MY_DATA_0   MY_DATA_PORT->BRR=MY_DATA_BIT
#define MY_DATA_1   MY_DATA_PORT->BSRR=MY_DATA_BIT

static void Send_Command(void);
static void Send_Latch(void);
static void Delayus(uint8_t del);

void RGBW::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(MY_CLK_GPIO_CLK|MY_DATA_GPIO_CLK, ENABLE);
	/* Configure PC10 and PC11 in output pushpull mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = MY_CLK_BIT;
	GPIO_Init(MY_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MY_DATA_BIT;
	GPIO_Init(MY_DATA_PORT, &GPIO_InitStructure);
}

static void Send_Command(void)
{
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_1;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_1;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  
  MY_DATA_0;
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
}

static void Send_Latch(void)
{
	uint8_t i;
	MY_DATA_0;
	Delayus(253);
	Delayus(253);
	Delayus(253);
	Delayus(253);
	Delayus(253);
	Delayus(253);
	for(i=0;i<4;i++)
	{
		MY_DATA_1;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		MY_DATA_0;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}

}

static void Send_Data(uint8_t a)
{
  uint8_t i;
  for(i=0;i<8;i++)
  {
    MY_DATA_0;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  }
  for(i=0;i<8;i++)
  {
    if(a&0x80)
    {
      MY_DATA_1;
    }
    else
    {
      MY_DATA_0;
    }
    a<<=1;
    MY_CLK_PORT->ODR^=(uint16_t)MY_CLK_BIT;
  }
}

static void Delayus(uint8_t del)
{
  uint8_t i;
  for(i=0;i<del;i++)
  {
    __NOP();
		__NOP();
		__NOP();
		__NOP();
  }
}

void RGBW::Send(uint8_t *data)
{
	__disable_irq();
	MY_CLK_0;
	Delayus(200);
	
	Send_Command();
	
	Send_Data(data[2]);
	Send_Data(data[1]);
	Send_Data(data[0]);
	
	Send_Data(data[5]);
	Send_Data(data[4]);
	Send_Data(data[3]);
	
	Send_Data(data[6]);
	Send_Data(data[7]);
	Send_Data(data[8]);
	
	Send_Data(data[9]);
	Send_Data(data[10]);
	Send_Data(data[11]);
	
	
	Send_Latch();
	__enable_irq();
}
void RGBW::update()
{
	__disable_irq();
	MY_CLK_0;
	Delayus(200);
	
	Send_Command();
	Send_Data(data[2]);
	Send_Data(data[1]);
	Send_Data(data[0]);
	Send_Data(data[5]);
	Send_Data(data[4]);
	Send_Data(data[3]);
	Send_Data(data[6]);
	Send_Data(data[7]);
	Send_Data(data[8]);
	Send_Data(data[9]);
	Send_Data(data[10]);
	Send_Data(data[11]);
	Send_Latch();
	__enable_irq();
}

void RGBW::flowing()
{
	if(start && ms<tick()) 
	//if(ms<tick())
	{
		ms=tick()+500;
		flow();
	}
	else if((ms+2000)<tick())
	{
		start=1;
	}
}
void RGBW::flow()
{
	memset(data, 0, sizeof(data));
	data[_flowing] = bright;//0x0F;
	_flowing++;
	if(_flowing>=12) _flowing=0;
	update();
}
void RGBW::show(uint8_t key)
{
	start=0;
	memset(data, 0, sizeof(data));
	if(key<sizeof(data)) data[key] = bright;
	update();
}
void RGBW::multi(uint16_t mask)
{
	uint16_t i=0;
	start=0;
	memset(data, 0, sizeof(data));
	for(i=0; i<sizeof(data); i++)
	{
		if(mask&0x1) data[i] = bright;
		mask >>=1;
	}
	update();
}



