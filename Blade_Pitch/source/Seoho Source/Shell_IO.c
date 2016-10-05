#include	<All_Header.h>
#include	<All_Extern_Variables.h>

#define	RUN_DIR_MODE	1

//--- debug2010-0831 start
double P1_0		=750.0;		// 정격 출력 
double P1_1		=220.0;		// 정격 전압   
double P1_2		=4.0	;		// 정격전류 
double P1_3		=60.0;		// 정격 주파수 
double P1_4		=4.0	;		// 모터 극수 
double P1_5		=1750.0;		// 정격 회전수  
double P1_6		=0.0	;		// 제어 변수  
double P1_7		=5000;		// 스위칭 주파수   


int P11_0		=9	;	// Analog Output Select
int P11_1		=0	;	// Analog Out Type 
double P11_2	=0.0;		// per unit
double P11_3	=0.0;	// per unit
double P11_4	=0.0;	// 20mA per unit
double P11_5	=100.0;		// % 정격 
int P11_6		=0;		// 반전 

// channel 2의 변수 

#define P11_0A	9		// Analog Output Select
#define P11_1A	0		//  
#define P11_2A	0.0		// 
#define P11_3A	0.4
#define P11_4A	1.0
#define P11_5A	100.0
#define P11_6A	1

#define Te_rat	24.0

#define USE_PER	0
#define USE_Hz	1
#define USE_RPM	2


int P9_16 = USE_RPM;

double P19_3	= 0.0;
double P9_1		= 100.0;
double P9_2		= 200.0;
double P9_3		= 300.0;
double P9_4		= 400.0;
double P9_5		= 500.0;
double P9_6		= 600.0;
double P9_7		= 700.0;
double P9_8		= 800.0;
double P9_9		= 900.0;
double P9_10	= 1000.0;
double P9_11	= 1100.0;
double P9_12	= 1200.0;
double P9_13	= 1300.0;
double P9_14	= 1400.0;
double P9_15	= 1500.0;


double	P9_0 = 0.1;					// JOG_SPEED

#define DIGIT_0		2
#define DIGIT_1		3
#define DIGIT_2		4
#define DIGIT_3		5

#define JOG_SPEED_IN 7
#define EX_FAULT_A	 9
#define EX_FAULT_B	 10


int P8_1 =	DIGIT_0;  //3번째 입력 포트의 기능 설정 
int P8_2 =	DIGIT_1;  //4번째 입력 포트의 기능 설정 
int P8_3 =	DIGIT_2;  //5번째 입력 포트의 기능 설정 
int P8_4 =	DIGIT_3;  //3번째 입력 포트의 기능 설정 
int P8_5 =	JOG_SPEED_IN;  //3번째 입력 포트의 기능 설정 
int P8_6 =	EX_FAULT_A;  //3번째 입력 포트의 기능 설정 
  
int input_state(int port)
{
	unsigned int xbus_in;
	int i,j,k;

	i = 1;

	j = port;

	for(k=0;k<j;k++){	i *= 2; }
	NOP;
	xbus_in = ( ZONE0_BUF[0x0050] & i )>>j;   
	if( xbus_in ) 	return 0;
	else 			return 1;
}

