################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/heap_stack/app_heap_stack.c 

SRC_OBJS += \
./system/heap_stack/app_heap_stack.o 

C_DEPS += \
./system/heap_stack/app_heap_stack.d 


# Each subdirectory must supply rules for building sources it contributes
system/heap_stack/app_heap_stack.o: ../system/heap_stack/app_heap_stack.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore ARM Bare Metal C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -ffunction-sections -fdata-sections -DCORE0 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-5b107aba3595f680048801dbf1a935b0.txt -Wall -c -mproc=ADSP-SC584 -msi-revision=1.0 -MMD -MP -MF"system/heap_stack/app_heap_stack.d" -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


