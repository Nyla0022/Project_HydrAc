################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/spu/adi_spu.c 

SRC_OBJS += \
./system/services/spu/adi_spu.doj 

C_DEPS += \
./system/services/spu/adi_spu.d 


# Each subdirectory must supply rules for building sources it contributes
system/services/spu/adi_spu.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/spu/adi_spu.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core2" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE2 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-fee9b107ec987eef7822883ddaaa9dcb.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/services/spu/adi_spu.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

