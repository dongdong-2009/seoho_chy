//###############################################################################################################
// 파일명   : ex08_can_f.c(ex01_cpu_timer ~ ex07_sci 예제 기능을 포함)
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : DSP28335 DSK의 CAN-A/B 포트를 사용한 CAN 통신 테스트 프로그램
//            LCD의 첫줄에는 수신한 CAN 통신 포트, 수신 ID, 수신 데이터 사이즈 정보를 표시하고
//			  LCD의 두째줄에는 수신한 데이터 정보를 표시합니다
//			  DSP28335의 내부 Flash에서 구동됨.
//
//            사용하는 CAN 통신 프로토콜 : CAN2.0B, 1M bps
//            CAN 통신 수신 데이터 : No Mask 처리, LCD에 CAN-A/B에 수신된 데이터 표시
//            CAN 통신 송신 데이터 : Key Switch(SW1 ~ SW4)을 누를때 해당 송신 데이터 송신
//            SW1(CAN-A 송신) : ID(0x1), Data Size(0x8), Data(0x1111111111111111)
//			  SW2(CAN-A 송신) : ID(0x2), Data Size(0x8), Data(0x2222222222222222)
//			  SW3(CAN-B 송신) : ID(0x3), Data Size(0x8), Data(0x3333333333333333)
//			  SW4(CAN-B 송신) : ID(0x4), Data Size(0x8), Data(0x4444444444444444)
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

/****************************************************************************************************************
 * [ CPU Timer 0/1/2 부연 설명 ]
 * F28335에는 3개의 16bit CPU Timer가 있으며 이중 CPU Timer0/1은 User Application에서 사용하며 CPU Timer2는 TI의
 * RTOS인 BIOS에서 사용되도록 예약되어 있읍니다. 그러나 BIOS를 사용하지 않으면 CPU Timer2도 User Application에서
 * 사용하실 수 있읍니다.
 ***************************************************************************************************************/
#include "DSK2833x_Define.h"
#include ".\anybus_ic\mb.h"

WORD m_cnt=0;
WORD t_cnt0=0, t_cnt1=0, t_cnt2=0; // CPU Timer0/1/2 counter
BYTE led_data=0;	// 외부 버스 CS0에 연결된 Digital Output 8개(8개 LED)에 출력할 값 
BYTE sw_data=0; 	// 외부 버스 CS0에 연결된 SW1 ~ SW4 입력 데이터
BYTE dip_sw_data=0;	// 외부 버스 CS0에 연결된 DIP1 ~ DIP4 입력 데이터

BYTE output_data=0; // GPIO58 ~ 61에 연결된 Digital Output 4개에 출력할 값
BYTE input_data=0;	// GPIO54 ~ 57에 연결된 Digital Input 4개의 입력 데이터

// 4개 Key Switch 처리를 위한 변수 -----------------------------------------------
enum _KEY_CODE_ {NOKEY_CODE, SW1_CODE, SW2_CODE, SW3_CODE, SW4_CODE};

#define CODE_MASK	0x0f    // 4개의 Key 입력 데이터를 위한 마스크
#define KEY_CONT	0x80	// Key를 1초 이상 계속 누르고 있는 경우
#define KEY_PRESSED	0x40	// Key 입력 데이터가 최소 10ms 이상으로 한번 누른 경우

BYTE key_code = NOKEY_CODE;			// 현재의 Key Code 값
BYTE prev_key_code = NOKEY_CODE;	// 이전의 Key Code 값
BYTE key_cnt = 0;					// 키 누름 카운터 값
BYTE cont_key_cnt = 0;              // 연속키 카운터 값
//--------------------------------------------------------------------------------

// External Interrupt 발생 횟수 카운터 변수 ------
BYTE xint3_cnt=0;	// XINT3
BYTE xint4_cnt=0;	// XINT4
BYTE xint5_cnt=0;	// XINT5
BYTE xint6_cnt=0;	// XINT6
//------------------------------------------------

