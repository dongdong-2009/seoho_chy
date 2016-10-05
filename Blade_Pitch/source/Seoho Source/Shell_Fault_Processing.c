#include	<All_Header.h>
#include	<All_Extern_Variables.h>

// 메 PWM에서 확인한다. 
void Fault_Recording(int Fault_code, double Fault_data,char * st)
{
//	Fault_Recording( ERR_OVER_CURRENT_U_PHASE, (double)(Ias),"OC U_phase");
	

	FaultInfo.CODE		= Fault_code;
	FaultInfo.DATA		= Fault_data;
	FaultInfo.CURRENT	= Is_mag;
	FaultInfo.VDC 		= Vdc;
	FaultInfo.RPM 		= Wrpm_det;
	strncpy( FaultInfo.MSG,st,20) ;
}	

void Fault_Processing( )
{
	double Temp;
	static int Vdc_check_Timer= 0.;



	strncpy(MonitorMsg," FAULT        ",20);
	PWM_ON_OFF(0);	
	Driver_ON=0;
	DC_CONTACT_OFF;		// Main Charge Relay Off
	Init_reference=0.;
	Flag.ETC.bit.AUTO_TUNING= 0;

/*
		if (Command == CMD_RESET)
		{
			State_Index = STATE_POWER_ON;
			NOP;
			asm (" .ref _c_int00"); // ;Branch to start of boot.asm in RTS library
			asm (" LB _c_int00"); // ;Branch to start of boot.asm in RTS library
		}
*/
	

	if (Flag.ETC.bit.FAULT_RESET)
	{
		if ( fabs(Ias) < Is_rate)	Flag.Fault.bit.OC_A= 0;
		if ( fabs(Ibs) < Is_rate)	Flag.Fault.bit.OC_B= 0;
		if ( fabs(Ics) < Is_rate)	Flag.Fault.bit.OC_C= 0;
		if ((Is_mag < Is_rate )&&(GpioDataRegs.GPADAT.bit.GPIO14!=0))	
			Flag.Fault.bit.OC_MAG= 0;

		Temp= Supply_Voltage * SQRT2; // 입력전압 Set 값을 가져와야함 
		if ((Vdc>(Temp*0.85))&&(Vdc<(Temp*1.1)))	Vdc_check_Timer++;
		else if (Vdc_check_Timer>0) Vdc_check_Timer--;
		if (Vdc_check_Timer>10)	
		{
			Flag.Fault.bit.INIT_CHARGE= 0;
			Flag.Fault.bit.OV= 0;
			Flag.Fault.bit.UV= 0;
		}
		if (fabs(Wrpm_det)<=Wrpm_max)	Flag.Fault.bit.OVER_SPEED= 0;
		if (GpioDataRegs.GPADAT.bit.GPIO12!=0) 	Flag.Fault.bit.DRIVER= 0;
		if (GpioDataRegs.GPADAT.bit.GPIO13!=0)	Flag.Fault.bit.DB= 0;	

			
	}

}







