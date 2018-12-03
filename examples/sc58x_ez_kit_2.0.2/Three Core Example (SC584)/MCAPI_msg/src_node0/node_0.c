/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node0
 * File:    node_0.c
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#include <sys/platform.h>
#include <stdio.h>
#include <assert.h>
#include <services/mcapi/mcapi.h>

#include "../MCAPI_msg.h"
#include "node_0.h"

/**
 * Messages are sent from, and responses received on, the local end-point
 */
static mcapi_endpoint_t local_endpoint;

/**
 * Messages are sent to a remote endpoint on another node (the "DSP")
 */
static mcapi_endpoint_t remote_endpoint;


/**
 * Creates local end-points on GPP and gets remote DSP end-points.
 * @return bool, true for success, false for failure
 */
bool initialize_comms(mcapi_node_t remoteNode, mcapi_port_t port) {

	mcapi_status_t status = MCAPI_ERR_GENERAL;

	/**
	 * Create local end-point for communication.
	 */
	local_endpoint = mcapi_endpoint_create(port, &status);
	if (MCAPI_SUCCESS != status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_endpoint_create failed\n",
				__FILE__, __LINE__, status);
		return false;
	}

	/**
	 * Query the remote end-point for communication. This call will block until
	 * the endpoint has been created on the remote node, so creating the local
	 * endpoint(s) before querying the remote one(s) avoids the deadlock which
	 * would result if both nodes wait for the remote endpoints to be created first.
	 */
	remote_endpoint = mcapi_endpoint_get(DOMAIN, remoteNode,
			port, MCAPI_TIMEOUT_INFINITE, &status);
	if (MCAPI_SUCCESS != status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_endpoint_get failed\n", __FILE__,
				__LINE__, status);
		return false;
	}

	return true;
}

/**
 * Close all the ports and shut down
 * @return bool, true for success, false for failure
 */
bool shutdown_comms(void) {

	mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
	const mcapi_uint32_t termCmd = DSP_CMD_TERMINATE;

	/** Close the processing on the DSP */
	fprintf(LOG_STREAM, "[CORE 0]: Sending command to DSP: (DSP_TERMINATE)\n");
	mcapi_msg_send(local_endpoint, remote_endpoint, &termCmd,
			sizeof(termCmd), 0, &mcapi_status);
	if (MCAPI_SUCCESS != mcapi_status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d send_dsp_cmd failed\n", __FILE__,
				__LINE__, mcapi_status);
		return false;
	}

	/** Delete the end-points */
	mcapi_endpoint_delete(local_endpoint, &mcapi_status);
	if (MCAPI_SUCCESS != mcapi_status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_endpoint_delete failed\n",
				__FILE__, __LINE__, mcapi_status);
		return false;
	}

	mcapi_finalize(&mcapi_status);
	if (MCAPI_SUCCESS != mcapi_status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_finalize failed\n", __FILE__,
				__LINE__, mcapi_status);
		return false;
	}

	return true;
}


/**
 * Example implementation
 * @param pMsg pointer to message struct to be sent for processing
 * @return bool, true for success, false for failure
 */
bool perform_dsp_function(struct DSP_MSG *pMsg) {

	mcapi_status_t status = MCAPI_ERR_GENERAL;
	size_t size = 0;
	const mcapi_uint32_t data_size = sizeof(struct DSP_MSG);

	fprintf(LOG_STREAM, "[CORE 0]: Sending %d bytes of message to DSP\n", data_size);

	/* Send command and data for processing to the DSP */
	pMsg->cmd = DSP_CMD_EXECUTE;
	pMsg->senderEp = local_endpoint;
	mcapi_msg_send(local_endpoint, remote_endpoint, pMsg,
			data_size, 0, &status);
	if (MCAPI_SUCCESS != status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d send_dsp_cmd failed\n", __FILE__,
				__LINE__, status);
		return false;
	}


	fprintf(LOG_STREAM, "[CORE 0]: Receive processed data back from DSP\n");
	mcapi_msg_recv(local_endpoint, pMsg, data_size, &size, &status);
	if (MCAPI_SUCCESS != status) {
		fprintf(ERR_STREAM, "[%s] %d status: %d mcapi_msg_recv failed\n", __FILE__,
				__LINE__, status);
		return false;
	}

	fprintf(LOG_STREAM, "[CORE 0]: Received %d bytes of data from DSP\n", size);

	return true;
}

