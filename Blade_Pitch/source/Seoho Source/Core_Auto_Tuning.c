#include	<All_Header.h>
#include	<All_Extern_Variables.h>


void Auto_Tuning()
{
	static long Auto_tuning_counter= 0, Ls_count= 0;
	static double Wrpm_scale_backup= 0.;
	static double Vdse_ref_sum= 0., Vqse_ref_sum= 0., Idse_sum= 0., Iqse_sum= 0., We_sum= 0.; 		
	static double Vdse_ref_avg0= 0., Vdse_ref_avg1= 0.;
	static double Error_integ= 0.0;
	static double Thetae= 0., Thetae_cm= 0.;
	static double Wrm_det_flt_sum= 0., Te_sum= 0., Wrm0= 0., dWrm_dt_sum= 0., Wrm_old= 0.;
	static double Idse0= 0., Lsigma_sum= 0.; 
	static int Flag_B= 0, Flag_J= 0, Flag_Lsigma= 0, Flag_Ls= 0, Flag_Rr= 0;
	double Error= 0., a= 0.;
	
	switch (Auto_tuning_index)
	{	
		case 0 :	// Flag check

			if (Flag.ETC.bit.AUTO_TUNING)
			{
				Wrpm_scale_backup= Wrpm_scale;
				Wrpm_scale= Wrpm_fw1*0.9;
				Accel_time_Auto_tuning= Decel_time_Auto_tuning= 10.0;
				Auto_tuning_index++;
				Auto_tuning_counter= 0.0;
				PWM_ON_OFF(1);	
				Driver_ON=1;
			}

		    break;

		case 1 :	// Dead time voltage_compensation & Stator resistance estimation 1
		
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.45; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=2000)
			{
				Vdse_ref_avg0= Vdse_ref_sum*0.001;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Auto_tuning_index++;
			}
			else if (Auto_tuning_counter>1000)
				Vdse_ref_sum+= Vdse_ref;

			break;

		case 2 : 	// Dead time voltage_compensation & Stator resistance estimation 2
			
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.9; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=2000)
			{
				Vdse_ref_avg1= Vdse_ref_sum*0.001;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Error= 2.0- Vdse_ref_avg1/Vdse_ref_avg0;
				Error_integ+= 1.e-6*TSAMP*4000*Error;
				T_dead_Tuning= 2.5e-8*Error + Error_integ;
			
				if ( (Error>(-0.01))&&(Error<0.01) )
				{
					Rs= (Vdse_ref_avg1-Vdse_ref_avg0)/(Idse_ref_max*0.45);
					Auto_tuning_counter= 0;
					Idse_ref= 0.;
					Error_integ= 0.;
					Ki_cc= Rs*2.*Wc_cc; //전류제어기 이득 Update
					Auto_tuning_index=7;
				}
				else	Auto_tuning_index--;
				
			}
			else if (Auto_tuning_counter>1000) // 회전자 시정수로 수정 할것 
				Vdse_ref_sum+= Vdse_ref;
			
			break;

		case 3 :	// Stator transient inductunce(Sigma_Ls) estimation
	
			Auto_tuning_counter++;
			Vqse_ref= 0;
		
			if (Auto_tuning_counter<=6)
			{
				PWM_ON_OFF(1);
				Driver_ON=1;
				if (Flag_Lsigma)
					Vdse_ref= Supply_Voltage*0.4;
				else Vdse_ref= Supply_Voltage*0.1;
			}
			else if (Auto_tuning_counter<50)
			{
				Vdse_ref= 0;
				PWM_ON_OFF(0);
				Driver_ON=0;
			}
			else	
			{
				Auto_tuning_counter= 0;	
				if (!Flag_Lsigma)
				{
					Ls_count++;
					// 초기 여자 시간 대기 
					if (Ls_count>=500)
					{
						Flag_Lsigma= 1;
						Ls_count= 0;
					}
				}
			}

			if (Flag_Lsigma)
			{	
				if (Auto_tuning_counter==3)
					Idse0= Idse;
				else if (Auto_tuning_counter==7)
				{
					Ls_count++;
					if (Ls_count>=600)
					{
						// Lsigma의 크기가 크게나와서 Hand Tuning 4->4-1 -> 검증 필요함 
						Lsigma= Supply_Voltage*0.5*(4-1)*TSAMP/(Lsigma_sum*0.002);
						Idse_sum= 0.;
						Lsigma_sum= 0.;
						Ls_count= 0;
						Flag_Lsigma= 0;
						Auto_tuning_counter= 0;
						Auto_tuning_index++;
						//전류제어기 이득 Update
						Kp_cc= Lsigma*Wc_cc; 
						Ka_cc= 3./Kp_cc;
						//약계자 제어 기준 Update
						K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           					+(Lsigma*Lsigma)*(Is_max*Is_max); 
						K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) ); 
						PWM_ON_OFF(1);
						Driver_ON=1;
					}
					else if (Ls_count>100)
						Lsigma_sum+= Idse-Idse0;
				}
			}

			break;

		case 4 :	// Ls, Lr, Lm estimation
				
			if (!Flag_Ls)
			{
				Auto_tuning_counter++;
				if (Auto_tuning_counter>=200)
				{
					Auto_tuning_counter= 0;
					Init_reference_Auto_tuning= 1.0;
					Flag_Ls= 1;
				}
				else 
				{
					Idse_ref= 0;
					Iqse_ref= 0;
				}
			}
			else 
			{
				Reference_Function_Auto_Tuning();
				if ((Final_reference>=1.0)&&(fabs(Wrpm_scale-Wrpm_det)<10))
				{
					Auto_tuning_counter++;
					Vqse_ref_sum+= Vqse_ref;
					Idse_sum+= Idse;
					Iqse_sum+= Iqse;
					We_sum+= We;
					if ( Auto_tuning_counter>=10000 )
					{
						Ls= Lr= (Vqse_ref_sum-Rs*Iqse_sum)/((We_sum*0.0001)*Idse_sum);
						Lm= sqrt((Ls-Lsigma)*Lr);
						Lls= Ls-Lm;
						Llr= Lr-Lm;
						Auto_tuning_counter= 0.0;
						// 전류 Model Update 
						K_CM1= (1.-(TSAMP_CC/2.)*(Rr/Lr))/(1.+(TSAMP_CC/2.)*(Rr/Lr));
						K_CM2= (TSAMP_CC/2.)*(Rr/Lr)*Lm/(1.+(TSAMP_CC/2.)*(Rr/Lr));
						// 자속각 보상 이득 Update
						Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
						Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;
						// 자속제어기 이득 Update
						Kp_fc= (Lr/Lm)*Wc_fc/Rr;
						Ki_fc= Wc_fc/Lm;
						Ka_fc= 2./Kp_fc;
						//약계자 제어 기준 Update
						K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
					           +(Lsigma*Lsigma)*(Is_max*Is_max);
						K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );

						Auto_tuning_index++;
					}
				}				
			}
			break;

		case 5 :	// Inertia & Coefficient of friction(B) estimation
			

			if (Final_reference>= 1.0)
				Flag_B= 1;

			if (Flag_J)
			{
				Auto_tuning_counter++;
				if (Auto_tuning_counter<= 1)
				{
					Wrm0= Wrm_det_flt;
					Wrm_old= Wrm_det_flt;
				}
				else
				{
					dWrm_dt_sum+= (Wrm_det_flt-Wrm_old)/TSAMP;
					Wrm_old= Wrm_det_flt;
				}

				if (Wrm_det_flt<=10)
				{
					Jm_est= (-Wrm0*Bm/(dWrm_dt_sum/(Auto_tuning_counter-1))); 
					Flag_J= 0;
					Init_reference_Auto_tuning= 0.;
					Auto_tuning_index= 7; // 임시 HHH
					PWM_ON_OFF(1);	
					Driver_ON=1;
				}
			}

			if (Flag_B)
			{					
				Auto_tuning_counter++;
				Te_sum+= Te;
				Wrm_det_flt_sum+= Wrm_det_flt;
				if (Auto_tuning_counter>=10000)
				{
					Bm= Te_sum/Wrm_det_flt_sum;
					Te_sum= 0.;
					Wrm_det_flt_sum= 0;
					Auto_tuning_counter= 0;
					Final_reference= 0.0;
					Flag_B= 0;
					Flag_J= 1;
					PWM_ON_OFF(0);	
					Driver_ON=0;
				}
			}

		    break; 
		    			 
		case 6 :	// Rotor time constant estimation

			Thetae= atan2(LAMqrs,LAMdrs);
			Thetae_cm= atan2(LAMqrs_cm,LAMdrs_cm);
