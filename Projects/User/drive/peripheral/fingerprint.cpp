
#include "fingerprint.h"
#include <string.h>
#include "main.h"
#include "delay.h"
#include "ssd1306syp.h"


__align(4)
static __IO uint8_t qs808_tx_buf[QS808_MAX_BUF_SIZE];
__align(4) 
static __IO uint8_t qs808_rx_buf[QS808_MAX_BUF_SIZE];

static inline void QS808_Send_Data(uint16_t len);
static uint16_t QS808_Checksum(uint8_t *data,uint16_t len);
static inline QS808_Status QS808_Read_State(void);


__IO QS808_Status qs808_state=QS808_STATE_IDLE;

__weak uint8_t menutask(uint8_t state,uint8_t change)
{
	if(change)
	{
		switch(state)
		{
			case 0:
			case 2:
			case 4:	
				display.clear(false);
				display.format(0, 0, 2, 500, "Collect image,%d",state);
				break;
			case 1:
			case 3:
			case 5:
				display.clear(false);
				display.format(0, 0, 2, 500, "Move fingers,%d",state);
				break;
			case 6:
				display.clear(false);
				display.format(0, 0, 2, "Synthesis template,%d",state);
				break;
			case 7:
				display.clear(false);
				display.format(0, 0, 2, "Empty template,%d",state);
				break;
			case 8:
				display.clear(false);
				display.format(0, 0, 2, "write template,%d",state);
				break;
		}
	}
	return 0;
}
void FingerPrint::Test(void)
{
	int8_t ret;
	uint16_t tmp_num=0;
	
	delay_ms(2000);
	
	ret=Delete_Fingerprint(NULL,1,100);
	if(ret==0)
	{
		display.clear(false);
		display.format(0, 0, 2, 2000, "删除成功");
	}
	else
	{
		display.clear(false);
		display.format(0, 0, 2, 2000, "删除失败");
	}
	
	while(0)
	{
		display.clear(false);
		display.format(0, 0, 2, 2000, "fingerprint");
		
		
		ret=Fingerprint_Entry(menutask,(uint16_t *)&tmp_num);
		if(ret==0)
		{
			display.clear(false);
			display.format(0, 0, 2, 2000, "写入 %d 成功",tmp_num);
		}
		else
		{
			display.clear(false);
			display.format(0, 0, 2, 2000, "写入 %d 失败",(int8_t)0-ret);
		}
		display.clear(false);
		display.format(0, 0, 2, 2000, "开始校验");
		
		ret=Verify_Fingerprint(NULL,(uint16_t *)&tmp_num);
		if(ret==0)
		{
			display.clear(false);
			display.format(0, 0, 2, 2000, "校验成功");
		}
		else
		{
			display.clear(false);
			display.format(0, 0, 2, 2000, "校验失败");
		}
	}
}

