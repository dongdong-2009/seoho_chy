#include	<All_Header.h>
#include	<All_Extern_Variables.h>

#define		SPD1			(500.*RPM2WRM)
#define		SPD2			(10.*RPM2WRM)

#if (!ENCODER_TYPE)

void  EQEP_Initialization(void)
{										// 0~59999
	EQep1Regs.QUPRD=SPEED_CALC_PRD-1;	// Unit Timer 
	EQep1Regs.QDECCTL.bit.QSRC=00;		// QEP quadrature count mode
		
	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=00;		// PCRM=00 mode - QPOSCNT reset on index event
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on QPOSCNT :0, unit time out :1
	EQep1Regs.QPOSMAX=0xffffffff;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable
		
	EQep1Regs.QCAPCTL.bit.UPPS=0;   	// 1/1 for unit position
	EQep1Regs.QCAPCTL.bit.CCPS=0;		// 1/1 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable
	
	EQep1Regs.QPOSCNT=0;
	EQep1Regs.QCTMR=0;
	EQep1Regs.QPOSLAT=0;
	EQep1Regs.QCTMRLAT=0;
	EQep1Regs.QCPRD=0xffff;


	EQep1Regs.QPOSLAT= 0;

}

void Speed_Calculation()
{

	long pls, mt, pulse_num, i;
	double MT_Time, a;
	double K_Wrm= 0.,K_Wrm1= 0., K_Wrm2= 0.;
	double Pulse2Thetarm= 0., Thetarm_enc= 0.; 
	double Te_tmp=0., Wrm_tmp=0., Thetarm_tmp=0., err_tmp=0., Tl_tmp=0.; 
	static long Pulse_Counter_MT= 0., Pulse_Counter_est=0.; 
	static long Dir_mt=1, Dir_est=1;
	static long	MT_Timer= 0.,  Overflow_num=0., Flag_first_MT= 0.;
	static double Tl_est= 0.;
	static double Wrm_mt= 0., Wrm_mt_flt= 0., Wrm_mt_old= 0.;
	static double Wrm_est= 0., Wrm_est_flt= 0., Wrm_est_old= 0.;
	static double Wrm_det_old= 0.; 
	static double Thetarm_err= 0., Thetarm_est= 0.; 
//	static double Wrpm_mt=0., Wrpm_est=0., Theta_rm_mt=0.;




	if(EQep1Regs.QFLG.bit.UTO==1) // If unit timeout (one 2500Hz period)
	{ 	
	/*----------------------------------------------*/
	/*					 MT							*/
	/*----------------------------------------------*/
		
		if (Flag.ETC.bit.FAULT_RESET)
			Flag.Fault.bit.SPEED_DETECTION= 0;

		
		/* to inititial routine */
		Pulse2Thetarm = (2.*PI)/(4.*PPR);

		if(Flag_first_MT==0) 
		{
			Flag_first_MT=1;
			Pulse_Counter_est = Pulse_Counter_MT = (EQep1Regs.QPOSLAT)&(0xffff);
			MT_Timer = (EQep1Regs.QCTMRLAT)&(0xffff);
		}
		else 
		{
			pls = (EQep1Regs.QPOSLAT)&(0xffff);
			mt = (EQep1Regs.QCTMRLAT)&(0xffff);

	
			/* MT Rountine */
			if(pls!=Pulse_Counter_MT) 
			{
				i = pls - Pulse_Counter_MT;
				if(i>(32768))		pulse_num = i - (65536);
				else if(i<-(32768))	pulse_num = i + (65536);
				else				pulse_num = i;
				Pulse_Counter_MT=pls;

   				/* Dir_mt : 1(positive) -1(negative) */		
				a = pulse_num;
				if(pulse_num*Dir_mt<0) 
				{
					a +=Dir_mt/2.;
					Dir_mt*=-1;
				} 

				MT_Time = TSAMP_SPD*(Overflow_num + 1)+ (1./CPU_CLOCK)*(float)(MT_Timer-mt);
				MT_Timer = mt;
//				Theta_rm_mt=Pulse2Thetarm*a;
				Wrm_mt = Pulse2Thetarm*a/MT_Time; 

				Overflow_num =0;
			}
			else 
			{
				Overflow_num ++;
//				Theta_rm_mt=0;
				a = TSAMP_SPD*Overflow_num + (1./CPU_CLOCK)*(float)(MT_Timer);
				if( (a>50.e-3) )	Wrm_mt=0.;
				else 
				{
					a = Pulse2Thetarm*Dir_mt/a;
					if( fabs(Wrm_mt)>fabs(a) )
						Wrm_mt = a;
				}
			}
		}

	/*----------------------------------------------*/
	/*		      Speed Observer					*/
	/*----------------------------------------------*/
//		L1_sp_est= 3*SPEED_OB_GAIN;
//		L2_sp_est= 3*SPEED_OB_GAIN*SPEED_OB_GAIN;
//		L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;
		L3_sp_est= L3_sp_tmp*Jm; 
	
		pls = (EQep1Regs.QPOSLAT)&(0xffff);
		i = pls - Pulse_Counter_est;
		if(i>(32768))		pulse_num = i - (65536);
		else if(i<-(32768))	pulse_num = i + (65536);
		else				pulse_num = i;
		Pulse_Counter_est=pls;

		/* Direction */
		a = pulse_num;
   		if(pulse_num*Dir_est<0) {
			a +=Dir_est/2.;
			Dir_est*=-1;
		}  

		/* Thetarm_enc */
		Thetarm_enc = Pulse2Thetarm*a;

		/* Calculate torque */
		Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;
		Te_tmp = Kt*Iqse_ref;

		/* Open Loop Estimation */
		Wrm_tmp = Wrm_est + (Te-Tl_est)*(TSAMP_SPD*inv_Jm);
		Thetarm_tmp = Thetarm_est + (TSAMP_SPD/2.)*(Wrm_est+Wrm_tmp);

		/* Closed Loop Estimation */
		err_tmp = Thetarm_enc-Thetarm_tmp;		
		a = Thetarm_err + err_tmp;
 		Tl_tmp = Tl_est + (L3_sp_est*a)*(TSAMP_SPD/2.);
  		Wrm_tmp = Wrm_est + (TSAMP_SPD/2.) 
  			* ( (Te+Te_tmp-Tl_est-Tl_tmp)*inv_Jm + L2_sp_est*a );
		Thetarm_tmp = Thetarm_est + TSAMP_SPD/2.*( (Wrm_est+Wrm_tmp) + L1_sp_est*a );

		/* Updating States */
		Thetarm_err = err_tmp;
		Thetarm_est = Thetarm_tmp - Thetarm_enc;
		Wrm_est = Wrm_tmp;
		Tl_est = Tl_tmp;
		Te = Te_tmp;



		//rpm calc.
//		Wrpm_mt = (60./(2.*PI))*Wrm_mt; 
//		Wrpm_est= (60./(2.*PI))*Wrm_est;
		


		Wrm_est_flt = 0.5*(Wrm_est+Wrm_est_old);
		Wrm_est_old = Wrm_est;

		Wrm_mt_flt = 0.5*(Wrm_mt+Wrm_mt_old);
		Wrm_mt_old = Wrm_mt;	

		a = fabs(Wrm_mt_flt);
		if(a>=(2.*SPD1))  	K_Wrm1=0.;
		else if(a<=SPD1) 	K_Wrm1=1.;
		else				K_Wrm1=2.-a/SPD1;

		a = fabs(Wrm_est_flt - Wrm_mt_flt);
		if( a < SPD2 ) {
			K_Wrm2 = -a*a/(2.*SPD2*SPD2)+1.;
		}
		else if ( a < (2.*SPD2) ) {
			a = a - (2.*SPD2);
			K_Wrm2 = a*a/(2.*SPD2*SPD2);
		}
		else	K_Wrm2 = 0.;
		K_Wrm = K_Wrm1*K_Wrm2; 
//		K_Wrm = 0.;
	
		Wrm_det = (1.-K_Wrm)*Wrm_mt_flt + K_Wrm*Wrm_est_flt;
		Wrm_det_flt = 0.5*(Wrm_det+Wrm_det_old);		// Wc = 2/T = 2/100u = 20,000 rad/s
		Wrm_det_old = Wrm_det;
		Wr = Wrm_det_flt*Pole_pair;
		Wrpm_det = (60./(2.*PI))*Wrm_det; 
		

//		if (fabs(Wrpm_det)>Wrpm_max)	Flag.Fault.bit.OVER_SPEED= 1;   chy masking for test

		// QEP initialization
		EQep1Regs.QCLR.bit.UTO=1;  // Clear Unit Timer Flag
  		EQep1Regs.QEPSTS.all=0x00; // Clear All flag
								   
	}
	else // ¼Óµµ ÃøÁ¤ ÇÏÁö ¾ÊÀ¸¸é  Fault ¹ß»ý
	{
		Flag.Fault.bit.SPEED_DETECTION= 1.;
	}
}


