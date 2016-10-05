/*******************************************************************************
********************************************************************************
**
** File Name
** ---------
**
** SD.C
**
********************************************************************************
********************************************************************************
**
** Description
** -----------
** This file contains routines for the Serial Interface
**
**
**
********************************************************************************
********************************************************************************
**                                                                            **
** COPYRIGHT NOTIFICATION (c) 1995,96,97,98,99 HMS Industrial Networks AB.    **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorised company official.                                         **
**                                                                            **
********************************************************************************
********************************************************************************
**
** Company: HMS Industrial Networks AB
**          Pilefeltsgatan 93-95
**          S-302 50  Halmstad
**          SWEDEN
**          Tel:     +46 (0)35 - 17 29 00
**          Fax:     +46 (0)35 - 17 29 09
**          e-mail:  info@hms.se
**
********************************************************************************
********************************************************************************
*/

/*******************************************************************************
********************************************************************************
**
** Change Log
** ----------
**
** Latest Revision:
**
**    Rev 0.10    12 jun 2002   Created by AnN
**    Rev 1.00    12 jul 2002   First Release
**
********************************************************************************
********************************************************************************
*/

#include "include.h"




/*******************************************************************************
**
** Public Services
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
** SD_Init()
**------------------------------------------------------------------------------
*/

void SD_Init( void )
{

  UBRR0 = 51;                                   // 19200 bps
  UCSR0A = 0x00;                                // asynchronous normal mode
  UCSR0B = 0x98;                                // interrupt enable, 8 data
  UCSR0C = 0x06;                                // no parity, 1 stop, 8 data
 // RXD0 = UDR0;                                   // dummy read

}/* end SD_Init */


/*------------------------------------------------------------------------------
** SD_PutByte()
**------------------------------------------------------------------------------
*/

void SD_PutChar( UCHAR bByte )
{
  while((UCSR0A & 0x20) == 0x00);		// data register empty ?
  UDR0 = bByte;

}/* end SD_PutChar */


/*------------------------------------------------------------------------------
** SD_GetBuffer()
**------------------------------------------------------------------------------
*/

UCHAR SD_GetChar( void )
{

   UCHAR bReturnChar;

   if( sd_s.iRxSize == 0 )
   {

      /*
      ** There is no characters in the buffer.
      */

      return 0;

   }/* end if */

   bReturnChar = sd_s.abRxBuffer[ sd_s.iRxGet ];


   /*
   ** Take a step so we point to the next character in the rx buffer, then
   ** decrement the usage of the buffer indicating that we have seen the
   ** character.
   */

   sd_s.iRxGet++;
   sd_s.iRxSize--;


   /*
   ** Handle the rotation of our ring buffer
   */

   if( sd_s.iRxGet == SD_RX_BUFFER_SIZE )
   {

      sd_s.iRxGet = 0;

   }/* end if */

   return bReturnChar;


}/* end SD_GetChar */


/*------------------------------------------------------------------------------
** SD_CharReceived()
**------------------------------------------------------------------------------
*/

BOOL SD_CharReceived( void )
{

   /*
   ** Check if there are any characters in the receive buffer.
   */

   if( sd_s.iRxSize == 0 )
   {

      return FALSE;

   }
   else
   {

      return TRUE;

   }/* end if */

} /* end of SD_CharReceived() */



/*******************************************************************************
**
** Private Services
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
** sd_RxInterrupt()
**------------------------------------------------------------------------------
*/


//void sd_RxInterrupt( void ) interrupt 4 using 1
ISR(USART0_RX_vect)		
{

   /*
   ** Wait until Serial Interrupt can be handled
   */

  // while(!RI0)  ;

   /*
   ** Get the character from the input "buffer"
   */

   sd_s.abRxBuffer[ sd_s.iRxPut ] = UDR0;

   /*
   ** go to the next possition in the rx buffer
   */

   sd_s.iRxSize++;
   sd_s.iRxPut++;

   /*
   ** Handle the rotation of our ring buffer
   */

   if( sd_s.iRxPut == SD_RX_BUFFER_SIZE )
   {

      sd_s.iRxPut = 0;

   }/* end if end of buffer */


   /*
   ** Set timeout to 3.5 char to know when a modbus message ends.
   */

   TM_SetTimer( MB_MESSAGE_END_9600 );


}/* end sd_RxInterrupt */


/*******************************************************************************
**
** End of SD.C
**
********************************************************************************
*/
