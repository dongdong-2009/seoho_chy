#include	<All_Header.h>
#include	<All_Extern_Variables.h>
//#include 	<Shell_SCI_BC.h> 

//PWM interrupt
interrupt void Driver_Calibration(void)
{
	static long	counter=0;
	static double Ias_sum=0., Ibs_sum=0.;
	static int INIT_CHARGE_Timer= 0.;
	double Temp;
	int j= 0.;

	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	// 5.5kW 7.5kW CT 지연시간 10us
	for (j=0; j<600; j++)
	 NOP; 

	Main_counter++;

	if( Main_counter & 0x1 )			
	{		
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
		#endif
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
		#endif 
	}

//	GetSensorValue();

	if (counter<=COUNT_START)
		counter++;
	else if (counter<=COUNT_END)
	{
		GetSensorValue();
		Temp= Supply_Voltage * SQRT2; // 입력전압 Set 값을 가져와야함 	
		if ((Vdc<(Temp*0.85))||(Vdc>(Temp*1.1)))	INIT_CHARGE_Timer++;
		else if (INIT_CHARGE_Timer>0) INIT_CHARGE_Timer--;
//		if (INIT_CHARGE_Timer>10)	Flag.Fault.bit.INIT_CHARGE= 1;           chy      테스트를 위해서 마스킹 시킨다.
		 
		Ias_sum += Ias;		
		Ibs_sum += Ibs;
		counter++;
	}
	else
	{
		GetSensorValue();
		Ias_offset = Ias_sum/(COUNT_END-COUNT_START);
		Ibs_offset = Ibs_sum/(COUNT_END-COUNT_START);
		Flag.ETC.bit.DRIVE_CAL= 1.;

		EALLOW;
		PieVectTable.EPWM1_INT 	= &MainPWM;
		EDIS;
	}


	EPwm1Regs.ETCLR.bit.INT = 1;	
	// Acknowledge interrupt to recieve more interrupts from PIE group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}



interrupt void MainPWM(void)
{
//	unsigned int ChACount,ChBCount;
	static int FaultDriverCount= 0.; 
//	static int FaultZCCount= 0.;
	double Temp;
	static int ms_cnt= 0;

	int j;

	Uint16 count1,count2;	// debug2010-0830


	Temp= (double)CpuTimer0Regs.TIM.all;
	/*---------------------------------------------**
	**    Seoho VD Drive Program             	   **
	** 		1. Shell Program(Wait ADC S/H)         **
	** 		2. ADC                                 **
	** 		3. Flux Estimation                     **
	** 		4. Current Controller                  **
	** 		5. SVPWM                               **
	** 		6-1. Speed Calculation                 **
	**      6-2. Speed Controller				   **
	**      6-3. Field Weakening				   **
	**      6-4. Flux Controller				   **
	**      7. DB Controller					   **
	**      8. Fault Check						   **
	**      9. State Management					   **
	**---------------------------------------------*/


	Main_counter++;


	ms_cnt++;

	if(ms_cnt >= 65535) ms_cnt = 0 ;

	//=========================================================================

	if(ms_cnt > 3) // 3
	{
		ms_cnt = 0;

		if (TXD_StackReadPtr != TXD_StackWritePtr)
		{
			if(ScicRegs.SCICTL2.bit.TXRDY) 				// 송신버퍼가 비어 있음
			{
				ScicRegs.SCITXBUF = TXD_Stack[TXD_StackReadPtr++] ;
				if (TXD_StackReadPtr >= TXD_STACK_LENGTH)
				{
					TXD_StackReadPtr = 0 ;
				}
			}
		}
	}
	//=========================================================================

	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	// 5.5kW 7.5kW CT 지연시간 10us
	for (j=0; j<300; j++)
	 NOP;

	GetSensorValue();			


//	Flux_Estimation();			// 3465	->	23.1us
	VF_Controller(); 
//	Current_Controller();		// 679	->	4.527us
	SVPWM();					// 1090	->	7.267us

	// Call Scheduled Routines 
	if( Main_counter & 0x1 )			// 2*CC Odd Routines : 1 
	{	
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		#endif

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		
//			x4_routine();
		    if (rx_complete != 1)  
		    {		    
		    		     mcbsp_xmit(0xFF,0xFF);
			}
		}
		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
		{	
//			x8_routine();
		}
		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
		{	
//			x16_routine();
		}	
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		#endif

//		x2_routine();				// 2*CC Even Routines : 0 						

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		
			#if (!ENCODER_TYPE)	
				Speed_Calculation();
			#else
				Speed_Calculation_ABS();
				rx_complete = 0;
//				dev_BackgroundDAC ();
			#endif
//			Wrpm_ref = Final_reference*Wrpm_scale;
//			Speed_Controller();
//			x4_routine();
		}
		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
		{	
//			Field_Weakening();
//			Flux_Controller();
//			x8_routine();
		}
		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
		{	
//			Field_Weakening();
//			Flux_Controller();
//			x16_routine();
		}		
	}
	
	DB_Controller();
	
	if (GpioDataRegs.GPADAT.bit.GPIO12==0) 	FaultDriverCount++;
	else if( FaultDriverCount > 0)			FaultDriverCount--;
	if (FaultDriverCount > 10 )	Flag.Fault.bit.DRIVER= 1;

