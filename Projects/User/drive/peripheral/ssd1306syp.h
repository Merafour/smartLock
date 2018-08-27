/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : ssd1306syp.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the ssd1306syp library.

******************************************************************************/
#ifndef _SSD1306SYP_H_
#define _SSD1306SYP_H_

#include <Draw.h>
#include <stdint.h>

using namespace std;

//#define BLACK 0
//#define WHITE 1

//common parameters
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_FBSIZE 1024 //128x8
#define SSD1306_MAXROW 8
//command macro
#define SSD1306_CMD_DISPLAY_OFF 0xAE//--turn off the OLED
#define SSD1306_CMD_DISPLAY_ON 0xAF//--turn on oled panel 

class ssd1306syp : public Draw{
public:
	ssd1306syp(voice_obj &voice);
	~ssd1306syp();
	//initialized the ssd1306 in the setup function
	virtual bool initialize();

	//update the framebuffer to the screen.
	virtual void update();
	//totoally 8 rows on this screen in vertical direction.
	virtual void updateRow(int rowIndex);
	virtual void updateRow(int startRow, int endRow);
	
	//draw one pixel on the screen.
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

	//clear the screen
	//void clear(bool isUpdateHW=false);
	//void clear(uint8_t isUpdateHW)
	virtual void clear(bool isUpdateHW)
	{
		memset(m_pFramebuffer,0,SSD1306_FBSIZE);//clear the back buffer.
		if(isUpdateHW) update();//update the hw immediately
	}
	void off(void)
	{
		writeCommand(SSD1306_CMD_DISPLAY_OFF);//display off
	}
	void on(void)
	{
		writeCommand(SSD1306_CMD_DISPLAY_ON);//--turn on oled panel 
	}
protected:
	//write one byte to the screen.
	//void writeByte(unsigned char  b);
	void writeCommand(unsigned char  cmd);
	void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
	void OLED_Set_Pos(uint8_t x,uint8_t y);

protected:
	//unsigned char* m_pFramebuffer;//the frame buffer for the adafruit gfx. size=64x8 bytes
	unsigned char m_pFramebuffer[SSD1306_FBSIZE];//the frame buffer for the adafruit gfx. size=64x8 bytes
};

extern ssd1306syp display;
#endif
