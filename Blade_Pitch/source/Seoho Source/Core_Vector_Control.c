#include	<All_Header.h>
#include	<All_Extern_Variables.h>

void VF_Controller()
{
	double a;

	freq_vf_set = Final_reference*Wrpm_scale/30.;
	if(freq_vf > freq_vf_set) {
		freq_vf -= 10.*TSAMP_VF;
		if(freq_vf < freq_vf_set) freq_vf = freq_vf_set;	
	}
	else {
		freq_vf += 10.*TSAMP_VF;
		if(freq_vf > freq_vf_set) freq_vf = freq_vf_set;	
	}
	We = (2.*PI)*freq_vf;
	a = Theta + We*TSAMP_VF;
	Theta = BOUND_PI(a);
	Sin_Theta_vtg = sin(Theta);    	Cos_Theta_vtg = cos(Theta);

	// Stationary -> Synchronous 
	Idse =  Cos_Theta*Idss + Sin_Theta*Iqss;
   	Iqse = -Sin_Theta*Idss + Cos_Theta*Iqss;

	Vdse_ref = 5. * freq_vf;
	Vqse_ref = 0.1;

	Ias_ref = Ias;	Ibs_ref = Ibs;	Ics_ref = Ics;
	if (!Driver_ON)	freq_vf=Theta=0.;

}

