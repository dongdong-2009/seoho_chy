//============================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : 128by64GLCD.C
//	description : 그래픽LCD
//
//	history : - Designed and programmed by Duck-Yong Yoon in 2006.
//            - Revised by Paul B.H Park(idonno2000) at 2008.06.17
//               ㅏ윤교수님 소스는 lcd를 직접 제어하는 방식이었음
//               ㅏ그러한 이유로 lcd를 자유롭게 제어하기 위하여는
//               ㅏ복잡한 명령어 조합이 필수~~
//               ㅏ그리하여 수정을 한것이 LCD_buffer라는 배열을 잡아
//               ㅏ메모리의 값들을 수정하고 한방에 GLCD에 뿌리는 방식
//               ㅏ그래서 추가한 것이 FullUpdate함수임 이점 참고..
//               ㅏ현재 putchar0508(),print0508()함수만 사용가능
//               ㅏ폰트를 추가하여야함
//               ㅏ참고자료(KH351_SDK GLCD소스)
//
//============================================================================
#include "include.h"
#include <avr/pgmspace.h>

unsigned char LCD_buffer[128*8];

prog_uchar 	Font0508[96][5] = { //5 by 8
	{0x00, 0x00, 0x00, 0x00, 0x00},		// 0x20 space
	{0x00, 0x00, 0x4f, 0x00, 0x00},		// 0x21 !
	{0x00, 0x07, 0x00, 0x07, 0x00},		// 0x22 "
	{0x14, 0x7f, 0x14, 0x7f, 0x14},		// 0x23 #
	{0x24, 0x2a, 0x7f, 0x2a, 0x12},		// 0x24 $
	{0x23, 0x13, 0x08, 0x64, 0x62},		// 0x25 %
	{0x36, 0x49, 0x55, 0x22, 0x50},		// 0x26 &
	{0x00, 0x05, 0x03, 0x00, 0x00},		// 0x27 '
	{0x00, 0x1c, 0x22, 0x41, 0x00},		// 0x28 (
	{0x00, 0x41, 0x22, 0x1c, 0x00},		// 0x29 )
	{0x14, 0x08, 0x3e, 0x08, 0x14},		// 0x2a *
	{0x08, 0x08, 0x3e, 0x08, 0x08},		// 0x2b +
	{0x00, 0x50, 0x30, 0x00, 0x00},		// 0x2c ,
	{0x08, 0x08, 0x08, 0x08, 0x08},		// 0x2d -
	{0x00, 0x60, 0x60, 0x00, 0x00},		// 0x2e .
	{0x20, 0x10, 0x08, 0x04, 0x02},		// 0x2f /
	{0x3e, 0x51, 0x49, 0x45, 0x3e},		// 0x30 0
	{0x00, 0x42, 0x7f, 0x40, 0x00},		// 0x31 1
	{0x42, 0x61, 0x51, 0x49, 0x46},		// 0x32 2
	{0x21, 0x41, 0x45, 0x4b, 0x31},		// 0x33 3
	{0x18, 0x14, 0x12, 0x7f, 0x10},		// 0x34 4
	{0x27, 0x45, 0x45, 0x45, 0x39},		// 0x35 5
	{0x3c, 0x4a, 0x49, 0x49, 0x30},		// 0x36 6
	{0x01, 0x71, 0x09, 0x05, 0x03},		// 0x37 7
	{0x36, 0x49, 0x49, 0x49, 0x36},		// 0x38 8
	{0x06, 0x49, 0x49, 0x29, 0x1e},		// 0x39 9
	{0x00, 0x36, 0x36, 0x00, 0x00},		// 0x3a :
	{0x00, 0x56, 0x36, 0x00, 0x00},		// 0x3b ;
	{0x08, 0x14, 0x22, 0x41, 0x00},		// 0x3c <
	{0x14, 0x14, 0x14, 0x14, 0x14},		// 0x3d =
	{0x00, 0x41, 0x22, 0x14, 0x08},		// 0x3e >
	{0x02, 0x01, 0x51, 0x09, 0x06},		// 0x3f ?
	{0x32, 0x49, 0x79, 0x41, 0x3e},		// 0x40 @
	{0x7e, 0x11, 0x11, 0x11, 0x7e},		// 0x41 A
	{0x7f, 0x49, 0x49, 0x49, 0x36},		// 0x42 B
	{0x3e, 0x41, 0x41, 0x41, 0x22},		// 0x43 C
	{0x7f, 0x41, 0x41, 0x22, 0x1c},		// 0x44 D
	{0x7f, 0x49, 0x49, 0x49, 0x41},		// 0x45 E
	{0x7f, 0x09, 0x09, 0x09, 0x01},		// 0x46 F
	{0x3e, 0x41, 0x49, 0x49, 0x7a},		// 0x47 G
	{0x7f, 0x08, 0x08, 0x08, 0x7f},		// 0x48 H
	{0x00, 0x41, 0x7f, 0x41, 0x00},		// 0x49 I
	{0x20, 0x40, 0x41, 0x3f, 0x01},		// 0x4a J
	{0x7f, 0x08, 0x14, 0x22, 0x41},		// 0x4b K
	{0x7f, 0x40, 0x40, 0x40, 0x40},		// 0x4c L
	{0x7f, 0x02, 0x0c, 0x02, 0x7f},		// 0x4d M
	{0x7f, 0x04, 0x08, 0x10, 0x7f},		// 0x4e N
	{0x3e, 0x41, 0x41, 0x41, 0x3e},		// 0x4f O
	{0x7f, 0x09, 0x09, 0x09, 0x06},		// 0x50 P
	{0x3e, 0x41, 0x51, 0x21, 0x5e},		// 0x51 Q
	{0x7f, 0x09, 0x19, 0x29, 0x46},		// 0x52 R
	{0x26, 0x49, 0x49, 0x49, 0x32},		// 0x53 S
	{0x01, 0x01, 0x7f, 0x01, 0x01},		// 0x54 T
	{0x3f, 0x40, 0x40, 0x40, 0x3f},		// 0x55 U
	{0x1f, 0x20, 0x40, 0x20, 0x1f},		// 0x56 V
	{0x3f, 0x40, 0x38, 0x40, 0x3f},		// 0x57 W
	{0x63, 0x14, 0x08, 0x14, 0x63},		// 0x58 X
	{0x07, 0x08, 0x70, 0x08, 0x07},		// 0x59 Y
	{0x61, 0x51, 0x49, 0x45, 0x43},		// 0x5a Z
	{0x00, 0x7f, 0x41, 0x41, 0x00},		// 0x5b [
	{0x02, 0x04, 0x08, 0x10, 0x20},		// 0x5c (\)
	{0x00, 0x41, 0x41, 0x7f, 0x00},		// 0x5d ]
	{0x04, 0x02, 0x01, 0x02, 0x04},		// 0x5e ^
	{0x40, 0x40, 0x40, 0x40, 0x40},		// 0x5f _
	{0x00, 0x01, 0x02, 0x04, 0x00},		// 0x60 `
	{0x20, 0x54, 0x54, 0x54, 0x78},		// 0x61 a
	{0x7f, 0x48, 0x44, 0x44, 0x38},		// 0x62 b
	{0x38, 0x44, 0x44, 0x44, 0x20},		// 0x63 c
	{0x38, 0x44, 0x44, 0x48, 0x7f},		// 0x64 d
	{0x38, 0x54, 0x54, 0x54, 0x18},		// 0x65 e
	{0x08, 0x7e, 0x09, 0x01, 0x02},		// 0x66 f
	{0x0c, 0x52, 0x52, 0x52, 0x3e},		// 0x67 g
	{0x7f, 0x08, 0x04, 0x04, 0x78},		// 0x68 h
	{0x00, 0x04, 0x7d, 0x00, 0x00},		// 0x69 i
	{0x20, 0x40, 0x44, 0x3d, 0x00},		// 0x6a j
	{0x7f, 0x10, 0x28, 0x44, 0x00},		// 0x6b k
	{0x00, 0x41, 0x7f, 0x40, 0x00},		// 0x6c l
	{0x7c, 0x04, 0x18, 0x04, 0x7c},		// 0x6d m
	{0x7c, 0x08, 0x04, 0x04, 0x78},		// 0x6e n
	{0x38, 0x44, 0x44, 0x44, 0x38},		// 0x6f o
	{0x7c, 0x14, 0x14, 0x14, 0x08},		// 0x70 p
	{0x08, 0x14, 0x14, 0x18, 0x7c},		// 0x71 q
	{0x7c, 0x08, 0x04, 0x04, 0x08},		// 0x72 r
	{0x48, 0x54, 0x54, 0x54, 0x20},		// 0x73 s
	{0x04, 0x3f, 0x44, 0x40, 0x20},		// 0x74 t
	{0x3c, 0x40, 0x40, 0x20, 0x7c},		// 0x75 u
	{0x1c, 0x20, 0x40, 0x20, 0x1c},		// 0x76 v
	{0x3c, 0x40, 0x30, 0x40, 0x3c},		// 0x77 w
	{0x44, 0x28, 0x10, 0x28, 0x44},		// 0x78 x
	{0x0c, 0x50, 0x50, 0x50, 0x3c},		// 0x79 y
	{0x44, 0x64, 0x54, 0x4c, 0x44},		// 0x7a z
	{0x00, 0x08, 0x36, 0x41, 0x00},		// 0x7b {
	{0x00, 0x00, 0x77, 0x00, 0x00},		// 0x7c |
	{0x00, 0x41, 0x36, 0x08, 0x00},		// 0x7d }
	{0x08, 0x04, 0x08, 0x10, 0x08},		// 0x7e ~
	{0x00, 0x00, 0x00, 0x00, 0x00} 	// NULL : Unicode NULL
};



