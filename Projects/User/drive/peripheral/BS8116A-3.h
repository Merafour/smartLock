


#ifndef __BS8116A_3_H
#define __BS8116A_3_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define TOUCH_I2C_ADD    0xA0

#define KEY_NUM0   0x0800
#define KEY_NUM1   0x0002
#define KEY_NUM2   0x0001
#define KEY_NUM3   0x0080
#define KEY_NUM4   0x0008
#define KEY_NUM5   0x0020

#define KEY_NUM6   0x0040
#define KEY_NUM7   0x0010
#define KEY_NUM8   0x0200
#define KEY_NUM9   0x0100
#define KEY_LOCK   0x0004
#define KEY_BELL   0x1000

void Bs8116a_3_init(void);
void Key_Read(void);

extern uint16_t Key_value(void);

#ifdef __cplusplus
}
#endif

#endif



