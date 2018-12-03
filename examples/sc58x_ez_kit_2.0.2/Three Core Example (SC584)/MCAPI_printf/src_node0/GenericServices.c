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
 * GenericServers.c
 *********************************************************************************/

#include <stdio.h>
#include <services/mcapi/mcapi.h>

#include "GenericServices.h"

/* These arrays store the details of the registered services.
 */
static mcapi_request_t  s_vServiceRequests [MAX_SERVICES];
static PFServerFunction s_vServiceFunctions[MAX_SERVICES];
static mcapi_endpoint_t s_vServiceEndpoints[MAX_SERVICES];
static char            *s_vpBuffers        [MAX_SERVICES];
static size_t           s_vBufferSizes     [MAX_SERVICES];

/* StartService()
 *
 * This function provides generic registration of service functions and their
 * associated buffers. The service function will be called, by the generic
 * dispatcher (runServices(), below) whenever an incoming MCAPI message is
 * received on this service's port.
 */
mcapi_status_t StartService(
		char *pBuffer,
		size_t bufferSize,
		mcapi_port_t port,
		PFServerFunction pfFunction,
		enum ServiceIndex serviceIndex
	)
{
	mcapi_status_t mcapi_status;
	mcapi_endpoint_t serviceEndpoint;

	/* Check that the service index is within the allowed range, and hence
	 * within the bounds of the static arrays.
	 */
	if (serviceIndex >= MAX_SERVICES)
	{
		/* There is no MCAPI error code that fits this circumstance, as it
		 * is application-specific, so we return the general error code.
		 */
		return MCAPI_ERR_GENERAL;
	}

	/* Create the receive endpoint for the service, using its known port ID */
	serviceEndpoint = mcapi_endpoint_create(port, &mcapi_status);

	if (MCAPI_SUCCESS == mcapi_status)
	{
		/* Start the message receive */
		mcapi_msg_recv_i(serviceEndpoint,
						 pBuffer,
						 bufferSize,
				         &s_vServiceRequests[serviceIndex],
				         &mcapi_status);

		/* Pending and success are equivalent from the caller's
		 * point-of-view, as it will always be performing a wait
		 * on the request before doing anything else. So we
		 * represent both cases by the success code.
		 */
		if (MCAPI_PENDING == mcapi_status)
		{
			mcapi_status = MCAPI_SUCCESS;
		}

		if (MCAPI_SUCCESS == mcapi_status)
		{
			/* The receive was successfully started, so we store the details
			 * of the buffer, the endpoint, and the service function , in the
			 * static arrays.
			 */
			s_vServiceEndpoints[serviceIndex] = serviceEndpoint;
			s_vpBuffers[serviceIndex] = pBuffer;
			s_vBufferSizes[serviceIndex] = bufferSize;
			s_vServiceFunctions[serviceIndex] = pfFunction;
		}
		else
		{
			/* The receive failed so we don't need to keep the endpoint.
			 * Because we're already returning the error code from the receive
			 * we don't overwrite it with the delete's status, and since any
			 * error reported by the delete is secondary we don't need to
			 * check it.
			 */
			mcapi_status_t deleteStatus;
			mcapi_endpoint_delete(serviceEndpoint, &deleteStatus);
		}
	}

    return mcapi_status;
}


/* RunServices()
 *
 * This function checks for incoming request messages on the service endpoints
 * and dispatches them to the service functions. If there is no incoming activity
 * then MCAPI_SUCCESS is returned.
 *
 * This function does not block waiting for messages.
 */
mcapi_status_t RunServices(void)
{
    mcapi_status_t mcapi_status;
    size_t receivedSize;

    do
    {
		/* Wait for any of the active service requests to complete.
		 */
		uint32_t index = mcapi_wait_any(
				   MAX_SERVICES,
				   s_vServiceRequests,
				   &receivedSize,
				   MCAPI_TIMEOUT_IMMEDIATE,
				   &mcapi_status);

		if (MCAPI_SUCCESS == mcapi_status)
		{
			if (index < MAX_SERVICES)
			{
				/* Call the service function to process the received buffer */
				(*s_vServiceFunctions[index])(s_vpBuffers[index],
										      s_vBufferSizes[index],
										      receivedSize,
										      (enum ServiceIndex)index);

				/* Restart the message receive */
				mcapi_msg_recv_i(s_vServiceEndpoints[index],
						         s_vpBuffers[index],
						         s_vBufferSizes[index],
						         &s_vServiceRequests[index],
						         &mcapi_status);

				/* Pending and success are equivalent from our
				 * point-of-view, as we will always be performing a wait
				 * on the request before doing anything else. So we
				 * represent both cases by the success code.
				 */
				if (MCAPI_PENDING == mcapi_status)
				{
					mcapi_status = MCAPI_SUCCESS;
				}
			}
			else
			{
				printf("Unexpected service index: %ld\n", index);
			}
		}
    } while (MCAPI_SUCCESS == mcapi_status);

    /* We expect to timeout if there are no messages waiting, so we make
     * timeout equivalent to success from the caller's point-of-view.
     */
	if (MCAPI_TIMEOUT == mcapi_status)
	{
		mcapi_status = MCAPI_SUCCESS;
	}

	return mcapi_status;
}


