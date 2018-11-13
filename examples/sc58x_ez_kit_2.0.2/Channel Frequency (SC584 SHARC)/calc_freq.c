/*********************************************************************************

Copyright(c) 2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

#include <sys/platform.h>

uint32_t detectFreq(int32_t inSignal[]);

#if defined(__ADSPSHARC__)

#include <filter.h>
#include <math.h>
#include <stdint.h>

#define FFTSIZE    256u
#define THRESHOLD  40.0f
#define SAMPFREQ   192000u

uint32_t detectFreq(int32_t inSignal[])
{
	uint32_t n;
	float mag;
	uint32_t freq;
	uint32_t peak;

	complex_float r_output[FFTSIZE]; /* FFT of input signal */
	float r_temp[FFTSIZE];
	/* The following must be static so that it can be declared in pm memory */
	static complex_float __pm twiddle_table[FFTSIZE/2u];

	float spectrum[FFTSIZE];
	float sig[FFTSIZE];

	for (n=0u; n<FFTSIZE; n++)
	{
		sig[n] = (float)inSignal[n];
	}

	/* Initialize the twiddle table */
	twidfft (twiddle_table, (int32_t)FFTSIZE);

	/* Calculate the FFT of a real signal */
	rfft (sig, r_temp, r_output, twiddle_table, 1, (int32_t)FFTSIZE);

	fft_magnitude (r_output, spectrum, (int32_t)FFTSIZE, 2);

	freq = 0u;
	peak = 0u;
	for (n=2u; n<FFTSIZE/2u; n++) /* skip first bins (DC) */
	{
		mag = 10.0f*log10f(spectrum[n]);
		if (mag > THRESHOLD)
		{
			if (mag > (float)peak)
			{
				freq = n*(SAMPFREQ/FFTSIZE);
				peak = (uint32_t)mag;
			}
		}
	}

	return freq;
}
#else
uint32_t detectFreq(int32_t inSignal[])
{
	/* not supported on cortex A5 */
	return 0u;
}

#endif

