



#ifndef __POWER_H
#define __POWER_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"






#define Power_On()   GPIO_ResetBits(GPIOA,GPIO_Pin_10);
#define Power_Off()  GPIO_SetBits(GPIOA,GPIO_Pin_10);


void Power_Switch_Init(void);

#ifdef __cplusplus
}
#endif


#endif /* __MAIN_H */



