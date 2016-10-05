#ifndef		__STRUCT_VARIABLES_
#define		__STRUCT_VARIABLES_



//--------------------------------------//
struct FAULT_BITS	// bits   description 
{          
   Uint32	OC_A:				1;		// bit .      	 
   Uint32	OC_B:				1;		// bit .
   Uint32	OC_C:				1;		// bit .
   Uint32	OC_MAG:				1;		// bit .
   Uint32	INIT_CHARGE:		1;		// bit .
   Uint32	OV:					1;		// bit .          
   Uint32	UV:					1;		// bit .		
   Uint32	OH:					1;		// bit .      	 
   Uint32	IGBT:				1;		// bit .          
   Uint32	DRIVER:				1;		// bit .
   Uint32	DB:					1;		// bit .      	 
   Uint32	OVER_SPEED:			1;		// bit .          
   Uint32	SPEED_DETECTION:	1;		// bit .
   Uint32	ZC:					1;		// bit .      	 
   Uint32	EXT_FAULT:			1;		// bit .          
   Uint32	rsvd1:				17;		// bit 11~31.        
};

union FAULT_REG 
{
   Uint32					all;
   struct	FAULT_BITS		bit;
}; 

/*
struct STATE_BITS	// bits   description 
{          
   Uint16	Calibration:		1;		// bit 0.      	 
   Uint16	Accelerating:		1;		// bit 1.          
   Uint16	Decelerating:		1;		// bit 2.		
   Uint32	rsvd1:				13;		// bit 3~15.        
};

union STATE_REG 
{
   Uint32					all;
   struct	STATE_BITS		bit;
}; 
*/

//--------------------------------------//
struct DIGITAL_IN_BITS {      
	Uint16 DRIVE_ENABLE:1;      // 
	Uint16 MULTI_STEP_BIT0:1;	// 
	Uint16 MULTI_STEP_BIT1:1;	// 
	Uint16 MULTI_STEP_BIT2:1;	// 
	Uint16 MULTI_STEP_BIT3:1;	// 
	Uint16 FAULT_RESET:1;		// 
	Uint16 JOG:1;				//
	Uint16 AI_REF_ACTIVE:1;		//
	Uint16 AI_LOCAL_REMOTE:1;	//
	Uint16 EXT_FAULT_A:1;		//
	Uint16 EXT_FAULT_B:1;		//
	Uint16 MOTOR_SELECT:1;		//
	Uint16 MB_BRAKE_STATE:1;	//
	Uint16 ACCEL_DECEL_SWITCH:1;	//
	Uint16 REF_TUNING_INC:1;	//
	Uint16 REF_TUNING_DEC:1;	//
	Uint16 ACC_DEC_BYP:1;		//
	Uint16 PID_CNTL_ENABLE:1;	//
	Uint16 AUTO_PID_MODE:1;		//
	Uint16 PID_GAIN_SEL:1;		//
	Uint16 PID_I_RESET:1;		//
	Uint16 TRQ_REF_OPT_BYP:1;	//
	Uint16 TRQ_SIGN:1;			//
	Uint16 TRQ_OUT_ZERO:1;		//
	Uint16 TIMER_RUN_ENABLE:1;	//
	Uint16 SLAVE_RUN_STATUS:1;	//
	Uint16 SYNC_CTRL_OPT_BPY:1;	//
	Uint16 FLYING_START:1;		//
	Uint16 DISABLE_PROFIBUS:1;	//
	Uint16 INVERTER_DIR:1;		//
	Uint16 INVERTER_RUN:1;		//
};

struct DIGITAL_OUT_BITS{
	Uint16 DRIVE_READY:1;	// driver ready
	Uint16 FAULT_OUT_A:1;
	Uint16 FAULT_OUT_B:1;
	Uint16 MOTOR_BRAKE:1;
	Uint16 RUN_STOP:1;
	Uint16 WARNING:1;
	Uint16 DIRECTION:1;
	Uint16 JOG_INPUT:1;
	Uint16 VC_LIMIT:1;
	Uint16 FREE:1;
};

union DIGITAL_OUT_REG{
	Uint16					all;
	struct DIGITAL_OUT_BITS bit;
};

union DIGITAL_IN_REG
{
   Uint32					all;
   struct DIGITAL_IN_BITS	bit;
} ; 

struct ETC_BITS{
	Uint16 DRIVE_CAL:1;
	Uint16 AUTO_TUNING:1;
	Uint16 FAULT_RESET:1;
	Uint16 ACC:1;
	Uint16 DEC:1;
	Uint16 PWM_ON:1;
	Uint16 FW_MODE:2;
};

union ETC_REG{
	Uint16			all;
	struct ETC_BITS	bit;
};

typedef struct FLAG_DEFINE 
{
	union	FAULT_REG				Fault;
	union	DIGITAL_IN_REG			DI;
	union	DIGITAL_OUT_REG			DO;
	union	ETC_REG					ETC;
//	union	STATE_REG	State;
}FLAG;



//--------------------------------------//
typedef struct FAULT_INFO_DEFINE {
	float	CURRENT;			// 5
	float	VDC;				// 6
	float	RPM;				// 7
	float	DATA;				// 4
	char 	TIME[30];			// 3
	char 	MSG[40];			// 2
	int		CODE;				// 1
}FAULT_INFO;


  
 
//--------------------------------------//







#endif

