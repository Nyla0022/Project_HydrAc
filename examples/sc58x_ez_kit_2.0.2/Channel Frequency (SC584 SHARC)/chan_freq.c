/*********************************************************************************

Copyright(c) 2015-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      chan_freq.c
 * @brief     This example reads the ADC channels and calculates the detected frequency.
 * @version:  $Revision: 29599 $
 * @date:     $Date: 2017-05-04 10:37:52 -0400 (Thu, 04 May 2017) $
 *
 *
 */

/*=============  I N C L U D E S   =============*/

#include <sys/platform.h>
#include <services/spu/adi_spu.h>
#include <services/gpio/adi_gpio.h>
#include <drivers/adc/adau1977/adi_adau1977.h>
#include <drivers/twi/adi_twi.h>
#include <stdio.h>
#include <string.h>
#include "chan_freq.h"
#include "adi_initialize.h"

#include <stdlib.h>

#if !defined(ADI_CACHE_LINE_LENGTH)
/* The ADI_CACHE_* macros were introduced in CCES 2.4.0 in <sys/platform.h>.
 * If using an older toolchain, define them here.
 */
 #if defined(__ADSPARM__)
  #define ADI_CACHE_LINE_LENGTH (32uL)
  #define ADI_CACHE_ALIGN __attribute__((aligned(ADI_CACHE_LINE_LENGTH)))
 #elif defined(__ADSP215xx__)
  #define ADI_CACHE_LINE_LENGTH (64uL)
  #define ADI_CACHE_ALIGN _Pragma("align 64")
 #else
  #error Unknown ADI_CACHE_LINE_LENGTH
 #endif
 #define ADI_CACHE_ROUND_UP_SIZE(size, type) \
    (((((((size) * sizeof(type)) \
           + (ADI_CACHE_LINE_LENGTH - 1uL)) \
          / ADI_CACHE_LINE_LENGTH) * ADI_CACHE_LINE_LENGTH) \
      + (sizeof(type) - 1uL)) / sizeof(type))
#endif

/*==============  D E F I N E S  ===============*/

#define SUCCESS  (0u)
#define FAILURE  (1u)

/*=============  D A T A  =============*/

/* Twi  */
static uint32_t TwiMemory[ADI_TWI_MEMORY_SIZE];

/* Instance to submit TWI configuration */
static ADI_ADAU1977_TWI_CONFIG     TwiConfig;

/* Gpio */
static uint32_t GpioMemory[ADI_GPIO_CALLBACK_MEM_SIZE];
static uint32_t gpioMaxCallbacks;

/* ADAU1977 Sport */
static uint32_t Adau1977SportMemory[ADI_SPORT_DMA_MEMORY_SIZE];

/* Instance to submit SPORT configuration */
static ADI_ADAU1977_SPORT_CONFIG   SportConfig;

/* ADAU1977 ADC */
static ADI_ADAU1977_HANDLE phAdau1977;
static uint32_t Adau1977Memory[ADI_ADAU1977_MEMORY_SIZE];

/* Counter to keep track of number of ADC buffers processed */
static volatile uint32_t AdcCount = 0u;

/* error flag */
volatile bool bError = false;

ADI_CACHE_ALIGN static int16_t AdcBuf1[ADI_CACHE_ROUND_UP_SIZE(AUDIO_BUFFER_SIZE, int16_t)];
ADI_CACHE_ALIGN static int16_t AdcBuf2[ADI_CACHE_ROUND_UP_SIZE(AUDIO_BUFFER_SIZE, int16_t)];

#define MAXDATA  (AUDIO_BUFFER_SIZE*4u) //between 4MBytes and 8MBytes (for 400Ksamples @2B/sample)
										//why do I need so much memory for a single channel?
										//it does not make sense to me to have a channel buffer of 16N
										//when the audio buffer is 4N.

static int16_t Chan1Data[NUM_SAMPLES]; //N*4*4 = 16N
static int16_t Chan2Data[NUM_SAMPLES]; //16MB max for  (for 400Ksamples @2B/sample)
//static int16_t Chan3Data[NUM_SAMPLES];
//static int16_t Chan4Data[NUM_SAMPLES];


