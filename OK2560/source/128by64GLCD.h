//============================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : 128by64GLCD.h
//	description : ±×·¡ÇÈLCD
//
//	history : - Designed and programmed by Duck-Yong Yoon in 2006.
//            - Revised by Paul B.H Park(idonno2000) at 2008.06.17
//
//============================================================================

#ifndef _128by64GLCD_H_
#define _128by64GLCD_H_

#include <avr/pgmspace.h>

extern prog_uchar Font0508[96][5];
extern unsigned char LCD_buffer[128*8];



void GLCD_command(BYTE cs,BYTE command);       /* write a command */
void GLCD_data(BYTE signal,BYTE character);        /* write a data */
void GLCD_clear(void);				/* clear GLCD screen */
void GLCD_FullUpdate(void);

void GLCD_TEST(void);

//__________________________________________________________________________________________
void GLCD_putchar1632(unsigned char x, unsigned char y, unsigned char dat_c);
void GLCD_putchar0816(unsigned char x, unsigned char y, unsigned char dat_c);
void GLCD_putchar0508(unsigned char x, unsigned char y, unsigned char dat_c);
void GLCD_putcharUNICODE(unsigned char x, unsigned char y, unsigned int dat_c);

//_________________________________________________________________________________________
void GLCD_print0508(unsigned char x, unsigned char y, unsigned char* str);
void GLCD_print0816(unsigned char x, unsigned char y, unsigned char* str);
void GLCD_printIMAGE(unsigned char x, unsigned char y, unsigned char size_x, unsigned char size_y, unsigned char* str);
void GLCD_printUNICODE(unsigned char x, unsigned char y, unsigned int* str, unsigned char cnt);


#endif
