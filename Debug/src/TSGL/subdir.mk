################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TSGL/Arrow.cpp \
../src/TSGL/Canvas.cpp \
../src/TSGL/CartesianCanvas.cpp \
../src/TSGL/Circle.cpp \
../src/TSGL/Color.cpp \
../src/TSGL/ConcavePolygon.cpp \
../src/TSGL/ConvexPolygon.cpp \
../src/TSGL/Error.cpp \
../src/TSGL/Image.cpp \
../src/TSGL/IntegralViewer.cpp \
../src/TSGL/Line.cpp \
../src/TSGL/Polyline.cpp \
../src/TSGL/ProgressBar.cpp \
../src/TSGL/Rectangle.cpp \
../src/TSGL/Spectrogram.cpp \
../src/TSGL/Text.cpp \
../src/TSGL/TextureHandler.cpp \
../src/TSGL/Timer.cpp \
../src/TSGL/Triangle.cpp \
../src/TSGL/TriangleStrip.cpp \
../src/TSGL/VisualTaskQueue.cpp 

OBJS += \
./src/TSGL/Arrow.o \
./src/TSGL/Canvas.o \
./src/TSGL/CartesianCanvas.o \
./src/TSGL/Circle.o \
./src/TSGL/Color.o \
./src/TSGL/ConcavePolygon.o \
./src/TSGL/ConvexPolygon.o \
./src/TSGL/Error.o \
./src/TSGL/Image.o \
./src/TSGL/IntegralViewer.o \
./src/TSGL/Line.o \
./src/TSGL/Polyline.o \
./src/TSGL/ProgressBar.o \
./src/TSGL/Rectangle.o \
./src/TSGL/Spectrogram.o \
./src/TSGL/Text.o \
./src/TSGL/TextureHandler.o \
./src/TSGL/Timer.o \
./src/TSGL/Triangle.o \
./src/TSGL/TriangleStrip.o \
./src/TSGL/VisualTaskQueue.o 

CPP_DEPS += \
./src/TSGL/Arrow.d \
./src/TSGL/Canvas.d \
./src/TSGL/CartesianCanvas.d \
./src/TSGL/Circle.d \
./src/TSGL/Color.d \
./src/TSGL/ConcavePolygon.d \
./src/TSGL/ConvexPolygon.d \
./src/TSGL/Error.d \
./src/TSGL/Image.d \
./src/TSGL/IntegralViewer.d \
./src/TSGL/Line.d \
./src/TSGL/Polyline.d \
./src/TSGL/ProgressBar.d \
./src/TSGL/Rectangle.d \
./src/TSGL/Spectrogram.d \
./src/TSGL/Text.d \
./src/TSGL/TextureHandler.d \
./src/TSGL/Timer.d \
./src/TSGL/Triangle.d \
./src/TSGL/TriangleStrip.d \
./src/TSGL/VisualTaskQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/TSGL/%.o: ../src/TSGL/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/include/Cellar/glfw3/3.1.1/include/ -Isrc/TSGL/ -I/usr/include/ -I/usr/local/include/ -I/usr/local/include/freetype2 -I/usr/local/include/freetype2/freetype -I/opt/AMDAPP/include/ -I/usr/include/c++/4.6/ -I/usr/include/c++/4.6/x86_64-linux-gnu/ -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -I./ -I../include/TSGL/ -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fopenmp -Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fpermissive -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