volatile uint32_t nSample = 0u;

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/
/* Initialize GPIO and reset peripherals */
uint32_t    GpioInit(void);
/* Initializes ADC */
uint32_t    Adau1977Init(void);

/*=============  C A L L B A C K    F U N C T I O N    P R O T O T Y P E S =============*/

/* ADC callback */
void AdcCallback(void *pCBParam, uint32_t nEvent, void *pArg);

/*=============  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S =============*/

/* Configures soft switches */
extern void ConfigSoftSwitches(void);

extern uint32_t detectFreq(int32_t inSignal[]);

/*=============  C O D E  =============*/

/*
 * Main function
 */
int main()
{
	/**
	 * Initialize managed drivers and/or services that have been added to
	 * the project.
	 * @return zero on success
	 *
	 */
	uint32_t Result = SUCCESS, i=0, freq=0,time=0, m=0;

	FILE * fp;

#if defined(__ADSPBF707_FAMILY__) || defined(__ADSP215xx__)
	/* Memory required for the SPU operation */
	uint8_t  SpuMemory[ADI_SPU_MEMORY_SIZE];

	/* SPU handle */
	ADI_SPU_HANDLE hSpu;
#endif

	/* configure System Event Controller SEC and Signal Routing Unit SRU */
	adi_initComponents();

	/* Software Switch Configuration for the EZ-Board */
	ConfigSoftSwitches();

	Result = GpioInit();
	if (Result == FAILURE)
	{
		bError = true;
		DBG_MSG("GPIO Init failed\n");
	}

/* config SPU for ADSP-BF707 or ADSP-SC589 Processor family */
#if defined(__ADSPBF707_FAMILY__) || defined(__ADSPSC589_FAMILY__)

	/* Initialize SPU Service */
	if(adi_spu_Init(0u, SpuMemory, NULL, NULL, &hSpu) != ADI_SPU_SUCCESS)
	{
		bError = true;
		DBG_MSG("Failed to initialize SPU service\n");
	}

	/* Make SPORT 5A to generate secure transactions */
	if(adi_spu_EnableMasterSecure(hSpu, SPORT_5A_SPU_PID, true) != ADI_SPU_SUCCESS)
	{
		bError = true;
		DBG_MSG("Failed to enable Master secure for SPORT 5A\n");
	}

	/* Make SPORT 5A DMA to generate secure transactions */
	if(adi_spu_EnableMasterSecure(hSpu, SPORT_5A_DMA12_SPU_PID, true) != ADI_SPU_SUCCESS)
	{
		bError = true;
		DBG_MSG("Failed to enable Master secure for SPORT 5A DMA\n");
	}

#elif defined(__ADSPSC573_FAMILY__)

   /* Initialize SPU Service */
   if(adi_spu_Init(0u, SpuMemory, NULL, NULL, &hSpu) != ADI_SPU_SUCCESS)
   {
      bError = true;
      DBG_MSG("Failed to initialize SPU service\n");
   }

   /* Make SPORT 2A to generate secure transactions */
   if(adi_spu_EnableMasterSecure(hSpu, SPORT_2A_SPU_PID, true) != ADI_SPU_SUCCESS)
         {
      bError = true;
      DBG_MSG("Failed to enable Master secure for SPORT 2A\n");
   }

   /* Make SPORT 2A DMA to generate secure transactions */
   if(adi_spu_EnableMasterSecure(hSpu, SPORT_2A_DMA4_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      bError = true;
      DBG_MSG("Failed to enable Master secure for SPORT 2A DMA\n");
   }

#endif

	/* Initialize ADAU1977 */
	Result = Adau1977Init();
	if (Result == FAILURE)
	{
		bError = true;
		DBG_MSG("ADAU1977 Init failed\n");
	}

	/* Submit ADC buffer1 */ //N*4*2 (2B/sample) or N*4*4(4B/sample)
	// All it does is giving the buffer size in bytes not in samples. That is why it multiplies the size (in samples)
	//the number of bytes/sample
	if(adi_adau1977_SubmitBuffer(phAdau1977, &AdcBuf1[0u], AUDIO_BUFFER_SIZE*BYTES_PER_SAMPLE) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("submit buffer failed\n");
	}

	 //Submit ADC buffer2
	if(adi_adau1977_SubmitBuffer(phAdau1977, &AdcBuf2[0u], AUDIO_BUFFER_SIZE*BYTES_PER_SAMPLE) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("submit buffer failed\n");
	}

	/* Enable ADC data flow */
	if(adi_adau1977_Enable(phAdau1977, true) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("ADC enable failed\n");
	}

	printf("DATA FLOW ENABLED! processing callbacks...\n");

//	clock_t begin = clock();

	/* process samples for a while then exit */
	while(AdcCount < CALLBACK_COUNT)
	{
		printf("AdcCount==%d\n", AdcCount);
		/* process samples in the callback */
		if (bError)
		{
			DBG_MSG("ADC callback failed\n");
			break;
		}
    }

	/*clock_t end = clock();

	double exec_time = (double) (end-begin)/CLOCKS_PER_SEC;

	printf("*****EXECUTION TIME: %f*****\n", exec_time);*/


	/* Disable ADC data flow */
	if(adi_adau1977_Enable(phAdau1977, false) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("ADC disable failed\n");
	}

	/* Close ADC device */
	if (adi_adau1977_Close(phAdau1977) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("ADC close failed\n");
	}



	fp = fopen ("adc_data.txt", "w+");

		 fprintf(fp, "Time[s]\tChan 1[v]\tChan 2[v]\n");

		 for(m=0;m<NUM_CHANNELS;m++){
			 fprintf(fp, "%f\t%f\t%f\n", (double) time, (double)((int)Chan1Data[m]* ADC_CONV_F_16),(double)((int)Chan2Data[m]* ADC_CONV_F_16));
			 time = time + (double) 1.0/(192000);
		 }

		 fclose(fp);


	printf("\nChan 1[v]\tChan 2[v]\t\n");
	for (i=0u; i<NUM_SAMPLES; i++){
		printf("%f\t%f\t\n ",(double)((int)Chan1Data[i]* ADC_CONV_F_16),(double)((int)Chan2Data[i]* ADC_CONV_F_16));
	}

	printf("\n");

	/* calculate the detected freq (SHARC only) */
	freq = detectFreq(&Chan1Data[0]);
	printf("Chan1 freq: %d\n", (int)freq);

	printf("\n");

	if (bError == false)
	{
		printf("All done\n");
	}
	else
	{
		printf("Example failed\n");
	}

	return 0;
}


/*
 * Initializes GPIO service
 * A GPIO line is used to control reset of the ADC device
 */
uint32_t GpioInit(void)
{
	ADI_GPIO_RESULT result = ADI_GPIO_SUCCESS;
	/* Loop variable */
	volatile uint32_t i;

	result = adi_gpio_Init((void*)GpioMemory, ADI_GPIO_CALLBACK_MEM_SIZE, &gpioMaxCallbacks);
	if (result != ADI_GPIO_SUCCESS)
	{
		/* return error */
		return FAILURE;
	}

#if defined(__ADSPBF707_FAMILY__) || defined(__ADSPSC589_FAMILY__)
	/* connected to ADAU1962 reset */
	result = adi_gpio_SetDirection(ADI_GPIO_PORT_A, ADI_GPIO_PIN_14, ADI_GPIO_DIRECTION_OUTPUT);

	/* bring ADAU1962 reset high (enables clock output) */
	result = adi_gpio_Set(ADI_GPIO_PORT_A, ADI_GPIO_PIN_14);

	/* connected to ADAU1977 reset */
	result = adi_gpio_SetDirection(ADI_GPIO_PORT_A, ADI_GPIO_PIN_15, ADI_GPIO_DIRECTION_OUTPUT);

	/* bring ADAU1977 reset low */
	result = adi_gpio_Clear(ADI_GPIO_PORT_A, ADI_GPIO_PIN_15);

#elif defined(__ADSPSC573_FAMILY__)

	/* connected to ADAU1962 reset */
	result = adi_gpio_SetDirection(ADI_GPIO_PORT_A, ADI_GPIO_PIN_6, ADI_GPIO_DIRECTION_OUTPUT);

	/* bring ADAU1962 reset high (enables clock output) */
	result = adi_gpio_Set(ADI_GPIO_PORT_A, ADI_GPIO_PIN_6);

	/* connected to ADAU1977 reset */
	result = adi_gpio_SetDirection(ADI_GPIO_PORT_D, ADI_GPIO_PIN_0, ADI_GPIO_DIRECTION_OUTPUT);

	/* bring ADAU1977 reset low */
	result = adi_gpio_Clear(ADI_GPIO_PORT_D, ADI_GPIO_PIN_0);

#endif

	/* delay */
	for (i = DELAY_COUNT; i; i--)
	{
	}

#if defined(__ADSPBF707_FAMILY__) || defined(__ADSPSC589_FAMILY__)
	/* bring ADAU1977 reset high */
	result = adi_gpio_Set(ADI_GPIO_PORT_A, ADI_GPIO_PIN_15);
#elif defined(__ADSPSC573_FAMILY__)
	/* bring ADAU1977 reset high */
	result = adi_gpio_Set(ADI_GPIO_PORT_D, ADI_GPIO_PIN_0);
#endif

	/* delay */
	for (i = DELAY_COUNT; i; i--)
	{
	}

	return SUCCESS;
}

/*
 * Opens and initializes ADAU1977 ADC Device.
 */
uint32_t Adau1977Init(void)
{
	ADI_ADAU1977_RESULT result;
	bool bStat1, bStat2, bStat3;
	ADI_ADAU1977_MIC_BIAS_VOLT ebias;
	uint32_t i;

	/* open ADAU1977 instance */
    result = adi_adau1977_Open(0u,
    		ADI_ADAU1977_SERIAL_MODE_STEREO,
    		&Adau1977Memory,
    		ADI_ADAU1977_MEMORY_SIZE,
    		&phAdau1977);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_Open failed\n");
		return FAILURE;
	}

	/* TWI parameters required to open/configure TWI */
	TwiConfig.TwiDevNum 	= 0u;
	TwiConfig.TwiDevMemSize	= ADI_TWI_MEMORY_SIZE;
	TwiConfig.pTwiDevMem 	= &TwiMemory;
	TwiConfig.eTwiAddr 		= ADI_ADAU1977_TWI_ADDR_31;

	result = adi_adau1977_ConfigTwi(phAdau1977, &TwiConfig);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigTwi failed\n");
		return FAILURE;
	}

	/* SPORT parameters required to open/configure SPORT */
