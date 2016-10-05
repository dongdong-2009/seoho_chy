#include	<All_Header.h>
#include	<All_Extern_Variables.h>
#include <All_Module.h>
#include <Shell_ABS_Encoder.h>
#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"


void DecimalToBianary(Uint16 val, Uint16 bits, Uint16 binArr[])
{
	int i=0;
	if(val < 2)
	{
		binArr[bits-1] = val;
		for(i = bits-1; i > 0; i--) binArr[i-1] = 0;
			
	}//end if
	else
	{
		binArr[bits - 1] = val%2;
		val/=2;
		DecimalToBianary(val, bits-1, binArr);
	}//end else
}


void GrayToBinary(Uint16 N, Uint16 binArr[], Uint16 conG2B[])
{
	int length = N;
	int i;

	conG2B[0] = binArr[0];

	for(i=0; i<length; i++)
	{
		if(binArr[i+1] == 0)
		{
			if(conG2B[i] == 0)
				conG2B[i+1] = 0;

			else
				conG2B[i+1] = 1;
		}//end if
		else if(binArr[i+1] == 1)
		{
			if(conG2B[i] == 1)
				conG2B[i+1] = 0;

			else
				conG2B[i+1] = 1;
		}//end else if
	}//end for i
}

void BinaryToDecimal(Uint16 N, Uint16 conG2B[])
{
	Uint16	result;
	int sqNum=0;
	int i;

	for(i=N-1; i >=0; i--)
	{
		if(conG2B[i] == 1)
		{
			result += square(sqNum);
		}

		sqNum++;
	} //end for i
	result1 = result;
}


int square(int cnt)
{
	int ret = 1;
	int i;

	for(i=0; i < cnt; i++)
	{
		ret *= 2;
	}

	return ret;
}



//===========================================================================
// No more.
//===========================================================================