void FingerPrint::init(void)
{
	QS808_Init();
}
int8_t FingerPrint::add(uint16_t &id, const uint16_t timeout)
{
	return Fingerprint_Entry(menutask, &id);
}
int8_t FingerPrint::remove(const uint16_t id_start, const uint16_t id_end)
{
	int8_t ret;
	//return Delete_Fingerprint(NULL, id_start, id_end);
	ret=Delete_Fingerprint(NULL,id_start,id_end);
	if(ret!=0) ret=Delete_Fingerprint(NULL,id_start,id_end);
	return ret;
}
int8_t FingerPrint::verify(uint16_t &id, const uint16_t timeout)
{
	return Verify_Fingerprint(NULL, &id);
}
int8_t FingerPrint::poll(uint16_t &id)
{
	return Verify_Fingerprint(NULL, &id);
}
void FingerPrint::QS808_Init(void)
{
	USART_InitTypeDef  			 USART_InitStructure;
	GPIO_InitTypeDef  			 GPIO_InitStructure;
	DMA_InitTypeDef 				 DMA_InitStructure;
	EXTI_InitTypeDef   			 EXTI_InitStructure;
	NVIC_InitTypeDef  			 NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_TIM17, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if 1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
#else	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Connect EXTI0 Line to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);

	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(QS808_USARTx,&USART_InitStructure);

	USART_DMACmd(QS808_USARTx,USART_DMAReq_Tx,ENABLE); 
	USART_DMACmd(QS808_USARTx,USART_DMAReq_Rx,ENABLE); 

	USART_ITConfig(QS808_USARTx, USART_IT_IDLE, ENABLE);
	/* USART IRQ Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Common DMA configuration */
	DMA_InitStructure.DMA_BufferSize =QS808_MAX_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*DMA for QS808_USARTx rx*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)qs808_rx_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(QS808_USARTx->RDR);
	DMA_Init(QS808_DMA_RX, &DMA_InitStructure);

	/*DMA for QS808_USARTx tx*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)qs808_tx_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(QS808_USARTx->TDR);
	DMA_Init(QS808_DMA_TX, &DMA_InitStructure);

	/* DMA remap for QS808_USARTx Tx Rx */
	DMA_RemapConfig(DMA1, DMA1_CH2_USART1_TX); 
	DMA_RemapConfig(DMA1, DMA1_CH3_USART1_RX); 


	TIM_DeInit(QS808_TIMx);
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000)-1; //1ms
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //加
	TIM_TimeBaseStructure.TIM_Period = QS808_CMD_TIMEOUT;           //溢出值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //时钟预分频
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                //重复计数次数

	TIM_TimeBaseInit(QS808_TIMx, &TIM_TimeBaseStructure);
	TIM_ClearFlag(QS808_TIMx,TIM_FLAG_Update);                       //清除标志位
	TIM_ITConfig(QS808_TIMx,TIM_IT_Update,ENABLE);                   //使能溢出中断


	/* USART IRQ Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel =TIM17_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(QS808_DMA_RX, ENABLE);
	USART_Cmd(QS808_USARTx,ENABLE);
}

static uint16_t QS808_Checksum(uint8_t *data,uint16_t len)
{
	uint16_t checksum=0;
	while(len--)
	{
		checksum+=*data;
		data++;
	}
	return checksum;
}

static inline void QS808_Send_Data(uint16_t len)
{
	DMA_SetCurrDataCounter(QS808_DMA_TX,len);
	DMA_Cmd(QS808_DMA_TX, ENABLE);
	QS808_TIMx->CNT=0;
	TIM_Cmd(QS808_TIMx,ENABLE);
}

inline QS808_Status QS808_Read_State(void)
{
	return qs808_state;
}


/*
函数名:QS808_Test_Connection
功能:QS808发送测试连接状态命令
参数:无
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Test_Connection(void)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_TEST_CONNECTION;
		cmd_buf->len=0;
		memset(cmd_buf->data,0,16);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Set_Parameter
功能:QS808设置参数
参数type:0->ID
				 1->安全等级
				 2->指纹重复检测
				 3->波特率
				 4->指纹自学习
				 5->采集指纹超时时间
参数val:	ID                表示本设备编号。可设置 1 ~ 255 
					Security_Level    表示安全等级。可设置值：1~5 。默认为：3
					Duplication_Check 指纹重复检查状态开/关。可设置 0 或 1
					Baudrate          1:9600bps, 2:19200bps, 3:38400bps, 4:57600bps, 5:115200bps 6:230400bps, 7:460800bps, 8:921600bps 默认5
					Auto_Learn        表示指纹模板自学习状态开/关。可设置 0 或 1
					Fp TimeOut        表示采集指纹超时时间，可设置值：1 秒至 60 秒
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Set_Parameter(QS808_Parameter type,uint8_t val)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_SET_PARAM;
		cmd_buf->len=5;
		cmd_buf->data[0]=type;
		cmd_buf->data[1]=val;
		memset(&cmd_buf->data[2],0,14);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Get_Parameter
功能:QS808获取参数
参数:0->ID
		 1->安全等级
		 2->指纹重复检测
		 3->波特率
		 4->指纹自学习
		 5->采集指纹超时时间
状态参数: 1 byte
ID                表示本设备编号。可设置 1 ~ 255 
Security_Level    表示安全等级。可设置值：1~5 。默认为：3
Duplication_Check 指纹重复检查状态开/关。可设置 0 或 1
Baudrate          1:9600bps, 2:19200bps, 3:38400bps, 4:57600bps, 5:115200bps 6:230400bps, 7:460800bps, 8:921600bps 默认5
Auto_Learn        表示指纹模板自学习状态开/关。可设置 0 或 1
Fp TimeOut        表示采集指纹超时时间，可设置值：1 秒至 60 秒
*/
QS808_Status FingerPrint::QS808_Get_Parameter(QS808_Parameter type)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_PARAM;
		cmd_buf->len=1;
		cmd_buf->data[0]=type;
		memset(&cmd_buf->data[cmd_buf->len],0,16-cmd_buf->len);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

