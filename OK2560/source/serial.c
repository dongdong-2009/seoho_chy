//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.c
//	description : 시리얼통신 함수 정의
//	last revision : 2008-05-09 4:03오후
//	history : 
//
//====================================================

#include "include.h"
#include "serial.h"


void UART0_init(void)
{
  UBRR0 = 51;                                   // 19200 bps
  UCSR0A = 0x00;                                // asynchronous normal mode
  UCSR0B = 0x98;                                // interrupt enable, 8 data
  UCSR0C = 0x06;                                // no parity, 1 stop, 8 data
 // RXD0 = UDR0;                                   // dummy read
                                      // global interrupt enable
}

void TX0_char(unsigned char data)		/* transmit a character by USART0 */
{
  while((UCSR0A & 0x20) == 0x00);		// data register empty ?
  UDR0 = data;
}

void TX0_string(char *string)			/* transmit a string by USART0 */
{
  while(*string != 0x00)
    { TX0_char(*string);
       string++;
    }
}
/*
ISR(USART0_RX_vect)				// UART0 interrupt function 
{
}
*/