void DI_Function( int input_state, int function_option )
{

	switch( function_option )
	{
		case 0:		break;
		case 1:
			if (input_state)	Flag.DI.bit.DRIVE_ENABLE= 1;
			else				Flag.DI.bit.DRIVE_ENABLE= 0;	
			break;
		case 2:	
			if (input_state)	Flag.DI.bit.MULTI_STEP_BIT0= 1;
			else				Flag.DI.bit.MULTI_STEP_BIT0= 0;	
			break;	
		case 3:	
			if (input_state)	Flag.DI.bit.MULTI_STEP_BIT1= 1;
			else				Flag.DI.bit.MULTI_STEP_BIT1= 0;	
			break; 
		case 4:
			if (input_state)	Flag.DI.bit.MULTI_STEP_BIT2= 1;
			else				Flag.DI.bit.MULTI_STEP_BIT2= 0;	
			break;				
		case 5:
			if (input_state)	Flag.DI.bit.MULTI_STEP_BIT3= 1;
			else				Flag.DI.bit.MULTI_STEP_BIT3= 0;	
			break;		
		case 6:
			if (input_state)	Flag.DI.bit.FAULT_RESET= 1;
			else				Flag.DI.bit.FAULT_RESET= 0;	
			break;		
		case 7:	
			if (input_state)	Flag.DI.bit.JOG= 1;
			else				Flag.DI.bit.JOG= 0;	
			break;	
		case 8:	
			if (input_state)	Flag.DI.bit.AI_REF_ACTIVE= 1;
			else				Flag.DI.bit.AI_REF_ACTIVE= 0;	
			break;	
		case 9:	
			if (input_state)	Flag.DI.bit.AI_LOCAL_REMOTE= 1;
			else				Flag.DI.bit.AI_LOCAL_REMOTE= 0;	
			break;	
		case 10:
			if (input_state)	Flag.DI.bit.EXT_FAULT_A= 1;
			else				Flag.DI.bit.EXT_FAULT_A= 0;	
			break;	
		case 11:
			if (input_state)	Flag.DI.bit.EXT_FAULT_B= 1;
			else				Flag.DI.bit.EXT_FAULT_B= 0;	
			break;	
		case 12:
			if (input_state)	Flag.DI.bit.MOTOR_SELECT= 1;
			else				Flag.DI.bit.MOTOR_SELECT= 0;	
			break;	
		case 13:
			if (input_state)	Flag.DI.bit.MB_BRAKE_STATE= 1;
			else				Flag.DI.bit.MB_BRAKE_STATE= 0;	
			break;	
		case 14:
			if (input_state)	Flag.DI.bit.ACCEL_DECEL_SWITCH= 1;
			else				Flag.DI.bit.ACCEL_DECEL_SWITCH= 0;	
			break;	
		case 15:
			if (input_state)	Flag.DI.bit.REF_TUNING_INC= 1;
			else				Flag.DI.bit.REF_TUNING_INC= 0;	
			break;	
		case 16:
			if (input_state)	Flag.DI.bit.REF_TUNING_DEC= 1;
			else				Flag.DI.bit.REF_TUNING_DEC= 0;	
			break;	
		case 17:
			if (input_state)	Flag.DI.bit.ACC_DEC_BYP= 1;
			else				Flag.DI.bit.ACC_DEC_BYP= 0;	
			break;	
		case 18:
			if (input_state)	Flag.DI.bit.PID_CNTL_ENABLE= 1;
			else				Flag.DI.bit.PID_CNTL_ENABLE= 0;	
			break;	
		case 19:
			if (input_state)	Flag.DI.bit.AUTO_PID_MODE= 1;
			else				Flag.DI.bit.AUTO_PID_MODE= 0;	
			break;	
		case 20:
			if (input_state)	Flag.DI.bit.PID_GAIN_SEL= 1;
			else				Flag.DI.bit.PID_GAIN_SEL= 0;	
			break;	
		case 21:
			if (input_state)	Flag.DI.bit.PID_I_RESET= 1;
			else				Flag.DI.bit.PID_I_RESET= 0;	
			break;	
		case 22:
			if (input_state)	Flag.DI.bit.TRQ_REF_OPT_BYP= 1;
			else				Flag.DI.bit.TRQ_REF_OPT_BYP= 0;	
			break;	
		case 23:
			if (input_state)	Flag.DI.bit.TRQ_SIGN= 1;
			else				Flag.DI.bit.TRQ_SIGN= 0;	
			break;	
		case 24:
			if (input_state)	Flag.DI.bit.TRQ_OUT_ZERO= 1;
			else				Flag.DI.bit.TRQ_OUT_ZERO= 0;	
			break;	
		case 25:
			if (input_state)	Flag.DI.bit.TIMER_RUN_ENABLE= 1;
			else				Flag.DI.bit.TIMER_RUN_ENABLE= 0;	
			break;	
		case 26:
			if (input_state)	Flag.DI.bit.SLAVE_RUN_STATUS= 1;
			else				Flag.DI.bit.SLAVE_RUN_STATUS= 0;	
			break;	
		case 27:
			if (input_state)	Flag.DI.bit.SYNC_CTRL_OPT_BPY= 1;
			else				Flag.DI.bit.SYNC_CTRL_OPT_BPY= 0;	
			break;	
		case 28:
			if (input_state)	Flag.DI.bit.FLYING_START= 1;
			else				Flag.DI.bit.FLYING_START= 0;	
			break;	
		case 29:
			if (input_state)	Flag.DI.bit.DISABLE_PROFIBUS= 1;
			else				Flag.DI.bit.DISABLE_PROFIBUS= 0;	
			break;	
		default:	Flag.DI.all= 0;
	}
}

