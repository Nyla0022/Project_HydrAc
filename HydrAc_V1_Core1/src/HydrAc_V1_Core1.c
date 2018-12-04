/*****************************************************************************
 * HydrAc_V1_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>

#include <services/spu/adi_spu.h>
#include <services/gpio/adi_gpio.h>
#include <services/pwr/adi_pwr.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>

#include <drivers/adc/adau1977/adi_adau1977.h>
#include <drivers/twi/adi_twi.h>
#include <drivers/uart/adi_uart.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <filter.h>
#include <complex.h>


#include "adi_initialize.h"
#include "HydrAc_V1_Core1.h"


#define SAMPLES 4096
#define TAPS 411



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

/**
 * UART DEFINITIONS
 */

/* Rx/Tx Buffer size */
#define BUFFER_SIZE    	 	30

/*Angle No of Digits*/
#define ANGLE_SIZE 			10

/*Distance No of Digits*/
#define DIST_SIZE 			10

/*execution time No of Digits*/
#define EXECTIME_SIZE 		10

#define TOTAL_SAMPLES 		384000

/*=============  D A T A  =============*/


/**
 * 				ADC
 */

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

/* Memory required for the SPU operation */
static uint8_t  SpuMemory[ADI_SPU_MEMORY_SIZE];

/* SPU handle */
static ADI_SPU_HANDLE hSpu;

/*ADC buffers*/
ADI_CACHE_ALIGN static int16_t AdcBuf1[ADI_CACHE_ROUND_UP_SIZE(AUDIO_BUFFER_SIZE, int16_t)]; //Between 1MB and 2MB
ADI_CACHE_ALIGN static int16_t AdcBuf2[ADI_CACHE_ROUND_UP_SIZE(AUDIO_BUFFER_SIZE, int16_t)];

/*ADC channel buffers*/
static int16_t Chan1Data[NUM_SAMPLES]; //N
static int16_t Chan2Data[NUM_SAMPLES]; //approx in the middle of 256KB and 512KB



/**
 * 				Execution Time measurement
 */
static volatile clock_t clock_start;
static volatile clock_t clock_stop;
static volatile double exec_time;


/**
 * 				UART DEFINITIONS
 */


/* UART Handle */
static ADI_UART_HANDLE ghUART;

	/* Memory required for operating UART in interrupt mode */
static uint8_t gUARTMemory[ADI_UART_BIDIR_INT_MEMORY_SIZE];

	/* UART return code */
static ADI_UART_RESULT eResult;

	/* Flag which indicates whether to stop the program */
static bool bStopFlag = false;



/*
 * 				FILTERING DEFINITIONS
 */

/**LOW PASS FILTER COEFFICIENTS**/
static float HIGH_COEF[TAPS]=
	{
			#include "data/HIGH_4096.dat"
	};


	/**INPUT SIGNAL**/
static float channel_1[TOTAL_SAMPLES]=
	{
			#include "345_data/345_CH1.dat"
	};

static float channel_2[TOTAL_SAMPLES]=
	{
			#include "345_data/345_CH2.dat"
	};


static float out_ch_1[SAMPLES];
static float out_ch_2[SAMPLES];

#pragma align 512
	static float pm coeffs[TAPS]; /* coeffs array must be */
							/* initialized and in PM memory */

static float subset_channel_1 [SAMPLES];
static float subset_channel_2 [SAMPLES];

static float state[TAPS + 1];

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/

/* Initialize GPIO and reset peripherals */
uint32_t    GpioInit(void);
/* Initializes ADC */
uint32_t    Adau1977Init(void);

/*Initializes GPIO peripherals*/
void hydrac_gpio_init(void);
/*Initializes system ADC (ADAU19177)*/
void hydrac_adc_init(void);
/*Initializes system SPU*/
void hydrac_spu_init(void);
/*Initializes UART peripheral*/
void hydrac_uart_init(void);

/*Enable and open ADC*/
void hydrac_adc_enable(void);
/*Disable and close ADC*/
void hydrac_adc_disable(void);

/*Save data to text file*/
void save_chan_data_to_file(char*);

