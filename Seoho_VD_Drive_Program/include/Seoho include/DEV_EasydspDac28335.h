/*
******************************************************************************** 
* EnerWiz 
* 1, Hwayang-dong, Gwangjin-gu,
* Seoul, 143-701
* Korea
* 
* (c) Copyright 2009, EnerWiz
* 
* All rights reserved. Company Programmer Blackp's source code is an unpublished work and the 
* use of a copyright notice does not imply otherwise. This source code contains 
* confidential, trade secret material of EnerWiz, Inc. Any attempt or participation 
* in deciphering, decoding, reverse engineering or in any way altering the source 
* code is strictly prohibited, unless the prior written consent of EnerWiz 
* is obtained. 
* 
* Filename : DEV_EasydspDac28335.h
* Programmer(s): Blackp, Zun.
* Created : 2009/04/04
* Description : xxxx
********************************************************************************
*/

/*
********************************************************************************
* Revision history
********************************************************************************
*/

/*
********************************************************************************
* DEFINE
******************************************************************************** 
*/
//#define		CPU_8BIT
#define		CPU_16BIT
//#define		CPU_32BIT

//typedef		char			BOOL;
typedef		char			U8;
typedef		signed char		S8;
typedef		unsigned short	U16;
typedef		short			S16;
#ifdef CPU_32BIT 
typedef		unsigned int	U32;
typedef		int				S32;
typedef		unsigned long	U64;
typedef		signed long		S64;
#else
typedef		unsigned long	U32;
typedef		long			S32;
typedef		double			U64;
typedef		double			S64;
#endif
typedef 	float			F32;
typedef 	double			D64;

#define		INT_TYPE		1   
#define		FLOAT_TYPE		2 
#define		IQ_TYPE			3    

#define		DAC_CH1			1
#define		DAC_CH2			2
#define		DAC_CH3			3
#define		DAC_CH4			4

#define 	DAC1_CS			GpioDataRegs.GPBDAT.bit.GPIO55
#define 	DAC2_CS			GpioDataRegs.GPBDAT.bit.GPIO57

/*
********************************************************************************
* GLOBAL VARIABLE
******************************************************************************** 
*/
typedef volatile struct {
    U8  ucCh;
    U8  ucType;
	F32 fValue;
	F32 fScale;
	float *piChData;
} DAC;

extern DAC	deg_Dac [ 4 ];

/*
********************************************************************************
* MACRO
******************************************************************************** 
*/

/*
********************************************************************************
* PROTOTYPES
******************************************************************************** 
*/
extern void dev_DacSetup ( void );
extern void dev_InitDACVariable ( void );
extern void dev_BackgroundDAC ( void );

