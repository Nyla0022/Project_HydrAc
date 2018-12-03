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
 * PrintfService.h
 *********************************************************************************/

#ifndef PRINTFSERVICE_H_
#define PRINTFSERVICE_H_

/* startPrintfService()
 *
 * This function registers the printf service with the generic service
 * dispatcher, using the specified MCAPI port number and service index
 * number.
 */
extern mcapi_status_t startPrintfService(
		mcapi_port_t port
	);

#endif /* PRINTFSERVICE_H_ */
