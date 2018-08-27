/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : ssd1306syp.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the ssd1306syp library.

******************************************************************************/
#include "ssd1306syp.h"
#include "main.h"
#include "delay.h"
#include <string.h>

//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 				16
#define XLevelL				0x00
#define XLevelH				0x10
#define Max_Column		    128
#define Max_Row				64
#define	Brightness		    0xFF 
#define X_WIDTH 			128
#define Y_WIDTH 			64

//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOC,GPIO_Pin_13)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)


//使用4线串行接口时使用
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_3)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
ssd1306syp::ssd1306syp(voice_obj &voice):
Draw(voice,SSD1306_WIDTH,SSD1306_HEIGHT)
{
}
ssd1306syp::~ssd1306syp()
{
}
//initialized the ssd1306 in the setup function
bool ssd1306syp::initialize()
{
	memset(m_pFramebuffer,0,SSD1306_FBSIZE);//clear it.
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 

	OLED_WR_Byte(0xAE,OLED_CMD); //Set Display Off
	OLED_WR_Byte(0x00,OLED_CMD); //Set lower column start address
	OLED_WR_Byte(0x10,OLED_CMD); //Set higher column start address
	OLED_WR_Byte(0x40,OLED_CMD); //Set Display Start Line
	OLED_WR_Byte(0x81,OLED_CMD); //Set Contrast
	OLED_WR_Byte(0x7F,OLED_CMD); //Reset=0x7f
//	OLED_WR_Byte(0xa0,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	OLED_WR_Byte(0xc0,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA0,OLED_CMD); //Segment Remap
	OLED_WR_Byte(0xA6,OLED_CMD); //Set Normal Display
	OLED_WR_Byte(0xA8,OLED_CMD); //Multiplex ration mode:63
	OLED_WR_Byte(63,OLED_CMD);
	OLED_WR_Byte(0xC0,OLED_CMD); //Set COM Output Scan Direction
	OLED_WR_Byte(0xD3,OLED_CMD); //Set Display Offset
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); //Display divide ratio/osc. freq. mode
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); //Common pads hardware: alternative
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); //Set Display Offset
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); //VCOM deselect level mode
	OLED_WR_Byte(0x20,OLED_CMD); //Set Vcomh=0.77*Vcc(reset)
	OLED_WR_Byte(0xAF,OLED_CMD);
	//OLED_Clear();
	//OLED_ShowString(0,0,"AB1234567890");
	clear(1);
	//delay(10);//wait for the screen loaded.
	return true;
}
//void ssd1306syp::clear(bool isUpdateHW)
//{
//	memset(m_pFramebuffer,0,SSD1306_FBSIZE);//clear the back buffer.
//	if(isUpdateHW) update();//update the hw immediately
//}

void ssd1306syp::writeCommand(unsigned char cmd)
{
	OLED_WR_Byte(cmd,OLED_CMD);
}

void ssd1306syp::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	unsigned char row;
	unsigned char offset;
	unsigned char  preData;//previous data.
	unsigned char val;
	int16_t  index;

	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()) || ( m_pFramebuffer==0))	return;

	//get the previous data;
	row = y/8;
	offset =y%8;
	index = row*width() + x;
	preData = m_pFramebuffer[index];

	//set pixel;
	val = 1<<offset;
	if(color!=0)
	{//white! set bit.
		m_pFramebuffer[index] = preData | val;
	}else
	{//black! clear bit.
		m_pFramebuffer[index] = preData & (~val);
	}

}

void ssd1306syp::update()
{
#if 1
	unsigned int  i=0;
	unsigned char m,n;
	//memset(m_pFramebuffer, 0, sizeof(m_pFramebuffer));
	//memset(m_pFramebuffer, 0x01, sizeof(m_pFramebuffer));
	for(m=0;m<8;m++)
	{
		writeCommand(0xb0+m);	//page0-page1
		writeCommand(0x00);		//low column start address
		writeCommand(0x10);		//high column start address
		
		//startDataSequence();
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(m_pFramebuffer[i++],OLED_DATA);
		}
	}
#else
	updateRow(0,SSD1306_MAXROW);
#endif
}

void ssd1306syp::updateRow(int rowID)
{
	unsigned char x;
	unsigned int  index;
	if(rowID>=0 && rowID<SSD1306_MAXROW && m_pFramebuffer)
	{//this part is faster than else.
		//set the position
		OLED_Set_Pos(x,rowID);
		//start painting the buffer.
		for(x=0;x<SSD1306_WIDTH;x++)
		{
			index = rowID*SSD1306_WIDTH+x;
	  		//writeByte(m_pFramebuffer[index]);
			OLED_WR_Byte(m_pFramebuffer[index],OLED_DATA);
		}
	}
}
void ssd1306syp::updateRow(int startID, int endID)
{
	unsigned char y =0;
	for(y=startID; y<endID; y++)
	{
		updateRow(y);
	}
}

//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void ssd1306syp::OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}
void ssd1306syp::OLED_Set_Pos(uint8_t x,uint8_t y) 
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}

