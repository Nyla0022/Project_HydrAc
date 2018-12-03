/*********************************************************************************

Copyright(c) 2011-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.
*********************************************************************************/

/*****************************************************************************
 * MCAPI_remote_printf example
 *
 *    This example uses the MCAPI inter-core communication mechanism to
 *    allow core 1 to send logging or error strings to an output
 *    stream which is managed by core 0.
 *****************************************************************************/

/*********************************************************************************
 * ShutdownService.c
 *********************************************************************************/

#include <sys/platform.h>
#include <stdio.h>
#include "MCAPI_remote_printf_Node0.h"
#include "GenericServices.h"
#include "ShutdownService.h"

/* This buffer is declared static as it is used as the destination of a
 * non-blocking mcapi_msg_recv_i() call, and must therefore be persistent.
 *
 */
static char s_vShutdownStringBuffer[64];

/* doShutdownService()
 *
 * This function is called whenever a message is received on the associated
 * MCAPI endpoint.
 */
static void
doShutdownService(
		char *pBuffer,
		size_t bufferSize,
		size_t receivedSize,
		enum ServiceIndex serviceIndex
	)
{
	/* This code simply prints the received string to stdio, but can be
	 * replaced, in a real application, with code to write the string to
	 * a UART or to a log file.
	 */
	puts(s_vShutdownStringBuffer);
	fflush(stdout);
	exit(0);
}

/* StartShutdownService()
 *
 * This function registers the printf service with the generic service
 * dispatcher, using the specified MCAPI port number and service index
 * number. The FILE * argument 'fd' is stored for use by doPrintfService().
 */
mcapi_status_t StartShutdownService(
		mcapi_port_t port
	)
{
	return StartService(s_vShutdownStringBuffer,
						sizeof(s_vShutdownStringBuffer),
						port,
						&doShutdownService,
						SHUTDOWN_SERVICE);
}

