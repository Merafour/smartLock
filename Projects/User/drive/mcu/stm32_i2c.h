


#ifndef __I2C_H
#define __I2C_H


#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif



#define I2C1_SCL_PIN                  GPIO_Pin_8
#define I2C1_SCL_GPIO_PORT            GPIOB
#define I2C1_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define I2C1_SCL_SOURCE               GPIO_PinSource8
#define I2C1_SCL_AF                   GPIO_AF_1

#define I2C1_SDA_PIN                  GPIO_Pin_9
#define I2C1_SDA_GPIO_PORT            GPIOB
#define I2C1_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define I2C1_SDA_SOURCE               GPIO_PinSource9
#define I2C1_SDA_AF                   GPIO_AF_1

#define I2C2_SCL_PIN                  GPIO_Pin_6
#define I2C2_SCL_GPIO_PORT            GPIOF
#define I2C2_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define I2C2_SCL_SOURCE               GPIO_PinSource6
#define I2C2_SCL_AF                   GPIO_AF_1

#define I2C2_SDA_PIN                  GPIO_Pin_7
#define I2C2_SDA_GPIO_PORT            GPIOF
#define I2C2_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define I2C2_SDA_SOURCE               GPIO_PinSource7
#define I2C2_SDA_AF                   GPIO_AF_1



#define I2Cx_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2Cx_LONG_TIMEOUT         ((uint32_t)(10 * I2Cx_FLAG_TIMEOUT))

#define I2Cx_MAX_TRIALS_NUMBER     200

#define I2Cx_TIMING_90K          0xE0211010


typedef struct{
	uint16_t addr;	/* slave address			*/
	uint16_t flags;
#define I2C_M_RD						0x0001	/*¶ÁÊý¾Ý*/
#define I2C_M_STOP					0x8000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NOSTART				0x4000	/* if I2C_FUNC_NOSTART */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK		0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK			0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN			0x0400	/* length will be first received byte */
	uint16_t len;		/* msg length				*/
	uint8_t *buf;		/* pointer to msg data			*/
}I2C_Msg_Type;



void STM32_I2Cx_Init(I2C_TypeDef* I2Cx);
int8_t I2Cx_StandbyState(I2C_TypeDef* I2Cx,I2C_Msg_Type *msgs,__IO uint32_t Timeout);
int8_t I2C2_Transfer(I2C_TypeDef* I2Cx,I2C_Msg_Type *msgs,uint8_t num);

#ifdef __cplusplus
}
#endif
#endif



