/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : uart.h
* Author             : Merafour
* Last Modified Date : 04/28/2018
* Description        : This file provides the uart library.

******************************************************************************/

#include "main.h"
#include <string.h>
#include "uart.h"

#define BUFFER_SIZE       44

static uint8_t RxCounter= 0;
//static uint8_t TxCounter = 0;
/* Buffer used for transmission */
//static uint8_t aTxBuffer[BUFFER_SIZE] = "USART Example: 8xUsarts Tx/Rx Communication";
static uint8_t aRxBuffer[BUFFER_SIZE];
//uint16_t serial_count=0;
uint8_t uart_timeout;

void uart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* Configure clock GPIO, USARTs */
	//RCC_Configuration();
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Enable 8xUSARTs Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	/* Configure GPIO Pin Tx/Rx for Usart communication */
	//GPIO_Configuration();
	/* USART2 Pins configuration ************************************************/  
	/* Connect pin to Periph */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);    

	/* Configure pins as AF pushpull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	/* Configure NVIC */
	//NVIC_Configuration();
	/* USART2 IRQ Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 8xUSARTs configuration --------------------------------------------------*/
	/* 8xUSARTs  configured as follow:
	- BaudRate = 9600 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Prepare all uart to receive a data packet */
	USART_Init(USART2, &USART_InitStructure);
	/* Enable 8xUSARTs Receive interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* Enable the 8xUSARTs */
	USART_Cmd(USART2, ENABLE);
	memset(aRxBuffer, 0x0, sizeof(aRxBuffer));
}
static void EXTI2_3_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOE clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA3 and PA2 pins as input floating */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Connect EXTI3 Line to PA3 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);

  /* Connect EXTI2 Line to PA2 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

  /* Configure EXTI3 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure EXTI2 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI2_3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void uart_deinit(void)
{
	USART_Cmd(USART2, DISABLE);
	EXTI2_3_Config();
}

uint8_t uart_send(const uint8_t buf[], const uint8_t len)
{
	uint8_t i=0;
	for(i=0; i<len; i++)
	{
		/* Send one byte from USART1 to USARTx */
		USART_SendData(USART2, buf[i]);
		/* Loop until USART1 DR register is empty */ 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		{
			__nop();
		}
	}
	return i;
}
uint8_t uart_recv(uint8_t buf[], const uint8_t len)
{
	uint8_t i=0;
	if(0==RxCounter) return 0;
	while(uart_timeout) __nop();
	i=RxCounter;
	RxCounter = 0;
	if(i>len) i=len;
	memcpy(buf, aRxBuffer, i);
	memset(aRxBuffer, 0, sizeof(aRxBuffer));
	return i;
}

void uart_test(void)
{
	uint8_t TxCounter = 0;
	while(1)
	{
		if(RxCounter)
		{
//			while(1) // (serial_count<u16CNT)
//			{
//				delay_ms(1);
//				if(serial_count<RxCounter) serial_count=RxCounter;
//				if(serial_count>(RxCounter+2)) break;
//				else serial_count++;
//			}
			while(uart_timeout) __nop();
			//serial_count=0;
			TxCounter = 0;
			while(TxCounter < RxCounter)
			{   
				/* Send one byte from USART1 to USARTx */
				USART_SendData(USART2, aRxBuffer[TxCounter++]);
				/* Loop until USART1 DR register is empty */ 
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
				{}
			}
			RxCounter=0;
		}
	}
}


void USART2_IRQHandler(void)//解析收到的命令
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		aRxBuffer[RxCounter++] = USART_ReceiveData(USART2);
		if(RxCounter>=BUFFER_SIZE) RxCounter=BUFFER_SIZE-1;
		uart_timeout=2;
	}
	USART_ClearITPendingBit(USART2, USART_IT_ORE);
}



