/*******************************************************************************
********************************************************************************
**
** File Name
** ---------
**
** MB.C
**
********************************************************************************
********************************************************************************
**
** Description
** -----------
** This file contains routines for sending Modbus messages
** on the serial interface
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

#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // Examples Include File

#include "fd.h"

#include "abic.h"
#include "mb.h"
#include "crc.h"

/*******************************************************************************
**
** Public Globals
**
********************************************************************************
*/

UCHAR MB_bCRCCounter;
UCHAR MB_bTimeOutCounter;

/*******************************************************************************
**
** Private Globals
**
********************************************************************************
*/

UINT32 MB_iTimeOutTime;


/*******************************************************************************
**
** Public Services
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
** MB_Init()
**------------------------------------------------------------------------------
*/


void TM_StartTimer()
{
   MB_iTimeOutTime = 0;

}/* end TM_StartTimer */



BOOL TM_TimeOut()
{
	if( MB_DEFAULT_TIMEOUT < MB_iTimeOutTime )	return( TRUE );
	else
	{
		//MB_iTimeOutTime++;
		 return( FALSE );
	}

}


/*------------------------------------------------------------------------------
** MB_SendModbusMessage()
**------------------------------------------------------------------------------
*/

UCHAR MB_SendRecModbusMessage( UCHAR* pbData, UCHAR bSize, UCHAR* pbResponse )
{

   UCHAR abSendBuffer[ 30 ]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
   UINT16 iCrc;
   UCHAR bCharPos = 0;


   /*
   ** Check if the message is to long
   */

   if( bSize > 249 )
   {

      return( 0 );

   }/* end if message to long */

   /*
   ** Copy Data To send buffer
   */

   for( bCharPos = 0 ; bCharPos < bSize ; bCharPos++ )
   {

      abSendBuffer[ bCharPos ] = pbData[ bCharPos ];

   }/* end for */

   /*
   ** Generate CRC for the message to send
   */

   iCrc = GenerateCrc( abSendBuffer, bCharPos );

   /*
   ** Add CRC to the end of the message to send
   */

   abSendBuffer[ bCharPos ] = (( iCrc >> 8 ) & 0x00FF);
   abSendBuffer[ bCharPos + 1] = (iCrc & 0x00FF);

   /*
   ** Send Modbus Request
   */
#if 1
   for( bCharPos = 0; bCharPos < ( bSize + 2 ) ; bCharPos++ )
   {

      scib_putc( abSendBuffer[ bCharPos ] );

   }/* end for */
#endif



   TM_StartTimer();
   while( !TM_TimeOut() );


   /*
   ** Read response
   */

   bCharPos = 0;

   while( SD_CharReceived() )
   {

      pbResponse[ bCharPos ] = SD_GetChar();

      bCharPos++;

   }/* end while rx buffer not emty */


   if( bCharPos == 0 )
   {

      MB_bTimeOutCounter++;

   }
   else if( bCharPos == 1 )
   {

      MB_bCRCCounter++;

   }/* end if */


   if( bCharPos > 1 )
   {

      /*
      ** Generate CRC for the response message
      */

      iCrc = GenerateCrc( pbResponse, bCharPos - 2 );


      /*
      ** Check CRC
      */

      if( ( ( (UCHAR)( iCrc >> 8 ) ) == pbResponse[ bCharPos - 2 ] ) && ( ( (UCHAR)iCrc ) == pbResponse[ bCharPos - 1 ] ) )
      {

         /*
         ** Correct CRC return received length
         */

         return( bCharPos - 2 );

      }
      else
      {
		return( bCharPos - 2 );


         /*
         ** CRC error
         */

         MB_bCRCCounter++;

      }/* end if right crc */

   }/* end if response received */


   /*
   ** We failed to get a error free response
   */

   return( 0 );


}