///*
//函数名:QS808_Get_Device_Info
//功能:QS808获取设备信息
//返回值:
//*/
//QS808_Status FingerPrint::QS808_Get_Device_Info(void)
//{
//	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
//	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
//	{
//		DMA_Cmd(QS808_DMA_TX, DISABLE);
//		cmd_buf->prefix=CMD_TX_PREFIX;
//		cmd_buf->sid=0;
//		cmd_buf->did=0;
//		cmd_buf->cmd=CMD_GET_DEVICE_INFO;
//		cmd_buf->len=0;
//		memset(cmd_buf->data,0,16);
//		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
//		QS808_Send_Data(sizeof(QS808_CMD_Type));
//		qs808_state=QS808_STATE_BUSY;
//		return QS808_STATE_OK;
//	}
//	return QS808_STATE_FAIL;
//}

/*
函数名:QS808_Get_Image
功能:QS808获取图像
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Get_Image(void)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_IMAGE;
		cmd_buf->len=0;
		memset(cmd_buf->data,0,16);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Finger_Detect
功能:QS808检测手指
状态返回值:有指纹输入返回 QS808_STATE_OK，没有指纹输入返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Finger_Detect(void)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_FINGER_DETECT;
		cmd_buf->len=0;
		memset(cmd_buf->data,0,16);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Store_Char
功能:QS808 保存指纹模板数据到模块指纹库
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Store_Char(uint16_t tmp_num,uint16_t ram_num)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_STORE_CHAR;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_num;
		cmd_buf->data[1]=tmp_num>>8;
		cmd_buf->data[2]=ram_num;
		cmd_buf->data[3]=ram_num>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Load_Char
功能:读取指纹库中的指纹并暂存在指定的RamBuffer 中
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Load_Char(uint16_t tmp_num,uint16_t ram_num)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_LOAD_CHAR;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_num;
		cmd_buf->data[1]=tmp_num>>8;
		cmd_buf->data[2]=ram_num;
		cmd_buf->data[3]=ram_num>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Del_Char
功能:QS808删除指定编号范围内的指纹
状态返回值:成功返回 QS808_STATE_OK，失败返回 QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Del_Char(uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_DEL_CHAR;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_start;
		cmd_buf->data[1]=tmp_start>>8;
		cmd_buf->data[2]=tmp_end;
		cmd_buf->data[3]=tmp_end>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Get_Empty_Id
功能:QS808获取指定编号范围内可注册的首个编号 
状态返回参数:2 byte 可以注册的模板号
*/
QS808_Status FingerPrint::QS808_Get_Empty_Id(uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_EMPTY_ID;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_start;
		cmd_buf->data[1]=tmp_start>>8;
		cmd_buf->data[2]=tmp_end;
		cmd_buf->data[3]=tmp_end>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}



/*
函数名:QS808_Get_Status
功能:QS808 检查指定的编号是否已被注册
状态返回参数: 已经注册->QS808_STATE_OK，没有注册->QS808_STATE_FAIL
*/
QS808_Status FingerPrint::QS808_Get_Status(uint16_t tmp_num)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_STATUS;
		cmd_buf->len=2;
		cmd_buf->data[0]=tmp_num;
		cmd_buf->data[1]=tmp_num>>8;
		memset(&cmd_buf->data[2],0,14);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Get_Broken_Id
