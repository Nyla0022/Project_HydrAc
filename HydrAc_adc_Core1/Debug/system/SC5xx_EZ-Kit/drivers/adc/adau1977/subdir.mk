################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/ADSP-SC5xx_EZ-KIT_Lite-Rel2.0.2/ADSP-SC5xx_EZ-KIT/src/drivers/adc/adau1977/adi_adau1977.c 

SRC_OBJS += \
./system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.doj 

C_DEPS += \
./system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.d 


# Each subdirectory must supply rules for building sources it contributes
system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.doj: C:/Analog\ Devices/ADSP-SC5xx_EZ-KIT_Lite-Rel2.0.2/ADSP-SC5xx_EZ-KIT/src/drivers/adc/adau1977/adi_adau1977.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/SC5xx_EZ-Kit/drivers/adc/adau1977/adi_adau1977.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


