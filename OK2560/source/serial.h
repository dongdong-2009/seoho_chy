//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.h
//	description : �ø������ �������
//	last revision : 2008-05-09 3:58����
//	history : 
//
//====================================================

#ifndef _SERIAL_H_
#define _SERIAL_H_

void UART0_init(void);
void TX0_char(unsigned char data);		/* transmit a character by USART0 */
void TX0_string(char *string);			/* transmit a string by USART0 */
//ISR(USART0_RX_vect);				/* UART0 interrupt function */

#endif