/*computes bearing angle*/
void hydrac_compute_angle(double tau, double* angle, uint8_t);

/*float to array*/
void ftoa(float, char*);


/*=============  C A L L B A C K    F U N C T I O N    P R O T O T Y P E S =============*/

/* ADC callback */
void AdcCallback(void *pCBParam, uint32_t nEvent, void *pArg);

/*=============  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S =============*/
/* Configures soft switches */
extern void ConfigSoftSwitches(void);



int main(int argc, char *argv[]){

	//
	//-----------LOCAL VARIABLES------------------//
	//

	uint32_t i = 0;

	/*Distance and angle variables*/
	float distance = .070, angle = -90;

	/* distance string for UART*/
	char dist_c[10] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	/* angle string for Angle*/
	char angle_c[10] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
	/* angle string for execution time*/
	char exectime_c[10] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

	/*Tx buffer */
	char TxBuffer[BUFFER_SIZE];


	int j = 0;
	int location_ch1 =0;
	int location_ch2=0;
	int loc_min_ch1 =0;
	int loc_max_ch1 =0;
	int loc_min_ch2 =0;
	int loc_max_ch2 =0;
	int loc_ch1_final = 0;
	int loc_ch2_final = 0;
	static int direction =0;


	for (int i = TAPS-1; i >= 0; i--) {
		coeffs[i] = HIGH_COEF[(TAPS-1)-i];
	}




	//
	//---------PERIPHERALS INITALIZATION--------------//
	//


	/* Initialize managed drivers and/or services that have been added to the project*/
	adi_initComponents();


	/* Software Switch Configuration for the EZ-Board */
	ConfigSoftSwitches();

	/*Initialize GPIO peripheral for HydrAc System*/
	hydrac_gpio_init();

	/*Initialize SPU peripheral for HydrAc System*/
	hydrac_spu_init();


	/*Initialize ADC for HydrAc System (ADAU 1977)*/
	hydrac_adc_init();

	/* Initialize UART */
	hydrac_uart_init();


	//
	//---------MAIN PROGRAM--------------//
	//


		//printf("SIGNALS LOADED\n");



	for (int i = 0; i < TOTAL_SAMPLES; i++)
	{
		if (channel_1[i] >  1.505)
		{
			location_ch1 = i;
			break;
		}
	}

	for (int i = 0; i < TOTAL_SAMPLES; i++)
	{
		if (channel_2[i] >  1.505)
		{
			location_ch2 = i;
			break;
		}
	}



	if (( location_ch2 > 2047) && ( location_ch2 > 2047))
	{
			j = 2047;
			for (i = 0; i < 2048; i++)
			{
				subset_channel_1[i] =  channel_1[location_ch1 - j];
				subset_channel_2[i] =  channel_2[location_ch1 - j];
				j--;
			}

			j = 1;
			for (i = 2048; i < SAMPLES; i++)
			{
				subset_channel_1[i] =  channel_1[location_ch1 + j];
				subset_channel_2[i] =  channel_2[location_ch1 + j];
				j++;
			}
		}
	else
	{
			for (i = 0; i < SAMPLES; i++)
			{
				subset_channel_1[i] =  channel_1[i];
				subset_channel_2[i] =  channel_2[i];
			}
		}


	for (i = 0; i < TAPS + 1; i++)
		state[i] = 0; /* initialize state array */

	fir (subset_channel_1, out_ch_1, coeffs, state, SAMPLES, TAPS);

	for (i = 0; i < TAPS + 1; i++)
		state[i] = 0; /* initialize state array */

	//filter second signal
	fir (subset_channel_2, out_ch_2,coeffs, state, SAMPLES, TAPS);

	printf("%e, %e filtered\n", out_ch_1[0], out_ch_2[0]);

	for (int i=0; i<1024; i++) {
			out_ch_1[i] = 0;
			out_ch_2[i] = 0;
		}


	for(i=0; i<SAMPLES; i++){
		if (out_ch_1[i] > 0.3)
		{
			loc_max_ch1 = i;
			break;
		}
	}

	for(i=0; i<SAMPLES; i++){
		if (out_ch_2[i] > 0.3)
		{
			loc_max_ch2 = i;
			break;
		}
	}

	for(i=0; i<SAMPLES; i++){
		if (out_ch_1[i] < -0.3)
		{
			loc_min_ch1 = i;
			break;
		}
	}

	for(i=0; i<SAMPLES; i++){
		if (out_ch_2[i] < -0.3)
		{
			loc_min_ch2 = i;
			break;
		}
	}


	if (loc_max_ch1 < loc_min_ch1)
		loc_ch1_final = loc_max_ch1;
	else
		loc_ch1_final = loc_min_ch1;

	if (loc_max_ch2 < loc_min_ch2)
		loc_ch2_final = loc_max_ch2;
	else
		loc_ch2_final = loc_min_ch2;

	if (loc_ch1_final < loc_ch2_final)
	{
		direction = 2;
		printf("SIGNAL IS COMING FROM THE RIGHT. \n");
	}
	else if (loc_ch1_final > loc_ch2_final)
	{
		direction = 1;
		printf("SIGNAL IS COMING FROM THE LEFT. \n");
	}
	else
	{
		direction = -1;
		printf("IN FRONT OR ERROR . \n");
	}


	printf("going into infinite loop...\n");



	uint32_t loop=1;
	volatile uint32_t loop=1;
	uint32_t m =0;

	while(false){


//
//		/*
//		 * Fill ADC Buffers
//		 */
//
//		/*Enable dataflow and Open the ADC*/
//		hydrac_adc_enable();
//
//		/*Disable dataflow and close the ADC*/
//		hydrac_adc_disable();


		/*Disable dataflow and close the ADC*/
		hydrac_adc_disable();



		/*
		 * Compute angle and distance
		 */



		/*
		 * Prepare data to send via UART
		 */

		//***NOTE time is in miliseconds****

		ftoa(angle, angle_c); 				//convert angle data to string array
		ftoa(distance, dist_c); 			//convert distance data to string array
		ftoa((float)(exec_time*1e3),exectime_c);	//convert execution time data to string array

		for (i = 0; i < sizeof(angle_c); i++)
			TxBuffer[i] = angle_c[i];

		for (i = 0; i < sizeof(dist_c); i++)
			TxBuffer[i + 10] = dist_c[i];

		for (i = 0; i < sizeof(exectime_c); i++)
			TxBuffer[i + 20] = exectime_c[i];

		/*
		 * Send data via UART
		 */

		/* Write the character */
		printf("Transmitting Result#%d: %s\n\n",loop++, TxBuffer);
		/*comment when interfacing to arduino*/
		eResult = adi_uart_Write(ghUART, "Transmitting Result", 20);
		/*comment when interfacing to arduino*/
		eResult = adi_uart_Write(ghUART, "\n",2);
		for(i=0;i<(BUFFER_SIZE+20)-2;i++)
			eResult = adi_uart_Write(ghUART, "\b",2);

		eResult = adi_uart_Write(ghUART, &TxBuffer[0], BUFFER_SIZE);

		/*comment when interfacing to arduino*/
		eResult = adi_uart_Write(ghUART, "\n",2);


		/*
		 * Save data to file
		 */
		//save_chan_data_to_file("t.txt");
		//save_chan_data_to_file("t.txt");





			//reset both channels
			for (m = 0; m < NUM_SAMPLES; m++) {
				Chan1Data[m] = 0;
				Chan2Data[m] =0;
			}

	}

	return 0;




		/* Close ADC device */
		if (adi_adau1977_Close(phAdau1977) != ADI_ADAU1977_SUCCESS)
		{
			bError = true;
			DBG_MSG("ADC close failed\n");
		}



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

	/*Configure Serial Clock*/
	result = adi_adau1977_ConfigSerialClk(phAdau1977,
			BCLK_RISING_1977,
			LRCLK_HI_LO_1977);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_ConfigSerialClk failed\n");
		return FAILURE;
	}

	/*Set ADC Sample Rate (192KHz)*/
	result = adi_adau1977_SetSampleRate(phAdau1977, ADI_ADAU1977_SAMPLE_RATE_192000HZ);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_SetSampleRate failed\n");
		return FAILURE;
	}

	/*Set ADC word width (in bits) */
	result = adi_adau1977_SetWordWidth(phAdau1977, ADI_ADAU1977_WORD_WIDTH_16);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_SetWordWidth failed\n");
		return FAILURE;
	}

	/*Register the callback function that will notify when the ADC acquisition  data is ready*/
	result = adi_adau1977_RegisterCallback(phAdau1977, AdcCallback, NULL);
	if (result != ADI_ADAU1977_SUCCESS)
	{
		DBG_MSG("ADAU1977: adi_adau1977_RegisterCallback failed\n");
		return FAILURE;
	}

	/* Configure ADC high pass filters*/
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, false); //input has a DC voltage offset
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, false); //input has a DC voltage offset
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, true);
	result = adi_adau1977_HighPassChannel (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, true);



	/*Configure ADC microphone bias */
	result = adi_adau1977_ConfigMic (phAdau1977,
			ADI_ADAU1977_MIC_BIAS_VOLT_6_0,
		    false,
		    false,
		    false,
		    true); //disable bias

	/*Configure Post-ADC gain*/

	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL1, 0xa0u); //0dB
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL2, 0xa0u); //0dB
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL3, 0xa0u); //0dB
	result = adi_adau1977_SetVolume (phAdau1977, ADI_ADAU1977_AUDIO_CHANNEL4, 0xa0u); //0dB


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

	switch(nEvent){
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

		/* Copy single channel data to audio channel buffer */
		if (AdcCount == CALLBACK_COUNT) //Only enter if reached the last callback
			for (n = 0u; n < (NUM_SAMPLES); n++) {

				Chan1Data[n] = *pData++; /* primary slot1 */
				tmp = *pData++; /* secondary slot1 */
				Chan2Data[n] = *pData++; /* primary slot2 */
				tmp = *pData++; /* secondary slot2 */

				//data is in the following order 1-3-2-4,1-3-2-4,...,1-3-2-4 and repeats 4N times
			}

		break;
	default:
		bError = true;
		break;
	}

	return;
}


