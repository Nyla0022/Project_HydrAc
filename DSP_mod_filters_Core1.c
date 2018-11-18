/*****************************************************************************
 * DSP_mod_filters_Core1.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <filter.h>
#include <complex.h>
#define ELEMENT_COUNT(X) (sizeof(X) / sizeof((X)[0]))
#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "DSP_mod_filters_Core1.h"
#define SAMPLES 1024

/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
char __argv_string[] = "";

/**
 * This convolution function was obtained by the following
 * source: https://stackoverflow.com/questions/8424170/1d-linear-convolution-in-ansi-c-code
 * It calculates linear convolution
 */
void convolution(const float Signal[/* SignalLen */], size_t SignalLen,
              const float Kernel[/* KernelLen */], size_t KernelLen,
              float Result[ SignalLen + KernelLen - 1 ])
{
  size_t n;

  for (n = 0; n < SignalLen + KernelLen - 1; n++)
  {
    size_t kmin, kmax, k;

    Result[n] = 0;

    kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
    kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

    for (k = kmin; k <= kmax; k++)
    {
      Result[n] += Signal[k] * Kernel[n - k];
    }
  }

}

int main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	
	/**.............SIGNAL AQUISITION..............**/
	/**This chunk of code is in charge of reading
	 * data from a specified directory in our IDE.
	 * For this implementation we read coeficient
	 * values of filters generated in MATLAB. We
	 * also read input signals and expected values,
	 * also generated in MATLAB
	 */
	/**BAND PASS FILTER COEFFICIENTS**/
	float BAND_COEF[1024]=
	{
			#include "data/BAND_COEF.dat"
	};

	/**HIGH PASS FILTER COEFFICIENTS**/
	float HIGH_COEF[1024]=
	{
			#include "data/HIGH_COEF.dat"
	};

	/**LOW PASS FILTER COEFFICIENTS**/
	float LOW_COEF[1024]=
	{
			#include "data/LOW_COEF.dat"
	};

	/**EXPECTED FILTERED VALUES - BANDPASS**/
	float x_filtered_bandpass[1024]=
	{
			#include "data/x_filtered_bandpass.dat"
	};

	/**EXPECTED FILTERED VALUES - HIGH**/
	float x_filtered_high[1024]=
	{
			#include "data/x_filtered_high.dat"
	};

	/**EXPECTED FILTERED VALUES - LOW**/
	float x_filtered_low[1024]=
	{
			#include "data/x_filtered_low.dat"
	};

	/**INPUT SIGNAL**/
	float x_FILTER[1024]=
	{
			#include "data/x_FILTERS.dat"
	};

	/**THERE VARIABLES ARE FOR STORING CONVOLUTED
	 * SIGNALS IN CODE
	 */
	float x_calc_low_con[1024];
	float x_calc_high_con[1024];
	float x_calc_bandpass_con[1024];
	/**
	 * THESE VARIABLE STORE AVERAGE DIFFERENACE
	 * BETWEEN CALCULATED AND EXPECTED VALUES
	 */
	float low_p =0;
	float high_p=0;
	float bp_p=0;
	float temp=0;


	printf("Signals Acquired! \n");

	/**
	 * FILTERING USING CONVOLUTION
	 */
	convolution(x_FILTER, ELEMENT_COUNT(x_FILTER), LOW_COEF, ELEMENT_COUNT(LOW_COEF), x_calc_low_con);
	convolution(x_FILTER, ELEMENT_COUNT(x_FILTER), HIGH_COEF, ELEMENT_COUNT(HIGH_COEF), x_calc_high_con);
	convolution(x_FILTER, ELEMENT_COUNT(x_FILTER), BAND_COEF, ELEMENT_COUNT(BAND_COEF), x_calc_bandpass_con);

	/**
	 * THE FOLLOWING FOR LOOPS ARE FOR ERROR CALCULATION
	 */
	for (int i=0; i<SAMPLES; i++)
	{
		temp = abs( x_filtered_low[i] - x_calc_low_con[i]);
		low_p = low_p + temp;
	}

	for (int i=0; i<SAMPLES; i++)
	{
		temp = abs(x_filtered_high[i] - x_calc_high_con[i]);
		high_p = high_p + temp;
	}


	for (int i=0; i<SAMPLES; i++)
	{
		temp = abs(x_filtered_bandpass[i] - x_calc_bandpass_con[i]);
		bp_p = bp_p + temp;
	}

	/**CALCULATING AVERAGE**/
	low_p = (low_p) / (float)1024 ;
	high_p = (high_p)/ (float)1024 ;
	bp_p = (bp_p)/ (float)1024;

	printf("AVERAGE DIFFERENCE BETWEEN VALUES (LOW): %e \n", low_p);
	printf("AVERAGE DIFFERENCE BETWEEN VALUES (HIGH): %e \n", high_p);
	printf("AVERAGE DIFFERENCE BETWEEN VALUES (BANDPASS): %e \n", bp_p);

	for (int i=0; i<SAMPLES; i++)
	{
		printf("EXPECTED: %e     GOT: %e \n", x_filtered_low[i], x_calc_low_con[i]);
	}

	return 0;
}

