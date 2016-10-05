//###############################################################################################################
// ���ϸ�   : SCI_C_usb.c
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.10.20
// �ۼ���   : R.N.K
// ����     : DSP28335 DSK KIT�� SCI-C�� ����� FTDI FT232R USB ���� ���� ���� �ڵ� ����
//            ����ڲ����� ��뿡 �ռ� ������ CD�� USB Driver ������ �ִ� USB Device Driver�� ���� ��ġ�Ͻñ�
//            �ٶ��ϴ�. ������ USB Device Driver�� ��ġ�Ͻø� ����� PC�� ���� �ø��� ��Ʈ�� �����˴ϴ�.
//            ������ ��ġ�������� ��Ʈ �������� ������ ���� �ø��� ��Ʈ�� �������� Ȯ���Ͻʽÿ�.
//            �׸����� ������ �͹̳� ���α׷��� ����Ͽ� ������ ���� �ø��� ��Ʈ�� ���� �׽�Ʈ�Ͻø� �˴ϴ�.
//
//            [ FTDI FT232R USB���ڿ��� �����ϴ� BaudRate ]
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
//			  ����ڲ����� �Ʒ��� ���ǵ� SCIC_BAUDRATE ���� ���� ����Ͻñ� �ٶ��,
//            �̶� ����Ͻ� Baudrate�� �������� ����Ͽ� �����Ͻʽÿ�.
//
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe ����) - 2008.10.20
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30329.exe ����) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe ����) - 2008.10.20
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.20 ��� - 2008.10.20
//
// [ Revision History ]
//
//###############################################################################################################

#include "DSK2833x_Define.h"

// ������ ���������� ���ͷ�Ʈ ���� ��ƾ�� "ramfuncs" �̶�� ���ǿ� �Ҵ��ϰ�
// "ramfuncs" ������ ���� ���� ó���� ���� ���� RAM���� �����ϵ��� ��

// SCI-C ��Ʈ�� Interrupt Service Function ����
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");

interrupt void scic_tx_isr(void);
interrupt void scic_rx_isr(void);

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIC_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-C
#define	SCIC_BAUDRATE	115200L								// Baudrate(�Ʒ��� �������� ����Ͽ� ������)
#define	SCIC_BRR_VAL	(SCIC_LSPCLK/(8*SCIC_BAUDRATE)-1)	// SCI-C BaudRate ���� Register ��

/* BPS ������ ***************************************************************************************
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

// SCI-C�� ����� USB�� �۽� ó���� ���� ���� ���� ����
BYTE usb_tx_buf[USB_BUF_SIZE+1];
BYTE usb_tx_pos=0, usb_tx_end=0;

// ���� SCI-C ��Ʈ�� ����� USB�� ���ŵ� ������(ex08_can.c ���Ͽ� ���� ���ǵ�)
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

// USB���ڿ� ����� SCI-C �ʱ�ȭ �Լ�
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

