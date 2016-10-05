//--------------------------------------------------------
// This source is modified by J.S. RYU for RealDSP-UT
//--------------------------------------------------------

#pragma DATA_SECTION(ZONE0_BUF,"ZONE0DATA");
volatile unsigned int ZONE0_BUF[2048];  // by RYU 

#include	<All_Header.h>
#include	<All_Global_Variables.h>
#include	<All_Extern_Variables.h>
#include <All_Module.h>

#include <fd.h>
#include <mb.h>
#include <abic.h>


#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
interrupt void cpu_timer0_isr(void);

//--- debug2010-0831 start
int 	test_command;
double  test_ref;

void Nop()
{
  asm ("      nop");
}

//--- debug2010-0831 end

double DrivePower;
int option_board_ID;
void main( void )
{
   BOOL fResult;
   unsigned char abInData[ 2 ];
   unsigned char abOutData[ 2 ];

	F28335PowerOnSet();
	DrivePower = CheckPowerVersion( );

 fResult = ABIC_AutoBaud();
fResult = ABIC_NormalMode();

 fResult =  ABIC_WriteInData( 0, 1, abInData );

  // TM_StartTimer();
  // while( !TM_TimeOut() );
  // TM_StartTimer();
  // while( !TM_TimeOut() );

 fResult = ABIC_ReadOutData( 0, 1, abOutData );

	// Infinity Loop
	while(1)
	{
// fResult = ABIC_AutoBaud();
scib_putc(0x55);
	//delay_ms(100); 
	//test_led1_on();
	//delay_ms(100); 
	//test_led1_off();
	
//		State_Management();
//		Keypad_Serial_Communication();
//		DSPtoMonitoring();
//		DI_Processing( & test_command, &test_ref );
//		digital_out_proc();
//		GetAnalogRef( & test_ref);
//		AnalogInputProcessing(&test_command,&test_ref );
		

//		option_board_ID = CheckOptionCardID();
		
	//	dev_BackgroundDAC ();
		Nop();
	}

}

void F28335PowerOnSet()
{
	State_Index = STATE_POWER_ON;
	strncpy(FaultInfo.MSG  ," No Error  ",20); 
	strncpy(MonitorMsg," POWER ON     ",20);
	
	InitSysCtrl();
	InitXintf();
	InitGpio();
	test_led1_off();
//	InitMcbspaGpio();	
	dev_DacSetup ();                  // chy 2010.11.17   DAC 
	dev_InitDACVariable();            // chy 2010.11.17   DAC
	InitMcbspa();

	// FLASH Code Copy and 초기화 루틴 --------------------------------------------
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the FLASH.cmd file.
	//MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);  // by RYU
	
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
 	//InitFlash();                                                       // by RYU


	RESET_DRIVER_SET; //316J PWM on : Reset중 일때는 입력전원 차단이 되지 않는다 

	PWM_HARDWARE_OFF;		// pwm 출력 방지 
	BOOT_MODE_OFF;			// pwm 출력 방지 
	DC_CONTACT_OFF;

/*----------------Interrupt Enable-------------------*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	EPWM_Initialization();

	InitPieCtrl();
	InitCpuTimers();   // For this example, only initialize the Cpu Timers

	ConfigCpuTimer(&CpuTimer0, 150, 1000);	// debug 2008.07.16
	StartCpuTimer0();

// Initialize SCI-A for data monitoring 
	sci_debug_init(); // by RYU
	// Initialize SCI-B, SCI-C
	scib_init();    // masked by chy
//	scic_init();    // masked by chy
	InitAdc();
//	EQEP_Initialization();    // chy

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.EPWM1_INT 	= &Driver_Calibration;
  	EDIS;    // This is needed to disable write to EALLOW protected registers

// Enable PIE group 3 interrupt 1 for EPWM1_INT

	IER |= M_INT1;		// Enable CPU INT1 which is connected to CPU-Timer 0:
	IER |= M_INT3;

// chy added 2010.10.25 


  EALLOW;	// Allow access to EALLOW protected registers
   PieVectTable.MRINTA= &Mcbsp_RxINTA_ISR;
   PieVectTable.MXINTA= &Mcbsp_TxINTA_ISR;
   EDIS;   // Disable access to EALLOW protected registers

   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER6.bit.INTx5=1;     // Enable PIE Group 6, INT 5
   PieCtrlRegs.PIEIER6.bit.INTx6=1;     // Enable PIE Group 6, INT 6
   IER |=0x20;                            // Enable CPU INT6
 //  EINT;                               // Enable Global Interrupts

	ZONE0_BUF[0x0060] = 0x0006; //hhh DAC_reset on -> by RYU
	NOP;
	ZONE0_BUF[0x0060] = 0x0007; // DAC_reset off -> by RYU
	NOP;


	DC_CONTACT_ON;		// 메인 전원 on
	delay_ms(10); 
	test_led1_off();
	test_led2_off();
	RESET_DRIVER_CLEAR;	//316J Reset
	delay_ms(1);			
	RESET_DRIVER_SET;		//316J PWM on
	Variable_Initialization();

// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM

/*----------------Interrupt Enable End-------------------*/	 

//	StopCpuTimer0();
	
}

