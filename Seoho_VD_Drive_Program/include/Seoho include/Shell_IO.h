#ifndef		__DIGITAL_INOUT_H_
#define		__DIGITAL_INOUT_H_

#define test_led1_on()		(GpioDataRegs.GPASET.bit.GPIO11 	= 1)	// D01
#define test_led1_off()		(GpioDataRegs.GPACLEAR.bit.GPIO11 	= 1)	// D01
#define test_led1_toggle()	(GpioDataRegs.GPATOGGLE.bit.GPIO11 	= 1)	// D01

#define test_led2_on()		(GpioDataRegs.GPBSET.bit.GPIO37 	= 1)	// D01
#define test_led2_off()		(GpioDataRegs.GPBCLEAR.bit.GPIO37 	= 1)	// D01
#define test_led2_toggle()	(GpioDataRegs.GPBTOGGLE.bit.GPIO37 	= 1)	// D01

#define digital_out1_on()	(GpioDataRegs.GPBSET.bit.GPIO58 = 1)	// D01
#define digital_out1_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1)	// D01

#define digital_out2_on()	(GpioDataRegs.GPBSET.bit.GPIO59 = 1)	// D02
#define digital_out2_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1)	// D02

#define digital_out3_on()	(GpioDataRegs.GPBSET.bit.GPIO60 = 1)	// D03
#define digital_out3_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1)	// D03

#define digital_out4_on()	(GpioDataRegs.GPBSET.bit.GPIO48 = 1)	// GPIO.1
#define digital_out4_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1)	// GPIO.1

#define digital_out5_on()	(GpioDataRegs.GPBSET.bit.GPIO49 = 1)	// GPIO.2
#define digital_out5_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1)	// GPIO.2

#define digital_out6_on()	(GpioDataRegs.GPBSET.bit.GPIO50 = 1)	// GPIO.3
#define digital_out6_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)	// GPIO.3

#define digital_out7_on()	(GpioDataRegs.GPBSET.bit.GPIO51 = 1)	// GPIO.4
#define digital_out7_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)	// GPIO.4

#define digital_out8_on()	(GpioDataRegs.GPBSET.bit.GPIO52 = 1)	// GPIO.5
#define digital_out8_off()	(GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1)	// GPIO.5



#endif
//===================================================================
// No more.
//===================================================================
