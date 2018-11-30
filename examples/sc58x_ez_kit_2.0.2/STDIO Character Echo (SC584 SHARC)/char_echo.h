/*********************************************************************************

Copyright(c) 2012-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      char_echo.h
 *
 * @brief     This is the primary header file for UART character echo example.
 *
 */

#ifndef CHAR_ECHO_H
#define CHAR_ECHO_H

/*
 * User configurations
 */

#ifndef MACROS_DEFINED_ON_COMMAND_LINE

/* Un-comment the line below line to use DMA */
/* #define USE_DMA_MODE */

#endif

/* Baud rate to be used for char echo */
#define BAUD_RATE           9600u

/*
 * Macros used in the example
 */

/* Example result definitions */
#define FAILED              (-1)
#define PASSED              0

/* Macro for reporting errors */
#define REPORT_ERROR        printf

/*
 * Processor specific macros
 */

/* UART Device Number to test */
#define UART_DEVICE_NUM     0u

#define MHZTOHZ       (1000000u)
#define CLKIN         (25u  * MHZTOHZ)


#endif /* CHAR_ECHO_H */
