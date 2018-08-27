/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.c 
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
#include "RTC.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
//#define RTC_CLOCK_SOURCE_LSI  // LSI used as RTC source clock. The RTC Clock
                                // may varies due to LSI frequency dispersion

#define BKP_VALUE    0x32F0 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t AsynchPrediv = 0, SynchPrediv = 0;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
/* Private functions ---------------------------------------------------------*/

int CaculateWeekDay(int year,int month, int day)
{
	if(month==1||month==2) {
			month+=12;
			year--;
	}
	return ((day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7);
} 

uint8_t convert_month(char *str)
{
	if(strcmp(str,"Jan")==0)
		return 1;
	else if(strcmp(str,"Feb")==0)
		return 2;
	else if(strcmp(str,"Mar")==0)
		return 3;
	else if(strcmp(str,"Apr")==0)
		return 4;
	else if(strcmp(str,"May")==0)
		return 5;
	else if(strcmp(str,"Jun")==0)
		return 6;
	else if(strcmp(str,"Jul")==0)
		return 7;
	else if(strcmp(str,"Aug")==0)
		return 8;
	else if(strcmp(str,"Sep")==0)
		return 9;
	else if(strcmp(str,"Oct")==0)
		return 10;
	else if(strcmp(str,"Nov")==0)
		return 11;
	else if(strcmp(str,"Dec")==0)
		return 12;
	else
		return 0;
}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void sync_rtc_time(void)
{
	int year;  // 2018
	char month[8];  // 3   
	int day;    // 25
	int hour;   // 21     
	int minute; // 44
	int second; // 10 
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	
	// 2018.3.25, 21:44:10
	sscanf(__DATE__, "%s %d %d", month, &day, &year);  
	sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second); 
	Time.RTC_Hours = hour;
	Time.RTC_Minutes = minute+1;
	Time.RTC_Seconds = second;
	Date.RTC_Year = year-2000;
	//Date.RTC_Month = 4;
	Date.RTC_Month =convert_month(month);
	Date.RTC_WeekDay =CaculateWeekDay(year,Date.RTC_Month,day); 
	Date.RTC_Date = day;
	RTC_SetTime(RTC_Format_BIN, &Time);
	RTC_SetDate(RTC_Format_BIN, &Date);
}
void RTC_Timer_Init(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 

  RTC_InitTypeDef RTC_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  //EXTI_InitTypeDef  EXTI_InitStructure;
 
  /* Output a message on Hyperterminal using printf function */
  //printf("\n\r  *********************** RTC Hardware Calendar Example ***********************\n\r");
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
  {  
    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
      //printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    }

    /* Configure the time register */
    //RTC_TimeRegulate(); 
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      //---printf("\r\n Power On Reset occurred....\n\r");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      //---printf("\r\n External Reset occurred....\n\r");
    }

    //---printf("\n\r No need to configure RTC....\n\r");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

 #ifdef RTC_CLOCK_SOURCE_LSI
    /* Enable the LSI OSC */ 
    RCC_LSICmd(ENABLE);
 #endif /* RTC_CLOCK_SOURCE_LSI */

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Display the RTC Time and Alarm */
    RTC_TimeShow();
    RTC_AlarmShow();
  }
   
  /* Configure the external interrupt "KEY", "SEL" and "UP" buttons */
//  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_EXTI); 
//  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
//  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);

  /* Configure LEDs */
//  STM_EVAL_LEDInit(LED1);
//  STM_EVAL_LEDInit(LED2);

  /* Turn LED2 ON */
//  STM_EVAL_LEDOn(LED2);

  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
//  EXTI_ClearITPendingBit(EXTI_Line17);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Infinite loop */
//  while (1)
//  {
//  }
	//sync_rtc_time(); 
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  //printf("\n\r  The current time is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  /* Get the current Alarm */
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  //printf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    //STM_EVAL_LEDToggle(LED1);
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    //EXTI_ClearITPendingBit(EXTI_Line17);
  } 
}

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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
