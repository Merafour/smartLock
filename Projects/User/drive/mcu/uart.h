/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : uart.h
* Author             : Merafour
* Last Modified Date : 04/28/2018
* Description        : This file provides the uart library.

******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#ifdef __cplusplus
 extern "C" {
#endif

extern uint8_t uart_timeout;

extern void uart_init(void);
//extern uint16_t USARTx_send(const uint8_t buf[], const uint16_t len);
//extern uint16_t USARTx_str(const uint8_t *str);
//extern void USARTx_test(void);
extern uint8_t uart_send(const uint8_t buf[], const uint8_t len);
extern uint8_t uart_recv(uint8_t buf[], const uint8_t len);
extern void uart_test(void);
extern void uart_deinit(void);

#ifdef __cplusplus
}
#endif
#endif 



