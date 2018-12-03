/*********************************************************************************

Copyright(c) 2014-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*****************************************************************************
 * MCAPI_remote_printf_Node0.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include "adi_initialize.h"
#include "MCAPI_remote_printf_Node0.h"

#include <services/mcapi/mcapi.h>
#include <stdio.h>

#include "GenericServices.h"
#include "PrintfService.h"
#include "ShutdownService.h"

/* mcapiErrorCheck()
 *
 * This function checks the status code, and if it is not MCAPI_SUCCESS then
 * an error message is output and the program exits. This should typically
 * be called after every MCAPI call which returns a status, during development
 * and debugging at least.
 */
static void mcapiErrorCheck(mcapi_status_t mcapi_status, const char *psContext, int result)
{
	char errorStringBuff[64];

	if (MCAPI_SUCCESS != mcapi_status)
	{
		mcapi_display_status(mcapi_status, errorStringBuff, sizeof(errorStringBuff));
		printf("MCAPI Error %s, status = %d [%s]\n",
				psContext,
				mcapi_status,
				errorStringBuff);
		exit(result);
	}
}


int main(void)
{
	mcapi_status_t mcapi_status;

	/* Initialize managed drivers and/or services */
	if ( 0 != adi_initComponents() )
	{
		printf("[NODE0]: MCAPI_remote_printf: Error initializing components\n");
	}

	/**
	 * The default startup code does not include any functionality to allow
	 * core 0 to enable core 1. A convenient way to enable core 1 is to use the
	 * 'adi_core_1_enable' function. 
	 * This example can easily be ported to other processors so the core initialization
	 * is here for several of them
	 */

#if defined(__ADSPBF609__)
 adi_core_1_enable();
#elif defined(__ADSPBF561__)
 adi_core_b_enable();
#elif defined(__ADSP215xx__)

   adi_core_enable(ADI_CORE_SHARC0);
   adi_core_enable(ADI_CORE_SHARC1);
#else
#error "Processor not supported"
#endif

	/* Start the printf service */
    mcapi_status = startPrintfService(PRINTF_PORT);
	mcapiErrorCheck(mcapi_status, "while starting printf service", 2);

	/* Start the shutdown service */
    mcapi_status = StartShutdownService(SHUTDOWN_PORT);
	mcapiErrorCheck(mcapi_status, "while starting shutdown service", 3);

	/* Main processing loop
	 */
	do
	{
		/* The application's main processing goes here. It should
		 * not wait indefinitely on anything, but should continue
		 * around the loop at intervals, so as to give the MCAPI-based
		 * an opportunity to run.
		 */

		/* Now we give the services an opportunity to run. If there is
		 * no incoming message activity then this call will return
		 * rather than blocking.
		 */
		mcapi_status = RunServices();
		mcapiErrorCheck(mcapi_status, "during service processing", 4);

	} while (MCAPI_SUCCESS == mcapi_status);


	/* Begin adding your custom code here */

	return 0;
}

