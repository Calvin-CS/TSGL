AR=ar
CC=g++
RM=rm -f

SRC_PATH=src/TSGL/
TESTS_PATH=src/tests/
OBJ_PATH=build/

VPATH=SRC_PATH:TESTS_PATH:OBJ_PATH

HEADERS := $(wildcard src/TSGL/*.h)
SOURCES := $(wildcard src/TSGL/*.cpp)
OBJS := $(patsubst src/TSGL/%.cpp,build/%.o,${SOURCES})

CXXFLAGS=-O3 -g3 \
	-Wall -Wextra -pedantic-errors \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-Isrc/TSGL/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/opt/AMDAPP/include/ \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-I/usr/local/include/freetype2/ \
        -std=c++0x -fopenmp

LFLAGS=-LTSGL/ -ltsgl \
	-Llib/ \
	-L/opt/local/lib/ \
	-L/usr/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	-ltsgl -lfreetype -lpng -ljpeg \
	-lGLEW -lglfw \
	-lX11 -lGL -lXrandr \
	-fopenmp

DEPFLAGS=-MMD -MP

all: dif tsgl tests docs tutorial

debug: dif tsgl tests

dif: build/build

tsgl: lib/libtsgl.a

tests: bin/testTSGL bin/testInverter bin/testGraydient bin/testColorPoints bin/testLineChain bin/testLineFan bin/testSpectrum bin/testMandelbrot bin/testLangton bin/testLangtonColony bin/testLangtonRainbow bin/testDumbSort bin/testColorWheel bin/testFunction bin/testCosineIntegral bin/testGradientWheel bin/testAlphaRectangle bin/testAlphaLangton bin/testGradientMandelbrot bin/testNova bin/testVoronoi bin/testShadedVoronoi bin/testForestFire bin/testImage bin/testHighData bin/testText bin/testTextTwo bin/testPong bin/testImageCart bin/testTextCart bin/testGetPixels bin/testScreenshot bin/testScreenshotLangton bin/testGreyscale bin/testMouse bin/testConcavePolygon

docs: docs/html/index.html

tutorial: tutorial/docs/html/index.html

clean:
	$(RM) -r bin/* build/* docs/html/* lib/* tutorial/docs/html/* *~ *# *.tmp

-include build/*.d

build/build: ${HEADERS} ${SOURCES} src/tests/tests.cpp src/tests/testInverter.cpp
	@echo 'Files that changed:'
	@echo $(patsubst src/%,%,$?)

lib/libtsgl.a: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	$(AR) -r $@ $?
	@touch build/build

bin/testTSGL: build/tests.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testTSGL $(LFLAGS)
	@touch build/build
	
bin/testInverter: build/testInverter.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testInverter $(LFLAGS)
	@touch build/build
	
bin/testGraydient: build/testGraydient.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testGraydient $(LFLAGS)
	@touch build/build
	
bin/testColorPoints: build/testColorPoints.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testColorPoints $(LFLAGS)
	@touch build/build
	
bin/testLineChain: build/testLineChain.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testLineChain $(LFLAGS)
	@touch build/build
	
bin/testLineFan: build/testLineFan.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testLineFan $(LFLAGS)
	@touch build/build
	
bin/testSpectrum: build/testSpectrum.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testSpectrum $(LFLAGS)
	@touch build/build
	
bin/testMandelbrot: build/testMandelbrot.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testMandelbrot $(LFLAGS)
	@touch build/build
	
bin/testLangton: build/testLangton.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testLangton $(LFLAGS)
	@touch build/build
	
bin/testLangtonColony: build/testLangtonColony.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testLangtonColony $(LFLAGS)
	@touch build/build
	
bin/testLangtonRainbow: build/testLangtonRainbow.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testLangtonRainbow $(LFLAGS)
	@touch build/build
	
bin/testDumbSort: build/testDumbSort.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testDumbSort $(LFLAGS)
	@touch build/build
	
bin/testColorWheel: build/testColorWheel.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testColorWheel $(LFLAGS)
	@touch build/build
	
bin/testFunction: build/testFunction.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testFunction $(LFLAGS)
	@touch build/build
	
bin/testCosineIntegral: build/testCosineIntegral.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testCosineIntegral $(LFLAGS)
	@touch build/build
	
bin/testGradientWheel: build/testGradientWheel.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testGradientWheel $(LFLAGS)
	@touch build/build

bin/testAlphaRectangle: build/testAlphaRectangle.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testAlphaRectangle $(LFLAGS)
	@touch build/build
	
bin/testAlphaLangton: build/testAlphaLangton.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testAlphaLangton $(LFLAGS)
	@touch build/build
	
bin/testGradientMandelbrot: build/testGradientMandelbrot.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testGradientMandelbrot $(LFLAGS)
	@touch build/build
	
bin/testNova: build/testNova.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testNova $(LFLAGS)
	@touch build/build
	
bin/testVoronoi: build/testVoronoi.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testVoronoi $(LFLAGS)
	@touch build/build
	
bin/testShadedVoronoi: build/testShadedVoronoi.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testShadedVoronoi $(LFLAGS)
	@touch build/build
	
bin/testForestFire: build/testForestFire.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testForestFire $(LFLAGS)
	@touch build/build
	
bin/testImage: build/testImage.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testImage $(LFLAGS)
	@touch build/build
	
bin/testHighData: build/testHighData.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testHighData $(LFLAGS)
	@touch build/build
	
bin/testText: build/testText.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testText $(LFLAGS)
	@touch build/build
	
bin/testTextTwo: build/testTextTwo.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testTextTwo $(LFLAGS)
	@touch build/build
	
bin/testPong: build/testPong.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testPong $(LFLAGS)
	@touch build/build
	
bin/testImageCart: build/testImageCart.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testImageCart $(LFLAGS)
	@touch build/build
	
bin/testTextCart: build/testTextCart.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testTextCart $(LFLAGS)
	@touch build/build
	
bin/testGetPixels: build/testGetPixels.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testGetPixels $(LFLAGS)
	@touch build/build
	
bin/testScreenshot: build/testScreenshot.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testScreenshot $(LFLAGS)
	@touch build/build
	
bin/testScreenshotLangton: build/testScreenshotLangton.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testScreenshotLangton $(LFLAGS)
	@touch build/build
	
bin/testGreyscale: build/testGreyscale.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testGreyscale $(LFLAGS)
	@touch build/build
	
bin/testMouse: build/testMouse.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testMouse $(LFLAGS)
	@touch build/build
	
bin/testConcavePolygon: build/testConcavePolygon.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o bin/testConcavePolygon $(LFLAGS)
	@touch build/build
	
build/%.o: src/TSGL/%.cpp
	@echo 'Building $(patsubst src/TSGL/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/tests.o: src/tests/tests.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/testInverter.o: src/tests/testInverter.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/testGraydient.o: src/tests/testGraydient.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testColorPoints.o: src/tests/testColorPoints.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/testLineChain.o: src/tests/testLineChain.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testLineFan.o: src/tests/testLineFan.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testSpectrum.o: src/tests/testSpectrum.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testMandelbrot.o: src/tests/testMandelbrot.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testLangton.o: src/tests/testLangton.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testLangtonColony.o: src/tests/testLangtonColony.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testLangtonRainbow.o: src/tests/testLangtonRainbow.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testDumbSort.o: src/tests/testDumbSort.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testColorWheel.o: src/tests/testColorWheel.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testFunction.o: src/tests/testFunction.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testCosineIntegral.o: src/tests/testCosineIntegral.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testGradientWheel.o: src/tests/testGradientWheel.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testAlphaRectangle.o: src/tests/testAlphaRectangle.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testAlphaLangton.o: src/tests/testAlphaLangton.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testGradientMandelbrot.o: src/tests/testGradientMandelbrot.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testNova.o: src/tests/testNova.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testVoronoi.o: src/tests/testVoronoi.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testShadedVoronoi.o: src/tests/testShadedVoronoi.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testForestFire.o: src/tests/testForestFire.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testImage.o: src/tests/testImage.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testHighData.o: src/tests/testHighData.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testText.o: src/tests/testText.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testTextTwo.o: src/tests/testTextTwo.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testPong.o: src/tests/testPong.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testImageCart.o: src/tests/testImageCart.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testTextCart.o: src/tests/testTextCart.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testGetPixels.o: src/tests/testGetPixels.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testScreenshot.o: src/tests/testScreenshot.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testScreenshotLangton.o: src/tests/testScreenshotLangton.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testGreyscale.o: src/tests/testGreyscale.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testMouse.o: src/tests/testMouse.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
build/testConcavePolygon.o: src/tests/testConcavePolygon.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"
	
docs/html/index.html: ${HEADERS} Doxyfile
	@echo 'Generating Doxygen'
	@doxygen

tutorial/docs/html/index.html: ${HEADERS} TutDoxyfile
	@echo 'Generating Doxygen'
	@doxygen TutDoxyfile

.PHONY: all debug clean tsgl tests docs tutorial dif
.SECONDARY: ${OBJS} build/tests.o $(OBJS:%.o=%.d)

