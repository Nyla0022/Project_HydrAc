/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node1
 * File:    node_1.h
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef NODE_1_H_
#define NODE_1_H_

#include <stdbool.h>

bool dsp_initialize(mcapi_node_t remoteNode, mcapi_port_t port);
bool dsp_shutdown(void);
bool dsp_command_loop(bool (*pfProcess_data)(struct DSP_MSG *));

#define ERR_STREAM stdout
#define LOG_STREAM stdout

#endif /* NODE_1_H_ */