#if defined(__ADSPBF707_FAMILY__) || defined(__ADSPSC589_FAMILY__)
    SportConfig.SportDevNum     = 5u;
#elif defined(__ADSPSC573_FAMILY__)
    SportConfig.SportDevNum 	= 2u;
#else
#error "processor not defined"
#endif
	SportConfig.SportDevMemSize	= ADI_SPORT_DMA_MEMORY_SIZE;
	SportConfig.pSportDevMem 	= &Adau1977SportMemory;
	SportConfig.eSportChnl	    = ADI_ADAU1977_SPORT_A;
	SportConfig.eSportPri	    = ADI_ADAU1977_SERIAL_PORT_DSDATA1;
	SportConfig.eSportSec	    = ADI_ADAU1977_SERIAL_PORT_DSDATA2;
	SportConfig.bLsbFirst		= false;

	result = adi_adau1977_ConfigSport(phAdau1977, &SportConfig);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigSport failed\n");
		return FAILURE;
	}

	/* ADC is a master source of SPORT clk and FS, MCLK 24.576 MHz and PLL used MCLK */
	result = adi_adau1977_ConfigPllClk(phAdau1977,
			LR_B_CLK_MASTER_1977,
			ADI_ADAU1977_MCLK_IN_FREQ_24576000HZ,
			ADI_ADAU1977_PLL_SOURCE_MCLK);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigPllClk failed\n");
		return FAILURE;
	}

	result = adi_adau1977_ConfigSerialClk(phAdau1977,
			BCLK_RISING_1977,
			LRCLK_HI_LO_1977);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigSerialClk failed\n");
		return FAILURE;
	}

	result = adi_adau1977_SetSampleRate(phAdau1977, ADI_ADAU1977_SAMPLE_RATE_192000HZ);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_SetSampleRate failed\n");
		return FAILURE;
	}

	result = adi_adau1977_SetWordWidth(phAdau1977, ADI_ADAU1977_WORD_WIDTH_16);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_SetWordWidth failed\n");
		return FAILURE;
	}

	result = adi_adau1977_RegisterCallback(phAdau1977, AdcCallback, NULL);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_RegisterCallback failed\n");
		return FAILURE;
	}

	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, false); //was true
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, false); //was true
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, false); //was true
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, false); //was true

	/* setup mic */