double Multi_Step_Processing( )
{
	int temp = 0;

	double di_ref;

	if (Flag.DI.bit.MULTI_STEP_BIT0)	temp+= 1;
	if (Flag.DI.bit.MULTI_STEP_BIT1)	temp+= 2;
	if (Flag.DI.bit.MULTI_STEP_BIT2)	temp+= 4;
	if (Flag.DI.bit.MULTI_STEP_BIT3)	temp+= 8;

	switch(temp)
	{
		case 0:  di_ref = P19_3;  break;
		case 1:  di_ref = P9_1;  break;
		case 2:  di_ref = P9_2;  break;
		case 3:  di_ref = P9_3;  break;
		case 4:  di_ref = P9_4;  break;
		case 5:  di_ref = P9_5;  break;
		case 6:  di_ref = P9_6;  break;
		case 7:  di_ref = P9_7;  break;
		case 8:  di_ref = P9_8;  break;
		case 9:  di_ref = P9_9;  break;
		case 10: di_ref = P9_10; break;
		case 11: di_ref = P9_11; break;
		case 12: di_ref = P9_12; break;
		case 13: di_ref = P9_13; break;
		case 14: di_ref = P9_14; break;
		case 15: di_ref = P9_15; break;
		default: di_ref = 0.0;   break;
	}
	
	if(P9_16 == USE_PER ) di_ref /= 100.0;
	if(P9_16 == USE_Hz )  di_ref /= P1_3;
	if(P9_16 == USE_RPM ) di_ref /= Wrpm_rate;
	
	return di_ref; 
}

// option = 0  01--> forward ;  10 --> reverse 
// option = 1  01 --> forward;  11 --> reverse 




void DI_RUN_DIR( )
{
	int bit0_run_dir,bit1_run_dir;

	bit0_run_dir = input_state(0);	// 입력이 되면 1 
	bit1_run_dir = input_state(1);	// digital input state check
 
 // 
	if (!RUN_DIR_MODE)
	{			
		// FWD,REV 모드 
		if ((bit0_run_dir^bit1_run_dir)==1)
		{
			Flag.DI.bit.INVERTER_RUN= 1;
			if (bit1_run_dir)	Flag.DI.bit.INVERTER_DIR= 1;
			else 				Flag.DI.bit.INVERTER_DIR= 0; 
		}
		else	Flag.DI.bit.INVERTER_RUN= 0;
			
	}
 	else
 	{ 
		Flag.DI.bit.INVERTER_RUN = bit0_run_dir;
		Flag.DI.bit.INVERTER_DIR = bit1_run_dir;  // reverse run 
		
	}
}

//int input_function_option[11]={0,0,10,6,2,3,4,0,0,0,0};

void driver_enable_proc()
{
}


void DI_Processing(int * cmd, double * ref )
{
	int temp;

	* cmd = CMD_NULL;

	temp = input_state( 2 ); DI_Function( temp, P8_1);
	temp = input_state( 3 ); DI_Function( temp, P8_2);
	temp = input_state( 4 ); DI_Function( temp, P8_3);
	temp = input_state( 5 ); DI_Function( temp, P8_4);
	temp = input_state( 6 ); DI_Function( temp, P8_5);
	temp = input_state( 7 ); DI_Function( temp, P8_6);

	DI_RUN_DIR();				// run command and direction check	

	* ref = Multi_Step_Processing( );

//	if(( * ref == 0.0 ) &&( Flag.DI.bit.JOG == 1)) 
//		* ref = JOG_SPEED; 

	if (Flag.DI.bit.INVERTER_DIR)	* ref= -(* ref);

	if (Flag.DI.bit.INVERTER_RUN)	* cmd= CMD_RUN;
	else {	* cmd= CMD_STOP;	* ref= 0;	}	

	if( Flag.DI.bit.JOG == 1 ) * ref = P9_0; 

	if( Flag.DI.bit.DRIVE_ENABLE ) driver_enable_proc();

	if( Flag.DI.bit.EXT_FAULT_A ) Flag.DO.bit.FAULT_OUT_A = 1;
	else						  Flag.DO.bit.FAULT_OUT_A = 0;

	if( Flag.DI.bit.EXT_FAULT_B ) Flag.DO.bit.FAULT_OUT_B = 1;
	else						  Flag.DO.bit.FAULT_OUT_B = 0;

}


int digital_port_check( int out_function )
{
	int i;

	i = 0;

	switch (out_function)
	{
		case 1:		i= Flag.DO.bit.DRIVE_READY;		break;
		case 2:		i= Flag.DO.bit.FAULT_OUT_A;		break;
		case 3:		i= Flag.DO.bit.FAULT_OUT_B;		break;
		case 4:		i= Flag.DO.bit.MOTOR_BRAKE;		break;
		default:	i= 0;							break;
	}
	return i;
}  	

#define	RELAY_OFF			0
#define	DRIVER_READY_OUT	1
#define	FAULT_A_OUT			2
#define	FAULT_B_OUT			3
#define	MOTOR_BRAKE_OUT		4

