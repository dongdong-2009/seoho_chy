//###############################################################################################################
// 파일명   : DSK2833x_GlobalFunc.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : TMX(S)320F28335 교육용 키트인 RealSYS DSP28335 DSK 예제에서 공통으로 사용되는 함수 구현 코드
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

#include "DSK2833x_Define.h"

/*********** LCD interface (4 bit) **************************************
				   7  6  5  4  3  2  1  0
	Port: Px	=  D7 D6 D5 D4 x  E  x  RS      
	RS = 0(Control), 1(Data)	

Support LCD
// 16 char x 2
// 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F	-- 1st
// 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F	-- 2nd
************************************************************************/

#define LCD_CTRL			0 // LCD Control
#define LCD_DATA			1 // LCD Data

#define LCD_PORT(x)			(EXT_BUS_LCD = x)

WORD hex2_to_decimal(BYTE b){
WORD d100, d10, d1;
WORD dec=0;

	d100 = b/100;
	d10 = (b - d100*100)/10;
	d1 = (b - d100*100 - d10*10); 
	dec = (d100<<8) | (d10<<4) | d1;
	return dec;
}

LONG hex4_to_decimal(WORD w){
LONG d10000, d1000, d100, d10, d1;
LONG dec=0;

	d10000 = w/10000;
	d1000 = (w - d10000 * 10000)/1000;
	d100 = (w - d10000 * 10000 - d1000 * 1000)/100;
	d10 = (w - d10000 * 10000 - d1000 * 1000 - d100 * 100)/10;
	d1 = w - d10000 * 10000 - d1000 * 1000 - d100 * 100 - d10 * 10;
	dec = (d10000<<16) | (d1000<<12) | (d100<<8) | (d10<<4) | d1;
	return dec;
}

// DSP28335 DSK에서 Digital 입출력 포트 목적으로 사용되는 GPIO 포트를 위한 초기화 함수
// Digital Input  4개 - GPIO54/55/56/57
// Digital Output 4개 - GPIO58/59/60/61
// CPU Module LED 1개 - GPIO32
void Init_DSK_Gpio(void){
	EALLOW;
	// Configure GPIO32 as a GPIO output for CPU Module's LED
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;

	// Configure GPIO58 ~ 61 for Digital Output(0 ~ 3)
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
	
	// Configure GPIO54 ~ 57 for Digital Input(0 ~ 3)
	// Set GPIO Mode
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;
	
	// Set Input Mode
	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;
	
	// Set Input Qualification(3 Sample, 255 sampling period)
	// Input Qualification Time : 2 x 2 x 255 x 6.67ns = 6.8 us 
	GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 1;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 1;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 1;
	GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 1;
	
	// Set the sampling period for GPIO52 ~ 57
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0xff; // for GPIO48 ~ 55
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD3 = 0xff; // for GPIO56 ~ 63
	EDIS;
}

// DSP28335 DSK에서 Digital Input(4개)으로 사용한 GPIO의 값을 읽는 함수
// 입력이 High이면 1, 입력이 Low이면 0으로 함
// Digital Input  4개 - GPIO54/55/56/57
// 0x0 ~ 0xf 값을 리턴함
BYTE Get_Digital_Input(void){
LONG tmp;
BYTE d;
	
	tmp = GpioDataRegs.GPBDAT.all;
	d = (BYTE)((tmp >> 22) & 0xf);
	return d;
}

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 설정하는 함수
// Digital Output 4개 - GPIO58/59/60/61
// d : 0x0 ~ 0xf 값을 인수로 처리
void Set_Digital_Output(BYTE d){
LONG tmp;
	
	tmp = GpioDataRegs.GPBDAT.all;
	tmp &= 0xc3ffffff;
	tmp |= ((((LONG)(d & 0xf)) << 26) & 0x3c000000);
	GpioDataRegs.GPBDAT.all = tmp; 
}

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 비트 단위로 Set하는 함수
// Digital Output 4개 - GPIO58/59/60/61
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) 값을 인수로 처리
void Set_Digital_OutputBit(BYTE bit){
LONG tmp;
	
	tmp = GpioDataRegs.GPBDAT.all;
	SetBit(tmp, (LONG)(bit & 0x3));
	GpioDataRegs.GPBDAT.all = tmp;
}