void Flux_Estimation()
{
	long i,j; 
	double a, b;
 	double alpha=0.;
	double Sin_WeT=0., Cos_WeT=0.;
	double Sin_alpha=0., Cos_alpha=1.;
	double Sin_Thetar=0., Cos_Thetar=1.;
//	double LAMdrs=0., LAMqrs=0.; 
//	double LAMdrs_cm=0., LAMqrs_cm=0.;
	double LAMdrs_cm_alpha=0., LAMqrs_cm_alpha=0.;
	double LAMdrs_vm_alpha=0., LAMqrs_vm_alpha=0.;
	double LAMdrs_vm_predict=0., LAMqrs_vm_predict=0.;

	static int alpha_enb=0;
	static long Theta_rm_counter=0; 
	static double Thetar= 0.;
	static double Vdss=0., Vqss=0.;
	static double Idsr=0., Iqsr=0.;
	static double LAMdss_vm=0., LAMqss_vm=0.;
//	static double LAMdrs_vm=0., LAMqrs_vm=0.;
	static double LAMdrr_cm=0., LAMqrr_cm=0.;
	static double Edss=0., Eqss=0.;
	static double Err_LAMdst=0., Err_LAMqst=0.;
	 
//	static double LAMdss=0., LAMqss=0.;
//	static double Wc_Fst=0., Kp_Fst=0., Ki_Fst=0.,  Freq_Fst=10.;
//	static double LAMqre=0., LAMdse=0., LAMqse=0., LAMdre_cm=0., LAMqre_cm=0.;


	// Flux Estimation  
    // Current Model 
#if	(!ENCODER_TYPE)
	j = (EQep1Regs.QPOSLAT)&(0xffff);
	i = j - Theta_rm_counter;
	if(i>(32768))		i = i - (65536);
	else if(i<-(32768))	i = i + (65536);
	Theta_rm_counter=j;

	a = Thetar + (2.*PI)/(PPR*4)*(float)i*Pole_pair;
#else
	j = (int)(abs_position)&(0x1fff);
	i = j - Theta_rm_counter;
	if(i>(4096))		i = i - (8192);
	else if(i<-(4096))	i = i + (8192);
	Theta_rm_counter=j;

	a = Thetar + (2.*PI)/(PPR)*(float)i*Pole_pair; 
#endif

	Thetar = (float)(Driver_ON*Flux_build_up)*BOUND_PI(a);   
														  
   	Sin_Thetar = sin(Thetar);    	Cos_Thetar = cos(Thetar);

//	K_CM1=(1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
//	K_CM2=(Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

    a = Cos_Thetar*Idss + Sin_Thetar*Iqss;
       LAMdrr_cm=K_CM1*LAMdrr_cm+K_CM2*(a+Idsr);		Idsr=a;
	a= -Sin_Thetar*Idss + Cos_Thetar*Iqss;
       LAMqrr_cm=K_CM1*LAMqrr_cm+K_CM2*(a+Iqsr);		Iqsr=a;

	LAMdrs_cm = Cos_Thetar*LAMdrr_cm-Sin_Thetar*LAMqrr_cm;
	LAMqrs_cm = Sin_Thetar*LAMdrr_cm+Cos_Thetar*LAMqrr_cm; 

	// Delay Angle Compensation 
	a = We*TSAMP_CC;
   	Sin_WeT = sin(a);    	Cos_WeT = cos(a);

	LAMdrs_vm_predict =  Cos_WeT*LAMdrs_vm - Sin_WeT*LAMqrs_vm;
	LAMqrs_vm_predict =  Sin_WeT*LAMdrs_vm + Cos_WeT*LAMqrs_vm; 

	// Flux Estimation Controller 
//	Wc_Fst = (2.*PI)*Freq_Fst;
//	Kp_Fst=(Lm/Lr)*SQRT2*Wc_Fst;		Ki_Fst=(Lm/Lr)*Wc_Fst*Wc_Fst;

	// alpha 
	a = Ki_Fst*(Lr/Lm) - We*We;		b = Kp_Fst*(Lr/Lm)*We;
	a = PI-atan2(b,a);
	alpha = (double)alpha_enb*BOUND_PI(a);

    Sin_alpha = sin(alpha);    	Cos_alpha = cos(alpha);

	// Flux Estimation Controller 
  	a=LAMdrs_cm-LAMdrs_vm_predict; 
	Edss=Edss+Kp_Fst*(a-Err_LAMdst)+Ki_Fst*TSAMP_CC*a;
   	Err_LAMdst=a;
	Edss = BOUND(Edss,20.,-20.);// (HHH 범위에 대한 정확한 수식을 생각해보자)

	a=LAMqrs_cm-LAMqrs_vm_predict;  
	Eqss=Eqss+Kp_Fst*(a-Err_LAMqst)+Ki_Fst*TSAMP_CC*a;
	Err_LAMqst=a;
	Eqss = BOUND(Eqss,20.,-20.);

    // Voltage Model 
	a=-Rs*Idss+Edss; 
	LAMdss_vm=LAMdss_vm+TSAMP_CC*Vdss_ref_old+(TSAMP_CC/2.)*(a+Vdss);	
	Vdss=a;
    LAMdrs_vm=(Lr/Lm)*(LAMdss_vm-Lsigma*Idss);

	a=-Rs*Iqss+Eqss; 
	LAMqss_vm=LAMqss_vm+TSAMP_CC*Vqss_ref_old+(TSAMP_CC/2.)*(a+Vqss);	
	Vqss=a;
    LAMqrs_vm=(Lr/Lm)*(LAMqss_vm-Lsigma*Iqss);

	// alpha compensation 
   	LAMdrs_cm_alpha =  Cos_alpha*LAMdrs_cm + Sin_alpha*LAMqrs_cm;
    LAMqrs_cm_alpha = -Sin_alpha*LAMdrs_cm + Cos_alpha*LAMqrs_cm;
		
   	LAMdrs_vm_alpha =  Cos_alpha*LAMdrs_vm + Sin_alpha*LAMqrs_vm;
    LAMqrs_vm_alpha = -Sin_alpha*LAMdrs_vm + Cos_alpha*LAMqrs_vm;

	LAMdrs = LAMdrs_cm - LAMdrs_cm_alpha + LAMdrs_vm_alpha;
	LAMqrs = LAMqrs_cm - LAMqrs_cm_alpha + LAMqrs_vm_alpha;

//	LAMdss = (Lm/Lr)*LAMdrs + Lsigma*Idss;
//	LAMqss = (Lm/Lr)*LAMqrs + Lsigma*Iqss;

	if (!Driver_ON)	{
		Edss = Eqss = LAMdss_vm = LAMqss_vm = 0.;
	}


	//---- Flux Angle Detection ----//
	if (Auto_tuning_index!= 3)
		Theta = (float)(Driver_ON*Flux_build_up)*atan2(LAMqrs, LAMdrs); //hhh
//	else if (Auto_tuning_index!= 4) Theta= 0;
	else Theta= 0;

    Sin_Theta = sin(Theta);    	Cos_Theta = cos(Theta);

	// Stationary -> Synchronous 
  	LAMdre =  Cos_Theta*LAMdrs + Sin_Theta*LAMqrs;
//  LAMqre = -Sin_Theta*LAMdrs + Cos_Theta*LAMqrs;  
// 	LAMdse =  Cos_Theta*LAMdss + Sin_Theta*LAMqss;
//  LAMqse = -Sin_Theta*LAMdss + Cos_Theta*LAMqss;
// 	LAMdre_cm =  Cos_Theta*LAMdrs_cm + Sin_Theta*LAMqrs_cm;
//  LAMqre_cm = -Sin_Theta*LAMdrs_cm + Cos_Theta*LAMqrs_cm; 

} 

