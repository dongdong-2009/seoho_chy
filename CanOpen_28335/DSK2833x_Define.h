//###############################################################################################################
// 파일명   : DSK2833x_Define.h
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : TMX(S)320F28335 교육용 키트인 RealSYS DSP28335 DSK(Developer Start Kit) 예제에서 사용하기 위해
// 			  선언한 상수, 메크로 함수 정의이며 DSK28335 KIT 예제에 공통으로 쓰이는 헤더 파일을 Include함
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

#ifndef DSK2833x_DEFINE_H
#define DSK2833x_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define BOOL			char
#define BYTE			unsigned char
#define WORD			unsigned int
#define LONG			unsigned long
#define LLONG			unsigned long long

#define PBYTE(X)		*(volatile BYTE *)(X)
#define PWORD(X)		*(volatile WORD *)(X)
#define PLONG(X)		*(volatile LONG *)(X)
#define PLLONG(X)		*(volatile LLONG *)(X)

#define NOP				asm("	NOP")

#define NULL        	(void *)0

#define TRUE			1
#define FALSE			0

#define HIGH			1
#define LOW				0

#define	HI_BYTE(val)	((val >> 8) & 0xff)
#define	LO_BYTE(val)	(val & 0xff)

// Bit 연산시 일반적으로 쓰이는 부분을 매크로 함수로 정의함  
#define BIT_MASK(bit)				(1 << (bit))
#define GetBit(val, bit)			(((val) & BIT_MASK(bit)) >> (bit))
#define SetBit(val, bit)			(val |= BIT_MASK(bit))
#define ClearBit(val, bit)			(val &= ~BIT_MASK(bit))
#define ToggleBit(val, bit)			(val ^= BIT_MASK(bit))
#define bit_is_set(val, bit)		(val & BIT_MASK(bit))
#define bit_is_clear(val, bit)		(~val & BIT_MASK(bit))

/* Bit 위치 정의(주로 위에서 정의한 매크로 함수에서 사용하기 위해 정의함) */
#define BIT0_POS    	0
#define BIT1_POS    	1
#define BIT2_POS    	2
#define BIT3_POS    	3
#define BIT4_POS    	4
#define BIT5_POS    	5
#define BIT6_POS    	6
#define BIT7_POS    	7
#define BIT8_POS    	8
#define BIT9_POS    	9
#define BIT10_POS   	10
#define BIT11_POS   	11
#define BIT12_POS   	12
#define BIT13_POS   	13
#define BIT14_POS   	14
#define BIT15_POS   	15

/* Bit Mask Data 정의 */
#define BIT0_MASK    	0x0001
#define BIT1_MASK    	0x0002
#define BIT2_MASK    	0x0004
#define BIT3_MASK    	0x0008
#define BIT4_MASK    	0x0010
#define BIT5_MASK    	0x0020
#define BIT6_MASK    	0x0040
#define BIT7_MASK    	0x0080
#define BIT8_MASK    	0x0100
#define BIT9_MASK    	0x0200
#define BIT10_MASK   	0x0400
#define BIT11_MASK   	0x0800
#define BIT12_MASK   	0x1000
#define BIT13_MASK   	0x2000
#define BIT14_MASK   	0x4000
#define BIT15_MASK   	0x8000

// DSK28335 KIT의 예제에서 공통으로 쓰이는 헤더 파일-------------------
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "DSK2833x_GlobalFunc.h"

#include ".\anybus_ic\fd.h"

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSK2833x_DEFINE_H definition
