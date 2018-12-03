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
 * RemotePrintf.h
 *********************************************************************************/

#ifndef MACROS_DEFINED_ON_COMMAND_LINE

//#define BLOCKING

#endif

#ifndef REMOTEPRINTF_H_
#define REMOTEPRINTF_H_

#include <services/mcapi/mcapi.h>

/* RemotePrintf()
 *
 * This function is similar to fprintf(), except that the first argument is
 * an MCAPI endpoint ID. This ID references a "server", on a different node
 * or on the same node, which will output the specified text to some form
 * of output stream.
 *
 * This allows multiple clients, possibly on different nodes (cores), to print to
 * a shared and unified output log.
 */
extern mcapi_status_t RemotePrintf(
		mcapi_endpoint_t serverEndpoint,
		char *fmt,
		...
);


#endif /* REMOTEPRINTF_H_ */
