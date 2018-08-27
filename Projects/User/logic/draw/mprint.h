/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : mprint.h
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the print library.

******************************************************************************/
#ifndef __AP_HAL_UART_DRIVER_H__
#define __AP_HAL_UART_DRIVER_H__

#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdarg.h>
#include "ftoa_engine.h"

enum {
    BASE_DEFAULT = 0,
    BASE_BIN     = 2,
    BASE_OCT     = 8,
    BASE_DEC     = 10,
    BASE_HEX     = 16
};

/* Pure virtual UARTDriver class */
class mPrint {
private:
    //size_t printNumber(unsigned long, uint8_t);
    //size_t printFloat(float, uint8_t);
	void print_vprintf (unsigned char in_progmem, const char *fmt, va_list ap);
public:
    mPrint() {}
	virtual size_t write(uint8_t) = 0;
    //virtual size_t write(const uint8_t *buffer, size_t size) = 0;
	//size_t write(const char *str) { return write((const uint8_t *)str, strlen(str)); }
	

    /* Implementations of BetterStream virtual methods. These are
     * provided by AP_HAL to ensure consistency between ports to
     * different boards
     */
    void print_P(const prog_char_t *s);
    //void println_P(const prog_char_t *s);
    void printf(const char *s, ...)
            __attribute__ ((format(__printf__, 2, 3)));
    void _printf_P(const prog_char *s, ...)
            __attribute__ ((format(__printf__, 2, 3)));

    void vprintf(const char *s, va_list ap);
    void vprintf_P(const prog_char *s, va_list ap);

    virtual void format(int16_t x, int16_t y, uint8_t size, const char *s, ...)
            __attribute__ ((format(__printf__, 5, 6)));	
//	int snprintf(char * __restrict /*s*/, size_t /*n*/,
//                     const char * __restrict /*format*/, ...) __attribute__((__nonnull__(3)));
};

#endif // __AP_HAL_UART_DRIVER_H__

