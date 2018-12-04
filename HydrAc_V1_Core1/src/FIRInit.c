/*************************************************************************************************************************
									File Details
**************************************************************************************************************************
	File name 							: FIRInit.c
	Purpose								: Has definitions for various global variables, enum data types, and
										  functions required to use the FIR module in different modes.
**************************************************************************************************************************/

/*************************************************************************************************************************
									Include Files
*************************************************************************************************************************/
#include "FIRInit.h"

/*************************************************************************************************************************
									Global Variables
**************************************************************************************************************************/
volatile uint32_t FIR_DMA_Done=0;


/*************************************************************************************************************************
									Function Definitions
**************************************************************************************************************************/
/**************************************************************************************************************************
	Function name	:   FIRA_Init
    Description		:   This function can be used to configure the FIR global control register and start the FIR accelerator
    					operation
	Arguments		:	Parameter					 |	Description		 				| Valid values
						uiFIR_GCTL					 |  FIR global control register		| unsigned int 32 bit
						uiFIR_CHNPTR				 |  Start of first TCB				| 32 bit aligned TCB address
	Return value	:   None.
**************************************************************************************************************************/
void FIRA_Init(uint32_t uiFIR_GCTL, uint32_t* uiFIR_CHNPTR)
{
	*pREG_FIR0_CHNPTR=(uint32_t)uiFIR_CHNPTR;
	*pREG_FIR0_CTL1=uiFIR_GCTL;
}


/**************************************************************************************************************************
	Function name	:   FIR_DMA_Interrupt_Handler
    Description		:   FIR DMA done interrupt handler
	Arguments		:	Parameter					 |	Description		 				| Valid values
	Return value	:   None.
**************************************************************************************************************************/
void FIR_DMA_Interrupt_Handler(uint32_t iid, void* handlerArg)
{
	FIR_DMA_Done++;

}

/**************************************************************************************************************************
	Function name	:   FIRA_Find_Max_Diff
    Description		:   This function can be used to find the maximum difference between the expected and actual results
	Arguments		:	Parameter					 |	Description		 				| Valid values
						uiBuff_actual				 |  Actual Buffer Address			| unsigned int 32 bit
						uiBuff_expected				 |  Expected Buffer Address			| unsigned int 32 bit
						uiCount						 |  count							| unsigned int 32 bit
	Return value	:   float.
**************************************************************************************************************************/
float FIRA_Find_Max_Diff(float* uiBuff_actual, float* uiBuff_expected, unsigned int uiCount)
{
	uint32_t i; float temp, temp1;
	float max_diff=0; float diff;
	float* pt = uiBuff_actual; float*  pt1 = uiBuff_expected;

	for(i=0;i<uiCount;i++)
	{
		temp=*pt++; temp1=*pt1++;

		if(temp>temp1)
			diff = temp-temp1;
		else
			diff = temp1-temp;

		if(temp1!=0.0&&diff>max_diff)
			max_diff=diff/temp1;
	}

	return max_diff;
}
