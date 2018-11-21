################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/mcapi/GeneratedSources/mcapi_config.c 

SRC_OBJS += \
./system/mcapi/GeneratedSources/mcapi_config.o 

C_DEPS += \
./system/mcapi/GeneratedSources/mcapi_config.d 


# Each subdirectory must supply rules for building sources it contributes
system/mcapi/GeneratedSources/mcapi_config.o: ../system/mcapi/GeneratedSources/mcapi_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-1fcc57bf98f3df0b7889d3c61ae9258e.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=1.0 -MMD -MP -MF"system/mcapi/GeneratedSources/mcapi_config.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


