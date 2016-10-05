/*
//###############################################################################################################
// 파일명   : FLASH.cmd
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : TMS320F28335 교육용 키트인 RealSYS DSP28335 DSK(Developer Start Kit)의 메모리 맵을 기준으로
//            TI의 BIOS를 사용하지 않았을 때 내부 FLASH Memory에서 구동하는 예제의 Linker Command File
//			  TI의 SDK v1.20에서 DSP2833x_Headers_nonBIOS.cmd와 F28335.cmd를 가지고 수정함
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
*/

MEMORY
{
PAGE 0: /* Program Memory */
	/* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */
	/* BEGIN is used for the "boot to FLASH" bootloader mode */
		
	RAML_CODE   : origin = 0x008000, length = 0x004000     /* RAML0 ~ RAML3 - Secure Zone Dual Mapped */
	
	FLASH_CODE	: origin = 0x308000, length = 0x037F80	   /* FLASH Sector A ~ FLASH Sector G */
	CSM_RSVD    : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
    BEGIN       : origin = 0x33FFF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
    CSM_CODE    : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
   
	ADC_CAL     : origin = 0x380080, length = 0x000009	   /* ADC_cal function in Reserved memory */
	
	OTP         : origin = 0x380400, length = 0x000400     /* on-chip OTP */
	
	IQTABLES    : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
	IQTABLES2   : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
	FPUTABLES   : origin = 0x3FEBDC, length = 0x0006A0     /* FPU Tables in Boot ROM */
	BOOTROM     : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */
    RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM */
    VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */
    
PAGE 1: /* Data Memory */
	/* BOOT_RSVD is used by the boot ROM for stack.                                                          */
    /* This section is only reserved to keep the BOOT ROM from corrupting this area during the debug process */
    
	BOOT_RSVD   : origin = 0x000000, length = 0x000050     /* Part of M0, BOOT rom will use this for stack */               
	RAMM0       : origin = 0x000050, length = 0x0003B0     /* on-chip RAM block M0 */
	RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
	
	/* 제공된 예제에서는 DMA를 쓰지 않으므로 RAML4 ~ RAML5영역을 데이터 메모리로 쓰고 */
	/* RAML6 ~ RAM7 영역은 DMA용으로 할당함.                                          */
	RAML_DATA   : origin = 0x00C000, length = 0x002000
	
	RAML6       : origin = 0x00E000, length = 0x001000     /* DMA Accessible */
	RAML7       : origin = 0x00F000, length = 0x001000     /* DMA Accessible */
	
	FLASH_DATA	: origin = 0x300000, length = 0x008000	   /* FLASH Sector H */
    
	DEV_EMU     : origin = 0x000880, length = 0x000180     /* device emulation registers */
	FLASH_REGS  : origin = 0x000A80, length = 0x000060     /* FLASH registers */
	CSM         : origin = 0x000AE0, length = 0x000010     /* code security module registers */
	ADC_MIRROR  : origin = 0x000B00, length = 0x000010     /* ADC Results register mirror */
	XINTF       : origin = 0x000B20, length = 0x000020     /* external interface registers */
	CPU_TIMER0  : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers */
	CPU_TIMER1  : origin = 0x000C08, length = 0x000008     /* CPU Timer0 registers (CPU Timer1 & Timer2 reserved TI use) */
	CPU_TIMER2  : origin = 0x000C10, length = 0x000008     /* CPU Timer0 registers (CPU Timer1 & Timer2 reserved TI use) */
	PIE_CTRL    : origin = 0x000CE0, length = 0x000020     /* PIE control registers */
	PIE_VECT    : origin = 0x000D00, length = 0x000100     /* PIE Vector Table */
	DMA         : origin = 0x001000, length = 0x000200     /* DMA Rev 0 registers */
	MCBSPA      : origin = 0x005000, length = 0x000040     /* McBSP-A registers */
	MCBSPB      : origin = 0x005040, length = 0x000040     /* McBSP-B registers */
	ECANA       : origin = 0x006000, length = 0x000040     /* eCAN-A control and status registers */ 
	ECANA_LAM   : origin = 0x006040, length = 0x000040     /* eCAN-A local acceptance masks */
	ECANA_MOTS  : origin = 0x006080, length = 0x000040     /* eCAN-A message object time stamps */
	ECANA_MOTO  : origin = 0x0060C0, length = 0x000040     /* eCAN-A object time-out registers */
	ECANA_MBOX  : origin = 0x006100, length = 0x000100     /* eCAN-A mailboxes */
	ECANB       : origin = 0x006200, length = 0x000040     /* eCAN-B control and status registers */ 
	ECANB_LAM   : origin = 0x006240, length = 0x000040     /* eCAN-B local acceptance masks */
	ECANB_MOTS  : origin = 0x006280, length = 0x000040     /* eCAN-B message object time stamps */
	ECANB_MOTO  : origin = 0x0062C0, length = 0x000040     /* eCAN-B object time-out registers */
	ECANB_MBOX  : origin = 0x006300, length = 0x000100     /* eCAN-B mailboxes */
	EPWM1       : origin = 0x006800, length = 0x000022     /* Enhanced PWM 1 registers */
	EPWM2       : origin = 0x006840, length = 0x000022     /* Enhanced PWM 2 registers */
	EPWM3       : origin = 0x006880, length = 0x000022     /* Enhanced PWM 3 registers */
	EPWM4       : origin = 0x0068C0, length = 0x000022     /* Enhanced PWM 4 registers */
	EPWM5       : origin = 0x006900, length = 0x000022     /* Enhanced PWM 5 registers */
	EPWM6       : origin = 0x006940, length = 0x000022     /* Enhanced PWM 6 registers */
	ECAP1       : origin = 0x006A00, length = 0x000020     /* Enhanced Capture 1 registers */
	ECAP2       : origin = 0x006A20, length = 0x000020     /* Enhanced Capture 2 registers */
	ECAP3       : origin = 0x006A40, length = 0x000020     /* Enhanced Capture 3 registers */
	ECAP4       : origin = 0x006A60, length = 0x000020     /* Enhanced Capture 4 registers */         
	ECAP5       : origin = 0x006A80, length = 0x000020     /* Enhanced Capture 5 registers */         
	ECAP6       : origin = 0x006AA0, length = 0x000020     /* Enhanced Capture 6 registers */
	EQEP1       : origin = 0x006B00, length = 0x000040     /* Enhanced QEP 1 registers */
	EQEP2       : origin = 0x006B40, length = 0x000040     /* Enhanced QEP 2 registers */
	GPIOCTRL    : origin = 0x006F80, length = 0x000040     /* GPIO control registers */
	GPIODAT     : origin = 0x006FC0, length = 0x000020     /* GPIO data registers */
	GPIOINT     : origin = 0x006FE0, length = 0x000020     /* GPIO interrupt/LPM registers */
	SYSTEM      : origin = 0x007010, length = 0x000020     /* System control registers */
	SPIA        : origin = 0x007040, length = 0x000010     /* SPI-A registers */
	SCIA        : origin = 0x007050, length = 0x000010     /* SCI-A registers */
	XINTRUPT    : origin = 0x007070, length = 0x000010     /* external interrupt registers */
	ADC         : origin = 0x007100, length = 0x000020     /* ADC registers */
	SCIB        : origin = 0x007750, length = 0x000010     /* SCI-B registers */
	SCIC        : origin = 0x007770, length = 0x000010     /* SCI-C registers */
	I2CA        : origin = 0x007900, length = 0x000040     /* I2C-A registers */
	CSM_PWL     : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA. CSM password locations. */
	PARTID      : origin = 0x380090, length = 0x000001     /* Part ID register location */
}

