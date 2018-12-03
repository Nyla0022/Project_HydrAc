/*********************************************************************************

Copyright(c) 2014-2016 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*****************************************************************************
 * MCAPI_remote_printf_Node0.h
 *****************************************************************************/

#ifndef __MCAPI_REMOTE_PRINTF_NODE0_H__
#define __MCAPI_REMOTE_PRINTF_NODE0_H__

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
	PRINTF_PORT = MCAPI_NUM_RESERVED_PORTS,
	SHUTDOWN_PORT
};

#endif /* __MCAPI_REMOTE_PRINTF_NODE0_H__ */