interrupt void cpu_timer0_isr(void){
	CpuTimer0.InterruptCount++;

	MB_iTimeOutTime++;
	
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


void EPWM_Initialization()
{  
	EPwmPeriodCount =	(Uint16)( F_OSC * DSP28_PLLCR / SWITCH_FREQ / 2 / 2 );
	
	EPwm1Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                      	// Clear counter

	// Setup TBCLK
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	// Count up
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// Disable phase loading
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;					// Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.SYNCOSEL= TB_CTR_ZERO;

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	#if (DUAL_PWM_INTERRUPT)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif	  



	// Setup compare 
	EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount;
   
	// Set actions
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;					// Set PWM1A on Zero
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
   
	EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL 	= DB_ACTV_LOC;
	EPwm1Regs.DBCTL.bit.IN_MODE 	= DBA_ALL;
	EPwm1Regs.DBRED = DEAD_TIME_COUNT;					// debug set to 4usec
	EPwm1Regs.DBFED = DEAD_TIME_COUNT;
   
	// Interrupt where we will change the Deadband
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;			// Select INT on Zero event
	EPwm1Regs.ETPS.bit.INTPRD = 1;						// Generate INT on 1st debug   

	EPwm1Regs.ETSEL.bit.INTEN = 1;    		            // Enable INT
	
	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
	// ZC 회로 오류 해결해야됨 
	EALLOW;
	EPwm1Regs.TZSEL.all=0x0F00;
	EPwm1Regs.TZCTL.bit.TZA=0; // High impedance  
	EDIS;

	//---------------------------------
	// Sset PWM2   
	//---------------------------------

	// Setup TBCLK
	EPwm2Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm2Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        		// Disable phase loading
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       			// Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          			// Slow just to observe on the scope
	EPwm2Regs.TBCTL.bit.SYNCOSEL= TB_SYNC_IN;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;	
		

 	#if (DUAL_PWM_INTERRUPT)
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif 
	
	// Setup compare 
	EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount;

	// Set actions
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;             		// Set PWM2A on Zero
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	// Active Low complementary PWMs - setup the deadband
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED = DEAD_TIME_COUNT;
	EPwm2Regs.DBFED = DEAD_TIME_COUNT;
	EPwm2Regs.ETSEL.bit.INTEN = 0;                 		// Enable INT



	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
	EALLOW;
	EPwm2Regs.TZSEL.all=0x0F00;
	EPwm2Regs.TZCTL.bit.TZA=0; // High impedance
	EDIS;
	//------------------------------
	//InitEPwm3Example()
	//------------------------------

	EPwm3Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm3Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE; 				// 
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;				// 
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;				// Slow so we can observe on the scope
	EPwm3Regs.TBCTL.bit.SYNCOSEL= TB_SYNC_IN;

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;	

 	#if (DUAL_PWM_INTERRUPT)
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif 

	
	// Setup compare 
	EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount;

	// Set actions
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;              // Set PWM3A on Zero
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;

	// Active high complementary PWMs - Setup the deadband
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBRED = DEAD_TIME_COUNT;
	EPwm3Regs.DBFED = DEAD_TIME_COUNT;
	EPwm3Regs.ETSEL.bit.INTEN = 0;                  // Enable INT


	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
	EALLOW;
	EPwm3Regs.TZSEL.all=0x0F00; 
	EPwm3Regs.TZCTL.bit.TZA=0; // High impedance
	EDIS;
	//------------------------------
	//InitEPwm4   A == db B INIT_Charge 
	//------------------------------

	EPwm4Regs.TBPRD =  EPwmPeriodCount;			// Set timer period
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	// Count up
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE; 
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
 
 	#if (DUAL_PWM_INTERRUPT)
		EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
//		EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//		EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif  
 
	// Setup compare 
	EPwm4Regs.CMPA.half.CMPA =  EPwmPeriodCount;

	// Set actions
	EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM4A on Zero
	EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;

	// Active high complementary PWMs - Setup the deadband
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	EPwm4Regs.ETSEL.bit.INTEN = 0;                  // Enable INT
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;

 

	//------------------------------
	//InitEPwm5
	//------------------------------

	EPwm5Regs.TBPRD 				= EPwmPeriodCount;   // Set timer period
	EPwm5Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 		// Count up
	EPwm5Regs.TBCTL.bit.PHSEN 	= TB_DISABLE;        // 

	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // 
	EPwm5Regs.TBCTL.bit.CLKDIV 	= TB_DIV1;         // Slow so we can observe on the scope

	// Setup compare 
	EPwm5Regs.CMPA.half.CMPA 	= EPwmPeriodCount;
	EPwm5Regs.CMPB 	= EPwmPeriodCount;

	// Set actions
	EPwm5Regs.AQCTLA.bit.CAU 		= AQ_SET;             
	EPwm5Regs.AQCTLA.bit.CAD 		= AQ_CLEAR;

	EPwm5Regs.AQCTLB.bit.CBU 		= AQ_SET;              
	EPwm5Regs.AQCTLB.bit.CBD 		= AQ_CLEAR;
   
	EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_DISABLE;
	EPwm5Regs.ETSEL.bit.INTEN 		= 0;                 

	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

}