#if defined(ENABLE_MIC_BIAS)
	result = adi_adau1977_ConfigMic (phAdau1977,
			ADI_ADAU1977_MIC_BIAS_VOLT_8_5,
		    true,
		    true,
		    false,
		    true);

	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, 0x40u);
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, 0x40u);
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, 0x40u);
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, 0x40u);

#else
	result = adi_adau1977_ConfigMic (phAdau1977,
			ADI_ADAU1977_MIC_BIAS_VOLT_6_0,
		    false,
		    false,
		    false,
		    true);

	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, 0xa0u); //122
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, 0xa0u);
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, 0xa0u);
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, 0xa0u); //144u

	//Enable channel clipping
//	result = adi_adau1977_ClipChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, true);
//	result = adi_adau1977_ClipChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, true);
//	result = adi_adau1977_ClipChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, true);
//	result = adi_adau1977_ClipChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, true);


#endif

	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigMic failed\n");
		return FAILURE;
	}

	result = adi_adau1977_GetMicConfig (phAdau1977,
		    &ebias,
		    &bStat1,
		    &bStat2,
		    &bStat3);

	if (bStat1) {
		printf("\nMic bias: ON\n");
	} else {
		printf("\nMic bias: OFF\n");
	}

	for (i = DELAY_COUNT*1000u; i; i--)
	{
	}

	result = adi_adau1977_GetDCBoostStatus (phAdau1977,
	        &bStat1,
	        &bStat2,
	        &bStat3);

	if (bStat1) {
		printf("\nBoost good\n");
	} else {
		printf("\nBoost unstable\n");
	}

	return SUCCESS;
}

