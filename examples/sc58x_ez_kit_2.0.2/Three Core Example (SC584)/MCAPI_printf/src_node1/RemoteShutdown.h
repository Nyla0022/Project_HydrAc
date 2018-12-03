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
 * RemoteShutdown.h
 *********************************************************************************/

#ifndef REMOTESHUTDOWN_H_
#define REMOTESHUTDOWN_H_

#include <services/mcapi/mcapi.h>

/* RemoteShutdown()
 *
 * This function instructs the server (referenced by the endpoint argument)
 * to output a message (specified by the format string and argument list)
 * and then exit.
 */
extern mcapi_status_t RemoteShutdown(
		mcapi_endpoint_t serverEndpoint,
		char *fmt,
		...
);

#endif /* REMOTESHUTDOWN_H_ */
