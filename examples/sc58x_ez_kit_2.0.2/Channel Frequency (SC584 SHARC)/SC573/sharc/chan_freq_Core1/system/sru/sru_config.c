/*
 **
 ** sru_config.c source file generated on November 25, 2018 at 22:32:37.	
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
    /* SPT2B_CLK_I, SPT0B_CLK_I, SPT1B_CLK_I, SPT1A_CLK_I, SPT0A_CLK_I, SPT2A_CLK_I */
    *pREG_DAI0_CLK0 = (unsigned int) 0x24b633c2;

    /* SPT1A_D0_I, SPT0B_D0_I, SPT0B_D1_I, SPT0A_D0_I, SPT0A_D1_I */
    *pREG_DAI0_DAT0 = (unsigned int) 0x3ef84040;

    /* SPT2A_D1_I, SPT1A_D1_I, SPT1B_D0_I, SPT2A_D0_I, SPT1B_D1_I */
    *pREG_DAI0_DAT1 = (unsigned int) 0x0617efbe;

    /* SPT1A_FS_I, SPT2A_FS_I, SPT0B_FS_I, SPT2B_FS_I, SPT0A_FS_I, SPT1B_FS_I */
    *pREG_DAI0_FS0 = (unsigned int) 0x3d36b7c3;

    /* DAI0_PB18_I, DAI0_PB19_I, DAI0_INV_PB19_I, DAI0_PB17_I, DAI0_PB20_I, DAI0_INV_PB20_I */
    *pREG_DAI0_PIN4 = (unsigned int) 0x05494f9e;

    /* DAI0_PB06_I, DAI0_PB05_I, DAI0_PB08_I, DAI0_PB07_I */
    *pREG_DAI0_PIN1 = (unsigned int) 0x05474e16;

    /* DAI0_PB12_I, DAI0_PB10_I, DAI0_PB11_I, DAI0_PB09_I */
    *pREG_DAI0_PIN2 = (unsigned int) 0x04874e24;

    /* DAI0_PBEN07_I, DAI0_PBEN06_I, DAI0_PBEN09_I, DAI0_PBEN08_I, DAI0_PBEN10_I */
    *pREG_DAI0_PBEN1 = (unsigned int) 0x00000000;

    /* DAI0_PBEN15_I, DAI0_PBEN12_I, DAI0_PBEN11_I, DAI0_PBEN13_I, DAI0_PBEN14_I */
    *pREG_DAI0_PBEN2 = (unsigned int) 0x1a554000;

    /* DAI0_PBEN16_I, DAI0_PBEN17_I, DAI0_PBEN18_I, DAI0_PBEN20_I, DAI0_PBEN19_I */
    *pREG_DAI0_PBEN3 = (unsigned int) 0x0071f79b;


    return 0;
}

