/* 
 * File:   uart.h
 * Author: HP
 *
 * Created on August 27, 2026, 8:01 AM
 */

#ifndef UART_H
#define	UART_H

#include<xc.h>

void uart_char_transimit(unsigned char ch);
void uart_str_transmit(unsigned char *str);
unsigned char uart_receive(void);
void init_uart();


#endif	/* UART_H */

