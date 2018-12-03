/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node0
 * File:    node_0.h
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef NODE_0_H_
#define NODE_0_H_

#include <stdbool.h>
#include <mcapi.h>

extern bool initialize_comms(mcapi_node_t remoteNode, mcapi_port_t port);
extern bool shutdown_comms(void);
extern bool perform_dsp_function(struct DSP_MSG *pMsg);

#define ERR_STREAM stdout
#define LOG_STREAM stdout

#endif /* NODE_0_H_ */
