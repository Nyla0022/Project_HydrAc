/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node1
 * File:    node_1.c
 *
 * Copyright(c) 2013-2015 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <services/mcapi/mcapi.h>

#include "../MCAPI_msg.h"
#include "node_1.h"


/* Commands are received via local command end-point */
static mcapi_endpoint_t dsp_recv_endpoint;


/* Create endpoints and connection for DSP
 * returns bool, true for success, false for failure
 */
bool dsp_initialize(mcapi_node_t remoteNode, mcapi_port_t port) {
	mcapi_status_t status;

	/**
	 * Create end-point for data and command transfer.
	 */
	dsp_recv_endpoint = mcapi_endpoint_create(port, &status);
	if (MCAPI_SUCCESS != status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_endpoint_create failure\n",
				__FILE__, __LINE__, status);
		return false;
	}

	return true;
}

/* closes the ports and shuts down the DSP
 * returns bool, true for success, false for failure
 */
bool dsp_shutdown(void) {

	mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;

	/* Delete the local end-point */
	mcapi_endpoint_delete(dsp_recv_endpoint, &mcapi_status);
	if (MCAPI_SUCCESS != mcapi_status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_endpoint_delete failure\n",
				__FILE__, __LINE__, mcapi_status);
		return false;
	}

	/* Shutdown MCAPI */
	mcapi_finalize(&mcapi_status);
	if (MCAPI_SUCCESS != mcapi_status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_finalize failure\n",
				__FILE__, __LINE__, mcapi_status);
		return false;
	}

	return true;
}

/* DSP command loop
 * returns bool, true for success, false for failure
 */
bool dsp_command_loop(bool (*pfProcess_data)(struct DSP_MSG *)) {

	mcapi_status_t status;
	size_t msg_size = 0;
	struct DSP_MSG msg;

	/* Loop until a terminate command is received */
	do {
		/* Wait for a message */
		mcapi_msg_recv(dsp_recv_endpoint, &msg, sizeof(msg),
				&msg_size, &status);
		if (MCAPI_SUCCESS != status) {
			fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__,
					__LINE__, status);
			return false;
		}

		/* Verify that the message contained at least the 4 bytes of the command */
		assert(msg_size >= sizeof(msg.cmd));

		/* Interpret the command */
		switch (msg.cmd) {
		case DSP_CMD_TERMINATE:
			fprintf(LOG_STREAM, "[CORE 1]: Received command (DSP_TERMINATE)\n");
			break;

		case DSP_CMD_RESPONSE:
			fprintf(ERR_STREAM, "[%s] Error, DSP_RESPONSE command received\n", __FILE__,__LINE__);
			return false;


		default:
		case DSP_CMD_EXECUTE:
			assert(msg_size >= sizeof(struct DSP_MSG));
			fprintf(LOG_STREAM, "[CORE 1]: Received command (DSP_EXECUTE)\n");

			/* Call the provided data-processing function, this function may update the
			 * message struct. If the function returns true then the message must be
			 * sent back as a response.
			 */
			if ((*pfProcess_data)(&msg)) {
				/* Function returned true, send the response */
				fprintf(LOG_STREAM, "[CORE 1]: Processing complete, sending %d bytes of reply\n", msg_size);

				/* Change the command to indicate that this is a response */
				msg.cmd = DSP_CMD_RESPONSE;

				/* And send the updated message back to its original sender */
				mcapi_msg_send(dsp_recv_endpoint, msg.senderEp, &msg, sizeof(msg), 0, &status);
				if (MCAPI_SUCCESS != status) {
					fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_msg_send failure\n", __FILE__,
							__LINE__, status);
					return false;
				}
				fprintf(LOG_STREAM, "[CORE 1]: Sending data complete\n");
			}
			else
			{
				/* Function returned false, no response is expected */
				fprintf(LOG_STREAM, "[CORE 1]: Processing complete, no reply required\n");
			}
			break;
		}
	} while (msg.cmd != DSP_CMD_TERMINATE);

	return true;
}

