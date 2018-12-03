/*****************************************************************************
 * File:    ThreeCore_SC589.h
 *
 * Copyright(c) 2016 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary and confidential.  By using this software you
 * agree to the terms of the associated Analog Devices License Agreement.
 *****************************************************************************/

#ifndef THREECORE_SC589_H_
#define THREECORE_SC589_H_

/* The LED used by Core 0 to report its status.
*/
#define CORE0_LED             "LED10"
#define CORE0_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE0_LED_PIN         (ADI_GPIO_PIN_13)

/* The Push Button used by Core 0.
*/
#define CORE0_PB              "PB1"
#define CORE0_PB_PORT         (ADI_GPIO_PORT_F)
#define CORE0_PB_PORT_PIN     (ADI_GPIO_PIN_0)
#define CORE0_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_4)
#define CORE0_PB_PINT_PIN     (ADI_GPIO_PIN_0)
#define CORE0_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PFL_PINT4)
#define CORE0_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_0)

/* The LED used by Core 1 to report its status.
*/
#define CORE1_LED             "LED11"
#define CORE1_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE1_LED_PIN         (ADI_GPIO_PIN_14)

/* The Push Button used by Core 1.
*/
#define CORE1_PB              "PB2"
#define CORE1_PB_PORT         (ADI_GPIO_PORT_F)
#define CORE1_PB_PORT_PIN     (ADI_GPIO_PIN_1)
#define CORE1_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_5)
#define CORE1_PB_PINT_PIN     (ADI_GPIO_PIN_1)
#define CORE1_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PFL_PINT5)
#define CORE1_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_0)

/* The LED used by Core 2 to report its status.
*/
#define CORE2_LED             "LED12"
#define CORE2_LED_PORT        (ADI_GPIO_PORT_E)
#define CORE2_LED_PIN         (ADI_GPIO_PIN_15)

/* The Push Button used by Core 2.
*/
#define CORE2_PB              "PB2"
#define CORE2_PB_PORT         (ADI_GPIO_PORT_F)
#define CORE2_PB_PORT_PIN     (ADI_GPIO_PIN_1)
#define CORE2_PB_PINT         (ADI_GPIO_PIN_INTERRUPT_5)
#define CORE2_PB_PINT_PIN     (ADI_GPIO_PIN_1)
#define CORE2_PIN_ASSIGN      (ADI_GPIO_PIN_ASSIGN_PFL_PINT5)
#define CORE2_PIN_ASSIGN_BYTE (ADI_GPIO_PIN_ASSIGN_BYTE_0)

#endif /* THREECORE_SC589_H_ */
