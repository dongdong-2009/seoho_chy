//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : util,h
//	description : 유틸 헤더파일
//
//	history : - 2008-05-28 type.c에서 분리하여 새로 만듬
//            - 딜레이 함수를 윤교수님책에서 가져옴
//====================================================



#ifndef _UTIL_H_
#define _UTIL_H_


#define MAKEWORD(b, a)      ( (WORD)(a) | ( (WORD)(b) << 8) )
#define SWAPWORD(a)      	(( (((a)&0x00FF)<<8) | (((a)&0xFF00)>>8) ) )
#define LOBYTE(w)           ((BYTE)((w) & 0x00FF))
#define HIBYTE(w)           ((BYTE)(((w) & 0xFF00) >> 8))
#define MAKEDWORD(d,c,b,a)	( (DWORD)(a) | ((DWORD)(b) << 8) | ((DWORD)(c) << 16)  | ((DWORD)(d) << 24)  )
#define SPLITDWORD(d,c,b,a,dw){ 	d = ((dw)&0xFF000000)>>24;c = ((dw)&0x00FF0000)>>16;a = ((dw)&0x000000FF);}
#define BYTE3(a)	( ((a)&0xFF000000)>>24 )	
#define BYTE2(a)	( ((a)&0x00FF0000)>>16 )	
#define BYTE1(a)	( ((a)&0x0000FF00)>>8 )
#define BYTE0(a)	( ((a)&0x000000FF) )


#define	CHK_BIT(x, bit)		 ((x) & (bit))
#define	SET_BIT(x, bit)		 ((x) |= (bit))
#define	CLR_BIT(x, bit)		 ((x) &= ~(bit))

#define BIT7	0x80
#define BIT6	0x40
#define BIT5	0x20
#define BIT4	0x10
#define BIT3	0x08
#define BIT2	0x04
#define BIT1	0x02
#define BIT0	0x01



void Delay_us(unsigned char time_us);		/* time delay for us */
void Delay_ms(unsigned int time_ms);		/* time delay for ms */
void inttoa(int val, char *str, int len);



#endif

