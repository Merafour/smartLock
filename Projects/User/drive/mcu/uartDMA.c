/**
  ******************************************************************************
  * @file    USART/USART_8xUsartsOneBoard/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE       44
//#define USART_MAX_INDEX   1
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);

extern uint8_t aRxBuffer[BUFFER_SIZE];
__IO uint8_t RxCounter= 0, ReceiveState = 0;

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_8xUsartsOneBoard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "USART Example: 8xUsarts Tx/Rx Communication";
uint8_t aRxBuffer[BUFFER_SIZE];
/* Buffer used for reception */
USART_InitTypeDef USART_InitStructure;
__IO uint8_t TxCounter = 0;
extern __IO uint8_t ReceiveState;
//USART_TypeDef* UsartInstance[USART_MAX_INDEX] = {USART1, USART2, USART3, USART4, USART5, USART6, USART7, USART8};
//USART_TypeDef* UsartInstance[USART_MAX_INDEX] = {USART2};
uint8_t UsartIndex = 0;
/* Private function prototypes -----------------------------------------------*/
//static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);
//static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
unsigned short serial_count=0;
int main(void)
{
  uint8_t *buffptr;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f0xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f0xx.c file
  */ 
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* error */ 
    while (1);
  }
  /* Configure clock GPIO, USARTs */
  RCC_Configuration();
  
  /* Configure GPIO Pin Tx/Rx for Usart communication */
  GPIO_Configuration();
  
  /* Configure NVIC */
  NVIC_Configuration();
  
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
  //for(UsartIndex = 0; UsartIndex < USART_MAX_INDEX; UsartIndex++)
//  {
//    USART_Init(USART2, &USART_InitStructure);
//    
//    /* Enable 8xUSARTs Receive interrupts */
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    
//    /* Enable the 8xUSARTs */
//    USART_Cmd(USART2, ENABLE);
//  }
    USART_Init(USART2, &USART_InitStructure);
    /* Enable 8xUSARTs Receive interrupts */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /* Enable the 8xUSARTs */
    USART_Cmd(USART2, ENABLE);
  memset(aRxBuffer, 0x0, sizeof(aRxBuffer));
  /* Infinite Loop */
  while(1)
  {
    /* Set aRxBuffer to 0 */
    //memset(aRxBuffer, 0x0, sizeof(aRxBuffer));
#if 0    
    /* When data has been transferred start the next transfer */
    for(UsartIndex = 0; UsartIndex < USART_MAX_INDEX; UsartIndex++)
    { 
      /* Initialize the global variable to handle the data transfer */
      TxCounter = 0;
      ReceiveState = 0;
      
      /* Delay to let time to see counter incrementation on the screen */
      //Delay(2000);
      /* Set the data buffptr to use for the transfer */
      if (UsartIndex == 0)
      {
        buffptr = aTxBuffer;
      }
      else
      {
        buffptr = aRxBuffer;
      }
      
      while(TxCounter < BUFFER_SIZE)
      {   
        /* Send one byte from USART1 to USARTx */
        USART_SendData(UsartInstance[UsartIndex], buffptr[TxCounter++]);
        
        /* Loop until USART1 DR register is empty */ 
        while(USART_GetFlagStatus(UsartInstance[UsartIndex], USART_FLAG_TXE) == RESET)
        {}
      }
      
      //while(ReceiveState == 0);
	  while(RxCounter == 0);
      /* Compares two buffers */
	  TxCounter = 0;
		while(1) // (serial_count<u16CNT)
		{
			delay_ms(1);
			if(serial_count<RxCounter) serial_count=RxCounter;
			if(serial_count>(RxCounter+2)) break;
			else serial_count++;
		}
		serial_count=0;
      while(TxCounter < RxCounter)
      {   
        /* Send one byte from USART1 to USARTx */
        USART_SendData(USART2, aRxBuffer[TxCounter++]);
        
        /* Loop until USART1 DR register is empty */ 
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        {}
      }
	  memset(aRxBuffer, 0x0, sizeof(aRxBuffer));
	  RxCounter=0;
      ReceiveState = 0;
    }
#else
	if(RxCounter)
	{
		while(1) // (serial_count<u16CNT)
		{
			delay_ms(1);
			if(serial_count<RxCounter) serial_count=RxCounter;
			if(serial_count>(RxCounter+2)) break;
			else serial_count++;
		}
		serial_count=0;
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
#endif
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
//void TimingDelay_Decrement(void)
//{
//  if (TimingDelay != 0x00)
//  { 
//    TimingDelay--;
//  }
//}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{   
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Enable 8xUSARTs Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6|RCC_APB2Periph_USART7|RCC_APB2Periph_USART8|RCC_APB2Periph_USART1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);  

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
  
  /* USART3 Pins configuration  ***********************************************/     
  
  /* USART5 Pins configuration*************************************************/  
  
  /* USART6 Pins configuration*************************************************/  
  
  /* USART7 Pins configuration  *************************************************/  
    
  /* USART8 Pins configuration  *************************************************/  
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* USART1 IRQ Channel configuration */  
  /* USART2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
//static void Delay(__IO uint32_t nTime)
//{ 
//  TimingDelay = nTime;

//  while(TimingDelay != 0);
//}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *        >0 : pBuffer1 differs from pBuffer2
  */
//static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    if ((*pBuffer1) != *pBuffer2)
//    {
//      return BufferLength;
//    }
//    pBuffer1++;
//    pBuffer2++;
//  }
//  return 0;
//}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    aRxBuffer[RxCounter++] = USART_ReceiveData(USART2);
	  
	  if(RxCounter>=BUFFER_SIZE) RxCounter=BUFFER_SIZE-1;

//    if(RxCounter == BUFFER_SIZE)
//    {
//      ReceiveState = 1;
//      RxCounter=0;
//    }
  }
  USART_ClearITPendingBit(USART2, USART_IT_ORE);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
