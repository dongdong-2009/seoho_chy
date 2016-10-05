/*****************************************************************************
*
* File Name: SCI_BC.H
*
* Description: Header file for SCI_B, SCI_C driver
*
* Modules Included:
*
*****************************************************************************/


#ifndef __SCI_BC_H
#define __SCI_BC_H

#include "Core_DSP2833x_Device.h"
#include "Core_DSP2833x_Examples.h" 
#include "Shell_Bit_Define.h"

typedef	struct
{
	unsigned	OP	:8;	//MSB
	unsigned	OBJ	:8;	//LSB
} Packet_Head_flg ;

//-----
typedef	union
{
	unsigned word;
	struct
	{
		unsigned b0	:8;	// CRC16 하위 바이트
		unsigned b1	:8;	// CRC16 상위 바이트
	} byte;
} CRC_flg ;

typedef union {
	unsigned char	tmp_byte ;
	struct {
		unsigned bit0:1;		// LSB
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;		// MSB
	} _bit ;
} Bit_field_ ;

typedef	union {
	unsigned int	w0;
	struct
	{
		unsigned char b0	:8;
		unsigned char b1	:8;
	} b;	
} _par_code ;

extern _par_code _PAR_data ;
extern Bit_field_ flag0 ;   

// SCI-B, SCI-C Interrupt Service Function 선언
#pragma CODE_SECTION(scib_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scib_rx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIB_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-B
#define	SCIB_BAUDRATE	115200L								// SCI-B Baudrate
#define	SCIB_BRR_VAL	(SCIB_LSPCLK/(8*SCIB_BAUDRATE)-1)	// SCI-B BaudRate 설정 Register 값

#define	SCIC_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-C
#define	SCIC_BAUDRATE	115200L								// SCI-C Baudrate
//#define	SCIC_BAUDRATE	9600L								// SCI-C Baudrate
#define	SCIC_BRR_VAL	(SCIC_LSPCLK/(8*SCIC_BAUDRATE)-1)	// SCI-C BaudRate 설정 Register 값

/* BPS 에러율 *****************************************************************
*    BPS	   CPUCLK	  LSPCLK	         BRR_VAL	     BPS	error
*   4800	150000000	37500000	975.5625	976 	4797.851 	-0.045 
*   9600	150000000	37500000	487.28125	487 	9605.533 	0.058 
*  19200	150000000	37500000	243.140625	243 	19211.066 	0.058 
*  38400	150000000	37500000	121.0703125	121 	38422.131 	0.058 
*  57600	150000000	37500000	80.38020833	80 		57870.370 	0.469 
* 115200	150000000	37500000	39.69010417	40 		114329.268 	-0.756 
******************************************************************************/

#define	SCIB_BUF_SIZE	50


#define	SCIB_TX_START	{	if(ScibRegs.SCICTL2.bit.TXRDY){						\
								ScibRegs.SCICTL2.bit.TXINTENA=1;				\
								ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];	\
								if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos=0;	\
							}													\
							else ScibRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIB_TX_STOP	ScibRegs.SCICTL2.bit.TXINTENA=0

#define	SCIC_BUF_SIZE	50

#define	SCIC_TX_START	{	if(ScicRegs.SCICTL2.bit.TXRDY){						\
								ScicRegs.SCICTL2.bit.TXINTENA=1;				\
								ScicRegs.SCITXBUF = scic_tx_buf[scic_tx_pos++];	\
								if(scic_tx_pos >= SCIC_BUF_SIZE) scic_tx_pos=0;	\
							}													\
							else ScicRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIC_TX_iSTART	{	if(ScicRegs.SCICTL2.bit.TXRDY){						                  \
								ScicRegs.SCICTL2.bit.TXINTENA=1;				                  \
	                            ScicRegs.SCITXBUF = TXD_Stack[TXD_StackReadPtr++];                \
	                            if (TXD_StackReadPtr >= TXD_STACK_LENGTH) TXD_StackReadPtr = 0;   \
							}													                  \
							else ScicRegs.SCICTL2.bit.TXINTENA=1;				                  \
						}

#define	SCIC_TX_STOP	ScicRegs.SCICTL2.bit.TXINTENA=0

//---------------------------------------------------------
//-- RS232C 통신 전송제어 문자
#define	SOH		0x01		// 헤딩 개시(Start of Heading)
#define	STX		0x02		// 텍스트 개시 (Start of Text)
#define	ETX		0x03		// 텍스트 종료 (End of Text)
#define	EOT		0x04		// 전송 종료 (End of Transmission)
#define	ENQ		0x05		// 상태간 문의 (Enquiry)
#define	ACK		0x06		// 긍정 응답 (Acknowledge)
#define	DLE		0x10		// 각종 전송제어 (Data Link Escape)
#define	NAK		0x15		// 부정 응답 (Negative Acknowledge)
#define	SYN		0x16		// 동기 유지 (Synchronous Idle)
#define	ETB		0x17		// 블록 종료 (End of Transmission Block)		
#define	CR		0x0D		// Carriage Return	

#define	GEN_POLYNOMAL	0x8821		// CRC 젯수

//-- OP : operator
#define	OP_WRITE	0x40		// 쓰기	
#define	OP_READ		0x80		// 읽기
#define	OP_ANSWER	0xC0		// 응답


//-- <OBJ> : object 
#define	OBJ_PARAMETER_ACCESS		0x24		// 파라미터 읽기/쓰기 (R/W)
#define	OBJ_PARAMETER_BLOCK_ACCESS	0x25		// 파라미터 블록 단위로 읽기/쓰기 (R/W)
#define OBJ_Recv_Status				0xA0


