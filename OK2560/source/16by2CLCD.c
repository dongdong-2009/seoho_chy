/* ========================================================================== */
/*	  Exp17_1.c : ASCII(English)-Only Display on Graphic LCD Module       */
/* ========================================================================== */
/*			  Designed and programmed by Duck-Yong Yoon in 2006.  */

#include "include.h"
#include <avr/pgmspace.h>



void CLCD_command(unsigned char command)		/* write a command(instruction) to text LCD */
{
  PORTH = 0x00;					// E = 0, Rs = 0, R/-W = 0
  PORTJ = command;				// output command
  PORTH = 0x01;					// E = 1
  asm volatile(" PUSH  R0 ");			// delay for about 250 ns
  asm volatile(" POP   R0 ");
  PORTH = 0x00;					// E = 0
  _delay_us(50);
}

void CLCD_data(unsigned char data)		/* display a character on text LCD */
{
  PORTH = 0x02;					// E = 0, Rs = 1, R/-W = 0
  PORTJ = data;					// output data
  PORTH = 0x03;					// E = 1
  asm volatile(" PUSH  R0 ");			// delay for about 250 ns
  asm volatile(" POP   R0 ");
  PORTH = 0x02;					// E = 0
  _delay_us(50);
}

void CLCD_string(unsigned char command, char *string) /* display a string on LCD */
{
  CLCD_command(command);				// start position of string
  while(*string != '\0')			// display string
    { CLCD_data(*string);
       string++;
    }
}

void CLCD_initialize(void)			/* initialize text LCD module */
{
  CLCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
  CLCD_command(0x0C);				// display control(display ON, cursor OFF)
  CLCD_command(0x06);				// entry mode set(increment, not shift)
  CLCD_command(0x01);				// clear display
  _delay_ms(2);
}
