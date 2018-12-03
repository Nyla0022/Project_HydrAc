/*********************************************************************************

Copyright(c) 2014-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*****************************************************************************
 * MCAPI_remote_printf_Node1.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include <stdio.h>
#include <services/mcapi/mcapi.h>

#include "../../../../src_node0/MCAPI_remote_printf_Node0.h"
#include "RemotePrintf.h"
#include "RemoteShutdown.h"


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
    mcapi_endpoint_t loggingEndpoint;
    mcapi_endpoint_t shutdownEndpoint;
    int loopCount;

	/* Initialize managed drivers and/or services */
	if ( 0 != adi_initComponents() )
	{
		printf("[NODE1]: MCAPI_remote_printf: Error initializing components\n");
	}

	/* Begin adding your custom code here */
	/* Query the endpoint for the printf service */
    loggingEndpoint = mcapi_endpoint_get(DOMAIN,
    									NODE_CORE_0,
    									PRINTF_PORT,
    									MCAPI_TIMEOUT_INFINITE,
    									&mcapi_status);
	mcapiErrorCheck(mcapi_status, "getting PRINTF_PORT endpoint", 2);

	/* Query the endpoint for the shutdown service */
	shutdownEndpoint = mcapi_endpoint_get(DOMAIN,
    									  NODE_CORE_0,
    									  SHUTDOWN_PORT,
    									  MCAPI_TIMEOUT_INFINITE,
    									  &mcapi_status);
	mcapiErrorCheck(mcapi_status, "getting SHUTDOWN_PORT endpoint", 2);

	/* Main processing loop
	 */
	for(loopCount = 0; loopCount <= 10; ++loopCount)
	{
		mcapi_status = RemotePrintf(loggingEndpoint, "Hello World: loopCount = %d", loopCount);
		mcapiErrorCheck(mcapi_status, "during mcapiPrintf() processing", 3);
	}

	/* Now we tell the server side to shut down, before also returning from main().
	 */
	mcapi_status = RemoteShutdown(shutdownEndpoint, "All done");
	mcapiErrorCheck(mcapi_status, "during remoteShutdown() processing", 3);

	return 0;
}

