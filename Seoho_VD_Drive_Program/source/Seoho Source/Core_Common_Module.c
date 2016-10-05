#include	<All_Header.h>
#include	<All_Extern_Variables.h>

//------------------------------
//
//------------------------------

#define COMMAND_SOURCE		0	
void get_command( int * command, double * Init_reference )
{
	int digital_cmd;
	double digital_reference;

	
	DI_Processing( & digital_cmd, & digital_reference);

	
// button에 의한 정지는 옵션과 상관없이 무조건 실행이 된다.
	switch( COMMAND_SOURCE )
	{
	case 0: // 디지털 입력에 의한 시동과 정지 
		* command = digital_cmd;
		* Init_reference = digital_reference;
		break;
	case 1: // oerater (RS232)
		* command = CMD_NULL;
		* Init_reference = 0.0;
		break;
	case 2: // Synchronous_Ctrl : reserved
		* command = CMD_NULL ;
		* Init_reference = 0.0;
		break;
	case 3: // Fieldbus	; reseved  
		* command = CMD_NULL ;
		* Init_reference = 0.0;
		break;
	case 4: // Free Funcion Logic	; reseved  
		* command = CMD_NULL ;
		* Init_reference = 0.0;
		break;
	default:
		* command = CMD_STOP; 
		break;

		
	}
	if (Flag.Fault.all != 0)	* Init_reference = 0.0;
	
}




void Variable_Initialization()
{
	Vdc_max=0;

	Flag.DI.all= 0.;
	Flag.DO.all= 0.;
	Flag.Fault.all= 0.;
	Flag.ETC.all= 0.;
	FaultInfo.DATA= 0.;

	Load_Power= 1.5e+3;
	Supply_Voltage= 380.;    
//	Load_Power= 5.5e+3;
//	Supply_Voltage= 220.; 
//--------Motor Parameters--------//
/*
	Rs=2.0; Rr=0.9; Lsigma=12e-3; Lm=154e-3;
	Jm=0.005; inv_Jm=200; Pole_pair=2.; Bm= 0.; 
	LAMdre_rate = 0.777, LAMdre_ref = 0.777;              // 380V 수정 0.45*(380/220)
	Lr= Lls= Llr= 0.; 
	Is_rate=(20.0*SQRT2); Wrpm_rate=1470.; Wrpm_max=12000.;  // 380V시 수정
	PPR=4096;
*/
//------Motor Parameters End------//   
//--------Motor Parameters--------//
	Rs= 0.504;
	Rr= 0.244; 
	Lsigma= 5.49e-3;
	Ls= 61.52e-3;
	Lr= 60.90e-3;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm=0.005, inv_Jm=200, Pole_pair=2., Bm= 0.; 
	LAMdre_rate = 0.777, LAMdre_ref = 0.777; 
	Is_rate=(20.*SQRT2), Wrpm_rate=1470., Wrpm_max=3500.;
	PPR=4096;
 
 //------Motor Parameters End------//  


	Is_max = Is_rate;
	if (Is_max>25.)	Is_max= 25.;		



	// Motor Parmeter
 //	Ls = Lr = (Lsigma + sqrt(Lsigma*Lsigma + 4.*Lm*Lm) ) / 2.;

	//encoder
	EQep1Regs.QPOSLAT= 0;
	


	Init_reference=0.;
	Final_reference=0.; 
	Wrpm_scale=1500.;
	Accel_time=5.0000;	
	Decel_time=5.0000;
	 
	Theta=0.0;

	// 전류
	Is_mag=0.0;
	Is_mag_rms=0.0;


	// Flux Estimation  
	K_CM1= (1.-(TSAMP_CC/2.)*(Rr/Lr))/(1.+(TSAMP_CC/2.)*(Rr/Lr));
	K_CM2= (TSAMP_CC/2.)*(Rr/Lr)*Lm/(1.+(TSAMP_CC/2.)*(Rr/Lr));

	Freq_Fst= 10.;
	Wc_Fst= (2.*PI)*Freq_Fst;
	Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
	Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;

	// Current Controller
	Wc_cc= 1250.; //1250
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Wc_fc= 50.;
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;
	Idse_ref_max = Is_max/SQRT2; 		
	

	// Speed Controller 
	Wc_sc= 250.;
//	Kp_sc= Jm*Wc_sc/Kt;
//	Ki_sc= Kp_sc*(Wc_sc/7.);
//	Ka_sc= 2./Kp_sc;

	// Speed_Calc
	L1_sp_est= 3*SPEED_OB_GAIN;
	L2_sp_est= 3*SPEED_OB_GAIN*SPEED_OB_GAIN;
	L3_sp_tmp= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN);
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;

	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );


}


//---------------------------------
// end of file
//----------------------------------