// DSP28335 DSK에서 Digital Output(4개)으로 사용한 GPIO의 값을 비트 단위로 Clear하는 함수
// Digital Output 4개 - GPIO58/59/60/61
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) 값을 인수로 처리
void Clear_Digital_OutputBit(BYTE bit){
LONG tmp;
	
	tmp = GpioDataRegs.GPBDAT.all;
	ClearBit(tmp, (LONG)(bit & 3));
	GpioDataRegs.GPBDAT.all = tmp;
}

// DSP28335 DSK에서 외부 버스 영역 중 Zone0과  Zone7를 16비트 모드로 초기화하는 함수
// RealSYS DSP28335 DSK에서는 SCI-A와 CAN-A를 사용하므로 A17 ~ A19를 External Address 핀으로 사용하지 않음.
// 따라서 최대 쓸수 있는 영역 사이즈는 0x100000 -> 0x20000 임. 이점 유의하시길 바랍니다.
void Init_Ext_Bus(void){
	// Make sure the XINTF clock is enabled
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

	// Configure the GPIO for XINTF with a 16-bit data bus
	// RealSYS DSP28335 DSK에 맞춰 필요한 GPIO만을 할당함
	// RealSYS DSP28335 DSK에서는 SCI-A와 CAN-A를 사용하므로
	// A17 ~ A19를 External Address 핀으로 사용하지 않으며
	// 또한 Ready 신호를 사용하지 않음.
	EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;  // XD15
	GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // XD14
	GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;  // XD13
	GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;  // XD12
	GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;  // XD11
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // XD10
	GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;  // XD19
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // XD8
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;  // XD7
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // XD6
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;  // XD5
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;  // XD4
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;  // XD3
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;  // XD2
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;  // XD1
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;  // XD0

	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3;  // XA0/XWE1n
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3;  // XA1
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;  // XA2
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;  // XA3
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3;  // XA4
	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3;  // XA5
	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3;  // XA6
	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3;  // XA7

	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;  // XA8
	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;  // XA9
	GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;  // XA10
	GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;  // XA11
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;  // XA12
	GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;  // XA13
	GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;  // XA14
	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;  // XA15
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 3;  // XA16
//	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 3;  // XA17
//	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 3;  // XA18
//	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // XA19

//	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 3;  // XREADY
//	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 3;  // XRNW
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 3;  // XWE0

	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 3;  // XZCS0
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3;  // XZCS7
//	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // XZCS6
	
	// All Zones---------------------------------
	// Timing for all zones based on XTIMCLK = SYSCLKOUT
	XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
	// Buffer up to 3 writes
	XintfRegs.XINTCNF2.bit.WRBUFF = 3;
	// XCLKOUT is enabled
	XintfRegs.XINTCNF2.bit.CLKOFF = 0;
	// XCLKOUT = XTIMCLK
	XintfRegs.XINTCNF2.bit.CLKMODE = 0;
	
	// Zone 0(Access Time인 느린 외부 장치를 기준) ----------------
	// When using ready, ACTIVE must be 1 or greater
	// Lead must always be 1 or greater
	// Zone 0 write timing
	XintfRegs.XTIMING0.bit.XWRLEAD = 2;
	XintfRegs.XTIMING0.bit.XWRACTIVE = 3;
	XintfRegs.XTIMING0.bit.XWRTRAIL = 2;
	// Zone 0 read timing
	XintfRegs.XTIMING0.bit.XRDLEAD = 2;
	XintfRegs.XTIMING0.bit.XRDACTIVE = 3;
	XintfRegs.XTIMING0.bit.XRDTRAIL = 1;

	// don't double Zone 0 read/write lead/active/trail timing
	XintfRegs.XTIMING0.bit.X2TIMING = 0;

	// Zone 0 will not sample XREADY signal
	XintfRegs.XTIMING0.bit.USEREADY = 0;
	XintfRegs.XTIMING0.bit.READYMODE = 0;

	// Size must be either:
	// 0,1 = x32 or
	// 1,1 = x16 other values are reserved
	XintfRegs.XTIMING0.bit.XSIZE = 3;

	// Zone 7(Wiznet W3150A+를 기준) ------------------------------
	// 테스트 결과 아래의 Read/Write 타임 보다 작게 설정한 경우에는
	// W3150A+가 동작하지 않읍니다.
	// When using ready, ACTIVE must be 1 or greater
	// Lead must always be 1 or greater
	// Zone 7 write timing
	XintfRegs.XTIMING7.bit.XWRLEAD = 3;
	XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
	XintfRegs.XTIMING7.bit.XWRTRAIL = 2;
	// Zone 7 read timing
	XintfRegs.XTIMING7.bit.XRDLEAD = 3;
	XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
	XintfRegs.XTIMING7.bit.XRDTRAIL = 2;

	// don't double Zone 7 read/write lead/active/trail timing
	XintfRegs.XTIMING7.bit.X2TIMING = 0;

	// Zone 7 will not sample XREADY signal
	XintfRegs.XTIMING7.bit.USEREADY = 0;
	XintfRegs.XTIMING7.bit.READYMODE = 0;

	// 1,1 = x16 data bus
	// 0,1 = x32 data bus
	// other values are reserved
	XintfRegs.XTIMING7.bit.XSIZE = 3;

	// Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 7 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 3;
    XintfRegs.XBANK.bit.BCYC = 3;
	EDIS;

	//Force a pipeline flush to ensure that the write to
	//the last register configured occurs before returning.
	asm(" RPT #7 || NOP");
}

