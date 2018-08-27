/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : gb2312.h 
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the gb2312 font.

******************************************************************************/
#ifndef _GB2312_H_
#define _GB2312_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>


//typedef struct {
//	uint16_t gb;
//	const uint8_t _font[32];  // 16*16
//}gb2312_t;

//extern const gb2312_t gb2312_font[];
extern const uint8_t* gb2312_search(const uint8_t c);

#ifdef __cplusplus
}
#endif
#endif















