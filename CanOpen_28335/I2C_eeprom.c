//###############################################################################################################
// 파일명   : I2C_eeprom.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : RealSYS DSP28335 DSK에 있는 Serial EEPROM(AT24C16) 구동에 필요한 I2C 관련 함수 구현
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
// [ AT24C16 소자 특성 ]
// 1) 2048(0x800) * 8 Bytes(16K)
// 2) Two-wire serial interface
// 3) 100KHz(1.8V) and 400KHz(2.7V, 5V) in Clock Frequency, SCL
//    Clock Pulse Width Low  : 4.7us(1.8V), 1.2us(2.7V, 5V) - Minimum value
//	  Clock Pulse Width High : 4.0us(1.8V), 0.6us(2.7V, 5V) - Minimum value
// 4) 16 Byte Page Write Mode
// 5) Self-timed Write Cycle(5ms max)
// 6) organized with 128 pages of 16 bytes each, requires an 11-bit data word address
//
// Serial Clock(SCL) 	: is used to positive edge clock data into each EEPROM device
//                     	  and negative edge clock data out of each device
// Serial Data(SDA)  	: is bidirectional for serial data transfer. This pin is open-drain driven.
// Address(A0, A1, A2)	: does not use the device address pins
//
// [ 2 Byte Address 방식 사용 - A0, A1 ]
// 1) A0[D7 ~ D4] : 상위 4비트는 1010
// 2) A0[D3 ~ D1] : 최상위 3비트 Address로 사용
// 3) A0[D0]      : 1이면 Read 명령, 0이면 Write 명령
// 4) A1[D7 ~ D0] : 하위 8비트 Address로 사용   
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "DSK2833x_Define.h"

// Initialize I2C for serial eeprom(AT24C16) access
void Init_I2C_eeprom(void){
	// Setting GPIO for I2C
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)
	
	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)
	
	/* Configure I2C pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be I2C functional pins.
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
	EDIS;
	
	// Initialize I2C interface
	// Put I2C module in the reset state
	I2caRegs.I2CMDR.bit.IRS = 0;
	
	// Set Slave Address according to AT24C16 device
	I2caRegs.I2CSAR = 0x0050;

	// Set I2C module clock input
	I2caRegs.I2CPSC.all = 14; // need 7-12 Mhz on module clk (150/15 = 10MHz)
	
	// 400KHz clock speed in SCL for master mode(2.5us period)
	// Clock Pulse Width Low  : 1.5us
	// Clock Pulse Width High : 1.0us
	// F28335의 I2C User Guide에서 Master Clock 주기를 구하는 공식을 참조바람.
	I2caRegs.I2CCLKL = 10;
	I2caRegs.I2CCLKH = 5;
	
	// Disable I2C interrupt
	I2caRegs.I2CIER.all = 0x0;
	
	// Enable TX and RX FIFO in I2C module
	I2caRegs.I2CFFTX.all = 0x6000; // Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040; // Enable RXFIFO, clear RXFFINT

	// Enable I2C module
	I2caRegs.I2CMDR.all = 0x20; 
}

// refer to single byte write mode of AT24C16
void Write_EEPROM_Data(WORD addr, BYTE data){
WORD slave_addr, data_addr;
	
	slave_addr = 0x50 | ((addr >> 8) & 0x7);
	data_addr = addr & 0xff;
	 
	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 2;

	// Setup data to send
	I2caRegs.I2CDXR = data_addr; // Data Address
	I2caRegs.I2CDXR = data;		 // Write Data
	
	// Send start as master transmitter with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6E20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;

	// Wait the Write-Cycle Time for EEPROM
	// refer to AT29C16 datasheet
	delay_ms(5);
}

// refer to single byte random read mode of AT24C16
void Read_EEPROM_Data(WORD addr, BYTE* pData){
WORD slave_addr, data_addr;
	
	slave_addr = 0x50 | ((addr >> 8) & 0x7);
	data_addr = addr & 0xff;
	 
	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 1;

	// Setup data to send
	I2caRegs.I2CDXR = data_addr; // Data Address
	
	// Send start as master transmitter with STT(=1), STP(=0), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x2620;
	
	// Wait until ARDY status bit is set
	while(I2caRegs.I2CSTR.bit.ARDY == 0);
	
	// Wait until the STP bit is cleared
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = slave_addr;
	
	// Setup number of bytes to read
	I2caRegs.I2CCNT = 1;
	
	// Send start as master receiver with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x2C20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;
	
	*pData = (BYTE)(I2caRegs.I2CDRR & 0xff);  
}

