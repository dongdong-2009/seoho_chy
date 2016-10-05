//###############################################################################################################
// 파일명   : ADC.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : F28335의 내부 ADC 주변장치 관련 구현 코드
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

// ADC Interrupt Service Function 선언
#pragma CODE_SECTION(adc_isr, "ramfuncs");
interrupt void adc_isr(void);

// ex06_adc.c에 정의된 ADC-A(0~7)/B(0~7) 채널 12bit 아날로그 입력 데이터
extern WORD adc_a_data[8];
extern WORD adc_b_data[8];

// Initialize ADC operation mode
void Init_Adc_Mode(void){
	// Initialize ADC Power-up sequence
	// *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.
	
	// 아래 주석 처리한 부분은 DSP2833x_SysCtrl.c 파일에서 InitPeripheralClocks() 함수를 통해 실행됨 
	/*
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
	ADC_cal();
	EDIS;
	*/

	// To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.
	AdcRegs.ADCTRL3.all = 0x00E0; // Power up bandgap/reference/ADC circuits
	DELAY_US(5000L);         	  // Delay 5ms before converting ADC channels

	// Set ADC core clock prescaler
	// 12.5MHz = HSPCLK/((ADCTRL3.ADCCLKPS * 2)*(ADCTRL1.bit.CPS + 1)) = (150MHz/2)/6
	// 781.250 kHZ conversion rate
	AdcRegs.ADCTRL1.bit.CPS = 0;
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 3;
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf; // Acquisition window size
	
	// Set internal reference selection
	// F28335의 내부 ADC Reference를 사용하지 않고 외부 ADC Reference를 쓰는 경우에는
	// 아래의 값을 사용하는 Referecne Voltage에 따라 설정하십시요. 
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 내부 ADC Referecne 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 외부 ADC Referecne(2.048 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 외부 ADC Referecne(1.500 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 외부 ADC Referecne(1.024 V) 사용할 때
	
	// Setup 1 conv's on SEQ1
	AdcRegs.ADCMAXCONV.all = 0xf;
	
	// Set cascaded mode
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
	
	// Select start-stop mode
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
	
	// Set ADC input channel selection sequence	
	AdcRegs.ADCCHSELSEQ1.all = 0x3210;
	AdcRegs.ADCCHSELSEQ2.all = 0x7654;
	AdcRegs.ADCCHSELSEQ3.all = 0xBA98;
	AdcRegs.ADCCHSELSEQ4.all = 0xFEDC;

	// Enable SEQ1 interrupt (every EOS)
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
	
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.ADCINT = &adc_isr;
	EDIS;

	// Enable ADCINT in PIE
	IER |= M_INT1; // Enable CPU Interrupt 1

	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
	
	// SOC trigger for sequencer1 by software
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;		
}

interrupt void adc_isr(void){
	adc_a_data[0] = AdcRegs.ADCRESULT0 >> 4;
	adc_a_data[1] = AdcRegs.ADCRESULT1 >> 4;
	adc_a_data[2] = AdcRegs.ADCRESULT2 >> 4;
	adc_a_data[3] = AdcRegs.ADCRESULT3 >> 4;
	adc_a_data[4] = AdcRegs.ADCRESULT4 >> 4;
	adc_a_data[5] = AdcRegs.ADCRESULT5 >> 4;
	adc_a_data[6] = AdcRegs.ADCRESULT6 >> 4;
	adc_a_data[7] = AdcRegs.ADCRESULT7 >> 4;
	
	adc_b_data[0] = AdcRegs.ADCRESULT8 >> 4;
	adc_b_data[1] = AdcRegs.ADCRESULT9 >> 4;
	adc_b_data[2] = AdcRegs.ADCRESULT10 >> 4;
	adc_b_data[3] = AdcRegs.ADCRESULT11 >> 4;
	adc_b_data[4] = AdcRegs.ADCRESULT12 >> 4;
	adc_b_data[5] = AdcRegs.ADCRESULT13 >> 4;
	adc_b_data[6] = AdcRegs.ADCRESULT14 >> 4;
	adc_b_data[7] = AdcRegs.ADCRESULT15 >> 4;

	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1; // Clear INT SEQ1 bit
	
	// Re-start ADC conversion
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;

	// Acknowledge interrupt to PIE
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;		 
} 