//			Auto_tuning_index++;
			if (Flag_Rr==0)
			{
				Error_integ= Rr/Lr*1.5e+3/Load_Power;
				Accel_time_Auto_tuning= Decel_time_Auto_tuning= 0.2;
				Auto_tuning_counter++;
				if (Auto_tuning_counter>=200)
				{
					Auto_tuning_counter= 0;
					Init_reference_Auto_tuning= 0.3;
					Flag_Rr= 1;
				}
				else
				{
					Idse_ref= 0;
					Iqse_ref= 0;
				}
			} 
			else if (Flag_Rr==1)
			{
				
				Reference_Function_Auto_Tuning();
				if ((Final_reference==0.3)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Init_reference_Auto_tuning= 0.7;
				}
				else if ((Final_reference==0.7)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Init_reference_Auto_tuning= 0.3;
				}
				
				DA1=Thetae;	DA1_mid=0;	DA1_rng=4;  
				DA2=Thetae_cm;	DA2_mid=0;	DA2_rng=4;				
				a= Thetae-Thetae_cm;
				if (fabs(a)<(We*TSAMP))		a= 0.;
				Error= Iqse*BOUND_PI(a);
				Error_integ+= 0.5*TSAMP*Error;
				// 전류 Model Update
//				K_CM1= (1.-(TSAMP_CC/2.)*(Rr/Lr))/(1.+(TSAMP_CC/2.)*(Rr/Lr));
//				K_CM2= (TSAMP_CC/2.)*(Rr/Lr)*Lm/(1.+(TSAMP_CC/2.)*(Rr/Lr));
				// 자속 제어기 이득 Update
//				Kp_fc= (Lr/Lm)*Wc_fc/Rr;
//				Ka_fc= 2./Kp_fc;
		

			}
