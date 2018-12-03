/*****************************************************************************
 * Example: MCAPI_msg
 * File:    MCAPI_msg.h
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef __MCAPI_MSG_H__
#define __MCAPI_MSG_H__

#include <services/mcapi/mcapi.h>

/* MCAPI Domain number. Zero is the only supported domain at present. */
#define DOMAIN 0u

/**
 * MCAPI Node numbers for the processor cores.
 */
#define NODE_CORE_0 0u
#define NODE_CORE_1 1u

/**
 * MCAPI Port numbers used by the application.
 * Numbering should start at MCAPI_NUM_RESERVED_PORTS, which is an
 * implementation-defined constant and may be >= zero.
 */
enum {
  /**
   * @def PORT_COMMAND
   * @brief PORT for sending commands from GPP to DSP
   */
  PORT_COMMS = MCAPI_NUM_RESERVED_PORTS
};

/**
 * maximum size of the buffer array within the message struct
 */
#define MSG_MAX     128


/**
 * Commands for execution, Common between core A and Core B
 */
enum DSP_COMMAND {
  DSP_CMD_TERMINATE,
  DSP_CMD_RESPONSE,
  DSP_CMD_EXECUTE
};


struct DSP_MSG {
	enum DSP_COMMAND cmd;
	mcapi_endpoint_t senderEp;
	uint32_t buffSize;
	unsigned char buffer[MSG_MAX];
};


#endif /* __MCAPI_MSG_H__ */