/* 
	Allocate sections to memory blocks.
    Note :
    	 codestart : user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                     execution when booting to flash
         ramfuncs  : user defined section to store functions that will be copied from Flash into RAM
                     RamfuncsLoadStart, RamfuncsLoadEnd, RamfuncsRunStart is defined DSP2833x_GlobalPrototypes.h
*/ 
 
SECTIONS
{
	/* Allocate program areas */
	.cinit            : > FLASH_CODE,  PAGE = 0
	.pinit            : > FLASH_CODE,  PAGE = 0
	.text             : > FLASH_CODE,  PAGE = 0
	codestart         : > BEGIN,       PAGE = 0
	ramfuncs          : LOAD = FLASH_CODE, 
	                    RUN = RAML_CODE, 
	                    LOAD_START(_RamfuncsLoadStart),
	                    LOAD_END(_RamfuncsLoadEnd),
	                    RUN_START(_RamfuncsRunStart),
	                    PAGE = 0
	
	/* Code Security Module section in DSP2833x_CSMPasswords.asm */
	csmpasswds        : > CSM_CODE,    PAGE = 0
	csm_rsvd          : > CSM_RSVD,    PAGE = 0
	
	/* Allocate uninitalized data sections */
	.stack            : > RAMM1,       PAGE = 1
	.ebss             : > RAML_DATA,   PAGE = 1
	.esysmem          : > RAMM1,       PAGE = 1
	
	/* Initalized sections */
	.econst           : > FLASH_CODE,  PAGE = 0
	.switch           : > FLASH_CODE,  PAGE = 0      
	
	/* Allocate IQ math areas */
	IQmath            : > FLASH_CODE,  PAGE = 0
	IQmathTables      : > IQTABLES,    PAGE = 0, TYPE = NOLOAD
	 
    /* Uncomment the section below if calling the IQNexp() or IQexp()
      functions from the IQMath.lib library in order to utilize the 
      relevant IQ Math table in Boot ROM (This saves space and Boot ROM 
      is 1 wait-state). If this section is not uncommented, IQmathTables2
      will be loaded into other memory (SARAM, Flash, etc.) and will take
      up space, but 0 wait-state is possible.
    */
    /*
    IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD 
    {
        IQmath.lib<IQNexpTable.obj> (IQmathTablesRam)
    }
    */
    
	FPUmathTables     : > FPUTABLES,   PAGE = 0, TYPE = NOLOAD 
	     
	/* Allocate DMA-accessible RAM sections */
	/* 제공된 예제에서는 DMA를 쓰지 않으므로 RAML4 ~ RAML5영역을 데이터 메모리로 쓰고 */
	/* RAML6 ~ RAM7 영역은 DMA용으로 할당함.                                          */  
	DMARAML6          : > RAML6,       PAGE = 1
	DMARAML7          : > RAML7,       PAGE = 1
	
	/* .reset is a standard section used by the compiler.                            */
	/* It contains the the address of the start of _c_int00 for C Code.              */
	/* When using the boot ROM, this section and the CPU vector table is not needed. */
	/* Thus the default type is set here to DSECT                                    */ 
	.reset            : > RESET,       PAGE = 0, TYPE = DSECT
	vectors           : > VECTORS,     PAGE = 0, TYPE = DSECT
	
	/* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
	.adc_cal          : load = ADC_CAL, PAGE = 0, TYPE = NOLOAD
   
	PieVectTableFile  : > PIE_VECT,    PAGE = 1
	
	/*** Peripheral Frame 0 Register Structures ***/
	DevEmuRegsFile    : > DEV_EMU,     PAGE = 1
	FlashRegsFile     : > FLASH_REGS,  PAGE = 1
	CsmRegsFile       : > CSM,         PAGE = 1
	AdcMirrorFile     : > ADC_MIRROR,  PAGE = 1 
	XintfRegsFile     : > XINTF,       PAGE = 1
	CpuTimer0RegsFile : > CPU_TIMER0,  PAGE = 1
	CpuTimer1RegsFile : > CPU_TIMER1,  PAGE = 1
	CpuTimer2RegsFile : > CPU_TIMER2,  PAGE = 1  
	PieCtrlRegsFile   : > PIE_CTRL,    PAGE = 1     
	DmaRegsFile       : > DMA,         PAGE = 1 
	
	/*** Peripheral Frame 3 Register Structures ***/
	McbspaRegsFile    : > MCBSPA,      PAGE = 1
	McbspbRegsFile    : > MCBSPB,      PAGE = 1
	
	/*** Peripheral Frame 1 Register Structures ***/
	ECanaRegsFile     : > ECANA,       PAGE = 1
	ECanaLAMRegsFile  : > ECANA_LAM,   PAGE = 1   
	ECanaMboxesFile   : > ECANA_MBOX,  PAGE = 1
	ECanaMOTSRegsFile : > ECANA_MOTS,  PAGE = 1
	ECanaMOTORegsFile : > ECANA_MOTO,  PAGE = 1
	
	ECanbRegsFile     : > ECANB,       PAGE = 1
	ECanbLAMRegsFile  : > ECANB_LAM,   PAGE = 1   
	ECanbMboxesFile   : > ECANB_MBOX,  PAGE = 1
	ECanbMOTSRegsFile : > ECANB_MOTS,  PAGE = 1
	ECanbMOTORegsFile : > ECANB_MOTO,  PAGE = 1
	
	EPwm1RegsFile     : > EPWM1,       PAGE = 1   
	EPwm2RegsFile     : > EPWM2,       PAGE = 1   
	EPwm3RegsFile     : > EPWM3,       PAGE = 1   
	EPwm4RegsFile     : > EPWM4,       PAGE = 1   
	EPwm5RegsFile     : > EPWM5,       PAGE = 1   
	EPwm6RegsFile     : > EPWM6,       PAGE = 1
	
	ECap1RegsFile     : > ECAP1,       PAGE = 1   
	ECap2RegsFile     : > ECAP2,       PAGE = 1   
	ECap3RegsFile     : > ECAP3,       PAGE = 1   
	ECap4RegsFile     : > ECAP4,       PAGE = 1
	ECap5RegsFile     : > ECAP5,       PAGE = 1   
	ECap6RegsFile     : > ECAP6,       PAGE = 1
	
	EQep1RegsFile     : > EQEP1,       PAGE = 1   
	EQep2RegsFile     : > EQEP2,       PAGE = 1               
	
	GpioCtrlRegsFile  : > GPIOCTRL,    PAGE = 1
	GpioDataRegsFile  : > GPIODAT,     PAGE = 1
	GpioIntRegsFile   : > GPIOINT,     PAGE = 1
	
	/*** Peripheral Frame 2 Register Structures ***/
	SysCtrlRegsFile   : > SYSTEM,      PAGE = 1
	SpiaRegsFile      : > SPIA,        PAGE = 1
	SciaRegsFile      : > SCIA,        PAGE = 1
	XIntruptRegsFile  : > XINTRUPT,    PAGE = 1
	AdcRegsFile       : > ADC,         PAGE = 1
	ScibRegsFile      : > SCIB,        PAGE = 1
	ScicRegsFile      : > SCIC,        PAGE = 1
	I2caRegsFile      : > I2CA,        PAGE = 1
	          
	/*** Code Security Module Register Structures ***/
	CsmPwlFile        : > CSM_PWL,     PAGE = 1
	
	/*** Device Part ID Register Structures ***/
    PartIdRegsFile    : > PARTID,      PAGE = 1
}
