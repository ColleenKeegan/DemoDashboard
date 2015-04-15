################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../at90can/WInterrupts.c \
../at90can/pins_arduino.c \
../at90can/wiring.c \
../at90can/wiring_analog.c \
../at90can/wiring_digital.c \
../at90can/wiring_pulse.c \
../at90can/wiring_shift.c 

CPP_SRCS += \
../at90can/HardwareSerial.cpp \
../at90can/Print.cpp \
../at90can/Stream.cpp \
../at90can/WMath.cpp \
../at90can/WString.cpp \
../at90can/can_lib.cpp \
../at90can/new.cpp 

C_DEPS += \
./at90can/WInterrupts.d \
./at90can/pins_arduino.d \
./at90can/wiring.d \
./at90can/wiring_analog.d \
./at90can/wiring_digital.d \
./at90can/wiring_pulse.d \
./at90can/wiring_shift.d 

OBJS += \
./at90can/HardwareSerial.o \
./at90can/Print.o \
./at90can/Stream.o \
./at90can/WInterrupts.o \
./at90can/WMath.o \
./at90can/WString.o \
./at90can/can_lib.o \
./at90can/new.o \
./at90can/pins_arduino.o \
./at90can/wiring.o \
./at90can/wiring_analog.o \
./at90can/wiring_digital.o \
./at90can/wiring_pulse.o \
./at90can/wiring_shift.o 

CPP_DEPS += \
./at90can/HardwareSerial.d \
./at90can/Print.d \
./at90can/Stream.d \
./at90can/WMath.d \
./at90can/WString.d \
./at90can/can_lib.d \
./at90can/new.d 


# Each subdirectory must supply rules for building sources it contributes
at90can/%.o: ../at90can/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Users/thomaswillson/Documents/workspace/FE2015Dashboard/FTDI _V1" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

at90can/%.o: ../at90can/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


