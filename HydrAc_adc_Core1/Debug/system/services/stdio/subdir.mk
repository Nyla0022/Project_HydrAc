################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_jtag.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_null.c \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_uart.c 

SRC_OBJS += \
./system/services/stdio/adi_stdio.doj \
./system/services/stdio/adi_stdio_jtag.doj \
./system/services/stdio/adi_stdio_null.doj \
./system/services/stdio/adi_stdio_uart.doj 

C_DEPS += \
./system/services/stdio/adi_stdio.d \
./system/services/stdio/adi_stdio_jtag.d \
./system/services/stdio/adi_stdio_null.d \
./system/services/stdio/adi_stdio_uart.d 


# Each subdirectory must supply rules for building sources it contributes
system/services/stdio/adi_stdio.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_jtag.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_jtag.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_jtag.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_null.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_null.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_null.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/services/stdio/adi_stdio_uart.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/SHARC/lib/src/services/Source/stdio/adi_stdio_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="HydrAc_adc_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision 1.0 -g -DCORE1 -D_DEBUG -DADI_DEBUG -DADI_MCAPI @includes-a3be0183226ed47b80fc0747c911e904.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -gnu-style-dependencies -MD -Mo "system/services/stdio/adi_stdio_uart.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