功能:QS808 检查指定编号范围内的指纹库是否有数据损坏
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Get_Broken_Id(uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_BROKEN_ID;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_start;
		cmd_buf->data[1]=tmp_start>>8;
		cmd_buf->data[2]=tmp_end;
		cmd_buf->data[3]=tmp_end>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Get_Enroll_Count
功能:QS808 获取指定编号范围内已注册的指纹总
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Get_Enroll_Count(uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GET_ENROLL_COUNT;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp_start;
		cmd_buf->data[1]=tmp_start>>8;
		cmd_buf->data[2]=tmp_end;
		cmd_buf->data[3]=tmp_end>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Generate
功能:QS808 从暂存在 ImageBuffer 中的指纹图像产生模板,存入编号为 ram_num 的Ram Buffer
参数:Ram Buffer 编号
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Generate(uint16_t ram_num)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_GENERATE;
		cmd_buf->len=2;
		cmd_buf->data[0]=ram_num;
		cmd_buf->data[1]=ram_num>>8;
		memset(&cmd_buf->data[2],0,14);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Merge
功能:QS808 合成指纹模板数据用于入库
参数ram_num->合成后存在ram Buffer的编号
参数count->2(合成ram Buffer 0和1) ，3(合成ram Buffer 0、1、2)

返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Merge(uint16_t ram_num,uint8_t count)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_MERGE;
		cmd_buf->len=3;
		cmd_buf->data[0]=ram_num;
		cmd_buf->data[1]=ram_num>>8;
		cmd_buf->data[2]=count;
		memset(&cmd_buf->data[3],0,13);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}



/*
函数名:QS808_Match
功能:QS808 指定2个RamBuffer 之间的模板做比对 
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Match(uint16_t ram_num1,uint16_t ram_num2)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_MATCH;
		cmd_buf->len=4;
		cmd_buf->data[0]=ram_num1;
		cmd_buf->data[1]=ram_num1>>8;
		cmd_buf->data[2]=ram_num2;
		cmd_buf->data[3]=ram_num2>>8;
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}


/*
函数名:QS808_Search
功能:QS808 指定编号范围的1:N识别
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Search(uint16_t ram_num,uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_SEARCH;
		cmd_buf->len=6;
		cmd_buf->data[0]=ram_num;
		cmd_buf->data[1]=ram_num>>8;
		cmd_buf->data[2]=tmp_start;
		cmd_buf->data[3]=tmp_start>>8;
		cmd_buf->data[4]=tmp_end;
		cmd_buf->data[5]=tmp_end>>8;
		
		memset(&cmd_buf->data[6],0,10);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}

/*
函数名:QS808_Verify
功能:QS808 指定RamBuffer 与指纹库中指定编号的模板比对
返回值:成功返回0，失败返回-1
*/
QS808_Status FingerPrint::QS808_Verify(uint16_t tmp,uint16_t ram_num)
{
	QS808_CMD_Type *cmd_buf=(QS808_CMD_Type *)qs808_tx_buf;
	if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))
	{
		DMA_Cmd(QS808_DMA_TX, DISABLE);
		cmd_buf->prefix=CMD_TX_PREFIX;
		cmd_buf->sid=0;
		cmd_buf->did=0;
		cmd_buf->cmd=CMD_VERIFY;
		cmd_buf->len=4;
		cmd_buf->data[0]=tmp;
		cmd_buf->data[1]=tmp>>8;
		cmd_buf->data[2]=ram_num;
		cmd_buf->data[3]=ram_num>>8;
		
		memset(&cmd_buf->data[4],0,12);
		cmd_buf->cks=QS808_Checksum((uint8_t *)cmd_buf,sizeof(QS808_CMD_Type)-2);
		QS808_Send_Data(sizeof(QS808_CMD_Type));
		qs808_state=QS808_STATE_BUSY;
		return QS808_STATE_OK;
	}
	return QS808_STATE_FAIL;
}





/*
函数名:QS808_Cmd_Response
功能:QS808命令返回结果
参数:无
返回值:连接正常返回0，连接失败返回-1
*/


