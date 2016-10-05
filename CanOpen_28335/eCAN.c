//###############################################################################################################
// 파일명   : eCAN.c
// 회사명   : RealSYS
// 작성날짜 : 2008.10.20
// 작성자   : R.N.K
// 설명     : F28335의 내부 CAN-A/B 주변장치 관련 구현 코드
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

#include "DSK2833x_Define.h"

// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 위해 내부 RAM에서 동작하도록 함

// eCAN-A/B Interrupt Service Function 선언
#pragma CODE_SECTION(ecan0_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan0_intb_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_intb_isr, "ramfuncs");

interrupt void ecan0_inta_isr(void);
interrupt void ecan1_inta_isr(void);
interrupt void ecan0_intb_isr(void);
interrupt void ecan1_intb_isr(void);

// ex08_can.c에 정의된 eCAN-A/B 처리를 위한 변수 -----------------------------
extern BOOL cana_rx_flag;		// CAN-A에 새로운 수신 데이터를 받았는지 여부
extern LONG cana_rx_id;			// CAN-A 수신 ID
extern BYTE cana_rx_length;   	// CAN-A 수신 데이터 크기
extern LONG cana_rx_low_data;	// CAN-A 수신 Low Word Data
extern LONG cana_rx_high_data;	// CAN-A 수신 High Word Data

extern BOOL canb_rx_flag;		// CAN-B에 새로운 수신 데이터를 받았는지 여부
extern LONG canb_rx_id;			// CAN-B 수신 ID
extern BYTE canb_rx_length;   	// CAN-B 수신 데이터 크기
extern LONG canb_rx_low_data;	// CAN-B 수신 Low Word Data
extern LONG canb_rx_high_data;	// CAN-B 수신 High Word Data
//----------------------------------------------------------------------------

// CAN-A MBOX1을 사용하여 메시지를 전송함
void SendDataToECanA(LONG id, BYTE length, LONG low_data, LONG high_data){
	// 현재 CAN-A MBOX1가 전송 중이면 리턴함.
	if(ECanaRegs.CANTRS.bit.TRS1 == 1) return;
	
	ECanaRegs.CANME.bit.ME1 = 0;
	ECanaMboxes.MBOX1.MSGID.all = 0x80000000 | (id & 0x1fffffff);
	ECanaRegs.CANME.bit.ME1 = 1;

	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	ECanaMboxes.MBOX1.MDL.all = low_data;
	ECanaMboxes.MBOX1.MDH.all = high_data;
	
	ECanaRegs.CANTRS.bit.TRS1 = 1;
}

// CAN-B MBOX1을 사용하여 메시지를 전송함
void SendDataToECanB(LONG id, BYTE length, LONG low_data, LONG high_data){
	// 현재 CAN-B MBOX1가 전송 중이면 리턴함.
	if(ECanbRegs.CANTRS.bit.TRS1 == 1) return;
	
	ECanbRegs.CANME.bit.ME1 = 0;
	ECanbMboxes.MBOX1.MSGID.all = 0x80000000 | (id & 0x1fffffff);
	ECanbRegs.CANME.bit.ME1 = 1;
	
	ECanbMboxes.MBOX1.MDL.all = low_data;
	ECanbMboxes.MBOX1.MDH.all = high_data;
	ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	
	ECanbRegs.CANTRS.bit.TRS1 = 1;
}

