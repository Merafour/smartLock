#include "delay.h"
#include "stm32f0xx.h"

static uint8_t  fac_us=0;							//us延时倍乘数			   
//static uint16_t fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
static uint64_t uwTick=0;
static volatile uint16_t timeout_tick=0; // timeout timer
static uint16_t Seconds=0;
static uint32_t counter_time=0;
uint16_t backlight_delay=0;

//__weak void delay_tick(void)
//{
//	;//if(display_delay>0) display_delay--;
//}
//__weak void SecondsTick(void)
//{
//}
extern void dhcp_check(void);
__weak void dhcp_check(void)
{
	;
}
__weak void seconds_poll(void)
{
}
uint32_t GetSeconds(void)
{
	return counter_time;
}
void SetSeconds(uint32_t sec)
{
	counter_time = sec;
}
extern uint8_t uart_timeout;
//extern void delay_tick(void);
//systick中断服务函数,使用ucos时用到
//extern void timeout_Handler(void);
//extern void TimingDelay_Decrement(void);
void SysTick_Handler(void)
{	
	uwTick++;
	Seconds++;
	if(Seconds>=1000)
	{
		counter_time++;
		Seconds=0;
		dhcp_check();
		seconds_poll();
	}
	//if(timeout_tick<60000) timeout_tick++; // ms, 60000 ==> 60s
	if(timeout_tick>0) timeout_tick--; // ms
	if(backlight_delay>0) backlight_delay--; // ms
	if(uart_timeout>0) uart_timeout--;
	//delay_tick();
	//timeout_Handler();
	//TimingDelay_Decrement();
}
uint32_t waitting_time(const uint32_t ms)
{
	if(ms)
	{
		timeout_tick=ms;
		backlight_delay = ms+5000;
	}
	return timeout_tick;
}
//void set_timeout(const uint32_t ms)
//{
//	timeout_tick=ms;
//}
//volatile uint32_t get_timeout(void)
//{
//	return timeout_tick;
//}

uint64_t GetTick(void)
{
  return uwTick;
}

			   
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
	SysTick_Config(SystemCoreClock / 1000);
	fac_us=48;
	//fac_ms=(uint16_t)fac_us*1000;					//非OS下,代表每个ms需要的systick时钟数   
}								    

//延时nus
//nus为要延时的us数.
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的节拍数	  		 
	tcnt=0;
	//delay_osschedlock();						//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};
	//delay_osschedunlock();						//恢复OS调度									    
}
void delay_ms(uint16_t nms)
{
	delay_us((uint32_t)(nms*1000));					//普通方式延时  
}








































