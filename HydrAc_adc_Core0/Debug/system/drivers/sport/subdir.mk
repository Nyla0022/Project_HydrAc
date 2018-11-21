################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/ARM/arm-none-eabi/arm-none-eabi/lib/src/cortex-a5/drivers/Source/sport/adi_sport.c 

SRC_OBJS += \
./system/drivers/sport/adi_sport.o 

C_DEPS += \
./system/drivers/sport/adi_sport.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/sport/adi_sport.o: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/ARM/arm-none-eabi/arm-none-eabi/lib/src/cortex-a5/drivers/Source/sport/adi_sport.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-5b107aba3595f680048801dbf1a935b0.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=1.0 -MMD -MP -MF"system/drivers/sport/adi_sport.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