/*
 * Initialize HydrAc ADC Module
 */
void hydrac_adc_init(void){
	uint32_t Result = SUCCESS;

	/* Initialize ADAU1977 */
	Result = Adau1977Init();
	if (Result == FAILURE)
	{
		bError = true;
		DBG_MSG("ADAU1977 Init failed\n");
	}
}

/*
 * Enable HydrAc ADC data flow
 */
void hydrac_adc_enable(void){

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


    clock_start = clock(); //start counting cycles

	printf("DATA FLOW ENABLED! processing callbacks...\n");


	/* process samples and then exit */
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

	AdcCount =0u;
    clock_stop = clock();

	AdcCount=0; //reset Adc Counts


    clock_stop = clock(); //stop counting cycles


    /*compute execution time*/
    exec_time = 2*((double) (clock_stop - clock_start))
           / CLOCKS_PER_SEC; //used hrm and schematic to discover that the CLK is 25MHz
    	//then it is divided by two.


    printf("Time taken is %e seconds\n",exec_time);

	printf("ALL CALLBACKS PROCESSED!...\n");
}


/*
 *  Disable HydrAc ADC module data flow
 */
void hydrac_adc_disable(void){

	uint32_t m=0;


	/* Disable ADC data flow */
	if(adi_adau1977_Enable(phAdau1977, false) != ADI_ADAU1977_SUCCESS)
	{
		bError = true;
		DBG_MSG("ADC disable failed\n");
	}


}

