AR=ar
CC=g++
RM=rm -f

SRC_PATH=src/TSGL/
TESTS_PATH=src/tests/
OBJ_PATH=build/

VPATH=SRC_PATH:TESTS_PATH:OBJ_PATH

HEADERS := $(wildcard src/TSGL/*.h)
SOURCES := $(wildcard src/TSGL/*.cpp)
TESTS   := $(wildcard src/tests/*.cpp)
OBJS := $(patsubst src/TSGL/%.cpp,build/TSGL/%.o,${SOURCES})
TESTOBJS:= $(patsubst src/tests/%.cpp,build/tests/%.o,${TESTS})

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_GLFW := glfw
	OS_GL := -lGL
endif
ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_GLFW := glfw3
	OS_GL :=
endif

CXXFLAGS=-O3 -g3 \
	-Wall -Wextra \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.1.1/include/ \
	-Isrc/TSGL/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/opt/AMDAPP/include/ \
	-I/opt/local/include/freetype2 \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-I./ -I/usr/include/freetype2  \
	-I/usr/local/include/freetype2  \
  -std=c++0x -fopenmp \
  -Wno-narrowing -fpermissive
  # -pedantic-errors

LFLAGS=-LTSGL/ -ltsgl \
	-Llib/ \
	-L/opt/local/lib/ \
	-L/usr/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	-ltsgl -lfreetype -lGLEW -l${OS_GLFW} \
	-lX11 ${OS_GL} -lXrandr \
	-fopenmp ${OS_LFLAGS}
	# -lpng -ljpeg \

DEPFLAGS=-MMD -MP

BINARIES= bin/testTSGL bin/testInverter bin/testGraydient bin/testColorPoints \
	bin/testLineChain bin/testLineFan bin/testSpectrum bin/testMandelbrot \
	bin/testLangton bin/testLangtonColony bin/testLangtonRainbow \
	bin/testDumbSort bin/testColorWheel bin/testFunction \
	bin/testCosineIntegral bin/testGradientWheel bin/testAlphaRectangle \
	bin/testAlphaLangton bin/testGradientMandelbrot bin/testNova \
	bin/testVoronoi bin/testShadedVoronoi bin/testForestFire bin/testImage \
	bin/testHighData bin/testText bin/testTextTwo bin/testPong \
	bin/testImageCart bin/testTextCart bin/testGetPixels bin/testScreenshot \
	bin/testScreenshotLangton bin/testGreyscale bin/testMouse \
	bin/testConcavePolygon bin/testNewtonPendulum bin/testConway \
	bin/testProjectiles bin/testBallroom bin/testUnits bin/testSmartSort bin/testSeaUrchin \
	bin/testBuddhabrot bin/testMaster

all: dif tsgl tests docs tutorial

debug: dif tsgl tests

dif: build/build

tsgl: lib/libtsgl.a

tests: ${BINARIES}

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

#List dependencies for test binaries

bin/testTSGL: build/tests.o

bin/testConway: build/tests/Conway/LifeFarm.o

LANGTON_DEPS=build/tests/Langton/AntFarm.o build/tests/Langton/LangtonAnt.o
bin/testAlphaLangton: ${LANGTON_DEPS}
bin/testLangtonColony: ${LANGTON_DEPS}
bin/testLangtonRainbow: ${LANGTON_DEPS}
bin/testScreenshotLangton: ${LANGTON_DEPS}
bin/testLangton: ${LANGTON_DEPS}

bin/testVoronoi: build/tests/Voronoi.o
bin/testShadedVoronoi: build/tests/ShadedVoronoi.o

bin/testMandelbrot: build/tests/Mandelbrot/Mandelbrot.o
bin/testGradientMandelbrot: build/tests/Mandelbrot/Mandelbrot.o build/tests/Mandelbrot/GradientMandelbrot.o
bin/testBuddhabrot: build/tests/Mandelbrot/Mandelbrot.o build/tests/Mandelbrot/Buddhabrot.o

#Actual compilation recipe for test binaries (appended to earlier dependencies)

bin/test%: build/tests/test%.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

build/%.o: src/%.cpp
	mkdir -p ${@D}
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/tests.o: src/tests/tests.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/tests/test%.o: src/tests/test%.cpp
	mkdir -p ${@D}
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

docs/html/index.html: ${HEADERS} Doxyfile
	@echo 'Generating Doxygen'
	@doxygen

tutorial/docs/html/index.html: ${HEADERS} TutDoxyfile
	@echo 'Generating Doxygen'
	mkdir -p tutorial/docs/html
	doxygen TutDoxyfile

.PHONY: all debug clean tsgl tests docs tutorial dif
.SECONDARY: ${OBJS} ${TESTOBJS} build/tests.o $(OBJS:%.o=%.d)