// I2C Serial EEPROM 처리 관련 변수 --------------
#define EEPROM_MAX_ADDRESS	0x7FF

WORD eep_addr=0;
BYTE eep_write_data=0;
BYTE eep_read_data=0;
//------------------------------------------------

// SPI방식의 12비트 DAC 출력 데이터 변수 ---------
WORD dac0_data=0;
WORD dac1_data=0;
//------------------------------------------------

// ADC-A(0~7)/B(0~7) 채널 12bit 아날로그 입력 데이터 변수 ----
WORD adc_a_data[8]; // A 채널 입력 데이터
WORD adc_b_data[8]; // B 채널 입력 데이터
//------------------------------------------------------------

// SCI-B와 USB소자와 연결된 SCI-C의 현재 수신 문자 변수 -------------
char scib_rxd=' '; // SCI-B 수신 문자
char usb_rxd=' ';  // USB소자와 연결된 SCI-C의 수신 문자
//-------------------------------------------------------------------

// eCAN-A/B 처리를 위한 변수 ------------------------------------------------
BOOL cana_rx_flag=FALSE;	// CAN-A에 새로운 수신 데이터를 받았는지 여부
LONG cana_rx_id=0;			// CAN-A 수신 ID
BYTE cana_rx_length=0;    	// CAN-A 수신 데이터 크기
LONG cana_rx_low_data=0;	// CAN-A 수신 Low Word Data
LONG cana_rx_high_data=0;	// CAN-A 수신 High Word Data

BOOL canb_rx_flag=FALSE;	// CAN-B에 새로운 수신 데이터를 받았는지 여부
LONG canb_rx_id=0;			// CAN-B 수신 ID
BYTE canb_rx_length=0;    	// CAN-B 수신 데이터 크기
LONG canb_rx_low_data=0;	// CAN-B 수신 Low Word Data
LONG canb_rx_high_data=0;	// CAN-B 수신 High Word Data
//----------------------------------------------------------------------------

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// CPU Timer0/1/2의 Interrupt Service Function 선언
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
#pragma CODE_SECTION(cpu_timer2_isr, "ramfuncs");

interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);

// XINT3 ~ 6 Interrupt Service Function 선언
#pragma CODE_SECTION(xint3_isr, "ramfuncs");
#pragma CODE_SECTION(xint4_isr, "ramfuncs");
#pragma CODE_SECTION(xint5_isr, "ramfuncs");
#pragma CODE_SECTION(xint6_isr, "ramfuncs");

interrupt void xint3_isr(void);
interrupt void xint4_isr(void);
interrupt void xint5_isr(void);
interrupt void xint6_isr(void);

// These are defined by the linker (see FLASH.cmd)
// 이 변수는 메인 함수의 초기화 루틴에서 "ramfuncs" 이라고 정의된 코드 섹션을
// 내부 Flash에서 RAM영역으로 복사하기 위한 MemCopy 함수에서 사용됨.
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

void key_check(void){
BYTE tmp_key_code;

	sw_data = Get_SW_Data();
	if(sw_data == 0x0){
		prev_key_code = key_code = NOKEY_CODE;
		cont_key_cnt = 0;
		key_cnt = 0;
		return;
	}
	
	switch(sw_data){
	case 1: tmp_key_code = SW1_CODE; break;
	case 2: tmp_key_code = SW2_CODE; break;
	case 4: tmp_key_code = SW3_CODE; break;
	case 8: tmp_key_code = SW4_CODE; break;
	}

	if(tmp_key_code != (prev_key_code & CODE_MASK)){
		prev_key_code = tmp_key_code;
		key_code = KEY_PRESSED | tmp_key_code;
		key_cnt = 0;
	}
	else{
		if(key_cnt < 100) key_cnt++;
		else key_code |= KEY_CONT;
	}	
}