int P12_0 = 0;	// none
int P12_1 = DRIVER_READY_OUT;	// Driver Ready
int P12_2 = FAULT_A_OUT;	// Fault_out A
int P12_3 = FAULT_B_OUT;	// Fault_Out B
int P12_4 = MOTOR_BRAKE_OUT;	// Motor Brake
int P12_5 = 0;	// none
int P12_6 = 0;	// none
int P12_7 = 0;	// none


void digital_out_proc()
{
	int i;

	i = digital_port_check( P12_0 );
	if( i ) digital_out1_on();
	else 	digital_out1_off();

	i = digital_port_check( P12_1 );
	if( i ) digital_out2_on();
	else 	digital_out2_off();
	 	
	i = digital_port_check( P12_2 );
	if( i ) digital_out3_on();
	else 	digital_out3_off();

	i = digital_port_check( P12_3 );
	if( i ) digital_out4_on();
	else 	digital_out4_off();

	i = digital_port_check( P12_4 );
	if( i ) digital_out5_on();
	else 	digital_out5_off();

	i = digital_port_check( P12_5 );
	if( i ) digital_out6_on();
	else 	digital_out6_off();

	i = digital_port_check( P12_6 );
	if( i ) digital_out7_on();
	else 	digital_out7_off();

	i = digital_port_check( P12_7 );
	if( i ) digital_out8_on();
	else 	digital_out8_off();
}


#define IN_10V			0	// 
#define IN_BI_10V		1
#define IN_4TO20MA		2
#define IN_0_TO_20MA	3



double linear_eq(double x1, double x2, double y1, double y2, double x )
{
	double y;

	y = (( y2-y1) / ( x2 - x1 )) * x  + (( y1 * x2 - y2 * x1 )/ (x2- x1));
	return y;
}

#define Count20mA	1580
#define Count4mA	11750
#define Count0mA    14250


unsigned int ADTest;
//========================================================
//	start 2010-0830
//========================================================

void GetAnaCount(Uint16 * count1, Uint16 * count2)
{

	double x,x1,x2,y1,y2;
	double Output,RateValue;
	double temp;
	unsigned int count;

	switch( P11_0 ) //     
	{
	case 0: // frequency
		Output 		= We / 2/ PI; 	// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_3;		// 정격주파수 	
		break;

	case 1:	// motor speed
		Output 		= Wrpm_det; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_5;		
		break;

	case 2: // out_put Current
		Output 		= Is_mag; 			// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_2;			// 정격전류 	
		break;

	case 3: // 
		Output 		= Vdse_ref; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_1;				// 정격전압 	
		break;

	case 4: //  
		Output 		= Te; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= Te_rat;	// 정격전압 	
		break;

	case 5: //  
		Output 		= Vdse_ref * Is_mag ; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_0;			// 정격출력 	
		break;

	case 6: //  
		Output 		= Vdc; 			 
		RateValue 	= Supply_Voltage * SQRT2; 	
		break;

	case 7: //  
		Output 		= 0.0; 	    // free function  		 
		RateValue 	= 100.0;   	
		break;

	default:
		Output 		= 0.0; 			 
		RateValue 	= 100.0; 	
		break;
	}

	if ( P11_0 == 8) 			count = (unsigned int)Count0mA - (unsigned int)( P11_2 * ( Count20mA - Count0mA)) ; // 0mA 조정 출력 
	else if( P11_0 == 9)		count = Count4mA - (unsigned int)( P11_2 * ( Count20mA - Count0mA)) ; 
	else if( P11_0 == 10)		count = (unsigned int)Count20mA - (unsigned int)( P11_2 * ( Count20mA - Count0mA));
	else{

		if( P11_1 == 1 ) // 4~20mA 출력 
		{
			x  = Output;
			x1 = 0.0;
			x2 = RateValue * P11_5;
			y1 = P11_3;						// 4mA offset count
			y2 = P11_4;
			if(P11_6 == 1) // 0일 때 20mA
			{
				temp = linear_eq( x2, x1, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16) temp;
			}	
			else if(P11_6 == 0) // 0~20mA
			{
				temp = linear_eq( x1, x2, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16) temp;
			}
		}
		else // 0~ 20mA 출력
		{
			x  = Output;
			x1 = 0.0;
			x2 = RateValue * P11_5;
			y1 = P11_2;
			y2 = P11_4;
			if(P11_6 == 1) // 0일 때 20mA
			{
				temp = linear_eq( x2, x1, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16)temp;
			}	
			else if(P11_6 == 0) // 0~20mA
			{
				temp = linear_eq( x1, x2, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16)temp;
			}
		}
	}

	* count1 = count;

// channel 2 

	switch( P11_0A ) //      
	{
	case 0: // frequency
		Output 		= We / 2/ PI; 	// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_3;		// 정격주파수 	
		break;

	case 1:	// motor speed
		Output 		= Wrpm_det; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_5;			// 정격주파수 	
		break;

	case 2: // out_put Current
		Output 		= Is_mag; 			// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_2;			// 정격전류 	
		break;

	case 3: // out_put voltage
		Output 		= Vdse_ref; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_1;				// 정격전압 	
		break;

	case 4: // Actual Torque 
		Output 		= Te; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= Te_rat;	// 정격전압 	
		break;

	case 5: // Actual Torque 
//		Output 		= Power; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		Output 		= Vdse_ref * Is_mag ; 		// 연산을 빨리 하기 위한 상수 설정 고려 
		RateValue 	= P1_0;			// 정격출력 	
		break;

	case 6: //  
		Output 		= Vdc; 			 
		RateValue 	= Supply_Voltage * SQRT2; 	
		break;

	case 7: //  
		Output 		= 0.0; 	    // free function  		 
		RateValue 	= 100.0;   	
		break;

	default:
		Output 		= 0.0; 			 
		RateValue 	= 100.0; 	
		break;

	}

	if ( P11_0A == 8) 		count = P11_2A * EPwmPeriodCount; 
	else if( P11_0A == 9)	count = P11_3A * EPwmPeriodCount; 
	else if( P11_0A == 10)	count = P11_4A * EPwmPeriodCount; 
	else{

		if( P11_1A == 1 ) // 4~20mA 출력 
		{
			x  = Output;
			x1 = 0.0;
			x2 = RateValue * P11_5A;
			y1 = P11_3A;						// 4mA offset count
			y2 = P11_4A;
			if(P11_6A == 1) // 0일 때 20mA
			{
				temp = linear_eq( x2, x1, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16) temp;
			}	
			else if(P11_6A == 0) // 0~20mA
			{
				temp = linear_eq( x1, x2, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16) temp;
			}
		}
		else // 0~ 20mA 출력
		{
			x  = Output;
			x1 = 0.0;
			x2 = RateValue * P11_5A;
			y1 = P11_2A;
			y2 = P11_4A;
			if(P11_6A == 1) // 0일 때 20mA
			{
				temp = linear_eq( x2, x1, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16)temp;
			}	
			else if(P11_6A == 0) // 0~20mA
			{
				temp = linear_eq( x1, x2, y1, y2, x );
				temp = (1- temp) * EPwmPeriodCount;
				count = (Uint16)temp;
			}
		}
	}

//	* count1 = ADTest;
	* count2 = count;
}

