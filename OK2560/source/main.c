//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : main.c
//	description : 메인루프
//	last revision : 2008-05-09 4:02오후
//	history : 
//
//====================================================

#include "include.h"





void MCU_initialize(void)			/* initialize ATmege1280 MCU & OK-1280 kit */
{ 
	  XMCRA = 0xC1;					// 0x2200-0x7FFF=1 wait, 0x8000-0xFFFF=0 wait
	  XMCRB = 0x80;					// enable bus keeper, use PC0-PC7 as address

	  DDRB = 0xFF;					// PORTB = output (OC1A, OC1B, OC1C/OC0A)
	  PORTB = 0x00;
	  DDRE = 0x0C;					// PORTE3~2 = output (PE = buzzer)
	  PORTE = 0x00;					//    * buzzer off
	  DDRH = 0xFF;					// PORTH = output (LCD/GLCD control)
	  PORTH = 0x00;					//    * clear LCD/GLCD control signal
	  DDRJ = 0xFF;					// PORTJ = output (LCD/GLCD/7-segment data)
	  PORTJ = 0x00;
	  DDRK = 0xFF;					// PORTK = output (7-segment digit select)
	  PORTK = 0x00;					//    * clear 7-segment LED
	  DDRL = 0x0F;					// PORTL7-4 = input (KEY4-KEY1)
	  PORTL = 0x00;					// PORTL3-0 = output(LED4-LED1)  * clear
}



int main(void)
{
	unsigned char i;


   BOOL fResult;
   UINT8 bDisplayState = 0;
   UINT8 abInData[ 2 ];
   UINT8 abOutData[ 2 ];

   UINT8 bLoopCounter = 0;
   UINT8 bWriteErrorCounter = 0;
   UINT8 bReadErrorCounter = 0;

	

  MCU_initialize();                             // initialize MCU and kit
  _delay_ms(50);                                 // wait for system stabilization
  CLCD_initialize();                             // initialize text LCD module

  GLCD_clear();		               		// initialize GLCD screen

  
	GLCD_print0508(0,0,"    SEOHO ELEC. 2560KIT  ");	// display screen 1
	GLCD_print0508(0,1,"  B'D TEST Program V1.0  ");
	GLCD_print0508(0,2,"                         ");
	GLCD_print0508(0,3,"   - CANOPEN COMM TEST -");
	GLCD_print0508(0,4,"        2010/07/20       ");
	GLCD_print0508(0,5,"                         ");
	GLCD_print0508(0,6,"       Programed by      ");
	GLCD_print0508(0,7,"   Paul Byung-Hun Park   ");

	GLCD_FullUpdate();


	CLCD_string(0x80,"KEY -> serial   ");          // display title
	CLCD_string(0xC0,"                ");

//	UART0_init();


//TX0_string("DrawImage 'logo1.bmp',150,50;");// TFT LCD에 로고 띄우기(HMR-380)


   /*
   ** Initiate Serial Driver
   */
   SD_Init();
  sei();// 수신인터럽트 enable

   /*
   ** Make the AnyBus-IC autodetect our Baud Rate
   */
   fResult = ABIC_AutoBaud();


   if( !fResult )
   {

      /*
      ** The baud rate auto detection failed.
      ** Write error message and stay here for ever
      */

      CLCD_string(0xC0, "AutoBaud fault!");

      while( 1 )
      ;

   }/* end if */


   /*
   ** When the AnyBus-IC is configured and ready we will
   ** have to start the data excange by setting the Module Mode
   ** parameter ( #1 ) to 1 ( Normal Operation ).
   ** In this sample we assume that the configuration already are done
   ** and stored in flash. Then the only thing we have to do is to set
   ** the module in Normal Operational mode.
   */

   if( ABIC_NormalMode() )
   {

      /*
      ** The baud rate auto detection failed.
      ** Write error message and stay here for ever
      */

      CLCD_string(0xC0, "Set Mode Fault!");

      while( 1 )
      ;

   }/* end if */


   while( 1 )
   {

      /*
      ** Get indata values from the CPU AD converter
      */
      abInData[ 0 ] = 0xAB;
      abInData[ 1 ] = 0xCD;


      /*
      ** Write the In Data values to the AnyBus-IC
      */
      ABIC_WriteInData( 0, 1, abInData );



      /*
      ** Read the Out Data values from the AnyBus-IC
      */
      fResult = ABIC_ReadOutData( 0, 1, abOutData );

      if( fResult )
      {

         /*
         ** Print the Out Data values on the display
         */

			CLCD_command(0xC0);
			if((abOutData[ 0 ]>>4)<=9)CLCD_data((abOutData[ 0 ]>>4)+'0');
			else CLCD_data((abOutData[ 0 ]>>4)-10+'A');
			
			CLCD_command(0xC1);
			if((abOutData[ 0 ] & 0x0F)<=9)CLCD_data((abOutData[ 0 ] & 0x0F)+'0');
			else CLCD_data((abOutData[ 0 ] & 0x0F)-10+'A');

			CLCD_command(0xC2);
			if((abOutData[ 1 ]>>4)<=9)CLCD_data((abOutData[ 1 ]>>4)+'0');
			else CLCD_data((abOutData[ 1 ]>>4)-10+'A');
			
			CLCD_command(0xC3);
			if((abOutData[ 1 ] & 0x0F)<=9)CLCD_data((abOutData[ 1 ] & 0x0F)+'0');
			else CLCD_data((abOutData[ 1 ] & 0x0F)-10+'A');

		 

      }
   //   else
    //  {

      //   /*
       //  ** We could not get correct Out Data
        // */
        // CLCD_string( 0xC0,"----");

      //}/* end if */


_delay_ms(500);
_delay_ms(500);
_delay_ms(500);
_delay_ms(500);

   }/* end main loop */



/*
	while(1)
	{
		KeyProc();

		if(KeyPara.KeyValue != 0xFF)
		{
		//	_delay_ms(500);
			SD_PutChar(KeyPara.KeyValue);
			CLCD_command(0xC0);
			if((KeyPara.KeyValue>>4)<=9)CLCD_data((KeyPara.KeyValue>>4)+'0');
			else CLCD_data((KeyPara.KeyValue>>4)-10+'A');
			
			CLCD_command(0xC1);
			if((KeyPara.KeyValue & 0x0F)<=9)CLCD_data((KeyPara.KeyValue & 0x0F)+'0');
			else CLCD_data((KeyPara.KeyValue & 0x0F)-10+'A');
		}

	}
*/
}
