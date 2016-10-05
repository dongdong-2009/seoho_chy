#ifndef _INCLUDE_H_
#define _INCLUDE_H_


#define F_CPU 16000000UL // 사용하는 CPU 클럭을 설정
#define __OPTIMIZE__ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "util.h"
#include "type.h"
#include "128by64GLCD.h"
#include "16by2CLCD.h"
#include "key.h"

#include "fd.h"
#include "tm.h"
#include "sd.h"
#include "abic.h"


//#include "serial.h"
//#include "uart_16c.h"
//#include "hglcd.h"

#endif
