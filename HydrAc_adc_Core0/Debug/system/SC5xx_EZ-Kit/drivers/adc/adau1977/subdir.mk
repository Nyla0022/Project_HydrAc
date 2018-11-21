################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/ADSP-SC5xx_EZ-KIT_Lite-Rel2.0.2/ADSP-SC5xx_EZ-KIT/src/drivers/adc/adau1977/adi_adau1977.c 

SRC_OBJS += \
./system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.o 

C_DEPS += \
./system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.d 


# Each subdirectory must supply rules for building sources it contributes
system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.o: C:/Analog\ Devices/ADSP-SC5xx_EZ-KIT_Lite-Rel2.0.2/ADSP-SC5xx_EZ-KIT/src/drivers/adc/adau1977/adi_adau1977.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-5b107aba3595f680048801dbf1a935b0.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=1.0 -MMD -MP -MF"system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


