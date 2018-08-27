/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : draw.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the draw library.

******************************************************************************/
#include "Draw.h"
#include "glcdfont.c"
#include "gb2312.h"
#include "delay.h"
#include "data.h"
#include <stdlib.h>
#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

Draw::Draw(voice_obj &voice, int16_t w, int16_t h):
 _voice(voice),WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
}
//Draw::Draw(voice_obj &voice, int16_t w, int16_t h) :_voice(voice),WIDTH(w), HEIGHT(h) 
//{
//  _width    = WIDTH;
//  _height   = HEIGHT;
//  rotation  = 0;
//  cursor_y  = cursor_x    = 0;
//  textsize  = 1;
//  textcolor = textbgcolor = 0xFFFF;
//  wrap      = true;
//}

// Draw a circle outline
void Draw::drawCircle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void Draw::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void Draw::fillCircle(int16_t x0, int16_t y0, int16_t r,
			      uint16_t color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void Draw::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, uint16_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void Draw::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void Draw::drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
			    uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void Draw::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

void Draw::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

void Draw::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void Draw::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void Draw::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint16_t color) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x    , y+r  , h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void Draw::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint16_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void Draw::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void Draw::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

void Draw::drawBitmap(int16_t x, int16_t y,
			      const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color) {
	int16_t i, j;//, byteWidth = 1;//(w + 7) / 8;
//	int16_t index=0; 
//	int16_t row=0;
	int16_t y_tmp;
	int16_t flag;
	//const uint8_t size = 2;

	for (i=0; i<w; i++ ) {
		uint8_t line;
		for (j = 0; j<h; j++) {
			flag = j>>3; // j/8
			y_tmp = y + (flag*8);
			line = pgm_read_byte(bitmap+i+16*flag);
			//if (line & 0x1) {
			if (line & (0x1<<(j&7))) {
					drawPixel(x+i, y_tmp+j-flag*8, color);
					//fillRect(x+(i*size), y_tmp+(j*size), size, size, color);
//				} else if (bg != color) {
//					drawPixel(x+i, y_tmp+j-flag*8, bg);
			}
			//line >>= 1;
		}
	}
}
void Draw::drawCharCN(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size, const uint8_t map[]) {
int8_t i;
int8_t j;
int16_t y_tmp;
int16_t flag;
//uint8_t data;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 16 * size - 1) < 0) || // Clip left
     ((y + 16 * size - 1) < 0))   // Clip top
    return;

	for (i=0; i<16; i++ ) {
		uint8_t line;
//		line = pgm_read_byte(CN16x16+(c*32)+i);
//		y_tmp = y;
		for (j = 0; j<16; j++) {
			flag = j>>3; // j/8
			//if(flag)
			{
				y_tmp = y + (flag*8 * size);
				line = pgm_read_byte(map+(c*32)+i+16*flag);
			}
			//if (line & 0x1) {
			if (line & (0x1<<(j&7))) {
					if (size == 1) // default size
					  drawPixel(x+i, y_tmp+j-flag*8, color);
					else {  // big size
					  fillRect(x+(i*size), y_tmp+(j*size), size, size, color);
					} 
				} else if (bg != color) {
					if (size == 1) // default size
					  drawPixel(x+i, y_tmp+j-flag*8, bg);
					else {  // big size
					  fillRect(x+i*size, y_tmp+(j)*size, size, size, bg);
					}
			}
			//line >>= 1;
		}
	}
}
void Draw::drawCharCN_16x16(int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size, const uint8_t _font[]) {
int8_t i;
int8_t j;
int16_t y_tmp;
int16_t flag;
//uint8_t data;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 16 * size - 1) < 0) || // Clip left
     ((y + 16 * size - 1) < 0))   // Clip top
    return;

	for (i=0; i<16; i++ ) {
		uint8_t line;
//		line = pgm_read_byte(CN16x16+(c*32)+i);
//		y_tmp = y;
		for (j = 0; j<16; j++) {
			flag = j>>3; // j/8
			//if(flag)
			{
				y_tmp = y + (flag*8 * size);
				line = pgm_read_byte(_font+i+16*flag);
			}
			//if (line & 0x1) {
			if (line & (0x1<<(j&7))) {
					if (size == 1) // default size
					  drawPixel(x+i, y_tmp+j-flag*8, color);
					else {  // big size
					  fillRect(x+(i*size), y_tmp+(j*size), size, size, color);
					} 
				} else if (bg != color) {
					if (size == 1) // default size
					  drawPixel(x+i, y_tmp+j-flag*8, bg);
					else {  // big size
					  fillRect(x+i*size, y_tmp+(j)*size, size, size, bg);
					}
			}
			//line >>= 1;
		}
	}
}
//#if ARDUINO >= 100
//size_t Draw::write(uint8_t c) {
//#else
//void Draw::write(uint8_t c) {
//#endif
#if 0
const gb2312_t gb_font[2] = {
	{
		'ÎÒ',
		{0x10,0x60,0x02,0x8C,0x00,0x10,0x92,0x52,0x32,0x1E,0x32,0x52,0x92,0x12,0x10,0x00,
	     0x04,0x04,0x7E,0x01,0x00,0x21,0x18,0x40,0x80,0x7F,0x08,0x30,0x04,0x09,0x31,0x00,/*"?",0*/}
	}
};
static uint16_t gb2312=0;
#endif
size_t Draw::write(uint8_t c) {
	if(c>127)
	{
#if 0
		if(gb2312>127) 
		{
			uint16_t index=0;
			gb2312 = gb2312<<8 | c;
			for(index=0; index<sizeof(gb_font)/sizeof(gb_font[0]); index++)
			{
				if(gb2312==gb_font[index].gb)
				{
					drawCharCN_16x16(cursor_x, cursor_y, WHITE, BLACK, 1, gb_font[index]._font);
					cursor_x += textsize*16;
					if (wrap && (cursor_x > (_width - textsize*16))) {
					  cursor_y += textsize*16;
					  cursor_x = 0;
					}
				}
			}
			gb2312 = 0;
			return;
		}
		gb2312 = c;
#endif
		const uint8_t* _font = gb2312_search(c);
		if(NULL!=_font)
		{
			drawCharCN_16x16(cursor_x, cursor_y, WHITE, BLACK, 1, _font);
			cursor_x += 16;
			if (wrap && (cursor_x > (_width - 16))) {
			  cursor_y += 16;
			  cursor_x = 0;
			}
		}
		return 1;
	}
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
//#if ARDUINO >= 100
//  return 1;
//#endif
  return 1;
}

