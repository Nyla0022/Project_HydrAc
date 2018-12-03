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
 * RemoteShutdown.c
 *********************************************************************************/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <services/mcapi/mcapi.h>

#include "RemoteShutdown.h"

/* RemoteShutdown()
 *
 * This function instructs the server (referenced by the endpoint argument)
 * to output a message (specified by the format string and argument list)
 * and then exit.
 *
 * The implementation of this function is similar to the "blocking" version
 * of RemotePrintf(), which is given as an alternative in RemotePrintf.c
 * Since RemoteShutdown() will only be called once the greater complication
 * of a non-blocking implementation is unnecessary.
 */
mcapi_status_t RemoteShutdown(
		mcapi_endpoint_t serverEndpoint,
		char *psFormatString,
		...
)
{
	mcapi_status_t status;
	va_list argPtr;
	char vBuffer[64];
	mcapi_endpoint_t clientEndpoint;

	/* Process the format string and argument list into the string buffer */
	va_start(argPtr, psFormatString);
	vsnprintf (vBuffer, sizeof(vBuffer), psFormatString, argPtr);

	/* Create the anonymous endpoint that will be used as the source for the
	 * message send, below. The endpoint is only used temporarily and will be
	 * released (i.e. deleted) before mcapiPrintf() returns.
	 */
	clientEndpoint = mcapi_endpoint_create(MCAPI_PORT_ANY, &status);

	if (MCAPI_SUCCESS == status)
	{
		mcapi_status_t deleteStatus;

		/* Send the buffer contents to the destination endpoint. This call
		 * will not return until MCAPI has finished accessing vBuffer.
		 */
		mcapi_msg_send(clientEndpoint, serverEndpoint, vBuffer, 1 + strlen(vBuffer), 0, &status);

		/* Release the temporary anonymous endpoint.
		 * Because we're already returning the error code from the send
		 * we don't overwrite it with the delete's status, and since any
		 * error reported by the delete is secondary we don't need to
		 * check it.
		 */
		mcapi_endpoint_delete(clientEndpoint, &deleteStatus);
	}

	return status;
}
