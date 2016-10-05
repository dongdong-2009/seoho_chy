#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // Examples Include File

void InitGpio(void)
{ 

     EALLOW;
 
   // Enable PWM1-3 on GPIO0-GPIO5
   GpioCtrlRegs.GPAPUD.bit.GPIO0 	= 0;   // Disable pullup on GPIO0
   GpioCtrlRegs.GPAPUD.bit.GPIO1 	= 0;   // Disble pullup on GPIO1
   GpioCtrlRegs.GPAPUD.bit.GPIO2 	= 0;   // Disble pullup on GPIO2
   GpioCtrlRegs.GPAPUD.bit.GPIO3 	= 0;   // Disble pullup on GPIO3
   GpioCtrlRegs.GPAPUD.bit.GPIO4 	= 0;   // Disble pullup on GPIO4
   GpioCtrlRegs.GPAPUD.bit.GPIO5 	= 0;   // Disble pullup on GPIO5

   GpioCtrlRegs.GPADIR.bit.GPIO0 	= 1;  // GPIO0 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO1 	= 1;  // GPIO1 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO2 	= 1;  // GPIO2 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO3 	= 1;  // GPIO3 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO4 	= 1;  // GPIO4 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO5 	= 1;  // GPIO5 = Output

   GpioCtrlRegs.GPAMUX1.bit.GPIO0 	= 1;  // GPIO0 = PWM1A
   GpioCtrlRegs.GPAMUX1.bit.GPIO1 	= 1;  // GPIO1 = PWM1B
   GpioCtrlRegs.GPAMUX1.bit.GPIO2 	= 1;  // GPIO2 = PWM2A
   GpioCtrlRegs.GPAMUX1.bit.GPIO3 	= 1;  // GPIO3 = PWM2B
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 	= 1;  // GPIO4 = PWM3A
   GpioCtrlRegs.GPAMUX1.bit.GPIO5 	= 1;  // GPIO5 = PWM3B

   // GPIO6 DynamicBrakePWMOut
   GpioCtrlRegs.GPAPUD.bit.GPIO6 	= 1;  // Enable pullup on GPIO6
   GpioDataRegs.GPACLEAR.bit.GPIO6 	= 1;  // Load output latch
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 	= 1;  // GPIO6 = PWM4A
   GpioCtrlRegs.GPADIR.bit.GPIO6 	= 1;  // GPIO6 = Output

   // GPIO7 /RST_DRIVER
   GpioCtrlRegs.GPAPUD.bit.GPIO7 	= 1;  // Enable pullup on GPIO7
   GpioDataRegs.GPASET.bit.GPIO7 	= 1;  // Set Output
   GpioCtrlRegs.GPAMUX1.bit.GPIO7 	= 0;  // GPIO7 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO7 	= 1;  // GPIO7 = Output

   // GPIO8 PWM5A AnalogMonitorPWM1
   GpioCtrlRegs.GPADIR.bit.GPIO8 	= 1;  // GPIO5 = Output
   GpioCtrlRegs.GPAPUD.bit.GPIO8 	= 0;  // Disable pullup on GPIO8
   GpioCtrlRegs.GPAMUX1.bit.GPIO8 	= 1;  // GPIO8 = GPIO
   
   // GPIO9 PWM5B AnalogMonitorPWM2
   GpioCtrlRegs.GPADIR.bit.GPIO9 	= 1;  // GPIO5 = Output
   GpioCtrlRegs.GPAPUD.bit.GPIO9 	= 0;  // Disable pullup on GPIO9
   GpioCtrlRegs.GPAMUX1.bit.GPIO9 	= 1;  // GPIO9 = GPIO

   // GPIO10 OUT / DC_CONTACT Out
   GpioCtrlRegs.GPAPUD.bit.GPIO10 	= 1;  // Enable pullup on GPIO10
   GpioDataRegs.GPASET.bit.GPIO10 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO10 	= 0;  // GPIO10 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO10 	= 1;  // GPIO10 = Output

   // GPIO11 OUT TEST_LED1
   GpioCtrlRegs.GPAPUD.bit.GPIO11 	= 1;  // Enable pullup on GPIO11
   GpioDataRegs.GPASET.bit.GPIO11 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 	= 0;  // GPIO11 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO11 	= 1;  // GPIO11 = Output


   // GPIO12 /TZ1 : /FAULT_DRIVER INPUT 
   GpioCtrlRegs.GPAPUD.bit.GPIO12 	= 1;  // Pullup on GPIO12
   GpioDataRegs.GPASET.bit.GPIO12 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 	= 1;  // GPIO12 = /TZ1
   GpioCtrlRegs.GPADIR.bit.GPIO12 	= 0;  // GPIO12 = INPUT

   // GPIO13 /TZ2 INPUT : /FAULT_DB
   GpioCtrlRegs.GPAPUD.bit.GPIO13 	= 1;  // Disable pullup on GPIO13
   GpioDataRegs.GPASET.bit.GPIO13 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 	= 1;  // GPIO13 = /TZ2 
   GpioCtrlRegs.GPADIR.bit.GPIO13 	= 0;  // GPIO13 = Input

   // GPIO14 /TZ3 INPUT : /FAULT_OC
   GpioCtrlRegs.GPAPUD.bit.GPIO14 	= 1;  // Disable pullup on GPIO14
   GpioDataRegs.GPASET.bit.GPIO14 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 	= 1;  // GPIO14 = /TZ3 
   GpioCtrlRegs.GPADIR.bit.GPIO14 	= 0;  // GPIO14 = Input

   // GPIO15 /TZ4 INPUT : /FAULT_OV
   GpioCtrlRegs.GPAPUD.bit.GPIO15 	= 1;  // Disable pullup on GPIO15
   GpioDataRegs.GPASET.bit.GPIO15 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 	= 1;  // GPIO15 = /TZ4 
   GpioCtrlRegs.GPADIR.bit.GPIO15 	= 0;  // GPIO15 = Input

   // GPIO16 /TZ5 INPUT : FAULT_ZC == 오류가 있어서 해결해야됨 
   GpioCtrlRegs.GPAPUD.bit.GPIO16 	= 1;  // Disable pullup on GPIO16
   GpioDataRegs.GPASET.bit.GPIO16 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO16 	= 3;  // GPIO16 = /TZ5  
   GpioCtrlRegs.GPADIR.bit.GPIO16 	= 0;  // GPIO16 = Input

  // GPIO17 Detect.Option
   GpioCtrlRegs.GPAPUD.bit.GPIO17 	= 0;  // Disable pullup on GPIO17
   GpioDataRegs.GPASET.bit.GPIO17 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO17 	= 1;  // GPIO17 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO17 	= 0;  // GPIO17 = input

   // GPIO18 TXD
//   GpioCtrlRegs.GPAPUD.bit.GPIO18 	= 0;  // Enable pullup on GPIO18     
//   GpioDataRegs.GPACLEAR.bit.GPIO18	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO18 	= 2;  // GPIO18 = SCITXDB
   GpioCtrlRegs.GPADIR.bit.GPIO18 	= 1;  // 

   // GPIO19 RXD
//   GpioCtrlRegs.GPAPUD.bit.GPIO19 	= 1;  // Enable pullup on GPIO18     
//   GpioDataRegs.GPASET.bit.GPIO19 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO19 	= 2;  // GPIO19 = SCIRXDB
   GpioCtrlRegs.GPADIR.bit.GPIO19 	= 1;  // 

  

  //GPIO20 GPIO.4 MDXA 
   GpioCtrlRegs.GPAPUD.bit.GPIO20 	= 0;  // Enable pullup on GPIO20     
//   GpioDataRegs.GPASET.bit.GPIO20 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO20 	= 2;  // GPIO20 = MDXA
//   GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 3;   // Asynch input GPIO20 (MDXA)
//   GpioCtrlRegs.GPADIR.bit.GPIO20 	= 1;  // GPIO20 = Output

   //GPIO21 GPIO.5 MDRA
   GpioCtrlRegs.GPAPUD.bit.GPIO21 	= 0;  // Enable pullup on GPIO20     
//   GpioDataRegs.GPASET.bit.GPIO21 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO21 	= 2;  // GPIO21 = MDRA
   GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;   // Asynch input GPIO21 (MDRA)
//   GpioCtrlRegs.GPADIR.bit.GPIO21 	= 1;  // GPIO21 = Output

   // GPIO22 GPIO.2 MCLKXA
   GpioCtrlRegs.GPAPUD.bit.GPIO22 	= 0;  // enable pullup on GPIO21               
//   GpioDataRegs.GPASET.bit.GPIO22 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO22 	= 2;  // GPIO22 = MCLKXA
   GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;   // Asynch input GPIO22 (MCLKXA)
//   GpioCtrlRegs.GPADIR.bit.GPIO22 	= 1;  // GPIO22 = OUTPUT

   // GPIO23 GPIO.3 MFSXA
   GpioCtrlRegs.GPAPUD.bit.GPIO23 	= 0;  // Enable Pullup on GPIO20
//  GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO23 	= 2;  // GPIO23 = MFSXA
   GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;   // Asynch input GPIO23 (MFSXA)
//   GpioCtrlRegs.GPADIR.bit.GPIO23 	= 1;  // GPIO23 = Output

   //GPIO24 QEP A2 MDXB
   GpioCtrlRegs.GPAPUD.bit.GPIO24 	= 0;
   GpioCtrlRegs.GPAMUX2.bit.GPIO24 	= 3;  // GPIO24 = MDXB
   GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3; 
//   GpioCtrlRegs.GPADIR.bit.GPIO24 	= 0;  // GPIO24 = Input

   //GPIO25 QEP B2 MDRB
   GpioCtrlRegs.GPAPUD.bit.GPIO25 	= 0;
   GpioCtrlRegs.GPAMUX2.bit.GPIO25 	= 3;  // GPIO25 = MDXB
   GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;   // Asynch input GPIO25 (MDRB) (Comment as needed)
//   GpioCtrlRegs.GPADIR.bit.GPIO25 	= 0;  // GPIO25 = Input

   // GPIO26 QEP I2 MCLKXB
   GpioCtrlRegs.GPAPUD.bit.GPIO26 	= 0;  // Enable pullup on GPIO24   
//   GpioDataRegs.GPASET.bit.GPIO26 	= 1;  // SET Output High            
   GpioCtrlRegs.GPAMUX2.bit.GPIO26 	= 3;  // GPIO26 = MCLKXB
   GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;   // Asynch input GPIO26(MCLKXB) (Comment as needed)
//   GpioCtrlRegs.GPADIR.bit.GPIO26 	= 1;  // GPIO26 = Output

   // GPIO27 QEP S2 MFSXB
   GpioCtrlRegs.GPAPUD.bit.GPIO27 	= 0;  // Enable pullup on GPIO27
//   GpioDataRegs.GPASET.bit.GPIO27 	= 1;  // SET Output High            
   GpioCtrlRegs.GPAMUX2.bit.GPIO27 	= 3;  // GPIO27 = eQEP2 strobe 
   GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;   // Asynch input GPIO27 (MFSXB) (Comment as needed)
//   GpioCtrlRegs.GPADIR.bit.GPIO27 	= 1;  // GPIO27 = Output

   // GPIO28 RS232_SCIRXDA
//   GpioCtrlRegs.GPAPUD.bit.GPIO28 	= 0;  // Enable pullup on GPIO28      
//   GpioCtrlRegs.GPAMUX2.bit.GPIO28 	= 1;  // GPIO28 = SCIRXDA
//   GpioCtrlRegs.GPADIR.bit.GPIO28 	= 0;  // GPIO28 = Input

   // GPIO29 RS232_SCITXDA
//   GpioCtrlRegs.GPAPUD.bit.GPIO29	= 0;  // Enable pullup on GPIO29         
//   GpioCtrlRegs.GPAMUX2.bit.GPIO29 	= 1;  // GPIO29 = SCITXDA
//   GpioCtrlRegs.GPADIR.bit.GPIO28 	= 1;  // GPIO29 = Output

 
   
   // GPIO30 RXD_CAN
   GpioCtrlRegs.GPAMUX2.bit.GPIO30 	= 1;  // GPIO30 = RXD_CAN
   GpioCtrlRegs.GPADIR.bit.GPIO30 	= 0;  // GPIO30 = INPUT

   // GPIO31 TXD_CAN
   GpioCtrlRegs.GPAMUX2.bit.GPIO31 	= 1;  // GPIO31 = TXD_CAN
   GpioCtrlRegs.GPADIR.bit.GPIO31 	= 0;  // GPIO31 = OUTPUT

   // GPIO32 I2C_SDAA
   GpioCtrlRegs.GPBPUD.bit.GPIO32 	= 0;  // Enable pullup on GPIO32         
   GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input                     
   GpioCtrlRegs.GPBMUX1.bit.GPIO32 	= 1;  // GPIO32 = SDAA
   // GPIO33 I2C_SCLA
   GpioCtrlRegs.GPBPUD.bit.GPIO33 	= 0;  // Enable pullup on GPIO33
   GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input               
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 	= 1;  // GPIO33 = SCLA

   // GPIO34 PWN_EN
   GpioCtrlRegs.GPBPUD.bit.GPIO34 	= 1;  // Enable pullup on GPIO34   
   GpioDataRegs.GPBSET.bit.GPIO34	= 1;  // Set High              
   GpioCtrlRegs.GPBMUX1.bit.GPIO34 	= 0;  // GPIO34 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO34 	= 1;  // GPIO34 = Output


//=============================================================

  // GPIO35 BOOT_MODE
 
   GpioCtrlRegs.GPBPUD.bit.GPIO35 	= 1;  // Enable pullup on GPIO35   
   GpioDataRegs.GPBSET.bit.GPIO35	= 1;  // Set High              
   GpioCtrlRegs.GPBMUX1.bit.GPIO35 	= 0;  // GPIO35 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO35 	= 1;  // GPIO35 = Output

   // GPIO36 RXD  -> /XZCS0 by RYU
   GpioCtrlRegs.GPBMUX1.bit.GPIO36 	= 2;  // GPIO36 = /XZCS0
   GpioCtrlRegs.GPBDIR.bit.GPIO36 	= 1;  // GPIO36 = Output

   // GPIO37 /XZCS7
   GpioCtrlRegs.GPBMUX1.bit.GPIO37 	= 3;  // GPIO37 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO37 	= 1;  // GPIO37 = Output

   // GPIO38 XWE0
   GpioCtrlRegs.GPBMUX1.bit.GPIO38 	= 3;  // GPIO38 = /XWE0
   GpioCtrlRegs.GPBDIR.bit.GPIO38 	= 1;  // GPIO38 = OUTPUT

   // GPIO39 EEPROM_WRITE_PROTECT
   GpioCtrlRegs.GPBPUD.bit.GPIO39 	= 1;  // Enable pullup on GPIO35   
   GpioDataRegs.GPBSET.bit.GPIO39	= 1;  // Set High              
   GpioCtrlRegs.GPBMUX1.bit.GPIO39 	= 0;  // GPIO39 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO39 	= 1;  // GPIO39 = Output

//*************************************
//          40~47 XA00~XA07
//*************************************

// GPIO48 RSV_IO_1
   GpioCtrlRegs.GPBPUD.bit.GPIO48 	= 1;  // GPIO48 Enable pullup   
   GpioDataRegs.GPBSET.bit.GPIO48	= 1;  // GPIO48 Set High              
   GpioCtrlRegs.GPBMUX2.bit.GPIO48 	= 0;  // GPIO48 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO48 	= 1;  // GPIO48 = OUTPUT

   // GPIO49 422_485_CTRL
   GpioCtrlRegs.GPBPUD.bit.GPIO49 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO49	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO49 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO49 	= 1;  // GPIO49 = OUTPUT

   // GPIO50 QEP.A1
//   GpioCtrlRegs.GPBPUD.bit.GPIO50 	= 1;  // GPIO50 Enable pullup   
//   GpioDataRegs.GPBCLEAR.bit.GPIO50	= 1;  // GPIO50 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO50 	= 1;  // GPIO50 = EQEP1A
   GpioCtrlRegs.GPBDIR.bit.GPIO50 	= 0;  // GPIO50 = INPUT

   // GPIO51 QEP.B1
//   GpioCtrlRegs.GPBPUD.bit.GPIO51 	= 1;  // GPIO51 Enable pullup   
//   GpioDataRegs.GPBCLEAR.bit.GPIO51	= 1;  // GPIO51 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO51 	= 1;  // GPIO51 = EQEP1B
   GpioCtrlRegs.GPBDIR.bit.GPIO51 	= 0;  // GPIO51 = INPUT

   // GPIO52 QEP.S1
//   GpioCtrlRegs.GPBPUD.bit.GPIO52 	= 1;  // GPIO49 Enable pullup   
//   GpioDataRegs.GPBCLEAR.bit.GPIO52	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO52 	= 1;  // GPIO52 = EQEP1S
   GpioCtrlRegs.GPBDIR.bit.GPIO52 	= 1;  // GPIO52 = OUTPUT

   // GPIO53 QEP.I1
//   GpioCtrlRegs.GPBPUD.bit.GPIO53 	= 1;  // GPIO53 Enable pullup   
//   GpioDataRegs.GPBCLEAR.bit.GPIO53	= 1;  // GPIO53 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO53 	= 1;  // GPIO53 = EQEP1 INDEX
   GpioCtrlRegs.GPBDIR.bit.GPIO53 	= 1;  // GPIO53 = OUTPUT

 
  // GPIO54 SPISIMOA
   GpioCtrlRegs.GPBPUD.bit.GPIO54 	= 0;  
//   GpioDataRegs.GPBSET.bit.GPIO54	= 1;          
   GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3;     // Asynch input GPIO24 (SPISIMOB)
   GpioCtrlRegs.GPBMUX2.bit.GPIO54 	= 1; //GPIO54 = SPI slave in, master out
//   GpioCtrlRegs.GPBDIR.bit.GPIO54 	= 1; 
   
   // GPIO55  nDAC1_CS pin setup
//   GpioCtrlRegs.GPBPUD.bit.GPIO55 	= 1;  
//   GpioDataRegs.GPBSET.bit.GPIO55	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO55 	= 0;  // SPI-A slave out, master in
   GpioCtrlRegs.GPBDIR.bit.GPIO55 	= 1; 

   // GPIO56 SPICLKA
   GpioCtrlRegs.GPBPUD.bit.GPIO56 	= 0;  
//   GpioDataRegs.GPBSET.bit.GPIO56	= 1;          
   GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; 
   GpioCtrlRegs.GPBMUX2.bit.GPIO56 	= 1;  // GPIO56 = SPI-A clock input / output
//   GpioCtrlRegs.GPBDIR.bit.GPIO56 	= 1;  // GPIO56 = OUTPUT

   // GPIO57  nDAC2_CS pin setup
//   GpioCtrlRegs.GPBPUD.bit.GPIO57 	= 1;  
//   GpioDataRegs.GPBSET.bit.GPIO57	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO57 	= 0;  // GPIO57 = SPI-A slave transmit enable
   GpioCtrlRegs.GPBDIR.bit.GPIO57 	= 1;  // GPIO57 = OUTPUT

   // GPIO58 DO1
   GpioCtrlRegs.GPBPUD.bit.GPIO58 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO58	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO58 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO58 	= 1; 

   // GPIO59 DO2
   GpioCtrlRegs.GPBPUD.bit.GPIO59 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO59	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO59 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO59 	= 1; 
   
   // GPIO60 DO3
   GpioCtrlRegs.GPBPUD.bit.GPIO60 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO60	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO60 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO60 	= 1; 
   
   // GPIO61 RSV_IO_1
   GpioCtrlRegs.GPBPUD.bit.GPIO61 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO61	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO61 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO61 	= 1; 

/*
   // GPIO62 RS232_SCIRXDC
   GpioCtrlRegs.GPBMUX2.bit.GPIO62 	= 1;  
   GpioCtrlRegs.GPBDIR.bit.GPIO62 	= 0; 

   // GPIO63 RS232_SCITXDC
   GpioCtrlRegs.GPBMUX2.bit.GPIO63 	= 1;  
   GpioCtrlRegs.GPBDIR.bit.GPIO63 	= 1; 
    
*/
	// GPIO 64 PB_ID.4B
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 1;  // GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO64 	= 0;  // Input

	// GPIO 65 PB_ID.3B
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 1;  // 
    GpioCtrlRegs.GPCDIR.bit.GPIO65 	= 0;  // Input

	// GPIO 66 PB_ID.2B
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 1;  // 
    GpioCtrlRegs.GPCDIR.bit.GPIO66 	= 0;  // Input

	// GPIO 67 PB_ID.1B
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 1;  // 
    GpioCtrlRegs.GPCDIR.bit.GPIO67 	= 0;  // Input

	// GPIO 68 PB_ID.0B
    GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 1;  // 
    GpioCtrlRegs.GPCDIR.bit.GPIO68 	= 0;  // Input

//    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // 

	// GPIO 70 GPIO.1
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 1;  // 
    GpioCtrlRegs.GPCDIR.bit.GPIO70 	= 0;  // Input

//    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // 

     EDIS;
}