// Draw a character
void Draw::drawChar(int16_t x, int16_t y, const unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}
void Draw::drawStr(int16_t x, int16_t y, const uint8_t *str, uint16_t color,
  uint16_t bg, uint8_t size)
{
	uint16_t i=0;
	while('\0'!=str[i])
	{
		drawChar(x, y, str[i], color, bg, size);
	}
}
void Draw::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void Draw::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void Draw::setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void Draw::setTextColor(uint16_t c, uint16_t b) {
  textcolor   = c;
  textbgcolor = b; 
}

void Draw::setTextWrap(uint8_t w) {
  wrap = w;
}

uint8_t Draw::getRotation(void) {
  return rotation;
}

void Draw::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)
int16_t Draw::width(void) {
  return _width;
}
 
int16_t Draw::height(void) {
  return _height;
}

void Draw::invertDisplay(uint8_t i) {
  // Do nothing, must be subclassed if supported
}

void Draw::format(int16_t x, int16_t y, uint8_t size, const char *fmt, ...) 
{
    va_list ap;
	setCursor(x,y);
	setTextSize(size);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
	//update();
}
void Draw::format(int16_t x, int16_t y, uint8_t size, uint16_t delay, const char *fmt, ...) 
{
    va_list ap;
	setCursor(x,y);
	setTextSize(size);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
	update();
	if(delay>0) delay_ms(delay);
}
void Draw::format(bool isUpdateHW, int16_t x, int16_t y, uint8_t size, uint16_t delay, const char *fmt, ...) 
{
    va_list ap;
	clear(isUpdateHW);
	setCursor(x,y);
	setTextSize(size);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
	update();
	if(delay>0) delay_ms(delay);
}
void Draw::vformat(int16_t x, int16_t y, const char* info, uint16_t delay, const char *fmt, ...) 
{
    va_list ap;
	setCursor(x,y);
	setTextSize(2);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
	update();
	_voice.play_voice(DATA::get_volume(), 1, _voice.search(info)); // ²¥·ÅÓïÒô, search·µ»ØÓïÒô±àºÅ
	if(delay>0) delay_ms(delay);
}
void Draw::vformat(bool isUpdateHW, int16_t x, int16_t y, const char* info, uint16_t delay, const char *fmt, ...) 
{
    va_list ap;
	clear(isUpdateHW);
	setCursor(x,y);
	setTextSize(2);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
	update();
	_voice.play_voice(DATA::get_volume(), 1, _voice.search(info)); // ²¥·ÅÓïÒô, search·µ»ØÓïÒô±àºÅ
	if(delay>0) delay_ms(delay);
}

