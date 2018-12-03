/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node0
 * File:    MCAPI_msg_Node0.c
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "adi_initialize.h"
#include "../MCAPI_msg.h"
#include "node_0.h"

/* Initialize a buffer, to all 0xA5 bytes.
 */
static void init_data(unsigned char *buff, int size) {

	int count = 0;

	assert( NULL != buff);

	for (count = 0; count < size; count++) {
		buff[count] = 0xA5;
	}
}

/* Check the contents of a buffer. This assumes that the buffer
 * was initially prepared with init_data(), above, and checks
 * that it has been transformed as expected, by the process_data()
 * function on the remote node.
 */
static bool verify_data(unsigned char *buff, int size) {

	int count = 0;

	assert( NULL != buff);

	for (count = 0; count < size; count++) {
		if (0xAA != buff[count]) {
			return false;
		}
	}

	return true;
}

/* Start point for node 0
 * returns zero for success, non-zero for failure
 */
int main(void)
{
	struct DSP_MSG msg;

	/* Initialize managed drivers and/or services */
	if ( 0 != adi_initComponents() )
	{
		fprintf(LOG_STREAM, "[CORE 0]: MCAPI_msg: Error initializing components\n");
		return 1;
	}

	/* The default startup code does not include any functionality to allow
	 * core 0 to enable core 1. A convenient way to enable core 1 is to use the
	 * 'adi_core_1_enable' function. 
	 */
#if defined(__ADSPBF609__)
 adi_core_1_enable();
#elif defined(__ADSPBF561__)
 adi_core_b_enable();
#elif defined(__ADSP215xx__)

   adi_core_enable(ADI_CORE_SHARC0);
   adi_core_enable(ADI_CORE_SHARC1);
#else
#error "Processor not supported"
#endif

	/* Initialize communications with core 1 (the "DSP" core) */
	fprintf(LOG_STREAM, "[CORE 0]: Initializing communication\n");
	if (!initialize_comms(NODE_CORE_1, PORT_COMMS)) {
		fprintf(ERR_STREAM, "[%s] %d initialize_comms() failed\n", __FILE__, __LINE__);
		return 1;
	}

	/* Initialize buffer */
	msg.buffSize = sizeof(msg.buffer);
	init_data(msg.buffer, msg.buffSize);

	/* Send the message, containing the buffer, to core 1 for processing. In this simple
	 * example we just wait for core 1 to respond with the result, but a slightly more
	 * sophisticated version could use non-blocking MCAPI functions to allow core 0
	 * to proceed with other work while core 1 is computing.
	 */
	if (!perform_dsp_function(&msg)) {
		fprintf(ERR_STREAM, "[%s] %d perform_dsp_function() failed\n", __FILE__, __LINE__);
		return 1;
	}

	/* verify the received message buffer size */
	if (sizeof(msg.buffer) != msg.buffSize) {
		fprintf(ERR_STREAM, "[%s] %d incorrect returned buffer size %ld, expected %d\n",
				__FILE__, __LINE__,
				msg.buffSize, sizeof(msg.buffer)
		);
		return 1;
	}

	/* verify the received result */
	if (!verify_data(msg.buffer, msg.buffSize)) {
		fprintf(ERR_STREAM, "[%s] %d data_cmp failed\n", __FILE__, __LINE__);
		return 1;
	}

	/* Close the communication */
	fprintf(LOG_STREAM, "[CORE 0]: Shutting down communication\n");
	if (!shutdown_comms()) {
		fprintf(ERR_STREAM, "[%s] %d shutdown_comms failed\n", __FILE__, __LINE__);
		return 1;
	}

	fprintf(LOG_STREAM, "[CORE 0]: All done\n");

	return 0;
}