/*
			else if (Flag_Rr==1)
			{
				if ((Final_reference>=0.6)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Auto_tuning_counter++;
					if (Auto_tuning_counter>=10000)
					{
						LAMrs_mag_mean= Temp_HHH= LAMrs_mag_sum*0.0001;
						LAMrs_mag_sum= 0;
						Init_reference_Auto_tuning= 0.2;
						Flag_Rr= 2;
						Auto_tuning_counter= 0;
					}
					else LAMrs_mag_sum+= LAMrs_mag;

				}

			}
			else if (Flag_Rr==2)
			{
				if ((Final_reference<=0.2)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Error= -LAMrs_mag_mean+LAMrs_cm_mag;
					Error_integ+= 0.5*TSAMP*Error;
					Rr= 0.1*Error + Error_integ; // Tr= Lr/Rr
				}
			}
*/

/*
			else
			{
				if ((Final_reference>=0.5)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Init_reference_Auto_tuning= -0.5;
				}
				else if ((Final_reference<=-0.5)&&(fabs(Wrpm_ref-Wrpm_det)<10))
				{
					Init_reference_Auto_tuning= 0.5;
				}
			}
*/
/*				
				Error= Iqse*(Thetae-Thetae_cm) ;
				Error_integ+= 0.5*TSAMP*Error;
				Rr= (0.1*Error + Error_integ)*Lr; // Tr= Lr/Rr
*/			
/*				Error= Iqse*(LAMrs_mag-LAMrs_cm_mag);
				Error_integ+= 0.5*TSAMP*Error;
				Rr= 0.1*Error + Error_integ; // Tr= Lr/Rr
*/
//			}

			
/*
			if ( (Error>(-0.001))&&(Error<0.001) )
			{
				Auto_tuning_counter= 0;
				Error_integ= 0.;
				Auto_tuning_index++;					
			}							
*/

			break;

		case 7 :	// complete
			
			Wrpm_scale= Wrpm_scale_backup;	
			Init_reference_Auto_tuning= Final_reference= 0.0;	
			Flag.ETC.bit.AUTO_TUNING= 0.;
			PWM_ON_OFF(0);	
			Driver_ON=0;
			Flag_AT= 0;

		    break;

		default : 
			
			Auto_tuning_index= 0;
			Flag.ETC.bit.AUTO_TUNING= 0;
//			Temp_HHH1=0;
			PWM_ON_OFF(0);	
			Driver_ON=0;

			break;
	}

}	

void Reference_Function_Auto_Tuning()
{
	double Set_ref, Out_ref, fabs_Set_ref, fabs_Out_ref; 
	static double Accel_value= 0., Decel_value= 0.;
	static double Temp= 0., Accel_delta= 0., Decel_delta= 0.;

	Set_ref= Init_reference_Auto_tuning;
	Out_ref= Final_reference;
	fabs_Set_ref= fabs(Set_ref);
	fabs_Out_ref= fabs(Out_ref);
		
	if (Set_ref!=Temp)
	{
		if ((Set_ref*Out_ref)<0)
		{
			Accel_delta= Set_ref;
			Decel_delta= Out_ref;
		}
		else if (fabs_Set_ref>fabs_Out_ref)
		{
			Accel_delta= Set_ref-Out_ref;
			Decel_delta= 0.;
		}
		else if (fabs_Set_ref<fabs_Out_ref)		
		{
			Accel_delta= 0.;
			Decel_delta= Out_ref-Set_ref;
		}
		else
		{
			Accel_delta= 0.;
			Decel_delta= 0.;
		}
		
		Accel_value= Accel_delta/Accel_time_Auto_tuning*TSAMP;
		Decel_value= Decel_delta/Decel_time_Auto_tuning*TSAMP;
	}

	Temp= Set_ref;

	if ((fabs(Set_ref-Out_ref)<fabs(Decel_value))
		||(fabs(Set_ref-Out_ref)<fabs(Accel_value)))
	{	
		Out_ref= Set_ref; 
	}	
	else if ((Set_ref*Out_ref)<0)			Out_ref-= Decel_value; 
	else 
	{
		if (fabs_Set_ref>fabs_Out_ref)		Out_ref+= Accel_value;
		else if (fabs_Set_ref<fabs_Out_ref)	Out_ref-= Decel_value;
		else								Out_ref=  Set_ref;
	}
	
	Final_reference= BOUND(Out_ref,1,(-1));
}


// end of file

