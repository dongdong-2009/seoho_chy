//###############################################################################################################
// ���ϸ�   : ADC.c
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.10.20
// �ۼ���   : R.N.K
// ����     : F28335�� ���� ADC �ֺ���ġ ���� ���� �ڵ�
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

// ADC Interrupt Service Function ����
#pragma CODE_SECTION(adc_isr, "ramfuncs");
interrupt void adc_isr(void);

// ex06_adc.c�� ���ǵ� ADC-A(0~7)/B(0~7) ä�� 12bit �Ƴ��α� �Է� ������
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
	
	// �Ʒ� �ּ� ó���� �κ��� DSP2833x_SysCtrl.c ���Ͽ��� InitPeripheralClocks() �Լ��� ���� ����� 
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
	// F28335�� ���� ADC Reference�� ������� �ʰ� �ܺ� ADC Reference�� ���� ��쿡��
	// �Ʒ��� ���� ����ϴ� Referecne Voltage�� ���� �����Ͻʽÿ�. 
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // ���� ADC Referecne ����� ��
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // �ܺ� ADC Referecne(2.048 V) ����� ��
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // �ܺ� ADC Referecne(1.500 V) ����� ��
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // �ܺ� ADC Referecne(1.024 V) ����� ��
	
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