void key_process(void){
	lcd_gotoxy(5, 1);
	switch(key_code & CODE_MASK){
	case SW1_CODE: lcd_puts("SW1"); break;
	case SW2_CODE: lcd_puts("SW2"); break;
	case SW3_CODE: lcd_puts("SW3"); break;
	case SW4_CODE: lcd_puts("SW4"); break;
	default: lcd_puts("???"); break;
	}

	key_code &= ~KEY_PRESSED; 	/* Clear key flag */
}

void cont_key_process(void){
	cont_key_cnt++;
	lcd_gotoxy(9, 1);
	lcd_hex2(cont_key_cnt);
}

void eeprom_key_process(void){
	switch(key_code & CODE_MASK){
	case SW1_CODE:
		{
			eep_addr++;
			if(eep_addr > EEPROM_MAX_ADDRESS) eep_addr = 0;
			
			lcd_clear_line(1);
			
			lcd_gotoxy(2, 1);
			lcd_hex4(eep_addr);
			
			lcd_gotoxy(7, 1);
			lcd_hex2(eep_write_data);	
		}
		break;
	case SW2_CODE:
		{
			eep_write_data++;
			if(eep_write_data > 0xff) eep_write_data = 0;
			
			lcd_clear_line(1);
			
			lcd_gotoxy(2, 1);
			lcd_hex4(eep_addr);
			
			lcd_gotoxy(7, 1);
			lcd_hex2(eep_write_data);	
		}
		break;
	case SW3_CODE:
		{
			Write_EEPROM_Data(eep_addr, (BYTE)(eep_write_data & 0xff));
			
			lcd_clear_line(1);
			
			lcd_gotoxy(0, 1);
			lcd_putc('W');
			
			lcd_gotoxy(2, 1);
			lcd_hex4(eep_addr);
			
			lcd_gotoxy(7, 1);
			lcd_hex2(eep_write_data);
			
			lcd_gotoxy(13, 1);
			lcd_puts("OK");		
		}
		break;
	case SW4_CODE:
		{
			Read_EEPROM_Data(eep_addr, &eep_read_data);
			
			lcd_clear_line(1);
			
			lcd_gotoxy(0, 1);
			lcd_putc('R');
			
			lcd_gotoxy(2, 1);
			lcd_hex4(eep_addr);
			
			lcd_gotoxy(7, 1);
			lcd_hex2(eep_write_data);
			
			lcd_gotoxy(10, 1);
			lcd_hex2(eep_read_data);
			
			lcd_gotoxy(13, 1);
			lcd_puts("OK");	
		}
		break;
	}

	key_code &= ~KEY_PRESSED; 	/* Clear key flag */	
}

void can_key_process(void){
	switch(key_code & CODE_MASK){
	case SW1_CODE: SendDataToECanA(0x1L, 0x08, 0x11111111, 0x11111111); break;
	case SW2_CODE: SendDataToECanA(0x2L, 0x08, 0x22222222, 0x22222222); break;
	case SW3_CODE: SendDataToECanB(0x3L, 0x08, 0x33333333, 0x33333333); break;
	case SW4_CODE: SendDataToECanB(0x4L, 0x08, 0x44444444, 0x44444444); break;
	}

	key_code &= ~KEY_PRESSED; 	/* Clear key flag */	
}

void cana_rx_data_update(void){
BYTE i, tmp;
	
	lcd_clear_line(0);
	lcd_gotoxy(0, 0);
	lcd_putc('A'); // eCAN-A에서 수신됨.
	
	lcd_gotoxy(2, 0);
	lcd_hex8(cana_rx_id);
	
	lcd_gotoxy(11, 0);
	lcd_hex2(cana_rx_length);
	
	lcd_clear_line(1);
	lcd_gotoxy(0, 1);
	for(i=0; i<cana_rx_length; i++){
		if(i == 4) break;
		tmp = (BYTE)((cana_rx_low_data >> (8 * (3-i))) & 0xff);
		lcd_hex2(tmp);
	}
	
	if(cana_rx_length > 4){
		for(i=4; i<cana_rx_length; i++){
			tmp = (BYTE)((cana_rx_high_data >> (8 * (7-i))) & 0xff);
			lcd_hex2(tmp);
		}
	}

	cana_rx_flag = FALSE;
}

