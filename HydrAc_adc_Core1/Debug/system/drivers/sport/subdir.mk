################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/drivers/Source/sport/adi_sport.c 

SRC_OBJS += \
./system/drivers/sport/adi_sport.doj 

C_DEPS += \
./system/drivers/sport/adi_sport.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/sport/adi_sport.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/drivers/Source/sport/adi_sport.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/drivers/sport/adi_sport.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


