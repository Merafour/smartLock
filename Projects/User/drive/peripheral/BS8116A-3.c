
#include "bs8116a-3.h"
#include "stm32_i2c.h"



void Bs8116a_3_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
static __IO uint16_t key=0;
uint16_t Key_value(void)
{
	return key;
}

void Key_Read(void)
{
	int8_t reply;
	I2C_Msg_Type i2c_msg[2];
	uint8_t read_add;
	uint16_t read_data;

	i2c_msg[0].addr=TOUCH_I2C_ADD;
	reply=I2Cx_StandbyState(I2C2,i2c_msg,0x300);
	if(reply==0)
	{
		i2c_msg[0].flags=0;
		i2c_msg[0].len=1;
		i2c_msg[0].buf=&read_add;
		read_add=0x08;
		
		i2c_msg[1].addr=TOUCH_I2C_ADD;
		i2c_msg[1].flags=I2C_M_RD;
		i2c_msg[1].len=2;
		i2c_msg[1].buf=(uint8_t *)&read_data;
		I2C2_Transfer(I2C2,i2c_msg,2);
		key=read_data;
	}
}






































