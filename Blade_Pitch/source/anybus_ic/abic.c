/*******************************************************************************
********************************************************************************
**
** File Name
** ---------
**
** ABIC.C
**
********************************************************************************
********************************************************************************
**
** Description
** -----------
** This file contains functions for interfacing the AnyBus-IC
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
#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // Examples Include File

#include "fd.h"
#include "mb.h"
#include "abic.h"


/*******************************************************************************
**
** Public Services
**
********************************************************************************
*/

UCHAR abCheckRequest[ 6 ] = { 0x01, 0x04, 0x50, 0x01, 0x00, 0x01 } ;
UCHAR abSetRequest[ 6 ] = { 0x01, 0x06, 0x50, 0x01, 0x00, 0x01 } ;
UCHAR abRequest[ 11] = { 0x01, 0x03, 0x50, 0x01, 0x00, 0x01,0,0,0,0,0 } ;
UCHAR abResponse[ 10 ] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
/*------------------------------------------------------------------------------
** ABIC_AutoBaud()
**------------------------------------------------------------------------------
*/

BOOL  ABIC_AutoBaud( void )
{
 
  
   
   UCHAR bRetryCount;

   /*
   ** Do 15 tries to make the AnyBus-IC Autobaud.
   ** If no succes return FALSE
   */

   for( bRetryCount = 0; bRetryCount < 15 ; bRetryCount++ )
   {

      /*
      ** We send a request to read modbus address 0x5001 ( Parameter #1 )
      ** The response should be 6 byte long.
      */

      if( MB_SendRecModbusMessage( abRequest, 6, abResponse) == 5 )
      {

         /*
         ** We got an response on the AutoBaud Message,
         ** the AutoBaud is complete. Return TRUE,
         */

         return( TRUE );

      }/* end if Response received */

   }/* end for */

   return( FALSE );

}/* end AutoBaud */


/*------------------------------------------------------------------------------
** ABIC_NormalMode()
**------------------------------------------------------------------------------
*/

BOOL ABIC_NormalMode( void )
{

   /*
   ** Try to AnyBus-IC in Normal mode
   */

   if( MB_SendRecModbusMessage( abSetRequest, 6, abResponse ) != 6 )
   {

      return( FALSE );

   }/* end if */

   /*
   ** Check if the AnyBus-IC is in Normal mode.
   ** For different reasons the attemt to set it in normal mode can
   ** have failed. For example faulty IO configuration.
   */

   if( MB_SendRecModbusMessage( abCheckRequest, 6, abResponse ) == 6 )
   {

      /*
	  ** Byte 4 and 5 contains the parameter value and it should be 0x0001
	  */

      if( abResponse[ 4 ] == 0 && abResponse[ 5 ] == 1 )
      {

         return( TRUE );

      }/* end if */

   }/* end if */


   /*
   ** We faild to read the current mode
   */

   return( FALSE );


}/* end ABIC_NormalMode */