//-- 데이타 타입
#define	DATA_TYPE_BYTE		0x30		// ->	바이트 
#define	DATA_TYPE_WORD		0x31		// ->	2 바이트 정수
#define	DATA_TYPE_DWORD		0x32 		// ->	4 바이트 정수
#define	DATA_TYPE_STRING	0x33		// ->	스트링	

//-- 패널 데이타 그룹 코드
#define	GROUP_CODE_PANEL_DATA	107

//-- 패널 파라미터 코드 : 콘트롤보드 -> 패널
// 0 	~ 	15		->	라인 1 데이타				
// 16 	~	31		->	라인 2 데이타
#define	PANEL_INDEX_LCD_CONTRAST_DUTY	32		// 글자 선명도 조정
#define	PANEL_INDEX_LIGHT_TIME			33		// 백라이드 지속시간	
#define	PANEL_INDEX_KEY_REPEAT_TIME		34		// 키 연속 입력 반응 시간
#define	PANEL_INDEX_LED_DATA			35
#define	PANEL_INDEX_LCD_CURSOR			36
#define	PANEL_INDEX_LCD_CMD				37

//-- 패널 -> 콘트롤보드
#define	PANEL_INDEX_KEY					100
#define	PANEL_INDEX_CONNECTION_REFLESH	101
#define PANEL_INDEX_PARAMETER			102

//-----------------------------------------------------------------------------------------
//-- 로고 출력 시간
#define	LOGO_DISPLAY_TIME		0.5	

//-- 키에서 변경이 없을 경우 0.5초 이후에 다음 키를 읽는다.
#define	NEW_KEY_ENABLE_DELAY_TIME	0.5
	
//-- 키 변화 감지 타이머
#define	KEY_CHANGE_ENABLE_TIME		0.2

//-- 패널 연결 
#define	PANEL_CONNECTION_REFLESH_TIME	0.5

//-- LCD 화면 업데이트
#define	LCD_UPDATE_TIME			0.05	

//-- LCD 파라미터
#define	MAX_CONTRAST_DUTY		2       //50	// 선명도 조정 등급

//-- LCD RAM Address		
#define	ROW1_ADDR			0	// LCD Row1 Base Address : 0 ~ 15
#define	ROW2_ADDR			16	// LCD Row2 Base Address : 16 ~ 31

//-- 백라이트가 켜져 있는 시간
#define	DEFAULT_LIGHT_HOLD_TIME_minute	30	// 30분 후 LCD 자동 꺼짐

//-------------------------------------------------------------------



//-------------------------------------------------------------
//-- CRC-16 테이블 : 256 words
#if __CRC16_LOOK_UP_TABLE_

unsigned CRC16_Table[256] ={0,34849,39011,4162,47335,12486,8324,43173,63983,29134,
			24972,59821,16648,51497,55659,20810,31743,62430,58268,27581,
			49944,19257,23419,54106,33296,2609,6771,37458,15095,45782,
			41620,10933,63486,32735,28573,59324,20249,51000,55162,24411,
			3601,34352,38514,7763,46838,16087,11925,42676,35841,1056,
			5218,40003,13542,48327,44165,9380,30190,64975,60813,26028,
			52489,17704,21866,56651,26589,61436,65470,30623,57146,22299,
			18265,53112,40498,5651,1617,36464,9941,44788,48822,13975,
			7202,37891,33857,3168,42181,11492,15526,46215,58829,28140,
			32174,62863,23850,54539,50505,19816,36899,6146,2112,32865,
			10436,41189,45223,14470,27084,57837,61871,31118,53547,22794,
			18760,49513,60380,25597,29631,64414,21307,56090,52056,17273,
			4659,39442,35408,625,43732,8949,12983,47766,53178,18331,
			22489,57336,30557,65404,61246,26399,13909,48756,44598,9751,
			36530,1683,5841,40688,46149,15460,11302,41991,3234,33923,
			38081,7392,19882,50571,54729,24040,62797,32108,27950,58639,
			14404,45157,40999,10246,32931,2178,6336,37089,49579,18826,
			22984,53737,31052,61805,57647,26894,17339,52122,56280,21497,
			64348,29565,25407,60190,47700,12917,8759,43542,691,35474,
			39632,4849,43111,8262,12292,47141,4224,39073,35043,194,
			20872,55721,51691,16842,59759,24910,28940,63789,54168,23481,
			19451,50138,27519,58206,62236,31549,10871,41558,45588,14901,
			37520,6833,2803,33490,24473,55224,51194,20443,59262,28511,
			32541,63292,42614,11863,15893,46644,7825,38576,34546,3795,
			9318,44103,48133,13348,40065,5280,1250,36035,56713,21928,
			17898,52683,25966,60751,64781,29996};

#endif



//unsigned int Comm_array[3000];

void scib_init(void);
void scic_init(void);
void scib_TxChar(char c);
void scib_TxString(char *p);
void scic_TxChar(char c);
void scic_TxString(char *p);
void scib_tx_start(void);
void scic_tx_start(void);
void scib_putc(char d);
//void scic_putc(char d);
void scib_puts(char *p);
void scic_puts(char *p);
void sci_hex2(BYTE d);
char hex_to_asc(char hex);

interrupt void scib_tx_isr(void);
interrupt void scib_rx_isr(void);
interrupt void scic_tx_isr(void);
interrupt void scic_rx_isr(void);

void scic_test(void);
void Read_Parameter(unsigned char GROUP,unsigned char INDEX);
void Send_Parameter(unsigned char GROUP,unsigned char INDEX,unsigned DATA);
void Read_Answer(unsigned char GROUP,unsigned char INDEX,unsigned DATA);
void Write_Answer(unsigned char GROUP,unsigned char INDEX,unsigned DATA);
#endif



