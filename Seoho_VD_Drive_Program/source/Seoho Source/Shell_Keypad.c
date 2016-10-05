#include	<All_Header.h>
#include	<All_Extern_Variables.h> 



void DSPtoArray(unsigned char GROUP,unsigned char INDEX)
{
	Group_index = (unsigned int)(GROUP * 100 + INDEX);
	switch(Group_index)
	{
		case 100 : 	Load_Power = 100 * (float)(Comm_array[40]);
		 		 	break ;
		case 101 : 	Supply_Voltage = (float)(Comm_array[41]) / 10.;
		 		 	break ;
		case 102 : 	Is_rate = (float)(Comm_array[42] / 10.);
		 		 	break ;
/*		case 103 : 	F_rate = (float)(Comm_array[43]);
		 		 	break ;
		case 104 : 	Pn = Comm_array[44];
		 		 	break ;
*/		case 105 : 	Wrpm_rate = (float)(Comm_array[45]);
		 		 	break ;
/*		case 106 : 	Cntrl_Method = Comm_array[46];
		 		 	break ;
		case 107 : 	Fpwm = Comm_array[47];
		 		 	break ;
		case 109 : 	Vsup = (float)(Comm_array[49]) / 10.;
		 		 	break ;
		case 300 : 	Run_Stop_meth = Comm_array[80];
		 		 	break ;
		case 301 : 	RampFunc_Input_Src = Comm_array[81];
		 		 	break ; 
		case 308 : 	Accel_Time = (float)(Comm_array[88]);
		 		 	break ; 
		case 325 : 	Decel_Time = (float)(Comm_array[105]);
		 		 	break ; 
		case 500 : 	Curr_Lim = Comm_array[300] / 10;	
		 		 	break ; 
		case 507 : 	Max_Con_cur = Comm_array[307] / 10;
		 		 	break ; 
		case 508 : 	Over_Load = Comm_array[308] / 10;
		 		 	break ; 
		case 511 : 	OC_Trip = Comm_array[311] / 10;
		 		 	break ; 
		case 512 : 	I_zero_Lim = Comm_array[312] / 10;
		 		 	break ; 
		case 513 : 	OV_Ltd_Fn = Comm_array[313];
		 		 	break ; 
		case 514 : 	OV_Limit = (float)(Comm_array[314]) / 10.;
		 		 	break ; 
		case 515 : 	OV_Trip = (float)(Comm_array[315]) / 10.;
		 		 	break ; 
		case 518 : 	UV_Trip = (float)(Comm_array[318]) / 10.;
		 		 	break ; 
		case 519 : 	OP_Ph_Trip = Comm_array[319];
		 		 	break ;
		case 538 : 	Out_of_Cnl_t = Comm_array[338];
		 		 	break ; 
		case 1701: 	Min_Speed = (float)(Comm_array[951]);
		 		 	break ; 
		case 1702: 	Max_Speed = (float)(Comm_array[952]) / 10.;
		 		 	break ; 
		case 1703: 	Over_Spd_Lim = Comm_array[953] / 10;
		 		 	break ; 
		case 1711: 	Cur_Ctrl_Kp = Comm_array[961] / 10;
		 		 	break ;  
		case 1712: 	Cur_Ctrl_Ki = Comm_array[962] / 10;
		 		 	break ;
		case 1718: 	Spd_Ctrl_Kp = Comm_array[968] / 10;
		 		 	break ; 
		case 1719: 	Spd_Ctrl_Ki = Comm_array[969] / 10;
		 		 	break ;
		case 1750: 	Spd_Ctrl_B = (float)(Comm_array[1000]);
		 		 	break ;
		case 1751: 	Cur_Ctrl_B = (float)(Comm_array[1001]);
		 		 	break ;
		case 2109: 	Rstator = (float)(Comm_array[1359]) / 100000;
		 		 	break ;
		case 2110: 	Lds = (float)(Comm_array[1360] * 1e-8);
		 		 	break ;
		case 2111: 	Lqs = (float)(Comm_array[1361] * 1e-8);
		 		 	break ;
		case 2112: 	K_back_EMF = (float)(Comm_array[1362])*30./PI*1e-5;
		 		 	break ;
		case 2113: 	Inertia = (float)(Comm_array[1363]);
		 		 	break ;
		case 2114: 	B_fric = (float)(Comm_array[1364]);
		 		 	break ;
		case 2115: 	Base_spd = (float)(Comm_array[1365]);
		 		 	break ;
		case 5000: 	Local_Remote = Comm_array[2100];		//Local Remote
					break ;
		case 5002: 	Speed_ref = (float)(Comm_array[2102]);
					break ;
		case 5003: 	Frequency_ref = (float)(Comm_array[2103]);
					break ;
		case 5004: 	Torque_ref = (float)(Comm_array[2103]);
					break ;
		case 5006:	Run_Stop = Comm_array[2106];
					break ;
*/	}
}

void DSPtoMonitoring()
{
	Comm_array[2110] = (unsigned int)(Wrpm_det);
	Comm_array[2111] = (unsigned int)(Wrm_det);
	Comm_array[2112] = (unsigned int)(Vdc*10.);
	Comm_array[2113] = (unsigned int)(Is_mag/0.1414);
//	Comm_array[2114] = (unsigned int)(Vs_mag/0.1414);
}




