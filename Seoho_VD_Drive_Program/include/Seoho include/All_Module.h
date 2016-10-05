#ifndef		__FUNCTION_MODULE_DEFINITION_
#define		__FUNCTION_MODULE_DEFINITION_

#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"

interrupt void MainPWM(void);					// PWM Interrrupt
interrupt void Driver_Calibration(void); 		

interrupt void Mcbsp_TxINTA_ISR(void);       // chy added 2010.10.25 Mcbsp TX interrupt
interrupt void Mcbsp_RxINTA_ISR(void);          // chy added 2010.10.25 Mcbsp RX interrupt


extern void F28335PowerOnSet();					// DSP 초기 설정 함수
extern void EPWM_Initialization();  			// PWM 초기설정

extern void sci_debug_init(void); // by RYU		// SCI 통신 Realsys monitering 
extern void	scib_init();			// Initialize SCI-B, SCI-C
extern void scic_init();

extern void EQEP_Initialization();  
// #if (!ENCODER_TYPE)                                            
extern void Speed_Calculation(); 
extern void Speed_Calculation_ABS();			

extern void State_Management();					// 인버터의 현재 상태를 관리 
		
extern void VF_Controller();
extern void Flux_Estimation();
extern void Current_Controller();
extern void SVPWM();
extern void Field_Weakening();
extern void Flux_Controller();
extern void Speed_Controller();
extern void Auto_Tuning();

extern void PWM_ON_OFF(int On_Off);				// PWM ON/OFF (Software & Hardware)
extern void DB_Controller();					// DB Chopper Control

extern void DAC_Setup();						
extern void GetSensorValue( );					// ADC Sensing
extern void Variable_Initialization();				// 초기 변수값 설정

extern void Fault_Processing();					// Fault 발생시 상태 관리


extern void get_command(int * command, double * Init_reference );		// 외부입력 관리
extern void Reference_Function();									
extern void Reference_Function_Auto_Tuning();
extern void DO_Processing();
extern void	DI_Processing( int * cmd, double * Init_reference);  
extern void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB);
extern void Keypad_Serial_Communication();
extern void DSPtoArray(unsigned char GROUP,unsigned char INDEX);
extern void DSPtoMonitoring();

// chy

extern void InitXintf();       // chy
extern void mcbsp_xmit(int a, int b);   // chy   transmit data with mcbsp clock 
extern void DecimalToBianary(Uint16 val, Uint16 bits, Uint16 binArr[]); // chy
extern void GrayToBinary(Uint16 N, Uint16 binArr[], Uint16 conG2B[]);  // chy
extern void BinaryToDecimal(Uint16 N, Uint16 conG2B[]); // chy
extern int square(int cnt);


// extern	int motor_name_plate_check();

//TripProc.c
extern void Fault_Recording(int Fault_code,double Fault_data,char * st);




//========================================================
//	start 2010-0830
//========================================================

extern void GetAnaCount( unsigned int * count1, unsigned int * count2);
extern void digital_out_proc();
extern void GetAnalogRef( double * analog_ref);
extern void AnalogInputProcessing(int * cmd, double * ref );
extern double CheckPowerVersion( );
extern int CheckOptionCardID( );

#endif


