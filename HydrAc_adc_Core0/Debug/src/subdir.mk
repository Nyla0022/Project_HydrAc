################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HydrAc_adc_Core0.c 

SRC_OBJS += \
./src/HydrAc_adc_Core0.o 

C_DEPS += \
./src/HydrAc_adc_Core0.d 


# Each subdirectory must supply rules for building sources it contributes
src/HydrAc_adc_Core0.o: ../src/HydrAc_adc_Core0.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG -DADI_DEBUG @includes-1fcc57bf98f3df0b7889d3c61ae9258e.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=1.0 -MMD -MP -MF"src/HydrAc_adc_Core0.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


