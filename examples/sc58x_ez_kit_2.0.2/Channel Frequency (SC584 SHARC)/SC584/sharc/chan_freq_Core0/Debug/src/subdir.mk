################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chan_freq_Core0.c 

SRC_OBJS += \
./src/chan_freq_Core0.o 

C_DEPS += \
./src/chan_freq_Core0.d 


# Each subdirectory must supply rules for building sources it contributes
src/chan_freq_Core0.o: ../src/chan_freq_Core0.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG @includes-ef5c7d26c9c6f1394edc5de9683a7391.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=any -MMD -MP -MF"src/chan_freq_Core0.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