/*

void InitXintf(void)
{
    // This shows how to write to the XINTF registers.  The
    // values used here are the default state after reset.
    // Different hardware will require a different configuration.

    // For an example of an XINTF configuration used with the
    // F28335 eZdsp, refer to the examples/run_from_xintf project.

    // Any changes to XINTF timing should only be made by code
    // running outside of the XINTF.

    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = 1/2 SYSCLKOUT
    EALLOW;
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
    // No write buffering
    XintfRegs.XINTCNF2.bit.WRBUFF = 0;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK/2
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;


    // Zone 0------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING0.bit.X2TIMING = 1;

    // Zone will sample XREADY signal
//    XintfRegs.XTIMING0.bit.USEREADY = 1;
    XintfRegs.XTIMING0.bit.USEREADY = 0;
    XintfRegs.XTIMING0.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING0.bit.XSIZE = 3;

    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING6.bit.XWRLEAD = 3;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD = 3;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING6.bit.X2TIMING = 1;

    // Zone will sample XREADY signal
 //   XintfRegs.XTIMING6.bit.USEREADY = 1;
	XintfRegs.XTIMING6.bit.USEREADY = 0;	// 090617
    XintfRegs.XTIMING6.bit.READYMODE = 0;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;


    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING7.bit.X2TIMING = 1;

    // Zone will sample XREADY signal
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 0;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING7.bit.XSIZE = 3;

    // Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 7 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 7;
    XintfRegs.XBANK.bit.BCYC = 7;
    EDIS;
   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.

   InitXintf16Gpio();			// 090617 eunwho
// InitXintf32Gpio();

   asm(" RPT #7 || NOP");

}
*/

void InitXintf16Gpio()
{
    EALLOW;

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

     EDIS;
  
}

//===========================================================================
// No more.
//===========================================================================



