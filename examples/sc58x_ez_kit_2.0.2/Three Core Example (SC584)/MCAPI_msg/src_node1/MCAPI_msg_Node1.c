/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node1
 * File:    MCAPI_msg_Node1.c
 *
 * Copyright(c) 2013-2015 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#include <sys/platform.h>
#include <stdio.h>
#include <assert.h>

#include "adi_initialize.h"
#include "../MCAPI_msg.h"
#include "node_1.h"

/* Example data-processing function, which performs a very simple
 * transformation on the contents of a message. This can be replaced
 * with a more useful operation, such as a filter operation.
 *
 * Note that the function has access to the 'command' field within
 * the message structure, and so can implement different transformations
 * different command codes. The current example only defines one
 * data-processing command - DSP_CMD_EXECUTE - and so the command
 * is not examined here.
 */
static bool process_data(struct DSP_MSG *pMsg) {

	int count;

	for (count = 0; count < pMsg->buffSize; count++) {
		pMsg->buffer[count] += 0x5;
	}

	return true;
}


/**
 * Start point for node 1
 * @return zero for success, non-zero for failure
 */
int main(void)
{
	/* Initialize managed drivers and/or services */
	if ( 0 != adi_initComponents() )
	{
		fprintf(LOG_STREAM, "[CORE 1]: MCAPI_msg: Error initializing components\n");
		return 1;
	}

	/* Initialize the communication, to receive commands from the remote core */
	fprintf(LOG_STREAM, "[CORE 1]: Initializing communication\n");

	if (!dsp_initialize(NODE_CORE_0, PORT_COMMS)) {
		fprintf(ERR_STREAM, "[%s] %d dsp_initialize failure\n", __FILE__,
				__LINE__);
		return 1;
	}

	/* Run the command loop, passing the address of the processing function that
	 * will be called for each command. dsp_command_loop() will return once a DSP_CMD_TERMINATE
	 * is received.
	 */
	if (!dsp_command_loop(&process_data)) {
		fprintf(ERR_STREAM, "[%s] %d dsp_command_loop failure\n", __FILE__,
				__LINE__);
		return 1;
	}

	/* Close down the communication prior to exit */
	if (!dsp_shutdown()) {
		fprintf(ERR_STREAM, "[%s] %d dsp_shutdown failure\n", __FILE__,
				__LINE__);
		return 1;
	}

	fprintf(ERR_STREAM, "[CORE 1]: All done\n");

	return 0;
}

