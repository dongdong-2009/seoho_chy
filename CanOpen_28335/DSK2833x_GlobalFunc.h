//###############################################################################################################
// 파일명   : DSK2833x_GlobalFunc.h
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : TMX(S)320F28335 교육용 키트인 RealSYS DSP28335 DSK 예제에서 공통으로 사용되는 상수 정의 및 함수 선언
// 
// [ 사용한 개발 환경 ]
// 1) TI CCS3.3과 Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe 파일) - 2008.10.20
// 2) CCS3.3 Setup용 Target Driver Package(setupCCSPlatinum_v30329.exe 파일) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe 파일) - 2008.10.20
//
// [ 참조한 Software Developer Kit(SDK) ]
// 1) TI의 SDK인 C/C++ Header Files and Peripheral Examples v1.20 사용 - 2008.10.20
//
// [ Revision History ]
//
//###############################################################################################################

#ifndef DSK2833x_GLOBALFUNC_H
#define DSK2833x_GLOBALFUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#define CS0_START_ADDR		0x004000
#define CS0_END_ADDR		0x004FFF
#define CS0_SIZE			0x001000

#define CS6_START_ADDR		0x100000
#define CS6_END_ADDR		0x1FFFFF
#define CS6_SIZE			0x100000

#define CS7_START_ADDR		0x200000
#define CS7_END_ADDR		0x2FFFFF
#define CS7_SIZE			0x100000

// 16비트 모드로 설정한 External Bus Zone0에 할당된 Switch(SW1 ~ SW4와 DIP 스위치) 입력
#define EXT_BUS_SW			PWORD(0x004000)

// 16비트 모드로 설정한 External Bus Zone0에 할당된 2 Line Text LCD
#define EXT_BUS_LCD			PWORD(0x004100)

// 16비트 모드로 설정한 External Bus Zone0에 할당된 8개의 LED 출력
#define EXT_BUS_LED			PWORD(0x004200)

/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000)

/* 데이터 변환 관련 함수(LCD 및 통신 프로토콜 처리에서 사용함 */
extern char asc2hex(char asc);  // ASCII 데이터를 HEX 데이터로 변환(debug_2833x.c 파일에 구현)
extern char hex2asc(char hex);  // HEX 데이터를 ASCII 데이터로 변환(debug_2833x.c 파일에 구현)
WORD hex2_to_decimal(BYTE b);	// 1BYTE HEX 값에 대응하는 Decimal 값으로 변환(LCD 처리 루틴에서 사용)
LONG hex4_to_decimal(WORD w);	// 2BYTE HEX 값에 대응하는 Decimal 값으로 변환(LCD 처리 루틴에서 사용)

// DSP28335 DSK에서 Digital 입출력 포트 목적으로 사용되는 GPIO 포트를 위한 초기화 함수
// Digital Input  4개 - GPIO54/55/56/57
// Digital Output 4개 - GPIO58/59/60/61
// CPU Module LED 1개 - GPIO32
void Init_DSK_Gpio(void);

// DSP28335 DSK에서 Digital Input(4개)으로 사용한 GPIO의 값을 읽는 함수
// 입력이 High이면 1, 입력이 Low이면 0으로 함
// 0x0 ~ 0xf 값을 리턴함
BYTE Get_Digital_Input(void);

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 설정하는 함수
// d : 0x0 ~ 0xf 값을 인수로 처리
void Set_Digital_Output(BYTE d);

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 비트 단위로 Set하는 함수
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) 값을 인수로 처리
void Set_Digital_OutputBit(BYTE bit);

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 비트 단위로 Clear하는 함수
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) 값을 인수로 처리
void Clear_Digital_OutputBit(BYTE bit);

// DSP28335 DSK의 CPU Module의 GPIO32에 할당된 LED 동작 관련
#define CPU_MODULE_LED_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1)	
#define CPU_MODULE_LED_OFF		(GpioDataRegs.GPBSET.bit.GPIO32 = 1)
#define CPU_MODULE_LED_TOGGLE	(GpioDataRegs.GPBTOGGLE.bit.GPIO32 = 1)

// DSP28335 DSK에서 외부 버스 영역 중 Zone0과  Zone7를 16비트 모드로 초기화하는 함수
// RealSYS DSP28335 DSK에서는 SCI-A와 CAN-A를 사용하므로 A17 ~ A19를 External Address 핀으로 사용하지 않음.
// 따라서 최대 쓸수 있는 영역 사이즈는 0x100000 -> 0x20000 임. 이점 유의하시길 바랍니다.
void Init_Ext_Bus(void);
 
// External Bus Zone0에 할당된 SW1 ~ SW4 입력 D0 ~ D3 값을 읽는 함수
// 스위치를 누른 경우를 1로 처리함
// 0x0 ~ 0xf 값을 리턴함
BYTE Get_SW_Data(void);

// External Bus Zone0에 할당된 DIP 스위치 입력 D4 ~ D7 값을 읽는 함수
// DIP 스위치를 ON 위치일때를 1로 처리함
// 0x0 ~ 0xf 값을 리턴함
BYTE Get_DIP_Data(void);

// External Bus Zone0에 할당된 8개의 LED 출력에 값을 설정하는 함수
void Set_LED_Data(BYTE d);

// 2 Line Text LCD 관련 함수
void init_lcd(void);

void lcd_out(char data);
void lcd_out2(char rs, char data);
void lcd_gotoxy(int x, int y);
void lcd_clear(void);
void lcd_clear_line(char line);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_putc(char c);
void lcd_puts(char *str);

void lcd_hex2(BYTE c);
void lcd_hex4(WORD w);
void lcd_hex8(LONG l);
void lcd_decimal_byte(BYTE b);
void lcd_decimal_word(WORD w);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSK2833x_GLOBALFUNC_H definition