QS808_Status FingerPrint::QS808_Cmd_Response(uint8_t *parameter)
{
	QS808_CMD_Reply_Type  *cmd_rx=(QS808_CMD_Reply_Type *)qs808_rx_buf;
	//QS808_DATA_Reply_Type *data_rx=(QS808_DATA_Reply_Type *)qs808_rx_buf;
	if(qs808_state==QS808_STATE_READY)
	{
		switch(cmd_rx->rcm)
		{
			case CMD_TEST_CONNECTION:
			case CMD_SET_PARAM:
			case CMD_GET_IMAGE:	
			case CMD_DEL_CHAR:	
			case CMD_GENERATE:	
			case CMD_MERGE:
			case CMD_MATCH:
				if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==2)&&(cmd_rx->ret==0x00))
				{
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_OK;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_FAIL;
			case CMD_GET_PARAM:
				if(parameter!=NULL)
				{
					if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==6))
					{
						parameter[0]=cmd_rx->data[0];
						qs808_state=QS808_STATE_IDLE;
						return QS808_STATE_OK;
					}
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_FAIL;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_PARAM;
//			case CMD_GET_DEVICE_INFO:
//				if(parameter!=NULL)
//				{
//					if(data_rx->ret==0x00)
//					{
//						memcpy(parameter,data_rx->data,data_rx->len);
//						qs808_state=QS808_STATE_IDLE;
//						return QS808_STATE_OK;
//					}
//					qs808_state=QS808_STATE_IDLE;
//					return QS808_STATE_FAIL;
//				}
//				qs808_state=QS808_STATE_IDLE;
//				return QS808_STATE_PARAM;
			case CMD_FINGER_DETECT:	
			case CMD_GET_STATUS:
				if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==3)&&(cmd_rx->data[0]))
				{
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_OK;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_FAIL;	
			case CMD_STORE_CHAR:
			case CMD_LOAD_CHAR:
				if((qs808_state==QS808_STATE_READY)&&(cmd_rx->ret==0x00))
				{
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_OK;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_FAIL;
			case CMD_GET_EMPTY_ID:
				if(parameter!=NULL)
				{
					if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==4))
					{
						parameter[0]=cmd_rx->data[0];
						parameter[1]=cmd_rx->data[1];
						qs808_state=QS808_STATE_IDLE;
						return QS808_STATE_OK;
					}
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_FAIL;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_PARAM;
			case CMD_GET_BROKEN_ID:	
				if(parameter!=NULL)
				{
					if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==6)&&(cmd_rx->ret==0x00))
					{
						parameter[0]=cmd_rx->data[0];//总破损数目
						parameter[1]=cmd_rx->data[1];
						parameter[2]=cmd_rx->data[2];//第一个破损编号
						parameter[3]=cmd_rx->data[3];
						qs808_state=QS808_STATE_IDLE;
						return QS808_STATE_OK;
					}
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_FAIL;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_PARAM;
			case CMD_GET_ENROLL_COUNT:
				if(parameter!=NULL)
				{
					if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==4))
					{
						parameter[0]=cmd_rx->data[0];//已经注册的模板个数
						parameter[1]=cmd_rx->data[1];
						qs808_state=QS808_STATE_IDLE;
						return QS808_STATE_OK;
					}
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_FAIL;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_PARAM;
			case CMD_SEARCH:
			case CMD_VERIFY:	
				if(parameter!=NULL)
				{
					if((qs808_state==QS808_STATE_READY)&&(cmd_rx->len==5))
					{
						parameter[0]=cmd_rx->data[0];//模板编号
						parameter[1]=cmd_rx->data[1];
						parameter[2]=cmd_rx->data[2];//智能更新结果
						qs808_state=QS808_STATE_IDLE;
						return QS808_STATE_OK;
					}
					qs808_state=QS808_STATE_IDLE;
					return QS808_STATE_FAIL;
				}
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_PARAM;
				
			default:
				qs808_state=QS808_STATE_IDLE;
				return QS808_STATE_OK;		
		}
	}
	else if(qs808_state==QS808_STATE_TIMEOUT)
	{
		qs808_state=QS808_STATE_IDLE;
		return QS808_STATE_TIMEOUT;
	}
	else
	{
		return qs808_state;
	}
}
/*
函数名:Fingerprint_Entry
功能:录入指纹
参数:menutask->菜单显示函数
     etmp_num->录入的模板号
返回值:
0->成功
-1->用户取消
-2->模板生成失败
-3->获取空的模块ID号失败
-4->写入模板库失败
-5->存入ram Buffer失败
*/
int8_t FingerPrint::Fingerprint_Entry(uint8_t (*menutask)(uint8_t state,uint8_t change),uint16_t *etmp_num)
{
	uint8_t       finger_state=0;
	/*
		finger_state=0 采集第一次图像失败
		finger_state=1 手指离开
		finger_state=2 采集第二次图像失败
		finger_state=3 手指离开
		finger_state=4 采集第三次图像失败
		finger_state=5 手指离开
		finger_state=6 合成模板
		finger_state=7 获取空的模板编号
		finger_state=8 写入模板库
	*/
	QS808_Status  state_ret;
	__IO uint8_t       i;
	for(i=0;i<3;)//获取三次图像
	{
		while(1)
		{
			QS808_Get_Image();
			while(1)
			{
				state_ret=QS808_Read_State();
				if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
				{
					break;
				}
				if(menutask(finger_state,0)){return -1;}
			}
			if(QS808_Cmd_Response(NULL)==QS808_STATE_OK)
			{
				break;
			}
		}
		QS808_Generate(i);
		while(1)
		{
			state_ret=QS808_Read_State();
			if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
			{
				break;
			}
			if(menutask(finger_state,0)){return -1;}
		}
		if(QS808_Cmd_Response(NULL)!=QS808_STATE_OK)
		{
			continue;
		}
		i++;
		finger_state++;
		if(menutask(finger_state,1)){return -1;}
		while(1)
		{
			QS808_Finger_Detect();//检测手指
			while(1)
			{
				state_ret=QS808_Read_State();
				if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
				{
					break;
				}
				if(menutask(finger_state,0)){return -1;}
			}
			if(QS808_Cmd_Response(NULL)==QS808_STATE_FAIL)
			{
				break;
			}
		}
		finger_state++;
		if(menutask(finger_state,1)){return -1;}
	}
	if(menutask(finger_state,1)){return -1;}
	while(1)
	{
		QS808_Merge(0,3);//合成模板
		while(1)
		{
			state_ret=QS808_Read_State();
			if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
			{
				break;
			}
			if(menutask(finger_state,0)){return -1;}
		}
		if(QS808_Cmd_Response(NULL)==QS808_STATE_OK)
		{
			break;
		}
		else
		{
			return -2;//模板生成失败
		}
	}
	finger_state++;
	if(menutask(finger_state,1)){return -1;}
	while(1)
	{
		QS808_Get_Empty_Id(1,100);//获取空的模板号
		while(1)
		{
			state_ret=QS808_Read_State();
			if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
			{
				break;
			}
			if(menutask(finger_state,0)){return -1;}
		}
		if(QS808_Cmd_Response((uint8_t *)etmp_num)==QS808_STATE_OK)
		{
			break;
		}
		else
		{
			return -3;//获取空的模块ID号失败
		}
	}
	finger_state++;
	if(menutask(finger_state,1)){return -1;}
	while(1)
	{
		QS808_Store_Char(*etmp_num,0);
		while(1)
		{
			state_ret=QS808_Read_State();
			if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
			{
				break;
			}
			if(menutask(finger_state,0)){return -1;}
		}
		if(QS808_Cmd_Response((uint8_t *)&etmp_num)==QS808_STATE_OK)
		{
			return 0;
		}
		else
		{
			return -4;//写入模板库失败
		}
	}
}

