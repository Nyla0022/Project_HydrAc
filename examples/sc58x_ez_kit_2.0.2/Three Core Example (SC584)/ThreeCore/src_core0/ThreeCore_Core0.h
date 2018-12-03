/*****************************************************************************
 * File:    ThreeCore_Core0.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_CORE0_H
#define THREECORE_CORE0_H

/* Shared header file used by all three projects.
 */
#include "../ThreeCore.h"

#include "uart.h"

/* Prototypes for functions defined in Communications_Core0.c */
extern void initialize_comms(mcapi_port_t port);
extern void shut_down_comms(void);
extern void send_command(uint32_t core, enum COMMAND cmd);
extern void wait_for_command(enum COMMAND cmd);

/* Prototypes for functions defined in ThreeCore_Core0.c */
extern void print_core(int core, char *fmt, ...);
extern void print(char *fmt, ...);

#endif /* THREECORE_CORE0_H */
