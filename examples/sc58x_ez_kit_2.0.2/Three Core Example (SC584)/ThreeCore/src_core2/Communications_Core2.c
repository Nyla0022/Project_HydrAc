/*****************************************************************************
 * File:    Communications_Core2.c
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

/* This file implements communication between Core 2 and Core 0 on an
** ADSP-SC5xx processor.
*/

#include <stddef.h>
#include "ThreeCore_Core2.h"


/* Commands are received via local command end-point */
static mcapi_endpoint_t local_endpoint;

/* Commands are received via local command end-point */
static mcapi_endpoint_t core0_endpoint;

/* Create end-points and connection for DSP
 */
void initialize_comms(mcapi_port_t port) {
    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;

    /**
     * Create end-point for data and command transfer.
     */
    local_endpoint = mcapi_endpoint_create(port, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
        print("[%s] %d status: %d mcapi_endpoint_create failure\n", __FILE__, __LINE__, (int32_t)mcapi_status);
    }
    else
    {
        /**
         * Query the remote end-points for communication. This call will block until
         * the end-point has been created on the remote node, so creating the local
         * end-point(s) before querying the remote one(s) avoids the deadlock which
         * would result if both nodes wait for the remote end-points to be created first.
         */
        core0_endpoint = mcapi_endpoint_get(DOMAIN, (uint32_t)ADI_CORE_ARM, port, MCAPI_TIMEOUT_INFINITE, &mcapi_status);
        if (MCAPI_SUCCESS != mcapi_status)
        {
            print("[%s] %d status: %d mcapi_endpoint_get failed\n", __FILE__, __LINE__, (int32_t)mcapi_status);
        }
    }
}

/**
 * Close all the ports and shut down
 */
void shut_down_comms(void) {
    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;

    /* Delete the local end-point */
    mcapi_endpoint_delete(local_endpoint, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
        print("[%s] %d status: %d mcapi_endpoint_delete failure\n", __FILE__, __LINE__, (int32_t)mcapi_status);
    }

    /* Shutdown MCAPI */
    mcapi_finalize(&mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
        print("[%s] %d status: %d mcapi_finalize failure\n", __FILE__, __LINE__, (int32_t)mcapi_status);
    }
}

/**
 * Acknowledge receipt of a command back to Core 0.
 */
void acknowledge_command(void) {
    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
    const mcapi_uint32_t cmd_response = (mcapi_uint32_t)CMD_RESPONSE;

    /** Close the processing on Core 1 and Core 2 */
    print("Sending response to Core 0\n");
    mcapi_msg_send(local_endpoint, core0_endpoint, &cmd_response, sizeof(cmd_response), 0u, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
        print("[%s] %d status: %d send_dsp_cmd failed\n", __FILE__, __LINE__, (int32_t)mcapi_status);
    }
}

/**
 * Wait for a given command from any other core.
 */
void wait_for_command(enum COMMAND wait_cmd) {
    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
    size_t msg_size = 0u;
    uint32_t msg;

    mcapi_msg_recv(local_endpoint, &msg, sizeof(msg), &msg_size, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
        print("[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__, __LINE__, (int32_t)mcapi_status);
    }
    else if ((msg_size != 4u) || (msg != (uint32_t)wait_cmd))
    {
        print("[%s] %d received %d, not %d\n", __FILE__, __LINE__, msg, (int32_t)wait_cmd);
    }
    else
    {
        /* do nothing */
    }
}

#ifdef IO_TO_UART
struct Message outputMessage;

/**
 * Send an output string back to Core 0 for printing.
 */
void send_output_to_core0(void) {
    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
    outputMessage.cmd = (mcapi_uint32_t)CMD_PRINT;
    outputMessage.core = (uint32_t)adi_core_id();

    /** Write to UART from core 0 */
    mcapi_msg_send(local_endpoint, core0_endpoint, &outputMessage, sizeof(outputMessage), 0u, &mcapi_status);
    wait_for_command(CMD_RESPONSE);
}
#endif
