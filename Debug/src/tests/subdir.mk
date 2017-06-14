################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tests/testAlphaRectangle.cpp \
../src/tests/testAura.cpp \
../src/tests/testBallroom.cpp \
../src/tests/testBlurImage.cpp \
../src/tests/testCalcPi.cpp \
../src/tests/testColorPoints.cpp \
../src/tests/testColorWheel.cpp \
../src/tests/testConcavePolygon.cpp \
../src/tests/testConway.cpp \
../src/tests/testCosineIntegral.cpp \
../src/tests/testDumbSort.cpp \
../src/tests/testFireworks.cpp \
../src/tests/testForestFire.cpp \
../src/tests/testFunction.cpp \
../src/tests/testGetPixels.cpp \
../src/tests/testGradientWheel.cpp \
../src/tests/testGraydient.cpp \
../src/tests/testGreyscale.cpp \
../src/tests/testHighData.cpp \
../src/tests/testImage.cpp \
../src/tests/testImageCart.cpp \
../src/tests/testInverter.cpp \
../src/tests/testLangton.cpp \
../src/tests/testLineChain.cpp \
../src/tests/testLineFan.cpp \
../src/tests/testMandelbrot.cpp \
../src/tests/testMouse.cpp \
../src/tests/testNewtonPendulum.cpp \
../src/tests/testPhilosophers.cpp \
../src/tests/testPong.cpp \
../src/tests/testProducerConsumer.cpp \
../src/tests/testProgressBar.cpp \
../src/tests/testProjectiles.cpp \
../src/tests/testScreenshot.cpp \
../src/tests/testSeaUrchin.cpp \
../src/tests/testSmartSort.cpp \
../src/tests/testSpectrogram.cpp \
../src/tests/testSpectrum.cpp \
../src/tests/testText.cpp \
../src/tests/testTextCart.cpp \
../src/tests/testTextTwo.cpp \
../src/tests/testUnits.cpp \
../src/tests/testVoronoi.cpp \
../src/tests/test_specs.cpp 

OBJS += \
./src/tests/testAlphaRectangle.o \
./src/tests/testAura.o \
./src/tests/testBallroom.o \
./src/tests/testBlurImage.o \
./src/tests/testCalcPi.o \
./src/tests/testColorPoints.o \
./src/tests/testColorWheel.o \
./src/tests/testConcavePolygon.o \
./src/tests/testConway.o \
./src/tests/testCosineIntegral.o \
./src/tests/testDumbSort.o \
./src/tests/testFireworks.o \
./src/tests/testForestFire.o \
./src/tests/testFunction.o \
./src/tests/testGetPixels.o \
./src/tests/testGradientWheel.o \
./src/tests/testGraydient.o \
./src/tests/testGreyscale.o \
./src/tests/testHighData.o \
./src/tests/testImage.o \
./src/tests/testImageCart.o \
./src/tests/testInverter.o \
./src/tests/testLangton.o \
./src/tests/testLineChain.o \
./src/tests/testLineFan.o \
./src/tests/testMandelbrot.o \
./src/tests/testMouse.o \
./src/tests/testNewtonPendulum.o \
./src/tests/testPhilosophers.o \
./src/tests/testPong.o \
./src/tests/testProducerConsumer.o \
./src/tests/testProgressBar.o \
./src/tests/testProjectiles.o \
./src/tests/testScreenshot.o \
./src/tests/testSeaUrchin.o \
./src/tests/testSmartSort.o \
./src/tests/testSpectrogram.o \
./src/tests/testSpectrum.o \
./src/tests/testText.o \
./src/tests/testTextCart.o \
./src/tests/testTextTwo.o \
./src/tests/testUnits.o \
./src/tests/testVoronoi.o \
./src/tests/test_specs.o 

CPP_DEPS += \
./src/tests/testAlphaRectangle.d \
./src/tests/testAura.d \
./src/tests/testBallroom.d \
./src/tests/testBlurImage.d \
./src/tests/testCalcPi.d \
./src/tests/testColorPoints.d \
./src/tests/testColorWheel.d \
./src/tests/testConcavePolygon.d \
./src/tests/testConway.d \
./src/tests/testCosineIntegral.d \
./src/tests/testDumbSort.d \
./src/tests/testFireworks.d \
./src/tests/testForestFire.d \
./src/tests/testFunction.d \
./src/tests/testGetPixels.d \
./src/tests/testGradientWheel.d \
./src/tests/testGraydient.d \
./src/tests/testGreyscale.d \
./src/tests/testHighData.d \
./src/tests/testImage.d \
./src/tests/testImageCart.d \
./src/tests/testInverter.d \
./src/tests/testLangton.d \
./src/tests/testLineChain.d \
./src/tests/testLineFan.d \
./src/tests/testMandelbrot.d \
./src/tests/testMouse.d \
./src/tests/testNewtonPendulum.d \
./src/tests/testPhilosophers.d \
./src/tests/testPong.d \
./src/tests/testProducerConsumer.d \
./src/tests/testProgressBar.d \
./src/tests/testProjectiles.d \
./src/tests/testScreenshot.d \
./src/tests/testSeaUrchin.d \
./src/tests/testSmartSort.d \
./src/tests/testSpectrogram.d \
./src/tests/testSpectrum.d \
./src/tests/testText.d \
./src/tests/testTextCart.d \
./src/tests/testTextTwo.d \
./src/tests/testUnits.d \
./src/tests/testVoronoi.d \
./src/tests/test_specs.d 


# Each subdirectory must supply rules for building sources it contributes
src/tests/%.o: ../src/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/include/Cellar/glfw3/3.1.1/include/ -Isrc/TSGL/ -I/usr/include/ -I/usr/local/include/ -I/usr/local/include/freetype2 -I/usr/local/include/freetype2/freetype -I/opt/AMDAPP/include/ -I/usr/include/c++/4.6/ -I/usr/include/c++/4.6/x86_64-linux-gnu/ -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -I./ -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fpermissive -fopenmp -Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