/*
 * Initialize DSP GPIO peripherals
 */
void hydrac_gpio_init(void){
	uint32_t Result = GpioInit();
	if (Result == FAILURE)
	{
		bError = true;
		DBG_MSG("GPIO Init failed\n");
	}

}


/*
 * Initialize DSP SPU Peripheral
 */
void hydrac_spu_init(void){
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

}

/*
 * Save Channel data to Text File
 */
void save_chan_data_to_file(char* filename){
	uint32_t m = 0;
	double time=0;
	FILE * fp;

	/*copy acquisition data into a file*/
	printf("\n");
	fp = fopen(filename, "w+"); //open file

	fprintf(fp, "Time[s]\tChan 1[v]\tChan 2[v]\n");

	//save data for both channels and execution time
	for (m = 0; m < NUM_SAMPLES; m++) {
		fprintf(fp, "%f\t%f\t%f\n", (double) time,
				(double) ((int) Chan1Data[m] * ADC_CONV_F_16),
				(double) ((int) Chan2Data[m] * ADC_CONV_F_16));
		time = (double) (time + TIME_STEP);
	}

	fprintf(fp, "\n****Exec time:%f\n", exec_time);

	fclose(fp); //close file

	printf("data written to file\n");
}

/*
 * Compute Bearing Angle
 */
