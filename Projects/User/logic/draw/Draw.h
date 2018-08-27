/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : draw.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the draw library.

******************************************************************************/
#ifndef _DRAW_H
#define _DRAW_H

#include <stdint.h>
#include "mprint.h"
#include "object.h"


#ifndef  BLACK
#define BLACK 0
#endif
#ifndef  WHITE
#define WHITE 1
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }



//class Draw : public Print {
//class Draw {
class Draw : public mPrint {
private:
	voice_obj &_voice;
 public:

	Draw(voice_obj &voice, int16_t w, int16_t h); // Constructor 
	//Draw(voice_obj &voice, int16_t w, int16_t h) :_voice(voice),WIDTH(w), HEIGHT(h) {}
//  void format(int16_t x, int16_t y, uint8_t s, const char *s, ...)
//            __attribute__ ((format(__printf__, 5, 6)));
	virtual void format(int16_t x, int16_t y, uint8_t size, const char *s, ...)
            __attribute__ ((format(__printf__, 5, 6)));
	virtual void format(int16_t x, int16_t y, uint8_t size, uint16_t delay, const char *s, ...)
            __attribute__ ((format(__printf__, 6, 7)));
	/*
	  格式化输出函数
	  参数：  清屏, 坐标, 延时, 格式输出控制符同函printf数
	 **/
	virtual void format(bool isUpdateHW, int16_t x, int16_t y, uint8_t size, uint16_t delay, const char *s, ...)
            __attribute__ ((format(__printf__, 7, 8)));
	/*
	  格式化输出函数,带语音输出
	  参数：  坐标, 语音, 延时, 格式输出控制符同函printf数
	 **/
	virtual void vformat(int16_t x, int16_t y, const char* info, uint16_t delay, const char *s, ...)
            __attribute__ ((format(__printf__, 6, 7)));
	virtual void vformat(bool isUpdateHW, int16_t x, int16_t y, const char* info, uint16_t delay, const char *s, ...)
            __attribute__ ((format(__printf__, 7, 8)));
//{
//	setCursor(0,0);
//	setTextSize(2);
//	//mPrint::format
//	update();
//}
  // This MUST be defined by the subclass:
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillScreen(uint16_t color),
    invertDisplay(uint8_t i); // boolean

  // These exist only with Adafruit_GFX (no subclass overrides)
  void
    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color),
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color),
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawChar(int16_t x, int16_t y, const unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size),
    drawStr(int16_t x, int16_t y, const uint8_t *str, uint16_t color,
      uint16_t bg, uint8_t size),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c),
    setTextColor(uint16_t c, uint16_t bg),
    setTextSize(uint8_t s),
    setTextWrap(uint8_t w), // boolean
    setRotation(uint8_t r);
	void drawCharCN(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size, const uint8_t map[]);
	void drawCharCN_16x16(int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size, const uint8_t _font[]);

//#if ARDUINO >= 100
//  virtual size_t write(uint8_t);
//#else
//  virtual void   write(uint8_t);
//#endif
	virtual size_t write(uint8_t);
	virtual void update()=0;
	virtual void clear(bool isUpdateHW)=0;

  int16_t
    height(void),
    width(void);

  uint8_t getRotation(void);

 protected:
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y;
  uint16_t
    textcolor, textbgcolor;
  uint8_t
    textsize,
    rotation;
  uint8_t //boolean
    wrap; // If set, 'wrap' text at right edge of display
};

#endif // _ADAFRUIT_GFX_H