#define	ANA_INPUT_0_TO_10V		0
#define	ANA_INPUT_10V_TO_10V	1
#define	ANA_INPUT_4MA_TO_20MA	2
#define	ANA_INPUT_0_TO_20MA		3
#define	ANA_INPUT_10V	0
#define	ANA_INPUT_10V	0
#define	ANA_INPUT_10V	0
#define	ANA_INPUT_10V	0
 
//====================================
// 2010-0831 아날로그 입력 지령 처리 
//====================================

int P6_0	= 1;						// selec ana source
int P6_1	= 0;						// Disable
int P6_2	= ANA_INPUT_0_TO_10V;		// type
double P6_3	= 1.0;		// analog input Low Pass Filter Time const
double P6_4	= 0.0;		// offset
double P6_5 = 0.05;		// minimum value
double P6_6 = 10.0;		// maximum value
int P6_7	= 0;		// inversion
int P6_8	= 0;		// discret
int P6_9    = 0;       // 0--> 물리량 1--> 퍼센트 

double CH1_COUNT_AT_0MA	= 500.0;		// default 4mA input AD count
double CH1_COUNT_AT_4MA	= 1000.0;		// default 4mA input AD count
double CH1_COUNT_AT_20MA= 4000.0;		// default 4mA input AD count
double CH1_COUNT_AT_0V	= 2215.0;
double CH1_COUNT_AT_10V	= 4000.0;		//	

double CH2_COUNT_AT_0MA	=500.0;		// default 4mA input AD count
double CH2_COUNT_AT_4MA	=1000.0;	// default 4mA input AD count
double CH2_COUNT_AT_20MA=4000.0;		// default 4mA input AD count
double CH2_COUNT_AT_0V	=2215.0;
double CH2_COUNT_AT_10V	=4000.0;		//	

double CH3_COUNT_AT_0MA	=500.0;		// default 4mA input AD count
double CH3_COUNT_AT_4MA	=1000.0;	// default 4mA input AD count
double CH3_COUNT_AT_20MA=4000.0;	// default 4mA input AD count
double CH3_COUNT_AT_0V	=100.0;
double CH3_COUNT_AT_10V	=4000.0;		//	