void canb_rx_data_update(void){
BYTE i, tmp;
	
	lcd_clear_line(0);
	lcd_gotoxy(0, 0);
	lcd_putc('B'); // eCAN-B에서 수신됨.
	
	lcd_gotoxy(2, 0);
	lcd_hex8(canb_rx_id);
	
	lcd_gotoxy(11, 0);
	lcd_hex2(canb_rx_length);
	
	lcd_clear_line(1);
	lcd_gotoxy(0, 1);
	for(i=0; i<canb_rx_length; i++){
		if(i == 4) break;
		tmp = (BYTE)((canb_rx_low_data >> (8 * (3-i))) & 0xff);
		lcd_hex2(tmp);
	}
	
	if(canb_rx_length > 4){
		for(i=4; i<canb_rx_length; i++){
			tmp = (BYTE)((canb_rx_high_data >> (8 * (7-i))) & 0xff);
			lcd_hex2(tmp);
		}
	}

	canb_rx_flag = FALSE;
}

// DSP28335 DSK의 GPIO54 ~ 57에 연결된 Digital Input을 External Interrupt Mode로 초기화하는 함수
void Init_External_Interrupt(void){
	// Set GPIO Interrupt Select
	EALLOW; 
	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 22; // 54 - 32
	GpioIntRegs.GPIOXINT4SEL.bit.GPIOSEL = 23; // 55 - 32
	GpioIntRegs.GPIOXINT5SEL.bit.GPIOSEL = 24; // 56 - 32;
	GpioIntRegs.GPIOXINT6SEL.bit.GPIOSEL = 25; // 57 - 32;

	PieVectTable.XINT3 = &xint3_isr;
	PieVectTable.XINT4 = &xint4_isr;
	PieVectTable.XINT5 = &xint5_isr;
	PieVectTable.XINT6 = &xint6_isr;
	EDIS;

	// Set XINT Control Register(Falling Edge)
	XIntruptRegs.XINT3CR.bit.POLARITY = 0;
	XIntruptRegs.XINT4CR.bit.POLARITY = 0;
	XIntruptRegs.XINT5CR.bit.POLARITY = 0;
	XIntruptRegs.XINT6CR.bit.POLARITY = 0;

	// Enable External Interrupt
	XIntruptRegs.XINT3CR.bit.ENABLE = 1;
	XIntruptRegs.XINT4CR.bit.ENABLE = 1;
	XIntruptRegs.XINT5CR.bit.ENABLE = 1;
	XIntruptRegs.XINT6CR.bit.ENABLE = 1;

	IER |= M_INT12;	// Enable CPU int12 which is connected to XINT3/4/5/6

	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;	// Enable XINT3 in the PIE: Group 12 interrupt 1
	PieCtrlRegs.PIEIER12.bit.INTx2 = 1;	// Enable XINT4 in the PIE: Group 12 interrupt 2
	PieCtrlRegs.PIEIER12.bit.INTx3 = 1;	// Enable XINT5 in the PIE: Group 12 interrupt 3
	PieCtrlRegs.PIEIER12.bit.INTx4 = 1;	// Enable XINT6 in the PIE: Group 12 interrupt 4
}

