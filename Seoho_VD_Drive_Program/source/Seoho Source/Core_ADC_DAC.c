#include	<All_Header.h>
#include	<All_Extern_Variables.h>

void InitAdc()
{
	AdcRegs.ADCMAXCONV.all = 7;       // 16 channel  
	AdcRegs.ADCTRL1.bit.SUSMOD = 1;
	AdcRegs.ADCTRL1.bit.ACQ_PS = 15;
	AdcRegs.ADCTRL1.bit.CPS = 0;
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
	AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;// Enable SOCA from ePWM to start SEQ1

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // current U
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // current V
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // current W
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // voltage Vdc
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // current 1
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // voltage 1
	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6; // current 2
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 7; // voltage 2
	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 8; // analog input 3
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 9; // analog input 4
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 10; // analog input 5
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 14; // temperature
	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 15; // voltage range

	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
	AdcRegs.ADCTRL3.all = 0x00F0;  // Power up bandgap/reference/ADC circuits
	AdcRegs.ADCREFSEL.all = 0x0000;

	//for adc irq 
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;   // Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on every event
	EPwm1Regs.ETCLR.bit.SOCA = 1;	// Clear SOCA flag

}



// 28335 Voltage Range: 0~3V
// 22kW CT Simulation Result: 50A=4V(CT Output)=2.43V(28335 input), by 1.5V Offset
// I=(50/0.93)*(3/4095)*(bit-2047)
// (100413) 재수정
// 5.5 7.5kW에서 ADC 입력이 반전됨

// 50A=4V(CT Output)=-0.3V(28335 input), by 1.5V Offset
// -50A=-4V(CT Output)=3.3V(28335 input), by 1.5V Offset
// I=(50/-1.8)*(3/4095)*(bit-2047) 5.5kW
// I=(50/-1.43)*(3/4095)*(bit-2047) 7.5kW
// 전류 측정지연 32.81us(5.5) 33.42(7.5)
#define I_scale					(50.0/(1.8))
#define Vdc_scale				(900.)
#define Vdc_offset				(0.)



void GetSensorValue( )
{
	double Temp;
	static int First_step= 0., OV_Timer= 0., UV_Timer= 0.;
//	static int INIT_CHARGE_Timer= 0.;

	Adc_Uphase		= (int)(AdcRegs.ADCRESULT0	>> 4); // current U
	Adc_Vphase		= (int)(AdcRegs.ADCRESULT1  >> 4); // current V
	Adc_Wphase 		= (int)(AdcRegs.ADCRESULT2 	>> 4); // current W
	Adc_Vdc			= (int)(AdcRegs.ADCRESULT3 	>> 4); // voltage Vdc
	Adc_AIN_I1 		= (int)(AdcRegs.ADCRESULT4 	>> 4); // current 1
	Adc_AIN_V1 		= (int)(AdcRegs.ADCRESULT5 	>> 4); // voltage 1
	Adc_AIN_I2 		= (int)(AdcRegs.ADCRESULT6 	>> 4); // current 2
	Adc_AIN_V2 		= (int)(AdcRegs.ADCRESULT7 	>> 4); // voltage 2
	Adc_AIN_3		= (int)(AdcRegs.ADCRESULT8 	>> 4); // analog input 3
	Adc_AIN_4		= (int)(AdcRegs.ADCRESULT9 	>> 4); // analog input 4
	Adc_AIN_5	 	= (int)(AdcRegs.ADCRESULT10 >> 4); // analog input 5
	Adc_Temperature	= (int)(AdcRegs.ADCRESULT11 >> 4); // temperature
	Adc_V_Range 	= (int)(AdcRegs.ADCRESULT12 >> 4); // voltage range
 
	Temp= (double)(Adc_Uphase);
	Ias= I_scale * (3.0/4095.0) * (Temp-2047) - Ias_offset;
	Temp= (double)(Adc_Vphase);
	Ibs= I_scale * (3.0/4095.0) * (Temp-2047) - Ibs_offset;
	Ics= -(Ias+Ibs);

    Idss = Ias;
	Iqss = (Ibs-Ics)/SQRT3;
	Is_mag = sqrt(Idss*Idss+Iqss*Iqss);           
	Is_mag_rms = Is_mag/SQRT2;  

	Temp= (double)(Adc_Vdc);
	Vdc=  Vdc_scale *((double)Temp/4095.0) - Vdc_offset;

	
	if (Vdc>Vdc_max) Vdc_max=Vdc;
	Temp= Supply_Voltage*SQRT2;
	if (First_step>=COUNT_END)
	{
		if (fabs(Ias) > (Is_rate*1.2) ) Flag.Fault.bit.OC_A= 1;
		if (fabs(Ibs) > (Is_rate*1.2) ) Flag.Fault.bit.OC_B= 1;
		if (fabs(Ics) > (Is_rate*1.2) ) Flag.Fault.bit.OC_C= 1;

		if ((Is_mag > (Is_rate*1.2) )||(GpioDataRegs.GPADAT.bit.GPIO14==0))	
		{
			Flag.Fault.bit.OC_MAG= 1;
			Fault_Recording( 1, (double)(Is_mag),"  OC Mag  ");
		}
		if (Vdc>(Temp*1.2))	OV_Timer++;
		else if (OV_Timer>0)	OV_Timer--;
		if (OV_Timer>=10)	Flag.Fault.bit.OV= 1;
		if (Vdc<(Temp*0.8))	UV_Timer++;
		else if (UV_Timer>0)	UV_Timer--;
//		if (UV_Timer>=10)	Flag.Fault.bit.UV= 1;	 chy  masking for test
	 	
		First_step= COUNT_END+1.;





	}
	else First_step++;

 

	
//	if (Temp>???.)	OH_Timer++;
//	else if (Temp_Timer>0)	OH_Timer--;
//	if (OH_Timer>=10)	Flag.Fault.bit.OH= 1;	






	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
}


void DAC_Setup()
{	
	// Single Buffer
	DA1_val=((int)((DA1-DA1_mid)*0x7FF/(DA1_rng))+0x0800)&0x0FFF;
	DA2_val=((int)((DA2-DA2_mid)*0x7FF/(DA2_rng))+0x0800)&0x0FFF;
	DA3_val=((int)((DA3-DA3_mid)*0x7FF/(DA3_rng))+0x0800)&0x0FFF;
	DA4_val=((int)((DA4-DA4_mid)*0x7FF/(DA4_rng))+0x0800)&0x0FFF;


	ZONE0_BUF[0x0060] = 0x0003; //LDAC_ON, WRITE   

	ZONE0_BUF[0x0030] = DA1_val;
	asm ("      nop");
	ZONE0_BUF[0x0031] = DA2_val;
	asm ("      nop"); 
	ZONE0_BUF[0x0032] = DA3_val;
	asm ("      nop"); 
	ZONE0_BUF[0x0033] = DA4_val;
	asm ("      nop"); 

	ZONE0_BUF[0x0060] = 0x0007; //LDAC_ON, WRITE   

	ZONE0_BUF[0x0060] = 0x0005; //LDAC_OFF, READ
	ZONE0_BUF[0x0060] = 0x0007; //LDAC_ON, WRITE 

} 



// end of file

