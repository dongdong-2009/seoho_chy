#ifndef _16by2CLCD_H_
#define _16by2CLCD_H_

#include <avr/pgmspace.h>

void CLCD_command(unsigned char command);	
void CLCD_data(unsigned char data);
void CLCD_string(unsigned char command, char *string);
void CLCD_initialize(void);

#endif