void main(void)
{

   BOOL fResult;
   unsigned char abInData[ 2 ];
   unsigned char abOutData[ 2 ];

	
	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the DSP2833x_SysCtrl.c file.
	InitSysCtrl();

	// Step 2. Initalize GPIO:
	// RealSYS F28335 DSK에서 GPIO로 쓰이는 포트 초기화함.
	// refer to DSK2833x_GlobalFunc.c file
	Init_DSK_Gpio();
	
	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;

	// Initialize the PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the DSP2833x_PieCtrl.c file.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
	// This function is found in DSP2833x_PieVect.c.
	InitPieVectTable();

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.XINT13 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
	
	// FLASH Code Copy and 초기화 루틴 --------------------------------------------
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the FLASH.cmd file.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();
	//-----------------------------------------------------------------------------
	
	// Step 4. Initialize the Device Peripheral.
	// refer to DSK2833x_GlobalFunc.c file and
	Init_Ext_Bus();		// Initalize External Interface according to RealSYS DSP28335 DSK
	Set_LED_Data(0xff);	// Turn off all LEDs(CS0에 할당된 Digital Output 포트에 연결된 LED)
	init_lcd();			// Initialize LCD and Clear Display

	// This function can be found in DSP2833x_CpuTimers.c
	// For this example, only initialize the Cpu Timers
	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every 1 milliseconds:
	// 150MHz CPU Freq, 1 ms Period (uSeconds 단위)
	ConfigCpuTimer(&CpuTimer0, 150, 1000);
	ConfigCpuTimer(&CpuTimer1, 150, 1000);
	ConfigCpuTimer(&CpuTimer2, 150, 1000);
	
	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
	// below settings must also be updated.
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

	// Step 5. User specific code, enable interrupts:
	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	IER |= M_INT1;
	IER |= M_INT13;
	IER |= M_INT14;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	
	// Initialize SCI-A for data monitoring 
	sci_debug_init();
	
	// External Interrupt 초기화 함수
	Init_External_Interrupt();
	
	// Initialize I2C for access serial eeprom(AT24C16)
	Init_I2C_eeprom();
	
	// Initailze SPI-A for 2ch 12bit DAC(MCP4822)
	Init_Spi_Dac();
	
	// Initialize ADC operation mode
	Init_Adc_Mode();
	
	// Initialize SCI-B for echo-back testing
	scib_init();
	
	// Initialize SCI-C for usb echo-back testing
	scic_usb_init();

	// Initialize CAN-A/B
	init_can();
	
	// Enables the PIE module and CPU interrupts and global Interrupt INTM
	// refer to DSP2833x_PieCtrl.c file
	EnableInterrupts();

	// Enable Global realtime interrupt DBGM
	ERTM;   
	
	lcd_gotoxy(0, 0);
	lcd_puts("CanOpen 28335");
	
	while(TRUE){
		m_cnt++;
		dac0_data += 2;
		dac1_data += 1;
		
		sw_data = Get_SW_Data();
		dip_sw_data = Get_DIP_Data();
		input_data = Get_Digital_Input();
		
		DAC_Out(dac0_data, dac1_data);
		
		//if(key_code & KEY_PRESSED) can_key_process();

//scib_putc(0x55 );
		
		if(key_code & KEY_PRESSED) 
		{

			switch(key_code & CODE_MASK){
			case SW1_CODE: 

			//scib_putc(0x55 );
				
					   fResult = ABIC_AutoBaud();
					   if( !fResult )
					   {

					      /*
					      ** The baud rate auto detection failed.
					      ** Write error message and stay here for ever
					      */
						lcd_gotoxy(0, 0);
						lcd_puts("AutoBaud fault!");

					      //while( 1 )	      ;

					   }/* end if */
					   else
					   {
					   	lcd_gotoxy(0, 0);
						lcd_puts("AutoBaud done!");
					   }
				break;
			case SW2_CODE:
					   if( ABIC_NormalMode() )
					   {

					      /*
					      ** The baud rate auto detection failed.
					      ** Write error message and stay here for ever
					      */
						lcd_gotoxy(0, 0);
					      lcd_puts( "Set Mode Fault!");

					      //while( 1 )	      ;

					   }/* end if */
					   else
					   {
					   	lcd_gotoxy(0, 0);
					       lcd_puts(  "Set Mode done!");
					   }
				break;
			case SW3_CODE:
					      /*
				      ** Get indata values from the CPU AD converter
				      */
				      //abInData[ 0 ] = 0xAB;
				    //  abInData[ 1 ] = 0xCD;

				      /*
				      ** Write the In Data values to the AnyBus-IC
				      */
				     // ABIC_WriteInData( 0, 1, abInData );

					   TM_StartTimer();
					   while( !TM_TimeOut() );
					   TM_StartTimer();
					   while( !TM_TimeOut() );


				      /*
				      ** Read the Out Data values from the AnyBus-IC
				      */
				      fResult = ABIC_ReadOutData( 0, 1, abOutData );

					     ABIC_WriteInData( 0, 1, abOutData );

					   TM_StartTimer();
					   while( !TM_TimeOut() );
					   TM_StartTimer();
					   while( !TM_TimeOut() );

				      if( fResult )
				      {

				         /*
				         ** Print the Out Data values on the display
				         */
/*
							CLCD_command(0xC7);
							if((abOutData[ 0 ]>>4)<=9)CLCD_data((abOutData[ 0 ]>>4)+'0');
							else CLCD_data((abOutData[ 0 ]>>4)-10+'A');
							
							CLCD_command(0xC8);
							if((abOutData[ 0 ] & 0x0F)<=9)CLCD_data((abOutData[ 0 ] & 0x0F)+'0');
							else CLCD_data((abOutData[ 0 ] & 0x0F)-10+'A');

							CLCD_command(0xC9);
							if((abOutData[ 1 ]>>4)<=9)CLCD_data((abOutData[ 1 ]>>4)+'0');
							else CLCD_data((abOutData[ 1 ]>>4)-10+'A');
							
							CLCD_command(0xCA);
							if((abOutData[ 1 ] & 0x0F)<=9)CLCD_data((abOutData[ 1 ] & 0x0F)+'0');
							else CLCD_data((abOutData[ 1 ] & 0x0F)-10+'A');
*/
				      }
					else
					{
						//CLCD_string(0xC0, "Read Fault!!!!");
					}
				break;
			case SW4_CODE: break;
			}

			key_code &= ~KEY_PRESSED; 	/* Clear key flag */	




		}
		
		
		//if(cana_rx_flag) cana_rx_data_update();
		//if(canb_rx_flag) canb_rx_data_update();
	}
}