/*
 * ADC Callback.
 */
void AdcCallback(void *pCBParam, uint32_t nEvent, void *pArg)
{
	ADI_ADAU1977_RESULT eResult;
	uint16_t *pData;
	uint16_t tmp;
	uint32_t n;
	switch(nEvent)
	{
	case ADI_SPORT_EVENT_RX_BUFFER_PROCESSED:
        	/* Update callback count */
        	AdcCount++;

      		pData = pArg;

    		/* submit the ADC buffer */
		eResult = adi_adau1977_SubmitBuffer(phAdau1977, (void *) pArg, AUDIO_BUFFER_SIZE*BYTES_PER_SAMPLE);
		if (eResult != ADI_ADAU1977_SUCCESS)
		{
			bError = true;
		}

		/* copy single channel data to buffer */

		/***** A solution for the number of callbacks is to compare this variable with MAX_CALLBACKS. If not equal,
		 * then do not run the for loop as it is very inefficient */
		if(AdcCount==CALLBACK_COUNT){
				for (n=0u; (n<(NUM_SAMPLES)); n++)
				{
					Chan1Data[nSample] = *pData++;  /* primary slot1 */
					tmp =/*Chan3Data[nSample] = */*pData++;  /* secondary slot1 */

					Chan2Data[nSample] = *pData++;  /* primary slot2 */
					tmp =/*Chan4Data[nSample] =*/ *pData++;  /* secondary slot2 */

					//I must be doing something wrong here, or the buffer composition changes when I switch to 16 bits.
					//If the swith does not affect the buffer, then the data must be in the following order 1-3-2-4,1-3-2-4,...,1-3-2-4

					nSample++; //this happens because each channel buffer is declared with 16*N where N is the max number of samples
								//per channel. This seems very inefficient. They fill each channel buffer with 16x its actual capacity.
								//by the 17th callback each channel is overwritten. Why is this so?

					/*if (nSample == MAXDATA)	{ //MAXDATA is 16*N. why?
						nSample = 0u; // it is abusrd. the array is filled in its entirety 125 times. An each fill is 16 times its actual capacity.
					}
		*/
				}
		}

		break;
	default:
		bError = true;
		break;
	}

	return;
}

