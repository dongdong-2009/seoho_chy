//###############################################################################################################
// 파일명   : SPI_dac.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : RealSYS DSP28335 DSK에 있는 2ch 12bit DAC소자인 MCP4822를 구동하기 위한 SPI 관련 함수 구현
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// [ DAC 소자인 MCP4822 특성 ]
// 1) 12-bit Resolution, Dual Channel
// 2) +-0.2 LSb DNL, +-2 LSb INL
// 3) Rail-to-Rail output
// 4) SPI interface with 20MHz clock support
// 5) Simultaneous latching of the dual DACs wiht /LDAC pin
// 6) Fast settling time of 4.5us
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "DSK2833x_Define.h"

// Initialize SPI for MCP4822 DAC device
void Init_Spi_Dac(void){
	// Initialize GPIO for SPI-A according to MCP4822 DAC
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;  // Configure GPIO16 as SPISIMOA
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;  // Configure GPIO18 as SPICLKA
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;  // Configure GPIO19 as SPISTEA
	
	/* Configure /LDAC */
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;  // Configure GPIO17 as GPIO(/LDAC)
	GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;	  // Output Mode
	GpioDataRegs.GPASET.bit.GPIO17 = 1; 
	EDIS;
	
	// Disable the FIFO operation
	SpiaRegs.SPIFFTX.all = 0xA000;
	SpiaRegs.SPIFFRX.all = 0x2000;
	SpiaRegs.SPIFFCT.all = 0x0;

	SpiaRegs.SPICCR.all =0x000F;	// Reset on, rising edge, 16-bit char bits  
	SpiaRegs.SPICTL.all =0x000E;    // Enable master mode, one half-cycle delay phase,
                                    // Enable TALK, and SPI int disabled.

	// SPI clock speed가 20MHz(DAC소자가 지원하는 최대속도)가 넘지 않도록 설정함
	SpiaRegs.SPIBRR =0x0000;		// Maximum SPI speed(LSPCLK/4 = 150MHz/16 = 9.375MHz)									
    SpiaRegs.SPICCR.all =0x008F;	// Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;	// Set so breakpoints don't disturb xmission	 
}

// DAC 채널1/2을 출력하는 함수
void DAC_Out(WORD ch1, WORD ch2){
WORD d;

	// 채널 1에 데이터 보내기(Output Gain x2)
	d = (ch1 & 0xfff) | 0x1000;
	SpiaRegs.SPIDAT = d;
	while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
	d = SpiaRegs.SPIRXBUF; // clear SPI INT Flag

	// 채널 1에 데이터 보내기(Output Gain x2)
	d = (ch2 & 0xfff) | 0x9000;
	SpiaRegs.SPIDAT = d;
	while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
	d = SpiaRegs.SPIRXBUF; // clear SPI INT Flag
	
	// Set /LDAC signal timing according to MCP4822 DAC device datasheet
	DELAY_US(0.04L);
	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
	DELAY_US(0.1L);
	GpioDataRegs.GPASET.bit.GPIO17 = 1;	
}

 