interrupt void cpu_timer0_isr(void){
	CpuTimer0.InterruptCount++;
	t_cnt0++;

	MB_iTimeOutTime++;
	
	// 10ms period
	if((t_cnt0 % 10) == 0) key_check();
	
	// 50ms period
	if((t_cnt0 % 50) == 0) CPU_MODULE_LED_TOGGLE;
	
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void){
	CpuTimer1.InterruptCount++;
	t_cnt1++;
	
	// 50ms period
	if((t_cnt1 % 50) == 0){
		led_data++;
		Set_LED_Data(~led_data & 0xff);
	}
	
	// The CPU acknowledges the interrupt.
	EDIS;
}

interrupt void cpu_timer2_isr(void){
	EALLOW;
	CpuTimer2.InterruptCount++;
	t_cnt2++;
	
	// 50ms period
	if((t_cnt2 % 50) == 0){
		output_data++;
		Set_Digital_Output(output_data);
	}
	
	// The CPU acknowledges the interrupt.
	EDIS;
}

interrupt void xint3_isr(void){
	xint3_cnt++;

	// Acknowledge this interrupt to receive more interrupts from group 12	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void xint4_isr(void){
	xint4_cnt++;
	
	// Acknowledge this interrupt to receive more interrupts from group 12
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void xint5_isr(void){
	xint5_cnt++;
	
	// Acknowledge this interrupt to receive more interrupts from group 12
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void xint6_isr(void){
	xint6_cnt++;
	
	// Acknowledge this interrupt to receive more interrupts from group 12
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
