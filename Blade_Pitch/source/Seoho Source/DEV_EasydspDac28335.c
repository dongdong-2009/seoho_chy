/*
******************************************************************************** 
* Seoho 
* 900-3, HOKYE-2DONG, DONGAN-GU
* ANYANG-CITY, GYEONGGI-DO
* KOREA, 431-836
* 
* Filename : DEV_EasydspDac28335.c
* Programmer(s): 
* Created : 2010/08/19
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
* INCLUDE
******************************************************************************** 
*/
//#include "main_def.h"
#include	<All_Header.h>
#include	<All_Extern_Variables.h>

/*
********************************************************************************
* DEFINE
******************************************************************************** 
*/

/*
********************************************************************************
* GLOBAL VARIABLE
******************************************************************************** 
*/
DAC	deg_Dac [ 4 ];

/*
********************************************************************************
* LOCAL VARIABLE
******************************************************************************** 
*/
S16 deg_sDacTmp;

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
void dev_SendDAC ( DAC *pDac );
void DAC_Out(WORD ch1, WORD ch2);

void dev_DacSetup ( void )
{
	EALLOW;
/*
	// nDAC1_CS pin setup
	GpioCtrlRegs.GPBMUX2.bit.GPIO55  = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55   = 1;

	// nDAC2_CS pin setup
	GpioCtrlRegs.GPBMUX2.bit.GPIO57  = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO57   = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO54   = 0;     // Enable pull-up on GPIO24 (SPISIMOB)                                                  
    GpioCtrlRegs.GPBPUD.bit.GPIO56   = 0;     // Enable pull-up on GPIO26 (SPICLKB) 
    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3;     // Asynch input GPIO24 (SPISIMOB)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3;     // Asynch input GPIO26 (SPICLKB) 
    GpioCtrlRegs.GPBMUX2.bit.GPIO54  = 1;     // Configure GPIO24 as SPISIMOB
    GpioCtrlRegs.GPBMUX2.bit.GPIO56  = 1;     // Configure GPIO26 as SPICLKB         
*/

	SpiaRegs.SPICCR.bit.SPISWRESET   = 0;	   // SPI SW RESET = 0
	SpiaRegs.SPICCR.all              = 0x004F; // Reset on, falling edge, 16-bit char bits  0100 1111 
	SpiaRegs.SPICTL.all              = 0x0006; // Enable master mode, normal phase,         0000 0110
	SpiaRegs.SPICTL.bit.CLK_PHASE    = 1;     
	SpiaRegs.SPICCR.bit.CLKPOLARITY  = 1;
 	SpiaRegs.SPIBRR                  = 5;	    // 6.25MHz							
    SpiaRegs.SPIPRI.bit.FREE         = 1;      // Set so breakpoints don't disturb xmission
	SpiaRegs.SPICTL.bit.SPIINTENA    = 1;
	SpiaRegs.SPICCR.bit.SPISWRESET   = 1;      // SPI SW RESET = 1
 
    EDIS;
}

void dev_InitDACVariable ( void )
{
	char i;

	DAC	*deg_pDac;
	deg_pDac = &deg_Dac [ 0 ]; 

	for ( i = 0; i < 4; i++ )
	{
		deg_pDac->ucCh   = i + 1;
		deg_pDac->ucType = FLOAT_TYPE;
 		deg_pDac->fValue = 0;
		deg_pDac->fScale = 1;
		deg_pDac++;	
	}
}

unsigned short sTemp = 0;
//unsigned short ttt1 = 0;

void DAC_Out(WORD ch1, WORD ch2)
{
	WORD d;

	// 채널 1_A에  데이터 보내기(Output Gain x2)
		DAC1_CS = 0;
		d = (ch1 & 0xfff) | 0xC000;
		SpiaRegs.SPIDAT = d;
		while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
		d = SpiaRegs.SPIRXBUF; // clear SPI INT Flag
		DAC1_CS = 1;

	// 채널 1_B에 데이터 보내기(Output Gain x2)

		DAC1_CS = 0;
		d = (ch2 & 0xfff) | 0x4000;
		SpiaRegs.SPIDAT = d;
		while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
		d = SpiaRegs.SPIRXBUF; // clear SPI INT Flag
		DAC1_CS = 1;	
	// Set /LDAC signal timing according to MCP4822 DAC device datasheet

}




