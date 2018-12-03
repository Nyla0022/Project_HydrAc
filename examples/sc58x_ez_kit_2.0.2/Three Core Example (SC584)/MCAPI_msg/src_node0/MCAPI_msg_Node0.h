/*****************************************************************************
 * Example: MCAPI_msg
 * Project: MCAPI_msg_Node0
 * File:    MCAPI_msg_Node0.h
 *
 * Copyright(c) 2013-2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef __MCAPI_MSG_NODE0_H__
#define __MCAPI_MSG_NODE0_H__

/* Add your custom header content here */
#include <services/mcapi/mcapi.h>

/* MCAPI Domain number. Zero is the only supported domain at present. */
#define DOMAIN 0u

/**
 * MCAPI Node numbers for the cores.
 */
#define NODE_CORE_0 0u
#define NODE_CORE_1 1u

/**
 * MCAPI Port numbers. The ports used by the application should be added
 * to this enumeration, starting by replacing the entry PORT_PLACEHOLDER.
 * Numbering should start at MCAPI_NUM_RESERVED_PORTS, which is an
 * implementation-defined constant and may be zero.
 */
enum {
  /**
   * @def PORT_COMMAND
   * @brief PORT for sending commands from GPP to DSP
   */
  PORT_COMMAND = MCAPI_NUM_RESERVED_PORTS,
  /**
   * @def PORT_DATA
   * @brief PORT for sending data from GPP to DSP
   */
  PORT_DATA,
  /**
   * @def PORT_DATA_RECV
   * @brief PORT for receiving processed data from DSP to GPP
   */
  PORT_DATA_RECV
};

/**
 * @def MSG_MAX
 * @brief maximum size of the message sent
 */
#define MSG_MAX     (512)

#ifndef MCAPI_MAX_MSG_SIZE
#define MCAPI_MAX_MSG_SIZE 256
#endif


/**
 * Commands for execution, Common between core A and Core B
 */
enum {
  DSP_READY,
  DSP_DATA,
  DSP_CODE,
  DSP_TERMINATE,
  DSP_EXECUTE,
  COMPLETED
};

/**
 * @def PASS
 * @brief for checking PASS
 */
#ifndef PASS
#define PASS (0)
#endif

/**
 * @def FAIL
 * @brief for checking FAIL
 */
#ifndef FAIL
#define FAIL (~0)
#endif


extern mcapi_info_t  adi_mcapi_info;

/**
 * Start point for node 0
 * @return PASS/FAIL
 */
int main_node0(const mcapi_info_t * const mcapi_info );

/**
 * Start point for node 0
 * @return PASS/FAIL
 */
int main_node1(const mcapi_info_t * const mcapi_info);

#endif /* __MCAPI_MSG_NODE0_H__ */