double CH4_COUNT_AT_0MA	=500.0;		// default 4mA input AD count
double CH4_COUNT_AT_4MA	=1000.0;	// default 4mA input AD count
double CH4_COUNT_AT_20MA=4000.0;	// default 4mA input AD count
double CH4_COUNT_AT_0V	=100.0;
double CH4_COUNT_AT_10V	=4000.0;		//	

double CH5_COUNT_AT_0MA	=500.0;		// default 4mA input AD count
double CH5_COUNT_AT_4MA	=1000.0;	// default 4mA input AD count
double CH5_COUNT_AT_20MA=4000.0;	// default 4mA input AD count
double CH5_COUNT_AT_0V	=100.0;
double CH5_COUNT_AT_10V	=4000.0;	//	

// ADC의 범위는 -1.0 ~ 1.0 까지임 
	
void GetAnalogRef( double * analog_ref )
{

	double sign,x1,x2,y1,y2,y;
	double min,max;

	switch( P6_0)	// channel select
	{
	case 1:

		switch( P6_2 ) // input type
		{
		case 0:				// 0~10V
	 		x1 = CH1_COUNT_AT_0V; x2 = CH1_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_V1) + P6_4 ;	// 입력 볼트 


			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( y < min ) y = 0.0;
			else if( y > max ) y = max /10.0;
			else				y = y / 10.0;

			break;
		
		case 1 :	// -10 ~ 10V

	 		x1 = CH1_COUNT_AT_0V; x2 = CH1_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_V1) * 2 - 10.0 + P6_4 ;	// 입력 볼트 

			if( y < 0.0 ) sign = -1.0;
			else		  sign =  1.0;

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( (y * sign) < min ) 	 y = 0.0;
			else if( (y * sign) > max ) y = max / 10.0 * sign ;
			else						 y = y / 10.0;
			break;
		
		case 2:		// 4 ~ 20 mA
			 	 						// 0~ 10V
	 		x1 = CH1_COUNT_AT_4MA; x2 = CH1_COUNT_AT_20MA;				
			y1 = 4.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_I1) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 	   y = 0.0;
			else if( y > max ) y = max / 20.0;
			else{			
		 		x1 = 4.0	; x2 = 20.0;				
				y1 = 0.0	; y2 = 1.0;
				y = linear_eq( x1, x2, y1, y2, y ); 
			}
			break;

		case 3: // 0~ 20mA
			 	 						// 0~ 10V
	 		x1 = CH1_COUNT_AT_0MA; x2 = CH1_COUNT_AT_20MA;				
			y1 = 0.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_I1) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0 ;
			else				y = y / 20.0 ; 

			break;
		}

		if( P6_7 == 1) y = 1.0 - y ;		// y = -y
		if (( P6_8 > 7 )&&(P6_8 < 129)) y = (((int)( y * P6_8 )) / P6_8);  

		break;

	// ----------- ch2 select

	case 2:  
		switch( P6_2 ) // input type
		{
		case 0:				// 0~10V
	 		x1 = CH2_COUNT_AT_0V; x2 = CH2_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_V2) + P6_4 ;	// 입력 볼트 


			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( y < min ) y = 0.0;
			else if( y > max ) y = max /10.0;
			else				y = y / 10.0;

			break;
		
		case 1 :	// -10 ~ 10V

	 		x1 = CH2_COUNT_AT_0V; x2 = CH2_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_V2) * 2 - 10.0 + P6_4 ;	// 입력 볼트 

			if( y < 0.0 ) sign = -1.0;
			else		  sign =  1.0;

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( (y * sign) < min ) 	 y = 0.0;
			else if( (y * sign) > max ) y = max / 10.0 * sign ;
			else						 y = y / 10.0;
			break;
		
		case 2:		// 4 ~ 20 mA
			 	 						// 0~ 10V
	 		x1 = CH2_COUNT_AT_4MA; x2 = CH2_COUNT_AT_20MA;				
			y1 = 4.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_I2) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 	   y = 0.0;
			else if( y > max ) y = max / 20.0;
			else{			
		 		x1 = 4.0	; x2 = 20.0;				
				y1 = 0.0	; y2 = 1.0;
				y = linear_eq( x1, x2, y1, y2, y ); 
			}
			break;

		case 3: // 0~ 20mA
			 	 						// 0~ 10V
	 		x1 = CH2_COUNT_AT_0MA; x2 = CH2_COUNT_AT_20MA;				
			y1 = 0.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_I2) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0 ;
			else				y = y / 20.0 ; 

			break;
		}

		if( P6_7 == 1) y = 1.0 - y ;		// y = -y
		if (( P6_8 > 7 )&&(P6_8 < 129)) y = (((int)( y * 128 )) / 128.0);  

		break;

	// CH3 process
	case 3:  

		switch( P6_2 )
		{
		case 0:				// 0~10V
	 		x1 = CH3_COUNT_AT_0V; x2 = CH3_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3 ) + P6_4 ;	// 입력 볼트 


			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 10.0 ;
			else				y = y / 10.0;
			break;
		
		case 1 :	// -10 ~ 10V

	 		x1 = CH3_COUNT_AT_0V; x2 = CH3_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) * 2 - 10.0 + P6_4 ;	// 입력 볼트 

			if( y < 0.0 ) sign = -1.0;
			else		  sign =  1.0;

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( (y * sign) < min ) 	 y = 0.0;
			else if( (y * sign) > max ) y = max / 10.0 * sign ;
			else						 y = y / 10.0;
			break;
		
		case 2:		// 4 ~ 20 mA
			 	 						// 0~ 10V
	 		x1 = CH3_COUNT_AT_4MA; x2 = CH3_COUNT_AT_20MA;				
			y1 = 4.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else{			
		 		x1 = 4.0	; x2 = 20.0;				
				y1 = 0.0	; y2 = 1.0;
				y = linear_eq( x1, x2, y1, y2, y ); 
			}
			break;
		case 3: // 0~ 20mA
			 	 						// 0~ 10V
	 		x1 = CH3_COUNT_AT_0MA; x2 = CH3_COUNT_AT_20MA;				
			y1 = 0.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else				y = y / 20.0 ; 
			break;
		}

		if( P6_7 == 1) y = 1.0 - y ;		// y = -y
		if (( P6_8 > 7 )&&(P6_8 < 129)) y = (((int)( y * 128 )) / 128.0);  

		break;


	// CH4 process
	case 4:  

		switch( P6_2 )
		{
		case 0:				// 0~10V
	 		x1 = CH4_COUNT_AT_0V; x2 = CH4_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_4 ) + P6_4 ;	// 입력 볼트 


			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 10.0 ;
			else				y = y / 10.0;
			break;
		
		case 1 :	// -10 ~ 10V

	 		x1 = CH4_COUNT_AT_0V; x2 = CH4_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_4) * 2 - 10.0 + P6_4 ;	// 입력 볼트 

			if( y < 0.0 ) sign = -1.0;
			else		  sign =  1.0;

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( (y * sign) < min ) 	 y = 0.0;
			else if( (y * sign) > max ) y = max / 10.0 * sign ;
			else						 y = y / 10.0;
			break;
		
		case 2:		// 4 ~ 20 mA
			 	 						// 0~ 10V
	 		x1 = CH4_COUNT_AT_4MA; x2 = CH4_COUNT_AT_20MA;				
			y1 = 4.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else{			
		 		x1 = 4.0	; x2 = 20.0;				
				y1 = 0.0	; y2 = 1.0;
				y = linear_eq( x1, x2, y1, y2, y ); 
			}
			break;
		case 3: // 0~ 20mA
			 	 						// 0~ 10V
	 		x1 = CH4_COUNT_AT_0MA; x2 = CH4_COUNT_AT_20MA;				
			y1 = 0.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_4) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else				y = y / 20.0 ; 
			break;
		}

		if( P6_7 == 1) y = 1.0 - y ;		// y = -y
		if (( P6_8 > 7 )&&(P6_8 < 129)) y = (((int)( y * 128 )) / 128.0);  

		break;


	// CH3 process
	case 5:  

		switch( P6_2 )
		{
		case 0:				// 0~10V
	 		x1 = CH5_COUNT_AT_0V; x2 = CH5_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_5 ) + P6_4 ;	// 입력 볼트 


			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 10.0 ;
			else				y = y / 10.0;
			break;
		
		case 1 :	// -10 ~ 10V

	 		x1 = CH5_COUNT_AT_0V; x2 = CH5_COUNT_AT_10V;				
			y1 = 0	;y2 = 10.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_5) * 2 - 10.0 + P6_4 ;	// 입력 볼트 

			if( y < 0.0 ) sign = -1.0;
			else		  sign =  1.0;

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = P6_5 / 10.0; 	// 10V 일때 100 임 
				max = P6_6 / 10.0;
			}

			if( (y * sign) < min ) 	 y = 0.0;
			else if( (y * sign) > max ) y = max / 10.0 * sign ;
			else						 y = y / 10.0;
			break;
		
		case 2:		// 4 ~ 20 mA
			 	 						// 0~ 10V
	 		x1 = CH5_COUNT_AT_4MA; x2 = CH5_COUNT_AT_20MA;				
			y1 = 4.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else{			
		 		x1 = 4.0	; x2 = 20.0;				
				y1 = 0.0	; y2 = 1.0;
				y = linear_eq( x1, x2, y1, y2, y ); 
			}
			break;
		case 3: // 0~ 20mA
			 	 						// 0~ 10V
	 		x1 = CH5_COUNT_AT_0MA; x2 = CH5_COUNT_AT_20MA;				
			y1 = 0.0	;y2 = 20.0;
			y = linear_eq( x1, x2, y1, y2, Adc_AIN_3) + P6_4 ; 

			if( P6_9 == 0){
				min = P6_5; 	// 10V 일때 100 임 
				max = P6_6;
			}
			else if( P6_9 == 1 ){
				min = 20.0 * P6_5 / 100.0; 	// 10V 일때 100 임 
				max = 20.0 * P6_6 / 100.0;
			}

			if( y < min ) 		y = 0.0;
			else if( y > max ) y = max / 20.0;
			else				y = y / 20.0 ; 
			break;
		}

		if( P6_7 == 1) y = 1.0 - y ;		// y = -y
		if (( P6_8 > 7 )&&(P6_8 < 129)) y = (((int)( y * 128 )) / 128.0);  

		break;

	default:
		y = 0.0;
		break;
	}

	* analog_ref = y;
}

