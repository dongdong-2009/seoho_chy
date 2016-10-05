//###############################################################################################################
// 파일명   : SCI_C_usb.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : DSP28335 DSK KIT의 SCI-C에 연결된 FTDI FT232R USB 소자 관련 구현 코드 파일
//            사용자께서는 사용에 앞서 제공된 CD의 USB Driver 폴더에 있는 USB Device Driver를 먼저 설치하시길
//            바랍니다. 제공된 USB Device Driver를 설치하시면 사용자 PC에 가상 시리얼 포트가 생성됩니다.
//            제어판 장치관리자의 포트 정보에서 생성된 가상 시리얼 포트가 무엇인지 확인하십시요.
//            그리고나서 하이퍼 터미널 프로그램을 사용하여 생성된 가상 시리얼 포트를 열고 테스트하시면 됩니다.
//
//            [ FTDI FT232R USB소자에서 지원하는 BaudRate ]
// 			  #define FT_BAUD_300			300
//			  #define FT_BAUD_600			600
//			  #define FT_BAUD_1200			1200
//			  #define FT_BAUD_2400			2400
//			  #define FT_BAUD_4800			4800
//			  #define FT_BAUD_9600			9600
//			  #define FT_BAUD_14400			14400
//			  #define FT_BAUD_19200			19200
//			  #define FT_BAUD_38400			38400
//			  #define FT_BAUD_57600			57600
//			  #define FT_BAUD_115200		115200
//			  #define FT_BAUD_230400		230400
//			  #define FT_BAUD_460800		460800
//			  #define FT_BAUD_921600		921600  
//
//			  사용자께서는 아래의 정의된 SCIC_BAUDRATE 값에 맞춰 사용하시길 바라며,
//            이때 사용하실 Baudrate의 에러율을 고려하여 선택하십시요.
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

#include "DSK2833x_Define.h"

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// SCI-C 포트의 Interrupt Service Function 선언
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");

interrupt void scic_tx_isr(void);
interrupt void scic_rx_isr(void);

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIC_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-C
#define	SCIC_BAUDRATE	115200L								// Baudrate(아래의 에러율을 고려하여 정의함)
#define	SCIC_BRR_VAL	(SCIC_LSPCLK/(8*SCIC_BAUDRATE)-1)	// SCI-C BaudRate 설정 Register 값

/* BPS 에러율 ***************************************************************************************
*    BPS	   CPUCLK	   LSPCLK	            BRR_VAL	   calc. BPS	 error
*   4800    150000000    37500000    975.5625	    976    4797.851 	 -0.045 
*   9600	150000000	 37500000	 487.28125	    487    9605.533 	 0.058 
*  19200	150000000	 37500000	 243.140625	    243    19211.066 	 0.058 
*  38400	150000000	 37500000	 121.0703125    121    38422.131 	 0.058 
*  57600	150000000	 37500000	 80.38020833	80 	   57870.370 	 0.469 
* 115200	150000000	 37500000	 39.69010417	40 	   114329.268    -0.756
* 230400    150000000    37500000    19.34505208    19     234375        1.725
* 460800    150000000    37500000    9.172526042    9      468750        1.725
* 921600    150000000    37500000    4.086263021    4      937500        1.725
****************************************************************************************************/

#define	USB_BUF_SIZE	50

#define	USB_TX_START	{	if(ScicRegs.SCICTL2.bit.TXRDY){						\
								ScicRegs.SCICTL2.bit.TXINTENA=1;				\
								ScicRegs.SCITXBUF = usb_tx_buf[usb_tx_pos++];	\
								if(usb_tx_pos >= USB_BUF_SIZE) usb_tx_pos=0;	\
							}													\
							else ScicRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	USB_TX_STOP		ScicRegs.SCICTL2.bit.TXINTENA=0

// SCI-C에 연결된 USB의 송신 처리를 위한 버퍼 관련 변수
BYTE usb_tx_buf[USB_BUF_SIZE+1];
BYTE usb_tx_pos=0, usb_tx_end=0;

// 현재 SCI-C 포트에 연결된 USB에 수신된 데이터(ex08_can.c 파일에 변수 정의됨)
extern char usb_rxd; 

void usb_putc(char d){
	usb_tx_buf[usb_tx_end++] = d;
	if(usb_tx_end >= USB_BUF_SIZE) usb_tx_end = 0;
}

void usb_puts(char* p){
char rd;
	while(rd = *p++){             
		usb_tx_buf[usb_tx_end++] = rd;
		if(usb_tx_end >= USB_BUF_SIZE) usb_tx_end = 0;
	}
}

interrupt void scic_tx_isr(void){
	if(usb_tx_pos != usb_tx_end){
		ScicRegs.SCITXBUF = usb_tx_buf[usb_tx_pos++];
		if(usb_tx_pos >= USB_BUF_SIZE) usb_tx_pos = 0;
	}
	else{                              
		USB_TX_STOP;
	}

	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

interrupt void scic_rx_isr(void){
	usb_rxd = ScicRegs.SCIRXBUF.all;
	
	usb_puts("USB: rxd = ");
	usb_putc(usb_rxd);
	usb_puts("\r\n");
	USB_TX_START;
	
	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

// USB소자와 연결된 SCI-C 초기화 함수
void scic_usb_init(){
	ScicRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScicRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScicRegs.SCICCR.all = 0x0007;  			// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
	ScicRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

    ScicRegs.SCIHBAUD = SCIC_BRR_VAL >> 8;
    ScicRegs.SCILBAUD = SCIC_BRR_VAL & 0xff;

	ScicRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-C RX interrupt
    EALLOW;
	PieVectTable.SCIRXINTC = &scic_rx_isr;
    PieVectTable.SCITXINTC = &scic_tx_isr;
   
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

	/* Configure SCI-C pins using GPIO regs*/
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation
	EDIS;

    // Enable CPU INT8 for SCI-C
	IER |= M_INT8;
	
    // Enable SCI-C RX INT in the PIE: Group 8 interrupt 5
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;

    // Enable SCI-C TX INT in the PIE: Group 8 interrupt 6
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
}