// External Bus Zone0에 할당된 SW1 ~ SW4 입력 D0 ~ D3 값을 읽는 함수
// 스위치를 누른 경우를 1로 처리함
// 0x0 ~ 0xf 값을 리턴함	
BYTE Get_SW_Data(void){
BYTE d;

	d = EXT_BUS_SW;
	return (~d & 0xf);
}

// External Bus Zone0에 할당된 DIP 스위치 입력 D4 ~ D7 값을 읽는 함수
// DIP 스위치를 ON 위치일때를 1로 처리함
// 0x0 ~ 0xf 값을 리턴함
BYTE Get_DIP_Data(void){
BYTE d;

	d = EXT_BUS_SW;
	return ((~d >> 4) & 0xf);
}

// External Bus Zone0에 할당된 8개의 LED 출력에 값을 설정하는 함수
void Set_LED_Data(BYTE d){
	EXT_BUS_LED = d & 0xff;
}

// 2 Line Text LCD 관련 함수
void lcd_out(char data){
char d;

	d = data << 4;
	LCD_PORT(d);		// RS=0, E=0
	LCD_PORT(d | 0x04);	// RS=0, E=1
	delay_us(1);

	LCD_PORT(d);		// RS=0, E=0
	delay_us(1);
}

void lcd_out2(char rs, char data){
char r, d;

	if(rs) r = 0x01;
	else r = 0x00;
	delay_us(1);

	d = (data & 0xf0) | r;	// upper 4 bit
	LCD_PORT(d);		 	// E = 0
	delay_us(1);
	LCD_PORT(d | 0x04);		// E = 1
	delay_us(1);
	LCD_PORT(d);			// E = 0
	delay_us(1);

	d = (data<<4) | r;		// lower 4 bit
	LCD_PORT(d);			// E = 0
	delay_us(1);
	LCD_PORT(d | 0x04);		// E = 1
	delay_us(1);
	LCD_PORT(d);			// E = 0
	delay_us(1);
}