void dev_SendDAC ( DAC *pDac )
{

	static	S16 sTemp;


//	sTemp = ( int )( (float)( ( pDac->fValue * 1638.4 ) + 1638.4 ) );


    sTemp =  ( int )( (float)( ( pDac->fValue * 2048) + 2048 ));

//	sTemp = ( sTemp > 3276) ? 3276 : ( sTemp <= 0 ) ? 0 : sTemp;

	sTemp = ( sTemp > 4095) ? 4095 : ( sTemp <= 0 ) ? 0 : sTemp;


/*
	if ( sTemp < 3000 )
	{
		sTemp += 20;
	}
	else
	{
		sTemp = 0;
	}
*/

	switch ( pDac->ucCh )
	{
		case DAC_CH1 : 
		{
			DAC1_CS = 0;
			SpiaRegs.SPITXBUF = 0xC000 | sTemp & 0xfff;
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;			// empty RX buffer data  : to clear INT FLAG
			DAC1_CS = 1;
			break;
		}
		case DAC_CH2 : 
		{
			DAC1_CS = 0;
			SpiaRegs.SPITXBUF = 0x4000 | sTemp & 0xfff;
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC1_CS = 1;
			break;
		}
		case DAC_CH3 : 
		{
			DAC2_CS = 0;
			SpiaRegs.SPITXBUF = 0xC000 | sTemp  & 0xfff;
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC2_CS = 1;
			break;
		}
		case DAC_CH4 : 
		{
			DAC2_CS = 0;
			SpiaRegs.SPITXBUF = 0x4000 | sTemp  & 0xfff;
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC2_CS = 1;
			break;
		}
		default : 
			break;
	}

}

void dev_BackgroundDAC ( void )
{
	U8 i;
	DAC *deg_pDac;
	double Temp;
	Temp= (double)abs_position-1350.;
	

	
//deg_Dac[0].piChData=&sin_theta;
//	deg_Dac[0].piChData=&Wrpm_ref;
	deg_Dac[0].piChData=&Idse_ref;
	deg_Dac[0].fScale = 25;
	deg_Dac[0].ucType = 2;

//	deg_Dac[1].piChData=&cos_theta;
//	deg_Dac[1].piChData=&Wrpm_det;
	deg_Dac[1].piChData=&Iqse_ref;
	deg_Dac[1].fScale = 25;
	deg_Dac[1].ucType = 2;

//	deg_Dac[2].piChData=&sin_theta;
	deg_Dac[2].piChData=&Wrpm_det;
//	deg_Dac[2].piChData=&test6;
	deg_Dac[2].fScale = 5;
//	deg_Dac[2].fScale = 4100;
	deg_Dac[2].ucType = 2;

//	deg_Dac[3].piChData=&cos_theta;
//	deg_Dac[3].piChData=&Temp_HHH3;
	deg_Dac[3].piChData=&Iqse;
//	deg_Dac[3].piChData=&etest1;
	deg_Dac[3].fScale = 25;   // 50;
	deg_Dac[3].ucType = 2;


	deg_pDac = &deg_Dac [0];


	for ( i = 0; i < 4; i++ )
	{
		switch ( deg_pDac->ucType )
		{
			case INT_TYPE	: deg_pDac->fValue= *( int *)   deg_pDac->piChData / ( int )   deg_pDac->fScale;    break;
//			case FLOAT_TYPE	: deg_pDac->fValue= *( float *) deg_pDac->piChData / ( float ) deg_pDac->fScale;	break;
			case FLOAT_TYPE	: deg_pDac->fValue= *( double *) deg_pDac->piChData / ( double ) deg_pDac->fScale;	break;
			default 		: 																					break;
		}
//	deg_pDac->fValue=300;

		dev_SendDAC ( deg_pDac );
//	DAC_Out(0xfff,0xfff);
		deg_pDac++;
	}
}

