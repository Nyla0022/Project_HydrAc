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
 * ShutdownService.h
 *********************************************************************************/

#ifndef SHUTDOWNSERVICE_H_
#define SHUTDOWNSERVICE_H_

/* StartShutdownService()
 *
 * This function registers the printf service with the generic service
 * dispatcher, using the specified MCAPI port number and service index
 * number.
 */
extern mcapi_status_t StartShutdownService(
		mcapi_port_t port
	);

#endif /* SHUTDOWNSERVICE_H_ */