#else 


void Speed_Calculation_ABS()
{
	long pls, pulse_num, i;
	double ABS_Time, a;
	double K_Wrm= 0.,K_Wrm1= 0., K_Wrm2= 0.;
	double Pulse2Thetarm= 0., Thetarm_enc= 0.; 
	double Te_tmp=0., Wrm_tmp=0., Thetarm_tmp=0., err_tmp=0., Tl_tmp=0.; 
	static long Pulse_Counter_ABS= 0., Pulse_Counter_est=0.; 
	static long Dir_ABS=1, Dir_est=1;
	static long	Overflow_num=0., Flag_first_ABS= 0.;
	static double Tl_est= 0.;
	static double Wrm_ABS= 0., Wrm_ABS_flt= 0., Wrm_ABS_old= 0.;
	static double Wrm_est= 0., Wrm_est_flt= 0., Wrm_est_old= 0.;
	static double Wrm_det_old= 0.; 
	static double Thetarm_err= 0., Thetarm_est= 0.;  


	double turn_new, turn_delta;
    double step_new, step_delta;
//	Uint16 step_new, step_delta;
    double X_delta_buf, X_delta;
	static Uint16 turn_old = 0, step_old = 0;

	
 // 	 while( McbspaRegs.SPCR1.bit.RRDY == 0 ) { NOP; }

//		rdata2 = McbspaRegs.DRR2.all;		                             // Read DRR2 first.
//		rdata1 = McbspaRegs.DRR1.all;                      // Then read DRR1 to complete receiving of data

// 		while( McbspaRegs.SPCR1.bit.RRDY == 0 ) { NOP; }
		inv_rdata2 = ~rdata2;
		inv_rdata1 = ~rdata1;
		Steps_buf1 = 0x0007 & inv_rdata2;
		Number_of_turns_buf = (0x7FF8 & inv_rdata2)>>3;
		Steps_buf2 = (inv_rdata1 >> 7) & 0x03FF;
		Steps_per_turn_buf = (Steps_buf1 <<9) | Steps_buf2;
					
		gNum =Steps_per_turn_buf;
		DecimalToBianary(gNum, SIZE, bin);	
		GrayToBinary(SIZE, bin, g2b);	
		BinaryToDecimal(SIZE,g2b);
		Steps_per_turn = result1;

		gNum = Number_of_turns_buf;
		DecimalToBianary(gNum, SIZE, bin);	
		GrayToBinary(SIZE, bin, g2b);	
		BinaryToDecimal(SIZE,g2b);
		Number_of_turns = result1;

 /*
	if (Number_of_turns % 2 == 0)
	{
		abs_position = 	Steps_per_turn;
	}
	
	else abs_position = 4096 - Steps_per_turn;
*/
	if (Number_of_turns % 2 == 0)
	{
		abs_position = 	4096 - Steps_per_turn;
	}
	
	else abs_position = Steps_per_turn;



	turn_new = (double)Number_of_turns;
	turn_delta = turn_new - turn_old;

	if (turn_delta < 0.)
	{
   		turn_delta = fabs(turn_delta);
	    turn_delta = 4096. - turn_old + turn_new;
	}

	turn_old = turn_new; 

	step_new = (double)abs_position;
	step_delta = step_new - step_old;
	
	test3=step_delta;

	if (test3>10)
	{
//		test6 = step_new; 
//		test7 = step_old;
	}

/*
	if (step_delta < 0)
	{		
		step_delta = 4096 - step_old + step_new;
		turn_delta = turn_delta - 1.;   // (delta - 1turn)
	}
*/

			if(step_delta>(2048))		step_delta = step_delta - (4096);
			else if(step_delta<-(2048))	step_delta = step_delta + (4096);
			else				step_delta = step_delta;


	if (step_delta == 0)
	{
	
		Wrpm_det1 = 0;	
	}

//	È¸ÀüÀÚ ¯À§ X
//  X = 2 * PI * ( turn_delta*4096 ) + 2 * PI * (step_delta / 4096);
//	ü±â °¢¼Óµµ Wrm = X / T (800uS)  
//	Wrm = X / 800e-6    -> X * 1250
//	¼Óµµ (rpm) Nf = 60 / (2*PI) * Wrm
//	N = 60  * (turn_delta *4096 + step_delta / 4096);   
	
//	X_delta_buf = ( (turn_delta * 4096.) +  (step_delta / 4096.));

   		X_delta_buf = step_delta / 4096.;
		X_delta = 2. * PI * X_delta_buf;
		Wrn1 = X_delta * 1250.;
		Wrpm_det1 = 60. *  X_delta_buf * 1250.;
//		if ( (Wrpm_det1>=10000) || (Wrpm_det1<= -10000) )
//		{ Temp_HHH2= step_delta; Temp_HHH3=Wrpm_det1; test1 = step_new; test2=step_old;
//		  test4=step_delta;test5=test3;}
			step_old = step_new;

/*

	if (0 < step_delta < 1)
	{
		Wrpm_det1 = 0;
		step_delta = 0;
	}

	else
	{
 		X_delta_buf = step_delta / PPR;
		X_delta = 2. * PI * X_delta_buf;
		//	Temp_HHH2 = X_delta /TSAMP_SC;
		Wrpm_det1 = 60. *  X_delta_buf /TSAMP_SC;	
	}
*/

	Pulse2Thetarm = (2.*PI)/(PPR);

	if(Flag_first_ABS==0) 
	{
		Flag_first_ABS=1;
		Pulse_Counter_est = Pulse_Counter_ABS = (abs_position)&(0xfff);
		ABS_Time = TSAMP_SPD;
	}
	else 
	{
		pls = (long)(abs_position)&(0xfff);
//		Temp_HHH1= abs_position;
		/* MT Rountine */
		if(pls!=Pulse_Counter_ABS) 
		{
			i = pls - Pulse_Counter_ABS;
			
			if(i>(2048))		pulse_num = i - (4096);
			else if(i<-(2048))	pulse_num = i + (4096);
			else				pulse_num = i;
			Pulse_Counter_ABS=pls;
			
			
			/* Dir_mt : 1(positive) -1(negative) */		
			a = pulse_num;
			if(pulse_num*Dir_ABS<0) 
			{
				a +=Dir_ABS/2.;
				Dir_ABS*=-1;
			} 
			ABS_Time = TSAMP_SPD*(Overflow_num + 1);
			Wrm_ABS = Pulse2Thetarm*a/ABS_Time; 
			
			Overflow_num =0;
		}
		else 
		{
			Overflow_num ++;
//				Theta_rm_mt=0;
			a = TSAMP_SPD*Overflow_num;
			if( (a>50.e-3) )	Wrm_ABS=0.;
			else 
			{
				a = Pulse2Thetarm*Dir_ABS/a;
				if( fabs(Wrm_ABS)>fabs(a) )
					Wrm_ABS = a;
			}
		
		}
		a= (60./(2.*PI))*Wrm_ABS;
		if ( (a>=10000) || (a<= -10000) )
		{ Temp_HHH2= pls; Temp_HHH3=a;}
	}
	

	/*----------------------------------------------*/
	/*		      Speed Observer					*/
	/*----------------------------------------------*/
//	L1_sp_est= 3*SPEED_OB_GAIN;
//	L2_sp_est= 3*SPEED_OB_GAIN*SPEED_OB_GAIN;
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;
	L3_sp_est= L3_sp_tmp*Jm; 
	
	pls = (long)(abs_position)&(0xfff);
	i = pls - Pulse_Counter_est;	
	if(i>(2048))		pulse_num = i - (4096);
	else if(i<-(2048))	pulse_num = i + (4096);
	else				pulse_num = i;
	Pulse_Counter_est=pls;
	
	/* Direction */
	a = pulse_num;
		if(pulse_num*Dir_est<0) {
		a +=Dir_est/2.;
		Dir_est*=-1;
	}  

	/* Thetarm_enc */
	Thetarm_enc = Pulse2Thetarm*a;

	/* Calculate torque */
	Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;
	Te_tmp = Kt*Iqse_ref;

	/* Open Loop Estimation */
	Wrm_tmp = Wrm_est + (Te-Tl_est)*(TSAMP_SPD*inv_Jm);
	Thetarm_tmp = Thetarm_est + (TSAMP_SPD/2.)*(Wrm_est+Wrm_tmp);

	/* Closed Loop Estimation */
	err_tmp = Thetarm_enc-Thetarm_tmp;		
	a = Thetarm_err + err_tmp;
		Tl_tmp = Tl_est + (L3_sp_est*a)*(TSAMP_SPD/2.);
		Wrm_tmp = Wrm_est + (TSAMP_SPD/2.) 
			* ( (Te+Te_tmp-Tl_est-Tl_tmp)*inv_Jm + L2_sp_est*a );
	Thetarm_tmp = Thetarm_est + TSAMP_SPD/2.*( (Wrm_est+Wrm_tmp) + L1_sp_est*a );

	/* Updating States */
	Thetarm_err = err_tmp;
	Thetarm_est = Thetarm_tmp - Thetarm_enc;
	Wrm_est = Wrm_tmp;
	Tl_est = Tl_tmp;
	Te = Te_tmp;


	Wrm_est_flt = 0.5*(Wrm_est+Wrm_est_old);
	Wrm_est_old = Wrm_est;
	Wrm_ABS_flt = 0.5*(Wrm_ABS+Wrm_ABS_old);
	Wrm_ABS_old = Wrm_ABS;	
//	Wrpm_ABS = (60./(2.*PI))*Wrm_ABS_flt;
//	Wrpm_est= (60./(2.*PI))*Wrm_est_flt;
//	Temp_HHH1= (60./(2.*PI))*Wrm_ABS; 
//	Temp_HHH2= (60./(2.*PI))*Wrm_est; 

	a = fabs(Wrm_ABS_flt);
	if(a>=(2.*SPD1))  	K_Wrm1=0.;
	else if(a<=SPD1) 	K_Wrm1=1.;
	else				K_Wrm1=2.-a/SPD1;

	a = fabs(Wrm_est_flt - Wrm_ABS_flt);
	if( a < SPD2 ) {
		K_Wrm2 = -a*a/(2.*SPD2*SPD2)+1.;
	}
	else if ( a < (2.*SPD2) ) {
		a = a - (2.*SPD2);
		K_Wrm2 = a*a/(2.*SPD2*SPD2);
	}
	else	K_Wrm2 = 0.;
	K_Wrm = K_Wrm1*K_Wrm2; 
//		K_Wrm = 0.;

	Wrm_det = (1.-K_Wrm)*Wrm_ABS_flt + K_Wrm*Wrm_est_flt;
	Wrm_det_flt = 0.5*(Wrm_det+Wrm_det_old);		// Wc = 2/T = 2/100u = 20,000 rad/s
	Wrm_det_old = Wrm_det;
	Wr = Wrm_det_flt*Pole_pair;
	Wrpm_det = (60./(2.*PI))*Wrm_det;  


}

#endif //#if ENCODER_TYPE


