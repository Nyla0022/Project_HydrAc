################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/SC584/SoftConfig_SC584.c \
C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/calc_freq.c \
C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/chan_freq.c 

SRC_OBJS += \
./src/SoftConfig_SC584.doj \
./src/calc_freq.doj \
./src/chan_freq.doj 

C_DEPS += \
./src/SoftConfig_SC584.d \
./src/calc_freq.d \
./src/chan_freq.d 


# Each subdirectory must supply rules for building sources it contributes
src/SoftConfig_SC584.doj: C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/SC584/SoftConfig_SC584.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ChanFreq_SC584_SHARC_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision any -g -DCORE1 -DADI_DEBUG -D_DEBUG @includes-b5858349a8927fdb528e93abc854a4e0.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -absolute-path-dependencies -gnu-style-dependencies -MD -Mo "src/SoftConfig_SC584.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/calc_freq.doj: C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/calc_freq.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ChanFreq_SC584_SHARC_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision any -g -DCORE1 -DADI_DEBUG -D_DEBUG @includes-b5858349a8927fdb528e93abc854a4e0.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -absolute-path-dependencies -gnu-style-dependencies -MD -Mo "src/calc_freq.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/chan_freq.doj: C:/Users/JASP/Documents/Hydrac_Repo/Project_HydrAc/examples/sc58x_ez_kit_2.0.2/Channel\ Frequency\ (SC584\ SHARC)/chan_freq.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ChanFreq_SC584_SHARC_Core1" -proc ADSP-SC584 -flags-compiler --no_wrap_diagnostics -si-revision any -g -DCORE1 -DADI_DEBUG -D_DEBUG @includes-b5858349a8927fdb528e93abc854a4e0.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -char-size-8 -swc -absolute-path-dependencies -gnu-style-dependencies -MD -Mo "src/chan_freq.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


