################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tests/ImageInverter/ImageInverter.cpp 

OBJS += \
./src/tests/ImageInverter/ImageInverter.o 

CPP_DEPS += \
./src/tests/ImageInverter/ImageInverter.d 


# Each subdirectory must supply rules for building sources it contributes
src/tests/ImageInverter/%.o: ../src/tests/ImageInverter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/include/Cellar/glfw3/3.1.1/include/ -Isrc/TSGL/ -I/usr/include/ -I/usr/local/include/ -I/usr/local/include/freetype2 -I/usr/local/include/freetype2/freetype -I/opt/AMDAPP/include/ -I/usr/include/c++/4.6/ -I/usr/include/c++/4.6/x86_64-linux-gnu/ -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -I./ -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fpermissive -fopenmp -Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