/*
	if (GpioDataRegs.GPADAT.bit.GPIO16==0) 	FaultZCCount++;
	else if( FaultZCCount > 0)			FaultZCCount--;
	if (FaultZCCount > 10 )	Flag.Fault.bit.ZC= 1; 
*/
	
	if (Flag.Fault.all != 0)	State_Index= STATE_FAULT;

	switch(State_Index)
	{
		case STATE_STOP:
			
			if (Command==CMD_RUN)
			{	PWM_ON_OFF(1);	Driver_ON=1;	}
			else	
			{	PWM_ON_OFF(0);	Driver_ON=0;	}

			break;

		case STATE_ACCELERATING:	Reference_Function();			break;		
		case STATE_DECELERATING:	Reference_Function();			break;
		case STATE_RUNNING:			Reference_Function();			break;
		case STATE_TUNING:			Auto_Tuning();					break;			 
		case STATE_FAULT:			Fault_Processing();				break; 			 
		default: 					PWM_ON_OFF(0);	Driver_ON=0;	break;
	}


//========================================================
//	start 2010-0830
//========================================================

	GetAnaCount( &count1, &count2);

	EPwm5Regs.CMPA.half.CMPA 	= count1;
	EPwm5Regs.CMPB  			= count2;
	
//========================================================
//	end of 2010-0830 
//========================================================


//	DA1=Iqse_ref;	DA1_mid=0;	DA1_rng=40;  
//	DA2=Iqse;	DA2_mid=0;	DA2_rng=40;
//	DA3=Temp_HHH1;		DA3_mid=0;	DA3_rng=20;
//	DA4=Iqse;		DA4_mid=0;	DA4_rng=40;
//	DAC_Setup();  




//--- Ana Monitor 1,2 
//	GetAnaMonitCount(&ChACount,&ChBCount);
//	EPwm5Regs.CMPA.half.CMPA 	= ChACount;
//	EPwm5Regs.CMPB  			= ChBCount;
	


	Temp= (Temp-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max<Temp)	Interrupt_time_max= Temp; 


	EPwm1Regs.ETCLR.bit.INT = 1;	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	


	 
}

void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB)
{
	double fTemp;
	static double Theta_a=0;

	fTemp = (double)(EPwmPeriodCount >> 1); 


	Theta_a += 2.* PI * 20 * TSAMP;

	if (Theta_a>PI)			Theta_a-=2.*PI;
	else if (Theta_a<-PI)	Theta_a+=2.*PI;		

	* piChA = (unsigned int) ( fTemp *  ( sin(Theta_a) + 1 ));
	* piChB = (unsigned int) ( fTemp *  ( cos(Theta_a) + 1 ));
}

	

void PWM_ON_OFF(int On_Off)
{
	if (On_Off)
	{	
		EPwm1Regs.AQCSFRC.bit.CSFA= 0;
		EPwm2Regs.AQCSFRC.bit.CSFA= 0;
		EPwm3Regs.AQCSFRC.bit.CSFA= 0;
		PWM_HARDWARE_ON;
		BOOT_MODE_ON;
		Flag.ETC.bit.PWM_ON= 1.;
	}
	else
	{
		PWM_HARDWARE_OFF;
		BOOT_MODE_OFF;		
		EPwm1Regs.CMPA.half.CMPA= EPwmPeriodCount;
		EPwm2Regs.CMPA.half.CMPA= EPwmPeriodCount;
		EPwm3Regs.CMPA.half.CMPA= EPwmPeriodCount;
		EPwm1Regs.AQCSFRC.bit.CSFA= 2; //Forced High-Value
		EPwm2Regs.AQCSFRC.bit.CSFA= 2; //Forced High-Value
		EPwm3Regs.AQCSFRC.bit.CSFA= 2; //Forced High-Value
		Flag.ETC.bit.PWM_ON= 0.;
	}
}

void DB_Controller()
{
	static int FaultDBCount= 0.;

	if (GpioDataRegs.GPADAT.bit.GPIO13==0) 	FaultDBCount++;
	else if( FaultDBCount > 0)			FaultDBCount--;
	if (FaultDBCount > 10 )	Flag.Fault.bit.DB= 1; 
	
	if (Flag.Fault.bit.DB == 0)
	{
		if ( Vdc >  370.0)	EPwm4Regs.AQCSFRC.bit.CSFA= 1;
		else if ( Vdc <  350.0)	EPwm4Regs.AQCSFRC.bit.CSFA= 2;
	}
}


// chy added 2010.10.25

interrupt void Mcbsp_TxINTA_ISR(void)
{
//    McbspaRegs.DXR1.all= sdata;
//   sdata = (sdata+1)& 0x00FF ;
    // To receive more interrupts from this PIE group, acknowledge this interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

interrupt void Mcbsp_RxINTA_ISR(void)
{
//    rdata=McbspaRegs.DRR1.all;
//    if (rdata != ( (rdata_point) & 0x00FF) ) error();
//    rdata_point = (rdata_point+1) & 0x00FF;
    // To receive more interrupts from this PIE group, acknowledge this interrupt
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

//   if( McbspaRegs.SPCR1.bit.RRDY == 1 )
//   {
		rdata2 = McbspaRegs.DRR2.all;		                             // Read DRR2 first.
    	rdata1 = McbspaRegs.DRR1.all; 
	
	rx_complete = 1;
//	}
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}

//===========================================================================
// No more.
//===========================================================================







