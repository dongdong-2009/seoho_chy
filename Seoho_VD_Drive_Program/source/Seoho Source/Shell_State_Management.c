
#include	<All_Header.h>
#include	<All_Extern_Variables.h>


void State_Management()
{

	get_command( & Command, & Init_reference );

	if (Flag.Fault.all != 0)	State_Index= STATE_FAULT;

	switch( State_Index )
	{
		case STATE_POWER_ON:
						
			
/*			if(motor_name_plate_check() !=0 )
			{
  
			}
			else
			{

			}
*/			
			State_Index= STATE_CALIBRATION;
			strncpy(MonitorMsg," CALIBRATION     ",20);
			
			break;

		case STATE_CALIBRATION:
			
			if (Flag.ETC.bit.DRIVE_CAL)
			{
				strncpy(MonitorMsg," STOP            ",20);
				State_Index= STATE_STOP;
			}
			
			break;

		case STATE_STOP:
			
			// (Break On,Off)  의 상태 검사 Code 추가할것 
			// 실행은 Interrupt Routine 에서 처리 
			if ((Command == CMD_RUN)&&(Init_reference!=0))
			{
				if (Flag.ETC.bit.PWM_ON) // && (BREAK_ON)
				{
					strncpy(MonitorMsg," ACCELERATING ",20);
					State_Index= STATE_ACCELERATING;
				}
			}
			else if (Command == CMD_STOP)
			{
				if (!Flag.ETC.bit.PWM_ON) // && (BREAK_OFF)
				{
					strncpy(MonitorMsg," READY        ",20);
					State_Index= STATE_READY;
				}
			}

			break;

		case STATE_READY:

			if (Command == CMD_RUN)
			{
				strncpy(MonitorMsg," STOP         ",20);
				State_Index= STATE_STOP;
			}
			else if (Flag.ETC.bit.AUTO_TUNING)
			{
				strncpy(MonitorMsg," TUNING       ",20);
				State_Index= STATE_TUNING;
			}

			break;

		// STATE_RUNING 의 판단은 입력 속도의 정상상태로 결정한다
		// 실제 속도로 판단할 경우 수정이 필요하다.  
		case STATE_ACCELERATING:

			if (Final_reference == Init_reference)
			{
				strncpy(MonitorMsg," RUNNING      ",20);
				State_Index= STATE_RUNNING;
			}

			break; 
		
		case STATE_DECELERATING:

			if (Final_reference == Init_reference)
			{
				if (Init_reference == 0)
				{
					strncpy(MonitorMsg," STOP         ",20);
					State_Index= STATE_STOP;
				}
				else if (Command == CMD_RUN)
				{
					strncpy(MonitorMsg," RUNNING      ",20);
					State_Index= STATE_RUNNING;
				}
			}
			else if (Flag.ETC.bit.ACC)
			{
				strncpy(MonitorMsg," ACCELERATING ",20);
				State_Index= STATE_ACCELERATING;
			}

			break; 
		
		case STATE_RUNNING:

			if (Flag.ETC.bit.ACC)
			{
				strncpy(MonitorMsg," ACCELERATING ",20);
				State_Index= STATE_ACCELERATING;
			}
			else if (Flag.ETC.bit.DEC)
			{
				strncpy(MonitorMsg," DECELERATING ",20);
				State_Index= STATE_DECELERATING;
			}

			break;

		case STATE_TUNING:
			
			if (!Flag.ETC.bit.AUTO_TUNING)
			{
				strncpy(MonitorMsg," STOP         ",20);
				State_Index= STATE_STOP;
			}

			break;  

		case STATE_FAULT:
			
			if ( (Flag.ETC.bit.FAULT_RESET)&&(Flag.Fault.all == 0))
			{
				strncpy(MonitorMsg," STOP         ",20);
				State_Index= STATE_STOP;
				Flag.ETC.bit.FAULT_RESET= 0.;
			}

			break;  

		default:
			strncpy(MonitorMsg," NONE         ",20);

			break;
	}


}		

//===========================================================================
// End of file.
//===========================================================================