/*
函数名:Delete_Fingerprint
功能:删除指纹
参数:menutask->菜单显示函数
     tmp_start->开始模板号
		 tmp_end->结束模板号
*/
int8_t FingerPrint::Delete_Fingerprint(uint8_t (*menutask)(void),uint16_t tmp_start,uint16_t tmp_end)
{
	QS808_Status  state_ret;
	QS808_Del_Char(tmp_start,tmp_end);
	while(1)
	{
		state_ret=QS808_Read_State();
		if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
		{
			break;
		}
		if(menutask!=NULL)
		{
			if(menutask()){return -1;}
		}
	}
	if(QS808_Cmd_Response(NULL)==QS808_STATE_OK)
	{
		return 0;
	}
	else
	{
		return -1;//删除失败
	}
}



/*
函数名:Verify_Fingerprint
功能:验证指纹
参数:menutask->菜单显示函数
     tmp_num
*/
int8_t FingerPrint::Verify_Fingerprint(uint8_t (*menutask)(void),uint16_t *tmp_num)
{
	QS808_Status  state_ret;
	uint8_t tmp_buf[3];
	while(1)
	{
		while(1)
		{
			QS808_Get_Image();
			while(1)
			{
				state_ret=QS808_Read_State();
				if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
				{
					break;
				}
				if(menutask!=NULL)
				{
					if(menutask()){return -1;}
				}
			}
			if(QS808_Cmd_Response(NULL)==QS808_STATE_OK)
			{
				break;
			}
		}
		QS808_Generate(0);
		while(1)
		{
			state_ret=QS808_Read_State();
			if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
			{
				break;
			}
			if(menutask!=NULL)
			{
				if(menutask()){return -1;}
			}
		}
		if(QS808_Cmd_Response(NULL)==QS808_STATE_OK)
		{
			break;
		}
	}	
	QS808_Search(0,1,100);
	while(1)
	{
		state_ret=QS808_Read_State();
		if((state_ret==QS808_STATE_TIMEOUT)||(state_ret==QS808_STATE_READY))
		{
			break;
		}
		if(menutask!=NULL)
		{
			if(menutask()){return -1;}
		}
	}
	if(QS808_Cmd_Response(tmp_buf)==QS808_STATE_OK)
	{
		*tmp_num=(tmp_buf[1]<<8)|tmp_buf[0];
		return 0;
	}
	else
	{
		return -1;//没有找到
	}
}