void GLCD_command(BYTE cs,BYTE command)       /* write a command */
{ 
	PORTH = cs & 0xC0;			// D/-I = 0, R/-W = 0
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTJ = command;				// write command
	PORTH = (cs & 0xC0)|0x20;			// E = 1
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTH = cs & 0xC0;			// E = 0
	asm volatile(" NOP      ");
	PORTH = 0x00;					// clear all control signals
	_delay_us(10);                                 // wait for GLCD busy
}

void GLCD_data(BYTE signal,BYTE character)        /* write a data */
{
	PORTH = (signal & 0xC0)|0x10;			// D/-I = 1, R/-W = 0
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTJ = character;				// write data
	PORTH = (signal & 0xC0)|0x30;			// E = 1
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTH = (signal & 0xC0)|0x10;			// E = 0
	asm volatile(" NOP      ");
	PORTH = 0x00;					// clear all control signals
	_delay_us(10);                                 // wait GLCD busy
}

void GLCD_clear(void)				/* clear GLCD screen */
{
	unsigned char i, j, x;

	PORTH = 0x00;					// clear all control signals
	GLCD_command(0xC0,0x3F);			// CS1,CS2 display ON
	GLCD_command(0xC0,0xC0);			// CS1,CS2 display position

	x = 0xB8;
	for(i = 0; i <= 7; i++)
	{ GLCD_command(0xC0,x);                     // X start addtess
	  GLCD_command(0xC0,0x40);                  // Y start address
	  for(j = 0; j <= 63; j++)
	    GLCD_data(0xC0,0x00);			// clear CS1 and CS2 screen
	  x++;
	}
}