void hydrac_compute_angle(double tau, double* angle, uint8_t dir){
	double percentage=0;
	double b=0;
	/*check if tau does not exceed the maximum value*/
	if(tau <= MAX_TAU && tau >= (-1*MAX_TAU)){
		*angle = 180/PI*asin((SOUND_CONST*tau)/H_DIST);
		//b = 90 - angle;

		//check direction of arrival
		//continue Jan's code...
	}else
		*angle = 90;

	/*check if angle is negative (left side)*/
	if(!dir || tau<0)
		*angle = -1*(*angle); //left

}


/*
 * Convert float data type to Array
 */
void ftoa(float f, char *buf) {
	int pos = 0, ix, dp, num;
	if (f < 0) {
		buf[pos++] = '-';
		f = -f;
	}
	dp = 0;
	while (f >= 10.0) {
		f = f / 10.0;
		dp++;
	}
	for (ix = 1; ix < 8; ix++) {
		num = f;
		f = f - num;
		if (num > 9)
			buf[pos++] = '#';
		else
			buf[pos++] = '0' + num;
		if (dp == 0)
			buf[pos++] = '.';
		f = f * 10.0;
		dp--;
	}
}

/*
 * Initialize HydrAc UART module
 */

void hydrac_uart_init(){

	/*Power up UART module*/
	if (adi_pwr_Init(0u, UART_CLKIN) != ADI_PWR_SUCCESS) {
		DBG_MSG("Failed to initialize power service\n");
		bError= FAILURE;
	}

	/* Open UART driver */
	eResult = adi_uart_Open(UART_DEVICE_NUM, ADI_UART_DIR_BIDIRECTION,
			gUARTMemory,
			ADI_UART_BIDIR_INT_MEMORY_SIZE, &ghUART);
	if (eResult != ADI_UART_SUCCESS) {
		DBG_MSG("Could not open UART Device 0x%08X\n", eResult);
		bError= FAILURE;
	}

	/* Set the UART Mode */
	eResult = adi_uart_SetMode(ghUART, ADI_UART_MODE_UART);
	if (eResult != ADI_UART_SUCCESS) {
		DBG_MSG("Could not set the Mode 0x%08X\n", eResult);
		bError= FAILURE;
	}

	/* Set UART Baud Rate */
	eResult = adi_uart_SetBaudRate(ghUART, BAUD_RATE);
	if (eResult != ADI_UART_SUCCESS) {
		DBG_MSG("Could not set the Baud Rate 0x%08X\n", eResult);
		bError= FAILURE;
	}

	/* Set number of stop bits */
	eResult = adi_uart_SetNumStopBits(ghUART, ADI_UART_ONE_STOPBIT);
	if (eResult != ADI_UART_SUCCESS) {
		DBG_MSG("Could not set the stop bits 0x%08X\n", eResult);
		bError= FAILURE;
	}

	/* Set number of stop bits */
	eResult = adi_uart_SetWordLen(ghUART, ADI_UART_WORDLEN_8BITS);
	if (eResult != ADI_UART_SUCCESS) {
		DBG_MSG("Could not set word length 0x%08X\n", eResult);
		bError= FAILURE;
	}

}
