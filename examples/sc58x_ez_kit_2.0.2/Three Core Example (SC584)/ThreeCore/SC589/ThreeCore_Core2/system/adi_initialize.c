/*
** adi_initialize.c source file generated on April 29, 2016 at 17:28:24.
**
** Copyright (C) 2000-2016 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically. You should not modify this source file,
** as your changes will be lost if this source file is re-generated.
*/

#include <sys/platform.h>
#include <services/int/adi_sec.h>

#include "adi_initialize.h"

extern int32_t adi_mcapi_Init(void);

int32_t adi_initComponents(void)
{
    int32_t result = 0;

    result = adi_sec_Init();


    if (result == 0) {
        result = adi_mcapi_Init(); /* auto-generated code (order:6) */
    }

    return result;
}

