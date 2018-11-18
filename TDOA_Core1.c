/*****************************************************************************
 * TDOA_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "TDOA_Core1.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <filter.h>
#include <complex.h>
#define N_FFT 1024
#define ELEMENT_COUNT(X) (sizeof(X) / sizeof((X)[0]))

/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
char __argv_string[] = "";
/** Convolution function for filtering **/

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
	
	/*Begin adding your custom code here */

	/**.............SIGNAL AQUISITION..............**/
	/**This chunk of code is in charge of reading
	 * data from a specified directory in our IDE.
	 * For our implementation, we read floating
	 * point values of our signals A and B gene-
	 * rated in our simulation. We also read our
	 * filter coefficients for the FIR implemen-
	 * tation. These values are store in arrays.
	 */

/*	IDEAL CASE  */
// // *Signal Acquisition from Hydrophone A *
//	float InputA[1024]=
//	{
//			#include "signals/signal_x.dat"
//	};
//
//	//*Signal Acquisition from Hydrophone B *
//	float InputB[1024]=
//	{
//			#include "signals/signal_y.dat"
//	};

///*	RECIEVED FROM DIFFERENT HYDROPHONE
// * *Signal Acquisition from Hydrophone A */
//	float InputA[1024]=
//	{
//			#include "signals/signal_y.dat"
//	};
//
//	//*Signal Acquisition from Hydrophone B *
//	float InputB[1024]=
//	{
//			#include "signals/signal_x.dat"
//	};

/*	 GREATER TDOA
	*Signal Acquisition from Hydrophone A *
	float InputA[1024]=
	{
			#include "signals/x_1024_EXTREMES.dat"
	};

	*Signal Acquisition from Hydrophone B *
	float InputB[1024]=
	{
			#include "signals/y_1024_EXTREMES.dat"
	};*/

	/* CLIPPING */
/*	*Signal Acquisition from Hydrophone A *
	float InputA[1024]=
	{
			#include "signals/x_1024_CLIP.dat"
	};

	*Signal Acquisition from Hydrophone B *
	float InputB[1024]=
	{
			#include "signals/y_1024_CLIP.dat"
	};*/

/*	NOISY
  Signal Acquisition from Hydrophone A*/
	float InputA[1024]=
	{
			#include "signals/x_1024_NOISY.dat"
	};

	/*Signal Acquisition from Hydrophone B*/
	float InputB[1024]=
	{
			#include "signals/y_1024_NOISY.dat"
	};

///*	Q1
//  Signal Acquisition from Hydrophone A*/
//	float InputA[1024]=
//	{
//			#include "signals/x_1024_Q1.dat"
//	};
//
//	/*Signal Acquisition from Hydrophone B*/
//	float InputB[1024]=
//	{
//			#include "signals/y_1024_Q1.dat"
//	};

//	/*Q2
//  /*Signal Acquisition from Hydrophone A*/
//	float InputA[1024]=
//	{
//			#include "signals/x_1024_Q2.dat"
//	};
//
//	/*Signal Acquisition from Hydrophone B*/
//	float InputB[1024]=
//	{
//			#include "signals/y_1024_Q2.dat"
//	};

///*	Q3
//  Signal Acquisition from Hydrophone A*/
//	float InputA[1024]=
//	{
//			#include "signals/x_1024_Q3.dat"
//	};
//
//	//Signal Acquisition from Hydrophone B
//	float InputB[1024]=
//	{
//			#include "signals/y_1024_Q3.dat"
//	};



	float Coeff[1024]=
	{
			#include "signals/coefficients.dat"
	};


//	float Coeff[1024]=
//	{
//			#include "signals/COEF_1024_Q1.dat"
//	};



//	float Coeff[1024]=
//	{
//			#include "signals/COEF_1024_Q2.dat"
//	};


//	float Coeff[1024]=
//	{
//			#include "signals/COEF_1024_Q3.dat"
//	};


	float FilteredA[1024];
	float FilteredB[1024];

	printf("Signals Acquired! \n");

	/** We convolve our acquired signals with filte coeficients generated in MATLAB
	 */
	convolution(InputA, ELEMENT_COUNT(InputA), Coeff, ELEMENT_COUNT(Coeff), FilteredA);
	convolution(InputB, ELEMENT_COUNT(InputB), Coeff, ELEMENT_COUNT(Coeff), FilteredB);


	printf("Signals Filtered Successfully! \n");

	/** The following code uses the DSP's libraries to change from time domain to frequency domain**/
	complex_float outputA[N_FFT];
	float temp[N_FFT];
	float *r_temp = (float *) temp;

	static complex_float pm twiddle[N_FFT/2];
	int twiddle_stride = 1;

	/* Populate twiddle table */
	twidfft(twiddle, N_FFT);

	/* Compute Fast Fourier Transform */
	rfft(FilteredA, r_temp, outputA, twiddle, twiddle_stride, N_FFT);


	for(int i=1; i<(N_FFT/2); i++)
	{
		outputA[N_FFT - i] = conjf (outputA[i]);
	}

	complex_float outputB[N_FFT];
	float tempB[N_FFT];
	float *r_tempB = (float *) tempB;



	/* Compute Fast Fourier Transform */
	rfft(FilteredB, r_tempB, outputB, twiddle, twiddle_stride, N_FFT);

	complex_float Correlated[N_FFT];

	printf("Fast Fourier Transform applied successfully ! \n");

	/**Cross Correlation is applied using term-by-term multiplication**/
    for (int i = 0; i < N_FFT; i++)
    {
    	Correlated[i].re = outputA[i].re * outputB[i].re - outputA[i].im * outputB[i].im;
    	Correlated[i].im = outputA[i].re * outputB[i].im + outputA[i].im * outputB[i].re;
    }

    printf("Signals Correlated  ! \n");

    /**the following code is to apply inverse fourier transform**/

	#pragma align 1024
	static float dm input_r[N_FFT];
	#pragma align 1024
	static float pm input_i[N_FFT];

	for (int i = 0 ; i<1024 ; i++)
	{
		input_r[i] = Correlated[i].re;
		input_i[i] = Correlated[i].im;
	}


	complex_float output[N_FFT];
	complex_float temp2[N_FFT];

	/* Populate twiddle table */
	twidfft(twiddle, N_FFT);
	/* Compute Fast Fourier Transform */
	ifft(Correlated, temp2, output, twiddle, twiddle_stride, N_FFT);

	printf("Inverse Fast Fourier Transform applied successfully ! \n");

	complex_float maximum;
	int location =0;

	maximum = output[0];

	/**Maximum value of correlated signals**/
	for (int i = 1; i < N_FFT; i++)
	{
		if (maximum.re < output[i].re)
		{
			maximum = output[i];
			location = i + 1;
		}
	}

	/**TDOA estimation**/
	float TDOA;
	int zero = 745;
	float LAG = location;
	LAG = LAG - zero;

	TDOA = LAG / (float)160000;

	printf("ESTIMATED TDOA: %e s \n", TDOA);



	return 0;
}


