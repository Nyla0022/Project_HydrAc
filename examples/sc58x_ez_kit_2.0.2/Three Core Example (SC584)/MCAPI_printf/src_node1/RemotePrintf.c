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
 * RemotePrintf.c
 *********************************************************************************/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <services/mcapi/mcapi.h>

#include "RemotePrintf.h"

#ifdef BLOCKING

/* RemotePrintf()
 *
 * This function is similar to fprintf(), except that the first argument is
 * an MCAPI endpoint ID. This ID references a "server", on a different node
 * or on the same node, which will output the specified text to some form
 * of output stream.
 *
 * This allows multiple clients, possibly on different nodes (cores), to print to
 * a shared and unified output log.
 *
 * This is a straightforward implementation of RemotePrintf(), using blocking
 * MCAPI message sends. The message test buffer (vBuffer) is an automatic variable,
 * allocated on the stack, and the use of the blocking form of the MCAPI send
 * ensures that MCAPI has finished accessing the buffer before RemotePrintf() returns.
 * However, this does mean that RemotePrintf() does not return until the send
 * operation has completed (i.e. until the message has been received and responded
 * to by the destination).
 *
 * This implementation would be most suitable for use in a multithreaded application,
 * as other threads can run while the RemotePrintf() call is blocking, and the use
 * of an on-stack buffer avoids contention between threads for the use of a static
 * buffer.
 */
mcapi_status_t RemotePrintf(
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

#else

/* RemotePrintf()
 *
 * This function is similar to fprintf(), except that the first argument is
 * an MCAPI endpoint ID. This ID references a "server", on a different node
 * or on the same node, which will output the specified text to some form
 * of output stream.
 *
 * This allows multiple clients, possibly on different nodes (cores), to print to
 * a shared and unified output log.
 *
 * This is an alternative implementation of RemotePrintf(), which uses non-blocking
 * MCAPI message sends. This version is slightly more complicated, and uses more memory,
 * but has the advantage that it will not block waiting for the current operation to
 * complete, instead the operation continues independently. However, it *will* block
 * if the *previous* RemotePrintf() operation has not completed yet.
 *
 * This implementation would be most suitable for use in a singlethreaded application
 * (i.e. no RTOS), as the delay for the message send to complete can be overlapped with
 * other work, and because with only one thread there is no contention for the use of
 * the static buffer.
 */
mcapi_status_t RemotePrintf(
		mcapi_endpoint_t serverEndpoint,
		char *psFormatString,
		...
)
{
	mcapi_status_t status;
	va_list argPtr;
	size_t dummy_size;

	/* The following variables are declared static as their contents must persist
	 * between calls.
	 */
	static char vBuffer[64];
	static mcapi_endpoint_t clientEndpoint;
	static mcapi_boolean_t firstTime = MCAPI_TRUE;
	static mcapi_request_t request;

	/* In a multithreaded application it would be necessary to suspend
	 * scheduling around the test and update of firstTime, in order to
	 * avoid race conditions.
	 */
	if (firstTime)
	{
		/* This is the first time that the function has been called, so
		 * we must create the anonymous endpoint that will be used as the
		 * source for the message send, below.
		 */
		firstTime = MCAPI_FALSE;
	    clientEndpoint = mcapi_endpoint_create(MCAPI_PORT_ANY, &status);
	}
	else
	{
		/* This isn't the first time that the function has been called, so
		 * we must wait for the previous non-blocking request to complete.
		 * This releases the request object.
		 */
		mcapi_wait(&request, &dummy_size, MCAPI_TIMEOUT_INFINITE, &status);
	}

	if (MCAPI_SUCCESS == status)
	{
		/* Process the format string and argument list into the string buffer */
		va_start(argPtr, psFormatString);
		vsnprintf (vBuffer, sizeof(vBuffer), psFormatString, argPtr);

		/* Do a non-blocking MCAPI send. Unless this call returns an error
		 * status (i.e. an MCAPI_ERR status code) the resulting request object
		 * will persist until the next call to mcapiPrintf() does an mcapi_wait().
		 * It is unsafe to change the contents of the message buffer (vBuffer)
		 * until the request has completed.
		 */
		mcapi_msg_send_i(clientEndpoint, serverEndpoint, vBuffer, 1 + strlen(vBuffer), 0, &request, &status);

		/* We expect mcapi_msg_send_i() to return pending, as a send operation
		 * cannot typically complete immediately, so we make this status equivalent
		 * to success for the benefit of the caller.
		 */
		if (MCAPI_PENDING == status)
		{
			status = MCAPI_SUCCESS;
		}
	}

	return status;
}

#endif