void Current_Controller()
{
	double a, b;
	double Theta_vtg= 0.;
	double Idse_flt=0., Iqse_flt=0.;
	double Err_Idse= 0., Err_Iqse= 0.; 

	static double Vdse_ref_integ= 0., Vdse_ref_ff= 0.;
	static double Vqse_ref_ff= 0.;
//	static double Vdse_ref_fb=0., Vqse_ref_fb=0.; // 관측용 변수 

//	Kp_cc = Lsigma*1250.;
//	Ki_cc = Rs*2.*1250;
//	Ka_cc = 3./Kp_cc;

	a = Theta + We*(1.5*TSAMP_CC);
	Theta_vtg = BOUND_PI(a);				
	Sin_Theta_vtg = sin(Theta_vtg);    	Cos_Theta_vtg = cos(Theta_vtg);

	// Stationary -> Synchronous 
	a =  Cos_Theta*Idss + Sin_Theta*Iqss;
   	b = -Sin_Theta*Idss + Cos_Theta*Iqss;

	Idse_flt = a;
	Iqse_flt = b;

	Idse = a;
	Iqse = b;

	// d-axis Current Control 
	Err_Idse = Idse_ref-Idse_flt;
    Vdse_ref_integ+= Ki_cc*TSAMP_CC*(Err_Idse-Ka_cc*(Vdse_ref_fb+Vdse_ref_ff-Vdse_ref)); 
	Vdse_ref_fb=Vdse_ref_integ+Kp_cc*Err_Idse; // Vdse_ref_fb 은 0~Rs*Idse -> 관측용 전역변수 
	Vdse_ref_ff=-We*Lsigma*Iqse-Rr*Lm/(Lr*Lr)*LAMdre; 			
//	Vdse_ref_ff = 0.;
//	if ((Auto_tuning_index!= 3)&&(Auto_tuning_index!= 4))
	if (Auto_tuning_index!= 3)
	   Vdse_ref = Vdse_ref_fb + Vdse_ref_ff;  

	// q-axis Current Control 
	Err_Iqse = Iqse_ref-Iqse_flt;
	Vqse_ref_integ+= Ki_cc*TSAMP_CC*(Err_Iqse-Ka_cc*(Vqse_ref_fb+Vqse_ref_ff-Vqse_ref));
	Vqse_ref_fb=Vqse_ref_integ+Kp_cc*Err_Iqse;  // Vqse_ref_fb 은 0~Rs*Iqse -> 관측용 전역변수
	Vqse_ref_ff=We*Lsigma*Idse+Wr*Lm/Lr*LAMdre;
//	Vqse_ref_ff = 0.;
//	if ((Auto_tuning_index!= 3)&&(Auto_tuning_index!= 4))
	if (Auto_tuning_index!= 3)
	    Vqse_ref = Vqse_ref_fb + Vqse_ref_ff; 
	 

//	Vdse_ref = BOUND(Vdse_ref,20., -20.);
//	Vqse_ref = BOUND(Vqse_ref,20., -20.);

	// Current controller disable
	if (!Driver_ON)	{
		Vdse_ref=Vqse_ref=Vdse_ref_integ=Vqse_ref_integ=0.;
	}
	Idss_ref = Ias_ref = Cos_Theta_vtg*Idse_ref - Sin_Theta_vtg*Iqse_ref;
	Iqss_ref = Sin_Theta_vtg*Idse_ref + Cos_Theta_vtg*Iqse_ref;
	Ibs_ref = -0.5*(Idss_ref - SQRT3*Iqss_ref);
	Ics_ref = -(Ias_ref+Ibs_ref); 

} 

