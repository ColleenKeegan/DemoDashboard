################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CPFEAVRAnalog.cpp \
../CPFECANLib.cpp \
../CPFERotarySwitch.cpp \
../FEDashLCD.cpp \
../main.cpp 

OBJS += \
./CPFEAVRAnalog.o \
./CPFECANLib.o \
./CPFERotarySwitch.o \
./FEDashLCD.o \
./main.o 

CPP_DEPS += \
./CPFEAVRAnalog.d \
./CPFECANLib.d \
./CPFERotarySwitch.d \
./FEDashLCD.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -I"/Users/thomaswillson/Documents/workspace/FE2015Dashboard/FTDI _V1" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


