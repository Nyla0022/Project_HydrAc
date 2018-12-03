/*****************************************************************************
 * File:    Communications_Core0.c
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

/* This file implements communication between Core 0 and the other two
** cores on an ADSP-SC5xx processor.
*/

#include <sys/platform.h>
#include "ThreeCore_Core0.h"

/**
 * Messages are sent from, and responses received on, the local end-point
 */
static mcapi_endpoint_t local_endpoint;

/**
 * Messages sent to core 1
 */
static mcapi_endpoint_t core1_endpoint;

/**
 * Messages sent to core 2
 */
static mcapi_endpoint_t core2_endpoint;


/**
 * Creates local end-points on GPP and gets remote core end-points.
 */
void initialize_comms(mcapi_port_t port) {
    mcapi_status_t status = MCAPI_ERR_GENERAL;

    /**
     * Create local end-point for communication.
     */
    local_endpoint = mcapi_endpoint_create(port, &status);
    if (MCAPI_SUCCESS != status)
    {
        print("[%s] %d status: %d mcapi_endpoint_create failed\n", __FILE__, __LINE__, status);
    }
    else
    {
        /**
         * Query the remote end-points for communication. This call will block until
         * the end-point has been created on the remote node, so creating the local
         * end-point(s) before querying the remote one(s) avoids the deadlock which
         * would result if both nodes wait for the remote end-points to be created first.
         */
        core1_endpoint = mcapi_endpoint_get(DOMAIN, ADI_CORE_SHARC0,
                port, MCAPI_TIMEOUT_INFINITE, &status);
        if (MCAPI_SUCCESS != status)
        {
            print("[%s] %d status: %d mcapi_endpoint_get failed\n", __FILE__, __LINE__, status);
        }

        core2_endpoint = mcapi_endpoint_get(DOMAIN, ADI_CORE_SHARC1,
                port, MCAPI_TIMEOUT_INFINITE, &status);
        if (MCAPI_SUCCESS != status)
        {
            print("[%s] %d status: %d mcapi_endpoint_get failed\n", __FILE__, __LINE__, status);
        }
    }
}

/**
 * Close all the ports and shut down
 */
void shut_down_comms(void) {

    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;

    /** Close the processing on Core 1 and Core 2 */
    send_command(ADI_CORE_SHARC0, CMD_TERMINATE);
    send_command(ADI_CORE_SHARC1, CMD_TERMINATE);

    /** Delete the end-points */
    mcapi_endpoint_delete(local_endpoint, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
           print("[%s] %d status: %d mcapi_endpoint_delete failed\n", __FILE__, __LINE__, mcapi_status);
    }

    mcapi_finalize(&mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
           print("[%s] %d status: %d mcapi_finalize failed\n", __FILE__, __LINE__, mcapi_status);
    }
}

/**
 * Send a command, with no payload, to the specified core.
 */
void send_command(uint32_t core, enum COMMAND cmd) {

    mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
    const mcapi_uint32_t mcapi_cmd = (mcapi_uint32_t)cmd;

    /** Close the processing on Core 1 and Core 2 */
    print("Sending command %d to Core %d\n", (int)cmd, (int)core);
    mcapi_msg_send(local_endpoint, (core == ADI_CORE_SHARC0) ? core1_endpoint : core2_endpoint, &mcapi_cmd, sizeof(mcapi_cmd), 0, &mcapi_status);
    if (MCAPI_SUCCESS != mcapi_status)
    {
           print("[%s] %d status: %d send_command failed\n", __FILE__, __LINE__, mcapi_status);
    }
}

/**
 * Wait for a given command from any other core. While we're waiting
 * process any PRINT commands from other cores that are running.
 */
void wait_for_command(enum COMMAND cmd) {
    mcapi_status_t status;
    size_t msg_size = 0;

    struct Message msg;

    do {
        mcapi_msg_recv(local_endpoint, &msg, sizeof(msg), &msg_size, &status);
        if (MCAPI_SUCCESS != status)
        {
            print("[%s] %d status: %d mcapi_msg_recv failure\n", __FILE__, __LINE__, status);
            break;
        }
        else if (msg_size >= 4 && msg.cmd == CMD_PRINT)
        {
            /* print out the output, send a confirmation message, and continue to wait
             * for the expected command.
             */
            mcapi_uint32_t mcapi_cmd = (mcapi_uint32_t)CMD_RESPONSE;
            mcapi_status_t mcapi_status = MCAPI_ERR_GENERAL;
            print_core(msg.core, &msg.buffer[0]);
            mcapi_msg_send(local_endpoint, (msg.core == ADI_CORE_SHARC0) ? core1_endpoint : core2_endpoint, &mcapi_cmd,
                    sizeof(mcapi_cmd), 0, &mcapi_status);
        }
        else if (msg_size != 4 || msg.cmd != cmd)
        {
            print("[%s] %d received %d, not %d\n", __FILE__, __LINE__, (int)msg.cmd, (int)cmd);
            break;
        }
        else
        {
            break; /* Got the message we were expecting */
        }
    } while (true);
}