void LPF1(double Ts,double pole,double in,double *out)
{
	*out+=pole*(in-*out)*Ts;
}

void AnalogInputProcessing(int * cmd, double * ref )
{

	double adc_in0, adc_in1;
	 
	if((P6_1 == 1)||(P6_0 == 0)){
		* cmd = CMD_NULL;
		* ref = 0.0;
		return;
	}
	
	GetAnalogRef( & adc_in0);

	LPF1( TSAMP,P6_3, adc_in0, & adc_in1); // debug2010-0901


	* cmd = CMD_RUN;	// debug2010-0831 
	* ref = adc_in1;
//	* ref = adc_in0;

}

double CheckPowerVersion( )
{
	int xbus_in= 0;
	double ReturnVal;
	if (GpioDataRegs.GPCDAT.bit.GPIO64==1)	xbus_in+=1; 
	if (GpioDataRegs.GPCDAT.bit.GPIO65==1)	xbus_in+=2; 
	if (GpioDataRegs.GPCDAT.bit.GPIO66==1)	xbus_in+=4; 
	if (GpioDataRegs.GPCDAT.bit.GPIO67==1)	xbus_in+=8; 
	if (GpioDataRegs.GPCDAT.bit.GPIO68==1)	xbus_in+=16; 
	
//	xbus_in = ZONE0_BUF[0x0040] & 0x0000001f;
	switch( xbus_in )
	{
	case 0:	 	ReturnVal = 5.5e+3;		break;
	case 1:  	ReturnVal = 7.5e+3; 	break;
	case 2: 	ReturnVal = 11e+3; 		break;
	case 3: 	ReturnVal = 15e+3; 		break;
	case 4: 	ReturnVal = 18.5e+3; 	break;
	case 5: 	ReturnVal = 22e+3; 		break;
	case 6: 	ReturnVal = 30e+3; 		break;
	case 7: 	ReturnVal = 37e+3; 		break;
	case 8: 	ReturnVal = 45e+3; 		break;
	case 9: 	ReturnVal = 55e+3; 		break;
	case 10: 	ReturnVal = 75e+3; 		break;
	case 11: 	ReturnVal = 90e+3; 		break;
	case 12: 	ReturnVal = 110e+3; 	break;
	case 13: 	ReturnVal = 132e+3; 	break;
	case 14: 	ReturnVal = 160e+3; 	break;
	case 15: 	ReturnVal = 200e+3; 	break;
	case 16: 	ReturnVal = 250e+3; 	break;
	case 17: 	ReturnVal = 315e+3; 	break;
	case 18: 	ReturnVal = 400e+3; 	break;
	case 19: 	ReturnVal = 500e+3; 	break;
	case 20: 	ReturnVal = 560e+3; 	break;
	case 21: 	ReturnVal = 630e+3; 	break;
	case 22: 	ReturnVal = 715e+3; 	break;
	case 23: 	ReturnVal = 780e+3; 	break;
	case 30: 	ReturnVal = 2.2e+3; 	break;
	case 31: 	ReturnVal = 3.7e+3; 	break;
	}

	return ReturnVal;
		   
}	

int CheckOptionCardID( )
{
	// _XRD low --> _SRRB_RD0
	// OPTION_BOARD_i/o LOW

	int xbus_in;
	
  	GpioDataRegs.GPBCLEAR.bit.GPIO54	= 1;  // option_board_i/o clear
	
	xbus_in = ZONE0_BUF[0x0000] & 0x000000ff;
	
	GpioDataRegs.GPBSET.bit.GPIO54	= 1;  // option_board_i/o clear

	return xbus_in;

}

	
//----------------------------------
// end of file
//----------------------------------
