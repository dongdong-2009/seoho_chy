/*******************************************************************************
********************************************************************************
**
** File Name
** ---------
**
** TM.C
**
********************************************************************************
********************************************************************************
**
** Description
** -----------
** This file contains timer routines used to determin modbus timouts.
** - 3.5 char timout ( end of message )
** - Response timout ( no response )
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

#include "fd.h"
#include "tm.h"
#include "sd.h"
#include "crc.h"
#include "Reg517a.h"

/*******************************************************************************
**
** Public Globals
**
********************************************************************************
*/

UINT16 TM_iResponseTime;
UINT16 TM_iTimeOutTime;


/*******************************************************************************
**
** Public Services
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
** TM_StartTimer()
**------------------------------------------------------------------------------
*/

void TM_StartTimer()
{

   /*
   ** Timer 0 start
   */

   TR0=1;      /* Timer Start                 */
   TF0=0;      /* Clear timer flag            */
   ET0=0;      /* Disable interrupt           */

   TM_iResponseTime = 0;

}/* end TM_StartTimer */


/*------------------------------------------------------------------------------
** TM_SetTimer()
**------------------------------------------------------------------------------
*/

void TM_SetTimer( UINT16 iTime )
{

   /*
   ** If this is the first SetTimer since starttimer we should calculate
   ** the response time.
   */

   if( TM_iResponseTime == 0 )
   {

      TM_iResponseTime = ( ( TH0 << 8 ) + TL0 ) - ( 0xFFFF - TM_iTimeOutTime );

   }/* end if */

   TM_iTimeOutTime = iTime;


   /*
   ** Set new timout time
   */

   TH0 = ( 0xFFFF - iTime ) >> 8;
   TL0 = ( 0xFFFF - iTime );

   /*TH0=0xD8;    /* 10 ms: FFFF-D8EF=2710 (dec.10000) */
   /*TL0=0xEF;*/



}/* end TM_SetTimer */


/*------------------------------------------------------------------------------
** TM_SetTimer()
**------------------------------------------------------------------------------
*/

void TM_StopTimer()
{

   ET0=0;               /* Disable interrupt           */
   TR0=0;               /* Timer stop                  */
   TF0=0;               /* Clear timer flag            */

   if( TM_iResponseTime == 0 )
   {

      TM_iResponseTime = ( ( TH0 << 8 ) + TL0 ) - ( 0xFFFF - TM_iTimeOutTime );

   }/* end if */

}/* end TM_StopTimer */


/*------------------------------------------------------------------------------
** TM_TimeOut()
**------------------------------------------------------------------------------
*/

BOOL TM_TimeOut()
{

   /*
   ** Wait for response
   */

   if( TF0 )
   {

      /*
      ** Timer 0 stop
      */

      TR0 = 0;
      return( TRUE );

   }
   else
   {

      return( FALSE );

   }/* end if timer timed out */

}/* end TM_TimeOut */


/*******************************************************************************
**
** End of TM.C
**
********************************************************************************
*/