void SVPWM()
{
	long i;
	double a, b;
	double Vas_ref= 0., Vbs_ref= 0., Vcs_ref= 0.;
	double Van_ref= 0., Vbn_ref= 0., Vcn_ref= 0.;
	double V_max= 0., V_min= 0., V_offset= 0.;
	double V_dt= 0., K_dt= 0., inv_Vdc= 0.;
	
	#if (DUAL_PWM_INTERRUPT)
		static int Flag_UPDN_PWM= 0.;
	#endif   

	// Synchronous -> Stationary 
	Vdss_ref_old = Vdss_ref;		Vqss_ref_old = Vqss_ref;
	Vdss_ref = Vas_ref = Cos_Theta_vtg*Vdse_ref - Sin_Theta_vtg*Vqse_ref;
	Vqss_ref = Sin_Theta_vtg*Vdse_ref + Cos_Theta_vtg*Vqse_ref;
	
	// Stationary -> 3 Phases 
	Vbs_ref = -0.5*(Vdss_ref - SQRT3*Vqss_ref);
	Vcs_ref = -(Vas_ref+Vbs_ref);
	
	// Max & Min 
	if (Vas_ref > Vbs_ref)	{ 
		V_max = Vas_ref; 			V_min = Vbs_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
	}
	else { 
		V_max = Vbs_ref; 			V_min = Vas_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
 	}

	// Calculate and Add voltage offset, V_offset = voltage offset 
	V_offset = -(V_max+V_min)/2.;
	Van_ref = Vas_ref + V_offset;
	Vbn_ref = Vbs_ref + V_offset;
	Vcn_ref = Vcs_ref + V_offset;

	// Overmodulation 
//	Flag_OM=0; 
	if( (V_max-V_min) > Vdc) {
		a = Vdc/2.;		b=-a;

		Van_ref = BOUND(Van_ref,a,b);
		Vbn_ref = BOUND(Vbn_ref,a,b);
		Vcn_ref = BOUND(Vcn_ref,a,b);

		Vas_ref = (2.*Van_ref-Vbn_ref-Vcn_ref)/3.;
		Vbs_ref = (2.*Vbn_ref-Vcn_ref-Van_ref)/3.;
		Vcs_ref = -(Vas_ref + Vbs_ref);
		  
		Vdss_ref = Vas_ref;
		Vqss_ref = (Vbs_ref-Vcs_ref)/SQRT3;
		Vdse_ref =  Cos_Theta_vtg*Vdss_ref + Sin_Theta_vtg*Vqss_ref;
		Vqse_ref = -Sin_Theta_vtg*Vdss_ref + Cos_Theta_vtg*Vqss_ref;
//		Flag_OM=1; 
	}

	// Dead_Time Compensation 
	V_dt = Vdc*( (T_DEAD+T_dead_Tuning)/TSAMP_CC );	
	K_dt = V_dt/(I_DT);

	#if (DUAL_PWM_INTERRUPT)
		Flag_UPDN_PWM=1-Flag_UPDN_PWM;
		if(Flag_UPDN_PWM) {			// next down mode 
			a = K_dt*(Ias_ref+I_DT);	a = BOUND(a,V_dt,0.);		Van_ref += a;
			a = K_dt*(Ibs_ref+I_DT);	a = BOUND(a,V_dt,0.);		Vbn_ref += a;
			a = K_dt*(Ics_ref+I_DT);	a = BOUND(a,V_dt,0.);		Vcn_ref += a;
		}
		else {						// next up mode 
			a = K_dt*(Ias_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Van_ref += a;
			a = K_dt*(Ibs_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Vbn_ref += a;
			a = K_dt*(Ics_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Vcn_ref += a;
		}
		
	#else
		a= K_dt*Ias_ref;	a= BOUND(a, V_dt, (-V_dt));	Van_ref+= a;
		a= K_dt*Ibs_ref;	a= BOUND(a, V_dt, (-V_dt));	Vbn_ref+= a; 
		a= K_dt*Ics_ref;	a= BOUND(a, V_dt, (-V_dt));	Vcn_ref+= a; 
	#endif


	// Write PWM counter to ASIC 
	// Reference voltage --> Counter Value  	// max : dead_time 고려 
	inv_Vdc = 1./Vdc;
	i = (int)( (Van_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[u]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vbn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[v]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vcn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[w]=BOUND(i,EPwmPeriodCount,0);

	// HHH 수정 하였음 
	EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[u];
	EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[w];
	EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[v];

}




void Field_Weakening()
{
	double a, b;
	double Vs_max= 0.;
	double We_fw1= 0., We_fw2= 0.; 


//  DB control 시 Vdc 떨림 현상 발생 DB Control 추가할것 
//	Vs_max = (Vdc*0.95)/SQRT3;
//	Vs_max = (Vdc*0.9)/SQRT3;
	b= Supply_Voltage*SQRT2*1.25;
	if(Vdc>b) 	a = b;
	else		a = Vdc;
	Vs_max = (a*0.9)/SQRT3;

    We_fw1= sqrt( (Vs_max*Vs_max) / K_fw1 );
	We_fw2= K_fw2*(Vs_max/Is_max);
	Wrpm_fw1=We_fw1/Pole_pair*WRM2RPM;		Wrpm_fw2=We_fw2/Pole_pair*WRM2RPM;

    a=fabs(Wr)+fabs(Rr*Lm/Lr*Iqse_max_fw/LAMdre_ref);
    if(a<We_fw1) { 								
        LAMdre_ref=LAMdre_rate;
        b = LAMdre_ref/Lm;		  				Iqse_max_fw=sqrt(Is_max*Is_max-b*b);
		Flag.ETC.bit.FW_MODE=0.;
	}		
    else if( (a>We_fw1) && (a<We_fw2) ) { 			
        b= ( (Vs_max*Vs_max)/(a*a)-(Lsigma*Is_max)*(Lsigma*Is_max) ) / (Ls*Ls-Lsigma*Lsigma);
        LAMdre_ref=Lm*sqrt(b);        			Iqse_max_fw=sqrt(Is_max*Is_max-b);
		Flag.ETC.bit.FW_MODE=1.;
	}
    else {                               		
        LAMdre_ref=Lm*Vs_max/(SQRT2*a*Ls);		Iqse_max_fw=Vs_max/(SQRT2*a*Lsigma);
		Flag.ETC.bit.FW_MODE=2.;
    }
}

void Flux_Controller()
{
	double a;
	double Err_flux= 0.;
	static double Idse_ref_integ= 0.;
	static double Idse_ref_ff= 0., Idse_ref_fb= 0.;
//	Wc_fc=50.;
//	Kp_fc = (Lr/Lm)*Wc_fc/Rr;
//	Ki_fc = Wc_fc/Lm;
//	Ka_fc = 2./Kp_fc; 

	LAMdre_flt+= ((Wc_fc*5.)*TSAMP_FC)*(LAMdre-LAMdre_flt);
	if(!Driver_ON) {	  
		Idse_ref = Idse_ref_fb = Idse_ref_ff = Idse_ref_integ = 0.;
		Flux_build_up=0;
	}
	else {
		if(LAMdre_flt>(0.99*LAMdre_ref))	Flux_build_up=1;


		Err_flux = LAMdre_ref - LAMdre_flt;
		Idse_ref_integ += Ki_fc*TSAMP_FC*(Err_flux-Ka_fc*(Idse_ref_fb+Idse_ref_ff-Idse_ref));
		Idse_ref_fb = Kp_fc*Err_flux + Idse_ref_integ;
		Idse_ref_ff = LAMdre_ref/Lm;
		a = Idse_ref_fb + Idse_ref_ff;
		// Limit 
		// Lsigma tuning 실행시 -> 아래 주석처리
		if (Auto_tuning_index!= 3)
			Idse_ref  = BOUND(a, Idse_ref_max, (-Idse_ref_max));

	}	
}

void Speed_Controller()
{
	
	double a;
	double Iqse_ref_max= 0.;
	double Wsl= 0., Wrm_ref= 0., Err_Wrm= 0.;
	
	static double Wrm_flt= 0.;
	static double Iqse_ref_integ= 0.;
	static double Iqse_ref_fb= 0., Iqse_ref_ff= 0.;

	Wrm_flt+= (1250.*TSAMP_SC)*(Wrm_det_flt-Wrm_flt); //1250

//	Wc_sc = 250.;
	Kp_sc = Jm*Wc_sc/Kt;
	Ki_sc = Kp_sc*(Wc_sc/7.);
	Ka_sc = 2./Kp_sc; 

	Wrm_ref = RPM2WRM * Wrpm_ref;
	Err_Wrm = Wrm_ref - Wrm_flt;

	// Driver Off 
	if( (!Driver_ON) || (!Flux_build_up) ) {	
		Iqse_ref = Iqse_ref_fb = Iqse_ref_ff = Iqse_ref_integ = 0.;
		Wsl = We = 0.;
	}
	// Driver On 
	else {
		a = sqrt(Is_max*Is_max - Idse_ref*Idse_ref); 
//		Iqse_max_fw = a;		// temp
		if(a<Iqse_max_fw)	Iqse_ref_max = a;
		else				Iqse_ref_max = Iqse_max_fw;

		Iqse_ref_integ += Ki_sc*TSAMP_SC*(Err_Wrm-Ka_sc*(Iqse_ref_fb+Iqse_ref_ff-Iqse_ref));
		Iqse_ref_fb = Kp_sc*Err_Wrm + Iqse_ref_integ;
		Iqse_ref_ff = 0.;
		a = Iqse_ref_fb + Iqse_ref_ff;
		// Limit 
		Iqse_ref  = BOUND(a, Iqse_ref_max, (-Iqse_ref_max));
		// Angular Frequency 
		Wsl = Rr*Lm/Lr*Iqse_ref/LAMdre_ref;
		We = Wr + Wsl;	
  		   
	}

}


void Position_Control()
{
//	Wc_sc = 250.;	 (참고용) 
	double Wc_pc, a;
	static double Theta_fb, Theta_fb_flt;
	static double Theta_fb_old= 0.;
	static unsigned int first= 0;
	static double Turn_fb, Position_fb;
	static double Wrpm_ref_limit= 600.;

	// Position Control Gain
	Wc_pc= 62.5;
	
	

	// Sensing Turn & Position
	Turn_fb= (double)Number_of_turns;
	Position_fb= (double)abs_position;

	// Bound Turn_fb (0~4095 -> -2048~2047)
	if(Turn_fb>(2047))		Turn_fb -= 4096;

	// Scaling 2PI
	Turn_fb*= 2.*PI;
	Position_fb*= 2.*PI/8191.;

	// First sequence: Theta_fb_old Initialization
	if (first == 0)
	{
		Theta_fb_old= Turn_fb + Position_fb;
		first= 100;
	}	

	// Theta_fb Filtering
 	Theta_fb= Turn_fb + Position_fb;
	Theta_fb_flt= (Theta_fb + Theta_fb_old)/2.;
//	Theta_fb_flt= Theta_fb;
	Theta_fb_old= Theta_fb_flt;

//	Theta_ref = 2001;

	// Main Position Control
	Theta_err = Theta_ref - Theta_fb_flt;
	a= Wc_pc*Theta_err;

	// Saturation: Position Control Output= Speed Control Input= Speed reference
//	if (Wrpm_ref>= Wrpm_ref_limit)	Wrpm_ref= Wrpm_ref_limit;
//	else if (Wrpm_ref<= -Wrpm_ref_limit)	Wrpm_ref= -Wrpm_ref_limit;
	Wrpm_ref= BOUND(a, Wrpm_ref_limit, (-Wrpm_ref_limit));






/*
	turn_new1 = (double)Number_of_turns;
	turn_delta1 = turn_new1 - turn_old1;

	test5 = turn_new1 - turn_old1;

	if (turn_delta1 == 0.)
	{
		Theta_err = 10000.0 - Theta_fb;
		Wrpm_ref= Wc_pc*Theta_err;
	}

	else if (turn_delta1 == -1.)
	{
		Theta_err = 10000.0 - (4096-Theta_fb_old) + Theta_fb;
		Wrpm_ref= Wc_pc*Theta_err;
	}
	else 
	{
		Theta_err = 10000.0 - (4096-Theta_fb_old) + turn_delta*4096 + Theta_fb;
		Wrpm_ref= Wc_pc*Theta_err;
	}
*/
	if (!Driver_ON)	Wrpm_ref= 0.;

//	Wrpm_ref=0.;
}


// end of file

