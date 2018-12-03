/*************************************************************************************************************************
									File Details
**************************************************************************************************************************
	File name 							: main.c
	Purpose								: This code shows how to use FIR accelerator to filter two input buffers (channels):
										  1. Channel 1: TAPS = 4096, WINDOW SIZE = 1024
										  2. Channel 2: TAPS = 1024, WINDOW SIZE = 512
**************************************************************************************************************************/
/*************************************************************************************************************************
									Include Files
************************************************************************************************************************/
#include "main.h"

/*************************************************************************************************************************
									Global Variables
**************************************************************************************************************************/
int FIRA_TCB1[FIR_TCB_SIZE];
int FIRA_TCB2[FIR_TCB_SIZE];

float InputBuff1[TAP_LENGTH1+WINDOW_SIZE1-1]=
{
	#include "fir_input1.dat"
};

float InputBuff2[TAP_LENGTH2+WINDOW_SIZE2-1]=
{
	#include "fir_input2.dat"
};


float CoeffBuff1[TAP_LENGTH1]=
{
	#include "fir_coeffs1.dat"
};
float CoeffBuff2[TAP_LENGTH2]=
{
	#include "fir_coeffs2.dat"
};

float OutputBuff1[WINDOW_SIZE1];
float OutputBuff2[WINDOW_SIZE2];

float ExectedOutput1[WINDOW_SIZE1]=
{
	#include "fir_output1.dat"
};

float ExectedOutput2[WINDOW_SIZE2]=
{
	#include "fir_output2.dat"
};


float max_diff1;
float max_diff2;

/*************************************************************************************************************************
									Main Function
**************************************************************************************************************************/
int main()
{

	int i;

	adi_initComponents();

	//Configure FIR accelerator as secure master
	*pREG_SPU0_SECUREP155=BITM_SPU_SECUREP_MSEC;

	//Enable the interrupts globally
	*pREG_SEC0_GCTL=ENUM_SEC_GCTL_EN;
	*pREG_SEC0_CCTL1=ENUM_SEC_CCTL1_EN;
	adi_sec_EnableEdgeSense(INTR_FIR0_DMA, true);
	adi_int_InstallHandler(INTR_FIR0_DMA,FIR_DMA_Interrupt_Handler,0,true);

	//Initialize FIR TCBs for both the channels
	FIRA_TCB1[0]=((int)(FIRA_TCB2+12)>>2)|0xA000000;	//CP
	FIRA_TCB1[1]=TAP_LENGTH1;	//CL
	FIRA_TCB1[2]=1;	//CM
	FIRA_TCB1[3]=((int)CoeffBuff1>>2)|0xA000000;	//CI
	FIRA_TCB1[4]=((int)OutputBuff1>>2)|0xA000000;	//OB
	FIRA_TCB1[5]=WINDOW_SIZE1;	//OL
	FIRA_TCB1[6]=1;	//OM
	FIRA_TCB1[7]=((int)OutputBuff1>>2)|0xA000000;	//OI
	FIRA_TCB1[8]=((int)InputBuff1>>2)|0xA000000;	//IB
	FIRA_TCB1[9]=TAP_LENGTH1+WINDOW_SIZE1-1;	//IL
	FIRA_TCB1[10]=1;	//IM
	FIRA_TCB1[11]=((int)InputBuff1>>2)|0xA000000;	//II
	FIRA_TCB1[12]=(TAP_LENGTH1-1)|(WINDOW_SIZE1-1)<<14;	//FIRCTL2

	FIRA_TCB2[0]=0;	//CP
	FIRA_TCB2[1]=TAP_LENGTH2;	//CL
	FIRA_TCB2[2]=1;	//CM
	FIRA_TCB2[3]=((int)CoeffBuff2>>2)|0xA000000;	//CI
	FIRA_TCB2[4]=((int)OutputBuff2>>2)|0xA000000;	//OB
	FIRA_TCB2[5]=WINDOW_SIZE2;	//OL
	FIRA_TCB2[6]=1;	//OM
	FIRA_TCB2[7]=((int)OutputBuff2>>2)|0xA000000;	//OI
	FIRA_TCB2[8]=((int)InputBuff2>>2)|0xA000000;	//IB
	FIRA_TCB2[9]=TAP_LENGTH2+WINDOW_SIZE2-1;	//IL
	FIRA_TCB2[10]=1;	//IM
	FIRA_TCB2[11]=((int)InputBuff2>>2)|0xA000000;	//II
	FIRA_TCB2[12]=(TAP_LENGTH2-1)|(WINDOW_SIZE2-1)<<14;	//FIRCTL2

	//Initailize the FIR accelerator
	FIRA_Init(BITM_FIR_CTL1_EN|BITM_FIR_CTL1_DMAEN|BITM_FIR_CTL1_CCINTR|(CHANNEL_NO-1)<<BITP_FIR_CTL1_CH,(int*)(((int)(FIRA_TCB1+12)>>2)|0xA000000));

	i=0;

	//Wait till both the channels are processed
	while(FIR_DMA_Done<CHANNEL_NO);

	//Compare the FIR accelerator output with expected (MATLAB) output for both the channels.
	max_diff1=FIRA_Find_Max_Diff(OutputBuff1, ExectedOutput1,WINDOW_SIZE1);
	max_diff2=FIRA_Find_Max_Diff(OutputBuff2, ExectedOutput2,WINDOW_SIZE2);

	printf("\n\nMaximum normalized difference between actual and expected values for buffer 1 = %.*e\n\n",10,max_diff1);
	printf("\n\nMaximum normalized difference between actual and expected values for buffer 2= %.*e\n\n",10,max_diff2);

	if(max_diff1>0.0001||max_diff2>0.0001>0.0001)
		printf("\n\nFIR test failed...\n\n");
	else
		printf("\n\nFIR test passed...\n\n");

	return 0;
}
