#ifndef		__GLOBAL_VARIABLES_
#define		__GLOBAL_VARIABLES_

//------------- Core Variables Start -------------//

//--------Motor Parameters--------//

double Rs=2.0, Rr=0.9, Lsigma=12e-3, Lm=154e-3;
double Jm=0.005, inv_Jm=200, Pole_pair=2., Bm= 0.; 
double LAMdre_rate = 0.777, LAMdre_ref = 0.777;              // 380V 수정 0.45*(380/220)
double Lr, Lls= 0., Llr= 0.;
double Ls, Kt; 
double Is_rate=(3.4*SQRT2), Wrpm_rate=1425., Wrpm_max=12000.;  // 380V시 수정
//int PPR=4096;
int PPR = 8192;
//------Motor Parameters End------//  

double Load_Power= 1.5e+3;
//double Supply_Voltage= 220.;    // 380V시 수정
double Supply_Voltage= 380.;    // 380V시 수정
//--------Motor Parameters--------//
//double Rs=2.2, Rr=2.0, Lsigma=12e-3, Lm=154e-3;
//double Jm=0.005, inv_Jm=200, Pole_pair=2., Bm= 0.; 
//double LAMdre_rate = 0.45, LAMdre_ref = 0.45; 
//double Lr, Lls= 0., Llr= 0.;
//double Ls, Kt; 
//double Is_rate=(21.*SQRT2), Wrpm_rate=1755., Wrpm_max=3500.;   
//int PPR=1024;
//------Motor Parameters End------//

int Flag_AT= 0;  
int abs_position_final= 0;
double Theta_ref= 1.;

double Temp_HHH1= 0., Temp_HHH2= 0., Temp_HHH3= 0.;
double Jm_est= 0.;

double LAMdrs_cm= 0., LAMqrs_cm= 0.;
double LAMdrs_vm= 0., LAMqrs_vm= 0.;
double LAMdrs=0., LAMqrs=0.;

//입력 
int    Command= 0.;
double Init_reference= 0.;
double Init_reference_Auto_tuning= 0.;
double Final_reference= 0.; 
double Wrpm_scale= 0.;
double AnaCmdReference= 0.;
double Accel_time= 0.;	
double Decel_time= 0.;	
double Accel_time_Auto_tuning= 0.;	
double Decel_time_Auto_tuning= 0.;	


//전압  
double Vdc= 0., Vdc_max= 0.;
double Vdss_ref_old= 0., Vqss_ref_old= 0.;
double Vdse_ref_fb= 0., Vqse_ref_fb= 0.; // 관측용 변수 
double Vqse_ref_integ= 0.; 
double Vdss_ref= 0., Vqss_ref= 0., Vdse_ref= 0., Vqse_ref= 0.;


//전류 
double Ias=0., Ibs=0., Ics=0.;
double Ias_offset=0., Ibs_offset=0.;
double Ias_ref=0., Ibs_ref=0., Ics_ref=0.;  
double Is_max=0., Is_mag=0., Is_mag_rms=0.;
double Idss=0., Iqss=0., Idse=0., Iqse=0.; 
double Idss_ref=0., Iqss_ref=0., Idse_ref=0., Iqse_ref=0.;
double Idse_ref_max= 0., Iqse_max_fw=0.;
	

//자속 
double LAMdre=0.;
double LAMdre_flt=0.; //관측용 변수 


//속도 & 주파수 
double freq_vf=0., freq_vf_set=0.;
double We = 0.; 
double Wr=0.;
double Wrpm_ref=0.;
double Wrpm_fw1=0., Wrpm_fw2=0.;
double Wrm_det=0., Wrpm_det=0., Wrm_det_flt=0.; 

//이득
double K_CM1=0., K_CM2=0.;
double K_fw1=0., K_fw2=0.;
double Wc_Fst=0., Kp_Fst=0., Ki_Fst=0.,  Freq_Fst=0.; 
double Wc_fc=0., Kp_fc=0., Ki_fc=0., Ka_fc=0.;
double Wc_sc=0., Kp_sc=0., Ki_sc=0., Ka_sc=0.; 
double Wc_cc=0., Kp_cc=0., Ki_cc=0., Ka_cc=0.;
double L1_sp_est= 0., L2_sp_est= 0., L3_sp_est= 0., L3_sp_tmp= 0.;


//위상 
double Theta=0.; 
double Thetar=0.;

// 토크
double Te=0.;  

//sin&cos
double Cos_Theta_vtg=1., Sin_Theta_vtg=0.;
double Cos_Theta=1., Sin_Theta=0.; 

//switch
int Driver_ON=0.,Flux_build_up=0.;

//PWM Count 
double T_dead_Tuning= 0.;
Uint16 EPwmPeriodCount= 0.;
unsigned int DutyCount[3];


//기타 
long Main_counter= 0.;
int State_Index= 0.;
int Auto_tuning_index= 0.;
double Interrupt_time_max=0.;

