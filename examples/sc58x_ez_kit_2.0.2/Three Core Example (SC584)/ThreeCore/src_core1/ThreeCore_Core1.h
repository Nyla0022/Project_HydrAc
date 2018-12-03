/*****************************************************************************
 * File:    ThreeCore_Core1.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_CORE1_H
#define THREECORE_CORE1_H

/* Shared header file used by all three projects.
 */
#include "../ThreeCore.h"

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
#pragma diag(push)
#if defined(_MISRA_2004_RULES)
#pragma diag(suppress:misra_rule_16_1:"print implementation needs varargs arguments")
#pragma diag(suppress:misra_rule_20_9:"This example produces output")
#else
#pragma diag(suppress:misra2012_rule_21_6:"This example produces output")
#endif
#endif /* defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES) */

/* Prototypes for functions defined in Communications_Core1.c */
extern void initialize_comms(mcapi_port_t port);
extern void shut_down_comms(void);
extern void wait_for_command(enum COMMAND wait_cmd);
extern void acknowledge_command(void);

/* Prototypes for functions defined in ThreeCore_Core1.c */
extern void print(char *fmt, ...);

#if defined(IO_TO_UART)
   extern void send_output_to_core0(void);
   extern struct Message outputMessage;
#endif

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
#pragma diag(pop)
#endif /* defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES) */

#endif /* THREECORE_CORE1_H */
