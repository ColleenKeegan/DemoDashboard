################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arduino/WInterrupts.c \
../arduino/pins_arduino.c \
../arduino/wiring.c \
../arduino/wiring_analog.c \
../arduino/wiring_digital.c \
../arduino/wiring_pulse.c \
../arduino/wiring_shift.c 

CPP_SRCS += \
../arduino/HardwareSerial.cpp \
../arduino/Print.cpp \
../arduino/Stream.cpp \
../arduino/WMath.cpp \
../arduino/WString.cpp \
../arduino/can_lib.cpp \
../arduino/new.cpp 

C_DEPS += \
./arduino/WInterrupts.d \
./arduino/pins_arduino.d \
./arduino/wiring.d \
./arduino/wiring_analog.d \
./arduino/wiring_digital.d \
./arduino/wiring_pulse.d \
./arduino/wiring_shift.d 

OBJS += \
./arduino/HardwareSerial.o \
./arduino/Print.o \
./arduino/Stream.o \
./arduino/WInterrupts.o \
./arduino/WMath.o \
./arduino/WString.o \
./arduino/can_lib.o \
./arduino/new.o \
./arduino/pins_arduino.o \
./arduino/wiring.o \
./arduino/wiring_analog.o \
./arduino/wiring_digital.o \
./arduino/wiring_pulse.o \
./arduino/wiring_shift.o 

CPP_DEPS += \
./arduino/HardwareSerial.d \
./arduino/Print.d \
./arduino/Stream.d \
./arduino/WMath.d \
./arduino/WString.d \
./arduino/can_lib.d \
./arduino/new.d 


# Each subdirectory must supply rules for building sources it contributes
arduino/%.o: ../arduino/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Users/thomaswillson/Documents/workspace/FE2015Dashboard/FTDI _V1" -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu++98 -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

arduino/%.o: ../arduino/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