void init_lcd(void){
	LCD_PORT(0);   				/* 0x00 out */
	delay_ms(15);				/* wait 15ms */

	lcd_out(0x03);				/* function set 1 */
	delay_ms(4);
	lcd_out(0x03);				/* function set 2 */
	delay_ms(4);
	lcd_out(0x03);				/* function set 3 */
	delay_ms(4);
	lcd_out(0x02);				/* function set */
	delay_ms(4);

	lcd_out2(LCD_CTRL, 0x28);	/* function set */
	delay_us(40);
	lcd_out2(LCD_CTRL, 0x0c);	/* display on : Disp=ON, Cursor=OFF, Cursor_Blink=OFF */
	delay_us(40);
	lcd_out2(LCD_CTRL, 0x06);	/* entry mode */
	delay_us(40);
	lcd_out2(LCD_CTRL, 0x01);	/* clear display */
	delay_ms(2);	
}

void lcd_gotoxy(int x, int y){
int	pos;

	switch(y){
	case 0:	// 1st line
		pos = 0x80 | (x & 0x7f);
		break;
	case 1:	// 2nd line
		pos = 0x80 | ((0x40 + x) & 0x7f);
		break;
	case 2:	// 3rd line
		pos = 0x80 | ((0x14 + x) & 0x7f);
		break;
	case 3:	// 4th line
		pos = 0x80 | ((0x54 + x) & 0x7f);
		break;
	default: break;
	}

	lcd_out2(LCD_CTRL, pos);
	delay_us(40);
}          

void lcd_clear(void){
	lcd_out2(LCD_CTRL, 0x01);	/* clear display */
	delay_ms(2);
}             

void lcd_clear_line(char line){	/* 0=1st, 1=2nd, 2=3rd, 3=4th line */
int	i, len=16;

	lcd_gotoxy(0,line);
	if(line >= 2) len=20;
	for(i=0; i<len; i++){
		lcd_out2(LCD_DATA, ' ');
		delay_us(40);
	}
}

void lcd_cursor_on(void){                                           
	/*        0 0 0 0 1    D            C             B        */
	/* display on_off : Disp=ON(1), Cursor=OFF(0), Blink=ON(1) */							
	lcd_out2(LCD_CTRL, 0x0d);
	delay_us(40);
}

void lcd_cursor_off(void){
	/*        0 0 0 0 1    D            C             B        */
	/* display on_off : Disp=ON(1), Cursor=OFF(0), Blink=OFF(0) */								
	lcd_out2(LCD_CTRL, 0x0c);
	delay_us(40);
}   

void lcd_putc(char c){
	lcd_out2(LCD_DATA, c);
	delay_us(40);
}

void lcd_puts(char *str){
char c;

	while(1){
		c = *str++;
		if(!c) break;
		lcd_out2(LCD_DATA, c);
		delay_us(40);
	}
} 

void lcd_hex2(BYTE b){
	lcd_putc(hex2asc(b>>4));
	lcd_putc(hex2asc(b));
}	

void lcd_hex4(WORD w){
	lcd_putc(hex2asc(w>>12));
	lcd_putc(hex2asc(w>>8));
	lcd_putc(hex2asc(w>>4));
	lcd_putc(hex2asc(w));
}

void lcd_hex8(LONG l){
	lcd_putc(hex2asc(l>>28));
	lcd_putc(hex2asc(l>>24));
	lcd_putc(hex2asc(l>>20));
	lcd_putc(hex2asc(l>>16));
	lcd_putc(hex2asc(l>>12));
	lcd_putc(hex2asc(l>>8));
	lcd_putc(hex2asc(l>>4));
	lcd_putc(hex2asc(l));
}

void lcd_decimal_byte(BYTE b){
WORD w;
	w = hex2_to_decimal(b);
	lcd_putc(hex2asc(w>>8));
	lcd_putc(hex2asc(w>>4));
	lcd_putc(hex2asc(w));
}

void lcd_decimal_word(WORD w){
LONG l;
	l = hex4_to_decimal(w);
	lcd_putc(hex2asc(l>>16));
	lcd_putc(hex2asc(l>>12));
	lcd_putc(hex2asc(l>>8));
	lcd_putc(hex2asc(l>>4));
	lcd_putc(hex2asc(l));
}
