################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HydrAc_adc_Core2.c 

SRC_OBJS += \
./src/HydrAc_adc_Core2.doj 

C_DEPS += \
./src/HydrAc_adc_Core2.d 


# Each subdirectory must supply rules for building sources it contributes
src/HydrAc_adc_Core2.doj: ../src/HydrAc_adc_Core2.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core2" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE2 -D_DEBUG -DADI_DEBUG @includes-a2a4a14095c50bf2feee48b02b563832.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "src/HydrAc_adc_Core2.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


