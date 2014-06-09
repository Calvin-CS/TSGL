################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp 

OBJS += \
./main.o 

CPP_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/home/mbv26/summer2014/graphicOMP.git/libraries/include -I/opt/intel/composer_xe_2013_sp1.0.080/mkl/include -I/opt/intel/composer_xe_2013_sp1.0.080/tbb/include -I/usr/include -I/usr/include/c++/4.6 -O0 -g3 -Wall -c -fmessage-length=0 -fopenmp -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