void init_can(void){
	// Initialize CAN-A/B peripheral with 1M Bps
	// refer to DSP2833x_ECan.c file
	InitECanGpio();
	InitECan();
	
	// Initialize CAN-A Transmiter/Receiver MailBox
	// CAN-A MBOX0 -> Receiver MailBox
	// CAN-A MBOX1 -> Transmiter MailBox
	ECanaLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)
	ECanaMboxes.MBOX0.MSGID.all = 0xc0000000; // IDE=1, AME=1
	
	ECanaRegs.CANME.bit.ME0 = 1;
	
	ECanaRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanaRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-B Transmiter/Receiver MailBox
	// CAN-B MBOX0 -> Receiver MailBox
	// CAN-B MBOX1 -> Transmiter MailBox
	ECanbLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)
	ECanbMboxes.MBOX0.MSGID.all = 0xc0000000; // IDE=1, AME=1
	
	ECanbRegs.CANME.bit.ME0 = 1;
	
	ECanbRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanbRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-A/B interrupt vector
	EALLOW;
	PieVectTable.ECAN0INTA = &ecan0_inta_isr;
	PieVectTable.ECAN1INTA = &ecan1_inta_isr;
	PieVectTable.ECAN0INTB = &ecan0_intb_isr;
	PieVectTable.ECAN1INTB = &ecan1_intb_isr;
	EDIS;

	EALLOW;
	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt level
	// CAN-A/B Receiver interrupt level   -> 0
	// CAN-A/B Transmiter interrupt level -> 1
	ECanaRegs.CANMIL.bit.MIL0 = 0;
	ECanaRegs.CANMIL.bit.MIL1 = 1;
	ECanbRegs.CANMIL.bit.MIL0 = 0;
	ECanbRegs.CANMIL.bit.MIL1 = 1;

	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt mask
	ECanaRegs.CANMIM.bit.MIM0 = 1;
	ECanaRegs.CANMIM.bit.MIM1 = 1;
	ECanbRegs.CANMIM.bit.MIM0 = 1;
	ECanbRegs.CANMIM.bit.MIM1 = 1;
	
	// Initialize CAN-A/B Global interrupt mask
	ECanaRegs.CANGIM.bit.I0EN = 1;
	ECanaRegs.CANGIM.bit.I1EN = 1;
	ECanbRegs.CANGIM.bit.I0EN = 1;
	ECanbRegs.CANGIM.bit.I1EN = 1;
	EDIS;

	// Enable CPU INT9 for CAN-A/B interrupt
	IER |= M_INT9;
	
    // Enable CAN-A/B INT0/1 in the PIE: Group 9 interrupt 5/6/7/8
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx8 = 1;
}

// CAN-A MBOX0 수신 메시지 처리를 위한 인터럽트
interrupt void ecan0_inta_isr(void){
	if(ECanaRegs.CANRMP.bit.RMP0 == 1){
		if(!cana_rx_flag){
			cana_rx_id = ECanaMboxes.MBOX0.MSGID.all & 0x1fffffff;
			cana_rx_length = ECanaMboxes.MBOX0.MSGCTRL.bit.DLC;
			cana_rx_low_data = ECanaMboxes.MBOX0.MDL.all;
			cana_rx_high_data = ECanaMboxes.MBOX0.MDH.all;
			cana_rx_flag = TRUE;
		}
		
		// Clear receive pending flag
		ECanaRegs.CANRMP.bit.RMP0 = 1;	
	}	

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-A MBOX1 송신 메시지 처리를 위한 인터럽트
interrupt void ecan1_inta_isr(void){
	if(ECanaRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanaRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-B MBOX0 수신 메시지 처리를 위한 인터럽트
interrupt void ecan0_intb_isr(void){
	if(ECanbRegs.CANRMP.bit.RMP0 == 1){
		if(!canb_rx_flag){
			canb_rx_id = ECanbMboxes.MBOX0.MSGID.all & 0x1fffffff;
			canb_rx_length = ECanbMboxes.MBOX0.MSGCTRL.bit.DLC;
			canb_rx_low_data = ECanbMboxes.MBOX0.MDL.all;
			canb_rx_high_data = ECanbMboxes.MBOX0.MDH.all;
			canb_rx_flag = TRUE;
		}
		
		// Clear receive pending flag
		ECanbRegs.CANRMP.bit.RMP0 = 1;	
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-B MBOX1 송신 메시지 처리를 위한 인터럽트
interrupt void ecan1_intb_isr(void){
	if(ECanbRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanbRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
