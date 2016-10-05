//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : util.c
//	description : 유틸함수(delay)
//
//	history : - frist designed at 2008-05-28 4:02오후
//
//====================================================



#include "include.h"
#include "util.h"


void Delay_us(unsigned char time_us)		/* time delay for us */
{ register unsigned char i;

  for(i = 0; i < time_us; i++)			// 4 cycle +
    { asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle +
      asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle +
      asm volatile(" PUSH  R0 ");		// 2 cycle +
      asm volatile(" POP   R0 ");		// 2 cycle = 16 cycle = 1 us for 16MHz
    }
}

void Delay_ms(unsigned int time_ms)		/* time delay for ms */
{ register unsigned int i;

  for(i = 0; i < time_ms; i++)
    { Delay_us(250);
      Delay_us(250);
      Delay_us(250);
      Delay_us(250);
    }
}



/*--- INTERGER TO ASCII CONVERSION ---*/
void inttoa(int val, char *str, int len)
{
	int irem;
	
	switch( len ){
		case 1:	*(str+0) = (char)(val + 48);
			break;
		case 2:	*(str+0) = (char)(val/10 + 48);
			irem = val % 10;
			*(str+1) = (char)(irem + 48);
			break;
		case 3:	*(str+0) = (char)(val/100 + 48);
			irem = val % 100;
			*(str+1) = (char)(irem/10 + 48);
			irem = irem % 10;
			*(str+2) = (char)(irem + 48);
			break;
		case 4:	*(str+0) = (char)(val/1000 + 48);
			irem = val % 1000;
			*(str+1) = (char)(irem/100 + 48);
			irem = irem % 100;
			*(str+2) = (char)(irem/10 + 48);
			irem = irem % 10;
			*(str+3) = (char)(irem + 48);
			break;
		case 5:	*(str+0) = (char)(val/10000 + 48);
			irem = val % 10000;
			*(str+1) = (char)(irem/1000 + 48);
			irem = irem % 1000;
			*(str+2) = (char)(irem/100 + 48);
			irem = irem % 100;
			*(str+3) = (char)(irem/10 + 48);
			irem = irem % 10;
			*(str+4) = (char)(irem + 48);
			break;
		default: break;	
	}
}



