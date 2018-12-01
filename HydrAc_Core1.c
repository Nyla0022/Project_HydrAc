/*****************************************************************************
 * HydrAc_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "HydrAc_Core1.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <filter.h>
#include <complex.h>
#define ELEMENT_COUNT(X) (sizeof(X) / sizeof((X)[0]))


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
	
	/* Begin adding your custom code here */
	static float Channel_1[1024]=
	{
			#include "signals/75_CH1.dat"
	};

	//*Signal Acquisition from Hydrophone B *
	static float Channel_2[1024]=
	{
			#include "signals/75_CH2.dat"
	};

	//*Signal Acquisition from Hydrophone B *
	static float COEF[1024]=
	{
			#include "signals/HIGH_COEF.dat"
	};

  float Channel_1_F[1024];
  float Channel_2_F[1024];

  convolution(Channel_1, ELEMENT_COUNT(Channel_1), COEF, ELEMENT_COUNT(COEF), Channel_1_F);
  convolution(Channel_2, ELEMENT_COUNT(Channel_2), COEF, ELEMENT_COUNT(COEF), Channel_2_F);

	float MAX_CH1 = 0.0025;
	float MIN_CH1 = -0.0025;

	int INDEX_CH1 = 0;
	int INDEX_CH2 = 0;


	for(int i=0; i<1024; i++)
	{
		if(Channel_1_F[i] > MAX_CH1)
		{
			INDEX_CH1 = i;
			break;
		}
	}

	for(int i=0; i<1024; i++)
	{
		if(Channel_2_F[i] > MAX_CH1)
		{
			INDEX_CH2 = i;
			break;
		}
	}


//	for(int i=0; i<384000; i++)
//	{
//		printf("%f", Channel_1[i]);
//	}


//	if(INDEX_CH1 > INDEX_CH2)
//		printf("SIGNAL IS COMING FROM THE LEFT. \n");
//	else if (INDEX_CH1 < INDEX_CH2)
//		printf("SIGNAL IS COMING FROM THE RIGHT. \n");
//	else
//		printf("you worked!");

	int  m =7;

	printf("m=%d\n",m);


	return 0;
}

