//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : key.c
//	description : Key or Ir or interrupt Input state
//
//	history : - frist designed at 2008-05-28 4:02오후
//
//====================================================



#include "include.h"
#include "key.h"

unsigned char key_signal_A=0xFF;
unsigned char key_signal_B=0xFF;
unsigned char key_data = 0xFF;
unsigned int 	key_press_time=0;

unsigned char LongKeyFlag=0;


void KeyInit(void)
{
	KeyPara.KeyValue = 0xFF;
	KeyPara.LongKeyFlag = 0;
}

void KeyProc(void)
{
	KeyPara.KeyValue = KeyScan();
	KeyPara.LongKeyFlag = LongKeyFlag;
}

unsigned char KeyScan(void)
{
	unsigned char key_output=0xFF;
	unsigned char key = 0xFF;

	key = (PINL) | 0x0F;

	if(key != 0xFF)   	key_signal_A=0x00;
	else key_signal_A=0xFF;
	

	if((!key_signal_A)&&(!key_signal_B))//key pressing
	{
		key_press_time++;
		/*if(20000<key_press_time)//시스템 틱에 연동하여 하는게 졸을것 같기도
		{
			key_press_time=100;
			key_output = key_data;
			LongKeyFlag=1;
			return key_output;
		}*/
	}
	else if((!key_signal_A)&&(key_signal_B))// key down edge
	{
		key_press_time=0;
		LongKeyFlag=0;

		key_data = key;

	}
	else if((key_signal_A)&&(!key_signal_B))// key up edge
	{

		key_output = key_data;
		key_press_time=0;
	}
	else if((key_signal_A)&&(key_signal_B))// key release
	{
	}
	
	key_signal_B = key_signal_A;

	return key_output;
}




