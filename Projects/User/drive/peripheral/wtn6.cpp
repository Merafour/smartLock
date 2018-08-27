

#include "wtn6.h"
#include "delay.h"

__IO uint8_t wtn_bit_state;//位状态机标识
__IO uint8_t wtn_data_ptr;//数据指针
__IO uint8_t wtn_data[WTN_MAX_DATA_LEN];//数据内容
__IO uint8_t wtn_data_len;//数据长度
__IO uint16_t wtn_delay=0;

void Voice::Init(void)
{
	GPIO_InitTypeDef 				 GPIO_InitStructure;
	NVIC_InitTypeDef  			 NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_AHBPeriphClockCmd(WTN_CLK_GPIO_CLK | WTN_DATA_GPIO_CLK | WTN_POWER_GPIO_CLK | WTN_BUSY_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
	
	/* Configure PC10 and PC11 in output pushpull mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = WTN_CLK_BIT;
	GPIO_Init(WTN_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = WTN_DATA_BIT;
	GPIO_Init(WTN_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = WTN_POWER_BIT;
	GPIO_Init(WTN_POWER_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = WTN_BUSY_BIT;
	GPIO_Init(WTN_BUSY_PORT, &GPIO_InitStructure);
	
	TIM_DeInit(WTN_TIMx);
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/100000)-1; //10us
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //加
  TIM_TimeBaseStructure.TIM_Period = 5000;           //溢出值
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //时钟预分频
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                //重复计数次数
	
	TIM_TimeBaseInit(WTN_TIMx, &TIM_TimeBaseStructure);
	TIM_ClearFlag(WTN_TIMx,TIM_FLAG_Update);                       //清除标志位
	TIM_ITConfig(WTN_TIMx,TIM_IT_Update,ENABLE);                   //使能溢出中断
	

	/* USART IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel =TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	WTN_CLK_1;
}

int8_t Voice::Play_Num(uint8_t volume,uint8_t speed,char *str_num)
{
	uint8_t len,i;
	uint8_t num_tmp;
	if(WTN_TIMx->CR1&TIM_CR1_CEN)
	{
		return -1;
	}
	if(speed==0)
	{
		return -3;
	}
	wtn_bit_state=0;
	wtn_data_ptr=0;
	wtn_data_len=0;
	wtn_delay=speed*WTN_DELAY_BUSY;
	wtn_data[wtn_data_len++]=0xE0|(volume&0x0f);
	len=strlen(str_num);
	if(len>(WTN_MAX_DATA_LEN-1))
	{
		return -2;
	}
	for(i=0;i<len;i++)
	{
		if((str_num[i]>='0')&&(str_num[i]<='9'))
		{
			num_tmp=str_num[i]-'0';
			if(num_tmp==0)
			{
				wtn_data[wtn_data_len]=23;
			}
			else
			{
				wtn_data[wtn_data_len]=num_tmp+13;
			}
			wtn_data_len++;
		}
	}
	WTN_POWER_ON;
	WTN_CLK_1;
	WTN_TIMx->CNT=0;
	WTN_TIMx->ARR=WTN_DELAY_PWR;
	WTN_TIMx->CR1 |= TIM_CR1_CEN;
	return 0;
}

int8_t Voice::Play_voice(uint8_t volume,uint8_t speed,uint8_t num)
{
	if(WTN_TIMx->CR1&TIM_CR1_CEN)
	{
		return -1;
	}
	if(speed==0)
	{
		return -3;
	}
	wtn_bit_state=0;
	wtn_data_ptr=0;
	wtn_data_len=2;
	wtn_delay=speed*WTN_DELAY_BUSY;
	//wtn_data[0]=volume;
	wtn_data[0]=0xE0|(volume&0x0f);
	wtn_data[1]=num;
	WTN_POWER_ON;
	WTN_CLK_1;
	WTN_TIMx->CNT=0;
	WTN_TIMx->ARR=WTN_DELAY_PWR;
	WTN_TIMx->CR1 |= TIM_CR1_CEN;
	return 0;
}

int8_t Voice::Read_Busy(void)
{
	if(WTN_TIMx->CR1&TIM_CR1_CEN)
	{
		return -1;
	}
	return 0;
}

//uint8_t i;
//	while(!WTN_BUSY_READ);
//	WTN_CLK_0;
//	delay_ms(5); //延时 5ms
//	for(i=0;i<8;i++)
//	{
//		if(data&0x01)
//		{
//			WTN_DATA_1;
//		}
//		else
//		{
//			WTN_DATA_0;
//		}
//		WTN_CLK_0;
//		delay_us(300); //延时 300us
//		WTN_CLK_1; //拉高
//		delay_us(300); //延时 300us
//		data>>=1;
//	}
//	WTN_CLK_1;
extern "C" void TIM16_IRQHandler(void);

void TIM16_IRQHandler(void)
{
	static uint8_t bit=0;
	switch(wtn_bit_state)
	{
		case 0:
			WTN_CLK_0;
			WTN_TIMx->ARR=WTN_DELAY_PRE;//5ms延时
			bit=0x01;
			if(wtn_data[wtn_data_ptr]&bit)
			{
				WTN_DATA_1;
			}
			else
			{
				WTN_DATA_0;
			}
			wtn_bit_state++;
			break;
		case 1: //锁存 1bit
		case 3: //锁存 2bit
		case 5:	//锁存 3bit
		case 7: //锁存 4bit
		case 9:	//锁存 5bit
		case 11://锁存 6bit
		case 13://锁存 7bit
		case 15://锁存 8bit		
			WTN_CLK_1;
			WTN_TIMx->ARR=WTN_DELAY_DATA;//300us延时
			wtn_bit_state++;
			break;
		case 2: //数据准备 2bit
		case 4: //数据准备 3bit
		case 6: //数据准备 4bit
		case 8: //数据准备 5bit
		case 10://数据准备 6bit
		case 12://数据准备 7bit
		case 14://数据准备 8bit
			WTN_CLK_0;
			bit<<=1;
			if(wtn_data[wtn_data_ptr]&bit)
			{
				WTN_DATA_1;
			}
			else
			{
				WTN_DATA_0;
			}
			WTN_TIMx->ARR=WTN_DELAY_DATA;//300us延时
			wtn_bit_state++;
			break;
		case 16:
			WTN_CLK_0;
			WTN_TIMx->ARR=WTN_DELAY_DATA;//300us延时
			wtn_bit_state++;
			break;
		case 17://等待忙碌执行完毕
			if(WTN_BUSY_READ)
			{
				WTN_TIMx->ARR=wtn_delay;//
				if(wtn_data_ptr<(wtn_data_len-1))
				{
					wtn_data_ptr++;
				}
				else
				{
					WTN_TIMx->CR1 &= ~TIM_CR1_CEN;
					WTN_POWER_OFF;
				}
				wtn_bit_state=0;
			}
			break;
		default:
			WTN_TIMx->CR1 &= ~TIM_CR1_CEN;
			break;
	}
	
	WTN_TIMx->SR = (uint16_t)~TIM_IT_Update;
}