/*------------------------------------------------------------------------------
** ABIC_ReadOutData()
**------------------------------------------------------------------------------
*/
UCHAR bReadSize;
unsigned int ReadStep=0;
unsigned int ReadDoneFlag = 0;
BOOL ABIC_ReadOutData( UCHAR bOffset, UCHAR bSize, UCHAR* pData )
{

	   UCHAR bCount;
	
	 //  UCHAR abRequest[ 6 ];
	  UCHAR abSendBuffer[ 30 ]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
	   UINT16 iCrc;
	   UCHAR bCharPos = 0;

	if(ReadStep == 0)
	{
		if(!ReadDoneFlag) ReadStep = 1;
	}
	else if(ReadStep == 1)
	{
		/*
		** Set up Read Input registers request
		*/

		abRequest[ 0 ] = 0x01;           /* Modbus Address           */
		abRequest[ 1 ] = 0x04;           /* Modbus Function Code     */
		abRequest[ 2 ] = 0x10;           /* Modbus Address High Byte */
		abRequest[ 3 ] = bOffset;        /* Modbus Address Low Byte  */
		//abRequest[ 2 ] = 0x50;           /* Modbus Address High Byte */
		//abRequest[ 3 ] = 0x01;        /* Modbus Address Low Byte  */
		abRequest[ 4 ] = 0x00;           /* No. of Points High       */
		abRequest[ 5 ] = bSize;          /* No. of Points Low        */

		/*
		** Send the Request
		*/

		bSize = 6;

		/*
		** Check if the message is to long
		*/

		if( bSize > 249 )
		{

			ReadStep = 0;
			//return( 0 );

		}/* end if message to long */

		/*
		** Copy Data To send buffer
		*/

		for( bCharPos = 0 ; bCharPos < bSize ; bCharPos++ )
		{

			abSendBuffer[ bCharPos ] = abRequest[ bCharPos ];

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

		for( bCharPos = 0; bCharPos < ( bSize + 2 ) ; bCharPos++ )     scib_putc( abSendBuffer[ bCharPos ] );
	   
		MB_iTimeOutTime = 0;

		ReadStep = 2;

	}
	else if(ReadStep == 2)
	{
		if( MB_DEFAULT_TIMEOUT < MB_iTimeOutTime )ReadStep = 3;
		else 	MB_iTimeOutTime++;

	}
	else if(ReadStep == 3)//** Read response
	{
		bCharPos = 0;

		while( SD_CharReceived() )
		{
			  abResponse[ bCharPos ] = SD_GetChar();
			  bCharPos++;
		}/* end while rx buffer not emty */

		if( bCharPos == 0 ) MB_bTimeOutCounter++;
		else if( bCharPos == 1 )   MB_bCRCCounter++;

		if( bCharPos > 1 )
		{
			iCrc = GenerateCrc( abResponse, bCharPos - 2 );//Generate CRC for the response message

			//Check CRC
			if( ( ( (UCHAR)( iCrc >> 8 ) ) == abResponse[ bCharPos - 2 ] ) && ( ( (UCHAR)iCrc ) == abResponse[ bCharPos - 1 ] ) )
			{
				ReadStep = 4;
				bReadSize =  bCharPos - 2 -3;//Correct CRC return received length
			}
			else
			{
				ReadStep = 4;
				bReadSize =  bCharPos - 2 -3;

				MB_bCRCCounter++; //CRC error
			}/* end if right crc */

		}/* end if response received */
	}
	else if(ReadStep == 4)
	{
		if( bReadSize == ( bSize * 2 ) )
		{

			for( bCount = 0; bCount < ( bSize * 2 ) ; bCount++ )
			{

				 pData[ bCount ] = abResponse[ bCount + 3 ];
				 ReadDoneFlag = 1;

			}/* end for */

		}/* end if right size read */

		
		ReadStep = 0;
	}
}/* end ABIC_ReadOutData */


/*------------------------------------------------------------------------------
** ABIC_WriteInData()
**------------------------------------------------------------------------------
*/
unsigned int WriteStep=0;
unsigned int WriteDoneFlag = 0;
BOOL ABIC_WriteInData( UCHAR bOffset, UCHAR bSize, UCHAR* pData )
{

   UCHAR bCount;
   //UCHAR abRequest[ 11 ];

   UCHAR abSendBuffer[ 30 ]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
   UINT16 iCrc;
   UCHAR bCharPos = 0;


	if(WriteStep == 0)
	{
		if(!WriteDoneFlag) WriteStep = 1;
	}
	else if(WriteStep == 1)
	{
   
		/*
		** Set up Preset Multiple Registers request
		*/

		abRequest[ 0 ] = 0x01;           /* Modbus Address           */
		abRequest[ 1 ] = 0x10;           /* Modbus Function Code     */
		abRequest[ 2 ] = 0x00;           /* Starting Address High    */
		abRequest[ 3 ] = bOffset;        /* Starting Address Low     */
		abRequest[ 4 ] = 0x00;           /* No. of Registers High    */
		abRequest[ 5 ] = bSize;          /* No. of Registers Low     */
		abRequest[ 6 ] = bSize * 2;      /* Byte Count               */

		/*
		** Copy Data to Request
		*/

		for( bCount = 0; bCount < ( bSize * 2 ) ; bCount++ )
		{

		  abRequest[ bCount + 7 ] = pData[ bCount ];

		}/* end for */

		/*
		** Send the Modbus Request and check if the response size is
		** the length of a Preset Multiple Register Response ( 6 )
		*/

		//if( MB_SendRecModbusMessage( abRequest, 7 + ( 2 * bSize ), abResponse ) == 6 )


		/*
		** Check if the message is to long
		*/
		bSize =  7 + ( 2 * bSize );

		if( bSize > 249 )
		{

			WriteStep = 0;
			//return( 0 );

		}/* end if message to long */

		/*
		** Copy Data To send buffer
		*/

		for( bCharPos = 0 ; bCharPos < bSize ; bCharPos++ )
		{

		  abSendBuffer[ bCharPos ] = abRequest[ bCharPos ];

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

		for( bCharPos = 0; bCharPos < ( bSize + 2 ) ; bCharPos++ )
		{

		  scib_putc( abSendBuffer[ bCharPos ] );

		}/* end for */

		WriteStep = 2;

	}
	else if(WriteStep == 2)
	{
		if( MB_DEFAULT_TIMEOUT < MB_iTimeOutTime )WriteStep = 3;
		else 	MB_iTimeOutTime++;

	}
	else if(WriteStep == 3)//** Read response
	{
		bCharPos = 0;

		while( SD_CharReceived() )
		{
			  abResponse[ bCharPos ] = SD_GetChar();
			  bCharPos++;
		}/* end while rx buffer not emty */

		if( bCharPos == 0 ) MB_bTimeOutCounter++;
		else if( bCharPos == 1 )   MB_bCRCCounter++;

		if( bCharPos > 1 )
		{
			iCrc = GenerateCrc( abResponse, bCharPos - 2 );//Generate CRC for the response message

			//Check CRC
			if( ( ( (UCHAR)( iCrc >> 8 ) ) == abResponse[ bCharPos - 2 ] ) && ( ( (UCHAR)iCrc ) == abResponse[ bCharPos - 1 ] ) )
			{
				bCount =  bCharPos - 2;//Correct CRC return received length
			}
			else
			{
				bCount =  bCharPos - 2;

				MB_bCRCCounter++; //CRC error
			}/* end if right crc */

		}/* end if response received */

		
		WriteStep = 0;

		WriteDoneFlag = 1;
	}
}/* end ABIC_ReadOutData */