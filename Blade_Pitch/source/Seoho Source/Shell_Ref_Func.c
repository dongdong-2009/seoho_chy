#include	<All_Header.h>
#include	<All_Extern_Variables.h>
	


void Reference_Function()
{

//	static double Accel_delta, Decel_delta;
	double Set_ref, Out_ref, fabs_Set_ref, fabs_Out_ref; 
	static double Temp=0., Accel_value= 0., Decel_value= 0., Accel_delta= 0., Decel_delta= 0.;


	Set_ref= Init_reference;
	if ((!Driver_ON)||(Command==CMD_STOP)) Set_ref=0.;	
	Out_ref= Final_reference;
	fabs_Set_ref= fabs(Set_ref);
	fabs_Out_ref= fabs(Out_ref);
		
	if (Set_ref!=Temp)
	{
		if ((Set_ref*Out_ref)<0)
		{
			Accel_delta= Set_ref;
			Decel_delta= Out_ref;
		}
		else if (fabs_Set_ref>fabs_Out_ref)
		{
			Accel_delta= Set_ref-Out_ref;
			Decel_delta= 0.;
		}
		else if (fabs_Set_ref<fabs_Out_ref)		
		{
			Accel_delta= 0.;
			Decel_delta= Out_ref-Set_ref;
		}
		else
		{
			Accel_delta= 0.;
			Decel_delta= 0.;
		}
		
		Accel_value= Accel_delta/Accel_time*TSAMP;
		Decel_value= Decel_delta/Decel_time*TSAMP;
	}

	Temp= Set_ref;



	if ((fabs(Set_ref-Out_ref)<fabs(Decel_value))
		||(fabs(Set_ref-Out_ref)<fabs(Accel_value)))
	{	
		Out_ref= Set_ref; 
		Flag.ETC.bit.ACC= 0.;
		Flag.ETC.bit.DEC= 0.;
	}	
	else if ((Set_ref*Out_ref)<0)
	{	
		Out_ref-= Decel_value; 
		Flag.ETC.bit.ACC= 0.;
		Flag.ETC.bit.DEC= 1.;
	}
	else if (fabs_Set_ref>fabs_Out_ref)
	{
		Out_ref+= Accel_value;
		Flag.ETC.bit.ACC= 1.;
		Flag.ETC.bit.DEC= 0.;
	} 
	else if (fabs_Set_ref<fabs_Out_ref)
	{
		Out_ref-= Decel_value;
		Flag.ETC.bit.ACC= 0.;
		Flag.ETC.bit.DEC= 1.;
	}
	else 
	{
		Out_ref= Set_ref;
		Flag.ETC.bit.ACC= 0.;
		Flag.ETC.bit.DEC= 0.;
	}

	Final_reference= BOUND(Out_ref,1,(-1));
}	

//----------------------------------
// End of File
//----------------------------------