void GLCD_FullUpdate(void)
{
	unsigned int i, j;

	for (i=0;i<8;i++) 
	{
		GLCD_command(0xC0,0xB8 + i); 
		for (j=0;j<128;j++) 
		{
			if(j < 64)	GLCD_data(0x40,LCD_buffer[i*128+j]);       // write the data 
			else 		GLCD_data(0x80,LCD_buffer[i*128+j]);       // write the data 
		}
	}

}




void GLCD_putchar1632(unsigned char x, unsigned char y, unsigned char dat_c)
{
	unsigned char tx = 0;
	for( tx = 0; tx < 64 ; tx++)
	{
	//	LCD_buffer[(tx%16+x)+(128*(tx/16))+128*y] = Font1632[dat_c][tx];		
	}
}

void GLCD_putchar0816(unsigned char x, unsigned char y, unsigned char dat_c)
{
	unsigned char tx = 0;

	for( tx = 0; tx < 16 ; tx++)
	{
	//	LCD_buffer[(tx%8+x)+(128*(tx/8))+128*y] = Font0816[dat_c][tx];		
	}
}

void GLCD_putchar0508(unsigned char x, unsigned char y, unsigned char dat_c)
{
	unsigned char tx = 0;

	for( tx = 0; tx < 5 ; tx++)
	{
		LCD_buffer[(tx+x)+128*y] = pgm_read_byte(&Font0508[dat_c][tx]);		
	}
}

void GLCD_putcharUNICODE(unsigned char x, unsigned char y, unsigned int dat_c)
{
	unsigned char tx = 0;

//	SM_font_read(dat_c);

	for( tx = 0; tx < 32 ; tx++)
	{
	//	LCD_buffer[((tx%16)+x) + (128*(y+tx/16))] = unicode_font_data[tx];			
	}
}




//_________________________________________________________________________________________


void GLCD_print0508(unsigned char x, unsigned char y, unsigned char* str)
{
	unsigned char tu = 0;

	while (*str != 0x00) 
	{
		GLCD_putchar0508(x+tu*5,y,*str-0x20);
		str++;
		tu++;				
	}
}



void GLCD_print0816(unsigned char x, unsigned char y, unsigned char* str)
{
	unsigned char tu = 0;

	while (*str != 0x00) 
	{
		GLCD_putchar0816(x+tu*8,y,*str-0x20);
		str++;
		tu++;				
	}
}

void GLCD_printIMAGE(unsigned char x, unsigned char y, unsigned char size_x, unsigned char size_y, unsigned char* str)
{
	unsigned char a,b;
	for(b=y;b<(size_y+y);b++)
	{
		for(a=x;a<(size_x+x);a++)
		{
			LCD_buffer[(b<<7)+a] = *str++;
		}
	}
}



void GLCD_printUNICODE(unsigned char x, unsigned char y, unsigned int* str, unsigned char cnt)
{
	BYTE c;
	for(c=0;c<cnt;c++)
	{
		GLCD_putcharUNICODE(x+(c<<4), y, str[c]);	
	}
}
