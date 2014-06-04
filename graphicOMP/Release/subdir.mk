################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Canvas.cpp \
../Point.cpp \
../Shape.cpp \
../line.cpp \
../main.cpp 

OBJS += \
./Canvas.o \
./Point.o \
./Shape.o \
./line.o \
./main.o 

CPP_DEPS += \
./Canvas.d \
./Point.d \
./Shape.d \
./line.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


