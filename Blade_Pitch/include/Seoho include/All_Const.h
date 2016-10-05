#ifndef		__COMMON_CONSTANTS_
#define		__COMMON_CONSTANTS_

//------------- Core Define Start -------------//

#define NOP				asm("	NOP")

#define NULL        	(void *)0

#define TRUE			1
#define FALSE			0

#define HIGH			1
#define LOW				0

#define	HI_BYTE(val)	((val >> 8) & 0xff)
#define	LO_BYTE(val)	(val & 0xff) 


// 변경 필요 
#define	T_DEAD				(4.e-6)				// Dead time
#define DEAD_TIME_COUNT		(T_DEAD*1.5e+8) 		// PWM Dead Time Variable
#define IGBT_CURRENT		(50.)				
#define I_DT				(IGBT_CURRENT*0.01) // Dead Time Compensation Current Gain

//-------
/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000) 


#define F_OSC					(30.e+6)		// Osciilator Freq.
#define CPU_CLOCK				(150.e+6)	
#define SWITCH_FREQ				(5000.)			
#define DUAL_PWM_INTERRUPT		(0) 			//	PWM Top & Bottom Interrupt
#define ENCODER_TYPE			(1)				// 0: Incremental 1: Absolute 
#define VECTOR_CONTROL_MODE		(1)

#if (DUAL_PWM_INTERRUPT)
//	#define TSAMP			((1./SWITCH_FREQ)/2.)
//	#define TSAMP_VF		(1./SWITCH_FREQ)
//	#define	TSAMP_CC		((1./SWITCH_FREQ)/2.)
//	#define	TSAMP_SC		((1./SWITCH_FREQ)*2.)
//	#define	TSAMP_SPD		((1./SWITCH_FREQ)*2.)
//	#define TSAMP_FC		((1./SWITCH_FREQ)*2.)
	#define TSAMP			(100.e-6)
	#define TSAMP_VF		(100.e-6)
	#define	TSAMP_CC		(100.e-6)
	#define	TSAMP_SC		(400.e-6)
	#define	TSAMP_SPD		(400.e-6)
	#define TSAMP_FC		(400.e-6)  
#else
	#define TSAMP			(200.e-6)
	#define TSAMP_VF		(200.e-6)
	#define	TSAMP_CC		(200.e-6)
	#define	TSAMP_SC		(800.e-6)
	#define	TSAMP_SPD		(800.e-6)
	#define TSAMP_FC		(800.e-6) 
#endif  

#define	PI					(3.141593)		
#define	SQRT2				(1.414213562)
#define	inv_SQRT2			(0.707106781)
#define	SQRT3				(1.732050808)
#define	inv_SQRT3			(0.577350269) 

#define SPEED_CALC_PRD		(CPU_CLOCK*TSAMP_SPD) 		// EQEP Timer Period
#define SPEED_OB_GAIN		(1000.*(200.e-6/TSAMP_SPD))	// Speed Observer 
#define	RPM2WRM				(2.*PI/60.) 
#define	WRM2RPM				(60./(2.*PI)) 
#define	COUNT_START			((int)(0.02/TSAMP+0.5))		// 초기 ADC 안정화를 기다리는 시간 
#define	COUNT_END			((int)(0.3/TSAMP+0.5))   

#define atan2_IQ(y,x)		(_IQ29toF(_IQ29atan2(_IQ29(y), _IQ29(x))));

#define		u				(0)
#define		v				(1)
#define		w				(2)

/* Bound Macro */
#define	BOUND_PI(x)			((x)+(((x)>PI)?(-2.*PI):((x)<(-PI))?(2.*PI): 0.))
#define	BOUND(x,xmax,xmin)	(((x)>(xmax))?(xmax):((x)<(xmin))?(xmin): (x) )   

#define RESET_DRIVER_SET	(GpioDataRegs.GPASET.bit.GPIO7 		= 1)  
#define RESET_DRIVER_CLEAR	(GpioDataRegs.GPACLEAR.bit.GPIO7	= 1)  
#define DC_CONTACT_ON		(GpioDataRegs.GPACLEAR.bit.GPIO10	= 1)
#define DC_CONTACT_OFF		(GpioDataRegs.GPASET.bit.GPIO10		= 1)
#define PWM_HARDWARE_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO34	= 1)
#define PWM_HARDWARE_OFF	(GpioDataRegs.GPBSET.bit.GPIO34		= 1)
#define BOOT_MODE_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO35	= 1)
#define BOOT_MODE_OFF		(GpioDataRegs.GPBSET.bit.GPIO35		= 1)

//#define nBACKUP_ENABLE_CLEAR (GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1)
//#define nBACKUP_ENABLE_SET	 (GpioDataRegs.GPBSET.bit.GPIO39   = 1)




//------------- Core Define End -------------//



// - Status_Index
#define	STATE_POWER_ON			'Y'
#define	STATE_CALIBRATION		'Z'
#define	STATE_READY				'R'
#define	STATE_STOP				'S'
#define	STATE_ACCELERATING		'A'
#define	STATE_DECELERATING		'D'
#define	STATE_RUNNING			'O'
#define	STATE_TUNING			'T'
#define	STATE_FAULT				'F'


//-- uart 
#define CMD_NULL			  	0
#define CMD_RUN					0x0041	//'A'
#define CMD_STOP				0x0042	//'B'
#define CMD_RESET				0x0043	//'C'


//-- 송수신 스택 길이
#define	RXD_STACK_LENGTH	55		// 수신단 스택의 버퍼 길이
#define	TXD_STACK_LENGTH	20		// 송신단 스택의 버퍼 길이  

#define Read_request_flg	(flag0._bit.bit0)
#define Write_request_flg	(flag0._bit.bit1)
#define Data_rcv_Succ_flg	(flag0._bit.bit2)
#define Data_rcv_fail_flg	(flag0._bit.bit3)
#define Data_receive_flg	(flag0._bit.bit4) 



// chy
#define SIZE 16

#endif

