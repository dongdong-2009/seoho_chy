//###############################################################################################################
// ���ϸ�   : DSK2833x_GlobalFunc.h
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.10.20
// �ۼ���   : R.N.K
// ����     : TMX(S)320F28335 ������ ŰƮ�� RealSYS DSP28335 DSK �������� �������� ���Ǵ� ��� ���� �� �Լ� ����
// 
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe ����) - 2008.10.20
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30329.exe ����) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe ����) - 2008.10.20
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.20 ��� - 2008.10.20
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

// 16��Ʈ ���� ������ External Bus Zone0�� �Ҵ�� Switch(SW1 ~ SW4�� DIP ����ġ) �Է�
#define EXT_BUS_SW			PWORD(0x004000)

// 16��Ʈ ���� ������ External Bus Zone0�� �Ҵ�� 2 Line Text LCD
#define EXT_BUS_LCD			PWORD(0x004100)

// 16��Ʈ ���� ������ External Bus Zone0�� �Ҵ�� 8���� LED ���
#define EXT_BUS_LED			PWORD(0x004200)

/* micro-seconds delay function */
// TI SDK 1.10�� �ҽ� DSP2833x_usDelay.asm���� �����ϴ� DELAY_US �Լ��� ���
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
// TI SDK 1.10�� �ҽ� DSP2833x_usDelay.asm���� �����ϴ� DELAY_US �Լ��� ���
#define delay_ms(ms)		DELAY_US(ms*1000)

/* ������ ��ȯ ���� �Լ�(LCD �� ��� �������� ó������ ����� */
extern char asc2hex(char asc);  // ASCII �����͸� HEX �����ͷ� ��ȯ(debug_2833x.c ���Ͽ� ����)
extern char hex2asc(char hex);  // HEX �����͸� ASCII �����ͷ� ��ȯ(debug_2833x.c ���Ͽ� ����)
WORD hex2_to_decimal(BYTE b);	// 1BYTE HEX ���� �����ϴ� Decimal ������ ��ȯ(LCD ó�� ��ƾ���� ���)
LONG hex4_to_decimal(WORD w);	// 2BYTE HEX ���� �����ϴ� Decimal ������ ��ȯ(LCD ó�� ��ƾ���� ���)

// DSP28335 DSK���� Digital ����� ��Ʈ �������� ���Ǵ� GPIO ��Ʈ�� ���� �ʱ�ȭ �Լ�
// Digital Input  4�� - GPIO54/55/56/57
// Digital Output 4�� - GPIO58/59/60/61
// CPU Module LED 1�� - GPIO32
void Init_DSK_Gpio(void);

// DSP28335 DSK���� Digital Input(4��)���� ����� GPIO�� ���� �д� �Լ�
// �Է��� High�̸� 1, �Է��� Low�̸� 0���� ��
// 0x0 ~ 0xf ���� ������
BYTE Get_Digital_Input(void);

// DSP28335 DSK���� Digital Output(4��)���� ����� GPIO�� ���� �����ϴ� �Լ�
// d : 0x0 ~ 0xf ���� �μ��� ó��
void Set_Digital_Output(BYTE d);

// DSP28335 DSK���� Digital Output(4��)���� ����� GPIO�� ���� ��Ʈ ������ Set�ϴ� �Լ�
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) ���� �μ��� ó��
void Set_Digital_OutputBit(BYTE bit);

// DSP28335 DSK���� Digital Output(4��)���� ����� GPIO�� ���� ��Ʈ ������ Clear�ϴ� �Լ�
// bit : 0x0 ~ 0x3(BIT0 ~ BIT3) ���� �μ��� ó��
void Clear_Digital_OutputBit(BYTE bit);

// DSP28335 DSK�� CPU Module�� GPIO32�� �Ҵ�� LED ���� ����
#define CPU_MODULE_LED_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1)	
#define CPU_MODULE_LED_OFF		(GpioDataRegs.GPBSET.bit.GPIO32 = 1)
#define CPU_MODULE_LED_TOGGLE	(GpioDataRegs.GPBTOGGLE.bit.GPIO32 = 1)

// DSP28335 DSK���� �ܺ� ���� ���� �� Zone0��  Zone7�� 16��Ʈ ���� �ʱ�ȭ�ϴ� �Լ�
// RealSYS DSP28335 DSK������ SCI-A�� CAN-A�� ����ϹǷ� A17 ~ A19�� External Address ������ ������� ����.
// ���� �ִ� ���� �ִ� ���� ������� 0x100000 -> 0x20000 ��. ���� �����Ͻñ� �ٶ��ϴ�.
void Init_Ext_Bus(void);
 
// External Bus Zone0�� �Ҵ�� SW1 ~ SW4 �Է� D0 ~ D3 ���� �д� �Լ�
// ����ġ�� ���� ��츦 1�� ó����
// 0x0 ~ 0xf ���� ������
BYTE Get_SW_Data(void);

// External Bus Zone0�� �Ҵ�� DIP ����ġ �Է� D4 ~ D7 ���� �д� �Լ�
// DIP ����ġ�� ON ��ġ�϶��� 1�� ó����
// 0x0 ~ 0xf ���� ������
BYTE Get_DIP_Data(void);

// External Bus Zone0�� �Ҵ�� 8���� LED ��¿� ���� �����ϴ� �Լ�
void Set_LED_Data(BYTE d);

// 2 Line Text LCD ���� �Լ�
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
