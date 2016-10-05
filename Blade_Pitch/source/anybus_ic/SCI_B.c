//###############################################################################################################
// 파일명   : SCI_B.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : F28335 교육용 키트인 DSP28335 DSK의 SCI-B 관련 구현 코드 파일
// 
// [ 사용한 개발 환경 ]
// 1) TI CCS3.3과 Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe 파일) - 2008.10.20
// 2) CCS3.3 Setup용 Target Driver Package(setupCCSPlatinum_v30329.exe 파일) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe 파일) - 2008.10.20
//
// [ 참조한 Software Developer Kit(SDK) ]
// 1) TI의 SDK인 C/C++ Header Files and Peripheral Examples v1.20 사용 - 2008.10.20
//
// [ Revision History ]
//
//###############################################################################################################

#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // Examples Include File

#include "fd.h"

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// SCI-B Interrupt Service Function 선언
#pragma CODE_SECTION(scib_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scib_rx_isr, "ramfuncs");

interrupt void scib_tx_isr(void);
interrupt void scib_rx_isr(void);

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIB_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-B
#define	SCIB_BAUDRATE	38400L								// SCI-B Baudrate
#define	SCIB_BRR_VAL	(SCIB_LSPCLK/(8*SCIB_BAUDRATE)-1)	// SCI-B BaudRate 설정 Register 값

/* BPS 에러율 *****************************************************************
*    BPS	   CPUCLK	  LSPCLK	         BRR_VAL	     BPS	error
*   4800	150000000	37500000	975.5625	976 	4797.851 	-0.045 
*   9600	150000000	37500000	487.28125	487 	9605.533 	0.058 
*  19200	150000000	37500000	243.140625	243 	19211.066 	0.058 
*  38400	150000000	37500000	121.0703125	121 	38422.131 	0.058 
*  57600	150000000	37500000	80.38020833	80 		57870.370 	0.469 
* 115200	150000000	37500000	39.69010417	40 		114329.268 	-0.756 
******************************************************************************/

#define	SCIB_BUF_SIZE	50

#define	SCIB_TX_START	{	if(ScibRegs.SCICTL2.bit.TXRDY){						\
								ScibRegs.SCICTL2.bit.TXINTENA=1;				\
								ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];	\
								if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos=0;	\
							}													\
							else ScibRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIB_TX_STOP	ScibRegs.SCICTL2.bit.TXINTENA=0

#define SD_RX_BUFFER_SIZE     10


/* Variables for Serial Communication  */
char scib_tx_buf[SCIB_BUF_SIZE+1];
char scib_tx_pos=0, scib_tx_end=0;
char scib_rx_buf[SCIB_BUF_SIZE+1];
char scib_rx_pos=0, scib_rx_end=0;
char scib_rxd=' ';


typedef struct sd_DataType
{

   BOOL fEnabled;
   UINT8 abRxBuffer[ SD_RX_BUFFER_SIZE ];
   UINT16 iRxGet;
   UINT16 iRxPut;
   UINT16 iRxSize;
   UINT16 iTxGet;

   UINT16 iTxSize;

}
sd_DataType;

/*******************************************************************************
**
** Private Globals
**
********************************************************************************
*/

sd_DataType sd_s;


unsigned int tttt=0;

// 현재 SCI-B 포트에 수신된 데이터(ex08_can.c 파일에 변수 정의됨)
extern char scib_rxd; 

void scib_putc(char d){
	scib_tx_buf[scib_tx_end++] = d;
	if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;

	SCIB_TX_START;
}

void scib_puts(char* p){
char rd;
	while(rd = *p++){             
		scib_tx_buf[scib_tx_end++] = rd;
		if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
	}


	SCIB_TX_START;
}




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




interrupt void scib_tx_isr(void){
	if(scib_tx_pos != scib_tx_end){
		ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];
		if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos = 0;
	}
	else{                              
		SCIB_TX_STOP;
	}

	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


interrupt void scib_rx_isr(void){

   sd_s.abRxBuffer[ sd_s.iRxPut ] = ScibRegs.SCIRXBUF.all;

	//tttt = ScibRegs.SCIRXBUF.all;

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


	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// SCI-B 초기화 함수
void scib_init(){
	ScibRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScibRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScibRegs.SCICCR.all = 0x0007;  		// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
	ScibRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

    ScibRegs.SCIHBAUD = SCIB_BRR_VAL >> 8;
    ScibRegs.SCILBAUD = SCIB_BRR_VAL & 0xff;

	ScibRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-B RX interrupt
    EALLOW;
	PieVectTable.SCIRXINTB = &scib_rx_isr;
	PieVectTable.SCITXINTB = &scib_tx_isr;

#if 0
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0; // Enable pull-up for GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;  // Enable pull-up for GPIO9  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation
#endif
//========================================================================================
//========================================================================================
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0; // Enable pull-up for GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up for GPIO18  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;    // Configure GPIO18 for SCITXDB operation
	EDIS;
//========================================================================================
//========================================================================================
    // Enable CPU INT9 for SCI-B
	IER |= M_INT9;
	
    // Enable SCI-B RX INT in the PIE: Group 9 interrupt 3
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

    // Enable SCI-B TX INT in the PIE: Group 9 interrupt 4
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
}