extern "C" void USART1_IRQHandler(void);
extern "C" void TIM17_IRQHandler(void);
extern "C" void QS808_IRQ(void);

void USART1_IRQHandler(void)//解析收到的命令
{
	uint16_t rx_len=0;
	uint16_t checksum[2];
	QS808_CMD_Type       *cmd_tx=(QS808_CMD_Type *)qs808_tx_buf;
	QS808_CMD_Reply_Type *cmd_rx=(QS808_CMD_Reply_Type *)qs808_rx_buf;
	
	if(USART_GetITStatus(QS808_USARTx,USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(QS808_DMA_RX, DISABLE);
		rx_len=QS808_MAX_BUF_SIZE-QS808_DMA_RX->CNDTR;
		if(rx_len>=sizeof(QS808_CMD_Reply_Type))
		{
			checksum[0]=QS808_Checksum((uint8_t *)qs808_rx_buf,rx_len-2);
			checksum[1]=(qs808_rx_buf[rx_len-1]<<8)|qs808_rx_buf[rx_len-2];
			if((checksum[0]==checksum[1])&&(qs808_state==QS808_STATE_BUSY)&&(cmd_rx->prefix==CMD_RX_PREFIX)&&(cmd_tx->cmd==cmd_rx->rcm))//收到发送的命令回复
			{
				qs808_state=QS808_STATE_READY;
			}
			else
			{
				__NOP();
			}
//			else if((qs808_state==QS808_STATE_IDLE)||(qs808_state==QS808_STATE_READY))//收到未知的回复
//			{
//				__NOP();
//			}
//			else
//			{
//				__NOP();
//			}
		}
		QS808_DMA_RX->CNDTR=QS808_MAX_BUF_SIZE;
		DMA_Cmd(QS808_DMA_RX, ENABLE);
		USART_ClearITPendingBit(QS808_USARTx,USART_IT_IDLE);
	}
}


//超时中断
void TIM17_IRQHandler(void)
{
	if (TIM_GetITStatus(QS808_TIMx, TIM_IT_Update) != RESET)
	{
		TIM_Cmd(QS808_TIMx,DISABLE);
		qs808_state=QS808_STATE_TIMEOUT;
		TIM_ClearITPendingBit(QS808_TIMx, TIM_IT_Update);
	}
}

//唤醒中断 
void QS808_IRQ(void)
{
	__NOP();
}



































