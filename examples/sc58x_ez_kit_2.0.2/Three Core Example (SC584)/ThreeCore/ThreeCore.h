/*****************************************************************************
 * File:    ThreeCore.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_H
#define THREECORE_H

/* #define IO_TO_UART */  /* output is directed to the UART */
#define IO_TO_CONSOLE     /* output is directed to the view console window */

#if defined(IO_TO_CONSOLE) && defined(IO_TO_UART)
 #error Cannot build with both IO_TO_CONSOLE and IO_TO_UART defined.
#endif

#include <sys/adi_core.h>
#include <services/mcapi/mcapi.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__ADSPSC573__)
  #include "SC573/ThreeCore_SC573.h"
#elif defined(__ADSPSC584__)
  #include "SC584/ThreeCore_SC584.h"
#elif defined(__ADSPSC589__)
  #include "SC589/ThreeCore_SC589.h"
#else
  #error Not supported
#endif

#define UART_DEBUG_BUFFER_LINE_SIZE    200u

struct Message {
    uint32_t cmd;
    uint32_t core;
    char buffer[UART_DEBUG_BUFFER_LINE_SIZE];
};

/*
 * Commands for execution, Common between core A and Core B
 */
enum COMMAND {
  CMD_TERMINATE,
  CMD_RESPONSE,
  CMD_EXECUTE,
  CMD_PRINT
};

#define DOMAIN 0u

/**
 * MCAPI Port numbers used by the application.
 * Numbering should start at MCAPI_NUM_RESERVED_PORTS, which is an
 * implementation-defined constant and may be >= zero.
 */
enum {
  /**
   * PORT for sending commands from GPP to DSP
   */
  PORT_COMMS = MCAPI_NUM_RESERVED_PORTS
};

/**
 * Maximum size of the message sent
 */
#define MSG_MAX     (512)

#endif /* THREECORE_H */