#if 0
//변수 통신 관련
Uint16 Group_index= 0;
BYTE Comm_GROUP;
BYTE Comm_INDEX;
Uint16 Comm_array[3000];
Packet_Head_flg Packet_Head ;
CRC_flg	CRC ;
Bit_field_	flag0;


//-- Serial Data Stack  
/* Variables for Serial Communication  */

char scib_tx_buf[SCIB_BUF_SIZE+1];
char scib_tx_pos=0, scib_tx_end=0;
char scib_rx_buf[SCIB_BUF_SIZE+1];
char scib_rx_pos=0, scib_rx_end=0;
char scib_rxd=' ';

char scic_tx_buf[SCIC_BUF_SIZE+1];
char scic_tx_pos=0, scic_tx_end=0;
char scic_rx_buf[SCIC_BUF_SIZE+1];
char scic_rx_pos=0, scic_rx_end=0;
char scic_rxd=' ';

int  scic_tx_ibuf[SCIC_BUF_SIZE+1];
int  scic_rx_ibuf[SCIC_BUF_SIZE+1]; 

#endif //Canopen 통신 sci_BC.c로 옮김

unsigned char RRXD_Stack[RXD_STACK_LENGTH];
unsigned char RXD;							// 수신 데이타
unsigned char RXD_StackWritePtr;			// Serial 데이타의 수신 스택 포인터
unsigned char RXD_StackReadPtr;				// 수신 스택으로 부터 현재 읽혀져야 할 포인터
unsigned char RXD_Stack[RXD_STACK_LENGTH];	// 시리얼 데이타를 저장하는 수신 스택 메모리 공간
	
unsigned char TXD_StackWritePtr;			// Serial 데이타의 송신 스택 포인터
unsigned char TXD_StackReadPtr;				// 송신 스택으로 부터 현재 읽혀져야 할 포인터
unsigned char TXD_Stack[TXD_STACK_LENGTH];	// 시리얼 데이타를 저장하는 송신 스택 메모리 공간
unsigned char TTXD_Stack[20];

unsigned char NewFrame_StackPtr;			// 스택공간상에서 새로 검출된 프레임의 주소
unsigned char Frame_StackPtr;				// 스택공간상에서 전에 검출된 프레임의 주소
unsigned char Packet_StackPtr;				// 스택 공간에서 해당 패킷의 주소
	
unsigned  char NewFrame_Detect;				// 새로운 프레임 검출
	
unsigned  char NewFrame_Packet_State;		// Packet의 수신 단계 
unsigned  char NewFrame_ByteCnt;			// 수신된 바이트 수를 나타내는 인덱스
unsigned  char Frame_ByteCnt;
unsigned  char Packet_ByteCnt; 

// ADC
int	Adc_Uphase;
int	Adc_Vphase;
int	Adc_Wphase;
int	Adc_Vdc	;
int	Adc_AIN_I1; 
int	Adc_AIN_V1;
int	Adc_AIN_I2;
int	Adc_AIN_V2;
int	Adc_AIN_3;
int	Adc_AIN_4;
int	Adc_AIN_5 ;
int	Adc_Temperature;
int	Adc_V_Range; 
 
//====================================
// F28335 
// DAC Parameter
//==================================== 
double	DA1,DA1_rng,DA1_mid;
double	DA2,DA2_rng,DA2_mid;
double	DA3,DA3_rng,DA3_mid;
double	DA4,DA4_rng,DA4_mid;
int	DA1_val,DA2_val,DA3_val,DA4_val; 
	

FAULT_INFO FaultInfo = {0,0,0,0,"2008:07:24  00:01:23"," No Trip Data       ",0};
volatile FLAG Flag;

char MonitorMsg[30]={0};

// chy

Uint16 bin[SIZE];
Uint16 g2b[SIZE];

Uint16 result1;
Uint16 Number_of_turns_buf;
Uint16 Steps_per_turn_buf;
//Uint16 abs_position;
int abs_position=0, pre_abs_position=0;


Uint16 rdata1;
Uint16 rdata2;
Uint16 Number_of_turns;

Uint16 Steps_per_turn;
Uint16 Steps_buf1;
Uint16 Steps_buf2;
Uint16 inv_rdata1;
Uint16 inv_rdata2;

Uint16 gNum;

double Wrn1=0, Wrpm_det1=0;  
int test1=0, test2=0, test3=0, test4 = 0, test5=0; //, test6=0, test7=0, test8=0, test9=0; // chy
Uint16 test6=0, test7=0, test8=0, test9=0, test10=0;
double etest1=0, etest2=0, etest3=0, etest4=0;
Uint16 rx_complete = 0;
Uint16 tx_complete = 0, rx_int = 0;
float sin_theta = 0.0, cos_theta = 0.0;

double Theta_err=0;
 double turn_delta=0;
//------------- Core Variables End -------------//
#endif

