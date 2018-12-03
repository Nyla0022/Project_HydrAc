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
 * GenericServices.h
 *********************************************************************************/

#ifndef GENERICSERVICES_H_
#define GENERICSERVICES_H_

enum ServiceIndex
{
	STDOUT_SERVICE,
	SHUTDOWN_SERVICE,
	MAX_SERVICES
};

/* PFServerFunction
 *
 * This is the pointer-to-function type for a service function.
 * Functions of this form are called by the generic service
 * dispatcher - RunServices() - in response to incoming request
 * messages.
 */
typedef void (*PFServerFunction)(
		char *pBuffer,
		size_t bufferSize,
		size_t receivedSize,
		enum ServiceIndex serviceIndex
	);

/* StartService()
 *
 * This function provides generic registration of service functions and their
 * associated buffers. The service function will be called, by the generic
 * dispatcher (runServices(), below) whenever an incoming MCAPI message is
 * received on this service's port.
 */
extern mcapi_status_t StartService(
		char *pBuffer,
		size_t bufferSize,
		mcapi_port_t port,
		PFServerFunction pfFunction,
		enum ServiceIndex serviceIndex
	);

/* RunServices()
 *
 * This function checks for incoming request messages on the service endpoints
 * and dispatches them to the service functions. If there is no incoming activity
 * then MCAPI_SUCCESS is returned.
 *
 * This function does not block waiting for messages.
 */
extern mcapi_status_t RunServices(void);


#endif /* GENERICSERVICES_H_ */
