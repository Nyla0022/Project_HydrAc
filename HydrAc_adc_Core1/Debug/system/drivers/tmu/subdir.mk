################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/drivers/Source/tmu/adi_tmu.c 

SRC_OBJS += \
./system/drivers/tmu/adi_tmu.doj 

C_DEPS += \
./system/drivers/tmu/adi_tmu.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/tmu/adi_tmu.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/drivers/Source/tmu/adi_tmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/drivers/tmu/adi_tmu.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


