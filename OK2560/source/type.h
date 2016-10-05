//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : type.h
//	description : 코딩습관은 어쩔수 없다
//                내맘대로 쓰려고 여러가지를 정의해뒀다
//
//	history : frist designed at 2008-05-28 3:58오후
//
//====================================================

#ifndef _TYPE_H_
#define _TYPE_H_

#include <avr/sfr_defs.h>

//TYPE
#define CHAR char
#define UCHAR unsigned char
#define UINT8 unsigned char
#define UINT16 unsigned int
#define UINT32 unsigned long
#define INT32 long
#define INT16 short

//#define NULL 0
#define TRUE 1
#define FALSE 0
#define true 1
#define false 0

#define ON		1
#define OFF		0

#define LONG long
#define BOOL UCHAR

typedef unsigned char  BYTE;
typedef char      		SBYTE;
typedef unsigned int 	WORD;
typedef int 			SWORD;
typedef unsigned long 	DWORD;
typedef long 			SDWORD;


#if 0
//BIT redefine SFR_REG for Hsu300d_mmi
//PORT A
#define TRIP			0x80//out
#define ALARM		0x40//out
#define BYP_FEED	0x20//out
#define INV_FEED	0x10//out
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT B
#define TRIP			0x80
#define ALARM		0x40
#define BYP_FEED	0x20
#define INV_FEED	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT C
//#define BIT7	0x80
//#define BIT6	0x40
//#define BIT5	0x20
//#define BIT4	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT D
#define BUZZ			0x80//out
#define LCD_LIGHT	0x40//out
//#define BIT5	0x20
//#define BIT4	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD E
#define INV_FEED_LED		0x80
#define INV_ON_LED			0x40
#define BAT_dCHARGE_LED	0x20
#define BAT_CHARGE_LED		0x10
#define CONV_LED			0x08
#define INPUT_LED			0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD F
//#define BIT7	0x80
//#define BIT6	0x40
//#define BIT5	0x20
//#define BIT4	0x10
#define DIP_SW4	0x08
#define DIP_SW3	0x04
#define DIP_SW2	0x02
#define DIP_SW1	0x01

//PORD G
//#define BIT7	0x80
//#define BIT6	0x40
#define RS485_EN1	0x20
#define RS485_EN0	0x10
#define TEST_LED	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD H
#define CB0_LED			0x80
#define LED11			0x40
#define ALARM_LED		0x20
#define NORMAL_LED		0x10
#define SYNC_LED		0x08
#define BYP_FEED_LED	0x04
//#define BIT1	0x02
//#define BIT0	0x01


//PORD J
//#define BIT7	0x80
//#define BIT6	0x40
#define DC_LV4	0x20
#define DC_LV3	0x10
#define DC_LV2	0x08
#define DC_LV1	0x04
//#define BIT1	0x02
//#define BIT0	0x01


//PORD K
//#define BIT7	0x80
//#define BIT6	0x40
#define KEY_ENTER	0x20
#define KEY_RIGHT	0x10
#define KEY_LEFT		0x08
#define KEY_DN		0x04
#define KEY_UP		0x02
#define KEY_ESC		0x01


//PORD L
#define DC_LV0		0x80
#define BYP_OUT		0x40
#define SYSTEM_O	0x20
#define CB5			0x10
#define CB4			0x08
#define CB3			0x04
#define CB2			0x02
#define CB1			0x01
#endif

#endif
