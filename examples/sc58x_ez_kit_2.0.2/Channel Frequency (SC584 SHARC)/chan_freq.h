/*********************************************************************************

Copyright(c) 2015-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      chan_freq.h
 * @brief     Example to demonstrate the submission of DMA driven audio buffers
 *            to the ADC.
 * @version:  $Revision: 29599 $
 * @date:     $Date: 2017-05-04 10:37:52 -0400 (Thu, 04 May 2017) $
 *
 * @details
 *            This is the primary include file for ADC chan freq example that shows
 *            how to to submit DMA driven audio buffers to the ADC.
 *
 */

#ifndef __CHAN_FREQ_H__
#define __CHAN_FREQ_H__

/*=============  I N C L U D E S   =============*/

/*==============  D E F I N E S  ===============*/

#ifndef MACROS_DEFINED_ON_COMMAND_LINE
/* to enable MIC Bias uncomment this define */
//#define ENABLE_MIC_BIAS

#define ENABLE_DEBUG_INFO
#endif

/* Number of callbacks that occur before the program exits */
#define CALLBACK_COUNT  		(3u)

/* Macro to specify delay count for ADC/DAC reset */
#define DELAY_COUNT             (400000u)

/*
 * ADC settings
 */
/* ADAU1977 SPORT config parameters */
#define LR_B_CLK_MASTER_1977    (true)
#define BCLK_RISING_1977 	    (true)
#define LRCLK_HI_LO_1977 	    (true)

#define NUM_CHANNELS				(4u)
#define BYTES_PER_SAMPLE 			(2u)
#define NUM_SAMPLES 				(128u) //max 400K
#define ADC_CONV_F_24				330e-9 //ADC Conversion Factor
#define ADC_CONV_F_16				440.141e-6
/* Macro to set buffer size */
#define AUDIO_BUFFER_SIZE 	        (NUM_SAMPLES * NUM_CHANNELS)

#if defined(__ADSPBF707_FAMILY__) || defined(__ADSPSC589_FAMILY__)
/* SPU Peripheral ID */
#define	SPORT_5A_SPU_PID		    (ADI_SPU0_SPORT5A_ID)
#define	SPORT_5A_DMA12_SPU_PID		(ADI_SPU0_DMA12_ID)
#elif defined(__ADSPSC573_FAMILY__)
/* SPU Peripheral ID */
#define	SPORT_2A_SPU_PID		    (ADI_SPU0_SPORT2A_ID)
#define	SPORT_2A_DMA4_SPU_PID		(ADI_SPU0_DMA4_ID)
#endif

/* IF (Debug info enabled) */
#if defined(ENABLE_DEBUG_INFO)
#define DBG_MSG                     printf
#else
#define DBG_MSG(...)
#endif

#endif /* __CHAN_FREQ_H__ */
