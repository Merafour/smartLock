
#include "stm32_i2c.h"

void STM32_I2Cx_Init(I2C_TypeDef* I2Cx)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	/* GPIO configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	if(I2Cx==I2C1)
	{
		RCC_AHBPeriphClockCmd(I2C1_SCL_GPIO_CLK | I2C1_SDA_GPIO_CLK, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		GPIO_PinAFConfig(I2C1_SCL_GPIO_PORT, I2C1_SCL_SOURCE, I2C1_SCL_AF);
		GPIO_PinAFConfig(I2C1_SDA_GPIO_PORT, I2C1_SDA_SOURCE, I2C1_SDA_AF);

		GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
		GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
		GPIO_Init(I2C1_SDA_GPIO_PORT, &GPIO_InitStructure);
	}
	else if(I2Cx==I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);
		
		RCC_AHBPeriphClockCmd(I2C2_SCL_GPIO_CLK | I2C2_SDA_GPIO_CLK, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		
		GPIO_PinAFConfig(I2C2_SCL_GPIO_PORT, I2C2_SCL_SOURCE, I2C2_SCL_AF);
		GPIO_PinAFConfig(I2C2_SDA_GPIO_PORT, I2C2_SDA_SOURCE, I2C2_SDA_AF);
		
		GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
		GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
		GPIO_Init(I2C2_SDA_GPIO_PORT, &GPIO_InitStructure);
	}
	else
	{
		return ;
	}
	/* I2C configuration */
	/* sEE_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = I2Cx_TIMING_90K;

	/* Apply sEE_I2C configuration after enabling it */
	I2C_Init(I2Cx, &I2C_InitStructure);

	/* sEE_I2C Peripheral Enable */
	I2C_Cmd(I2Cx, ENABLE);
}

int8_t I2Cx_StandbyState(I2C_TypeDef* I2Cx,I2C_Msg_Type *msgs,__IO uint32_t Timeout)
{
	__IO uint32_t  Trials = 0;
  I2C_TransferHandling(I2Cx, msgs->addr, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);  
	do
  {
    /* Clear NACKF */
    I2C_ClearFlag(I2Cx, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
    /* Generate start */
    I2C_GenerateSTART(I2Cx, ENABLE);
		
		/* Wait until timeout elapsed */
    while (Timeout-- != 0);

    /* Check if the maximum allowed number of trials has bee reached */
    if(Trials++ == I2Cx_MAX_TRIALS_NUMBER)
    {
      return -1;
    }
  }
	while(I2C_GetFlagStatus(I2Cx, I2C_ISR_NACKF) != RESET);
	
	/* Clear STOPF */
  I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);

  /* Return sEE_OK if device is ready */
  return 0;
}



int8_t I2C2_Transfer(I2C_TypeDef* I2Cx,I2C_Msg_Type *msgs,uint8_t num)
{
	__IO uint32_t  Timeout;
	uint16_t			 num_send;
	
	while(num--)
	{
		if(msgs->flags&I2C_M_RD)//¶ÁÊý¾Ý
		{
			I2C_TransferHandling(I2Cx, msgs->addr, msgs->len, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

			num_send=0;
			while(num_send<msgs->len)
			{
				 /* Wait until RXNE flag is set */
				Timeout = I2Cx_LONG_TIMEOUT;
				while(I2C_GetFlagStatus(I2Cx, I2C_ISR_RXNE) == RESET)
				{
					if((Timeout--) == 0){
						STM32_I2Cx_Init(I2Cx);
						return -1;
					}
				}
				msgs->buf[num_send]=I2C_ReceiveData(I2Cx);
				num_send++;
			}
			Timeout = I2Cx_LONG_TIMEOUT;
			while(I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET)
			{
				if((Timeout--) == 0){
					STM32_I2Cx_Init(I2Cx);
					return -1;
				}
			}   
			/* Clear STOPF flag */
			I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);
		}
		else
		{
			I2C_TransferHandling(I2Cx, msgs->addr, 255, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
			/* Wait until TXIS flag is set */
			Timeout = I2Cx_LONG_TIMEOUT;
			while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET)
			{
				if((Timeout--) == 0){
					STM32_I2Cx_Init(I2Cx);
					return -1;
				}
			}
			num_send=0;
			while(num_send<msgs->len)
			{
				I2C_SendData(I2Cx, (uint8_t)msgs->buf[num_send]);
				Timeout = I2Cx_LONG_TIMEOUT;
				while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TXE) == RESET)
				{
					if((Timeout--) == 0){
						STM32_I2Cx_Init(I2Cx);
						return -1;
					}
				}
				num_send++;
			}
			/* Configure slave address, end mode */
      I2C_TransferHandling(I2Cx, msgs->addr, 0, I2C_AutoEnd_Mode, I2C_Generate_Stop); 
		}
		msgs++;
	}
	return 0;
}






