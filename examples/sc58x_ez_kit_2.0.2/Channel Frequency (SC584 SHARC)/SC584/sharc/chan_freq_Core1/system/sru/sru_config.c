/*
 **
 ** sru_config.c source file generated on November 19, 2018 at 14:56:56.	
 **
 ** Copyright (C) 2014-2018 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the SRU Configuration editor. Changes to the SRU configuration should
 ** be made by changing the appropriate options rather than editing this file.
 **
 ** Only registers with non-default values are written to this file.
 **
 */
 
#include <stdint.h>
#include <sys/platform.h>

int32_t adi_SRU_Init(void);

/*
 * Initialize the Signal Routing Unit
 */
int32_t adi_SRU_Init(void)
{
    /* SPT4A_CLK_I, SPT5A_CLK_I, SPT4B_CLK_I, SPT5B_CLK_I, SPT6B_CLK_I, SPT6A_CLK_I */
    *pREG_DAI1_CLK0 = (unsigned int) 0x2528a0c2;

    /* SPT5A_D1_I, SPT5B_D0_I, SPT6A_D0_I, SPT5B_D1_I, SPT6A_D1_I */
    *pREG_DAI1_DAT1 = (unsigned int) 0x1144bf8a;

    /* SPT4B_D1_I, SPT4B_D0_I, SPT5A_D0_I, SPT4A_D1_I, SPT4A_D0_I */
    *pREG_DAI1_DAT0 = (unsigned int) 0x09144040;

    /* SPT6B_FS_I, SPT4B_FS_I, SPT5A_FS_I, SPT6A_FS_I, SPT5B_FS_I, SPT4A_FS_I */
    *pREG_DAI1_FS0 = (unsigned int) 0x27389fc3;

    /* DAI1_PB09_I, DAI1_PB12_I, DAI1_PB11_I, DAI1_PB10_I */
    *pREG_DAI1_PIN2 = (unsigned int) 0x03664c22;

    /* DAI1_PB08_I, DAI1_PB06_I, DAI1_PB07_I, DAI1_PB05_I */
    *pREG_DAI1_PIN1 = (unsigned int) 0x05084b96;

    /* DAI1_PBEN11_I, DAI1_PBEN13_I, DAI1_PBEN12_I, DAI1_PBEN15_I, DAI1_PBEN14_I */
    *pREG_DAI1_PBEN2 = (unsigned int) 0x1a5545c0;

    /* DAI1_PBEN10_I, DAI1_PBEN09_I, DAI1_PBEN06_I, DAI1_PBEN08_I, DAI1_PBEN07_I */
    *pREG_DAI1_PBEN1 = (unsigned int) 0x0000030f;

    /* PADS0 DAI0 Port Input Enable Control Register */
    *pREG_PADS0_DAI0_IE = (unsigned int) 0x001FFFFE;

    /* PADS0 DAI1 Port Input Enable Control Register */
    *pREG_PADS0_DAI1_IE = (unsigned int) 0x001FFFFE;

    return 0;
}

