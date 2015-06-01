AR=ar
CC=g++
RM=rm -f

SRC_PATH=src/TSGL/
TESTS_PATH=src/tests/
OBJ_PATH=build/

VPATH=SRC_PATH:TESTS_PATH:OBJ_PATH

HEADERS := $(wildcard src/TSGL/*.h)
SOURCES := $(wildcard src/TSGL/*.cpp)
OBJS := $(patsubst src/TSGL/%.cpp,build/TSGL/%.o,${SOURCES})

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
	$$(pkg-config --cflags freetype2) \
  -std=c++0x -fopenmp

LFLAGS=-LTSGL/ -ltsgl \
	-Llib/ \
	-L/opt/local/lib/ \
	-L/usr/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	-ltsgl -lfreetype -lpng -ljpeg \
	$$(pkg-config --libs glew) \
	$$(pkg-config --libs glfw3) \
	$$(pkg-config --libs gl) \
	-fopenmp

DEPFLAGS=-MMD -MP

BIN_RECIPE= \
	@echo 'Building $(patsubst bin/%,%,$@)' \
	$(CC) $^ -o $@ $(LFLAGS) \
	@touch build/build

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
	bin/testConcavePolygon bin/testNewtonPendulum bin/testClock bin/testConway

LANGTON_DEPS=build/tests/Langton/AntFarm.o build/tests/Langton/LangtonAnt.o lib/libtsgl.a


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

bin/testTSGL: build/tests.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

LANGTON_DEPS=build/tests/Langton/AntFarm.o build/tests/Langton/LangtonAnt.o lib/libtsgl.a

bin/testConway: build/tests/testConway.o build/tests/Conway/ConwayAnt.o build/tests/Conway/LifeFarm.o
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testAlphaLangton: build/tests/testAlphaLangton.o ${LANGTON_DEPS}
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testLangtonColony: build/tests/testLangtonColony.o ${LANGTON_DEPS}
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testLangtonRainbow: build/tests/testLangtonRainbow.o ${LANGTON_DEPS}
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testLangton: build/tests/testLangton.o ${LANGTON_DEPS}
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testVoronoi: build/tests/testVoronoi.o build/tests/Voronoi.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testShadedVoronoi: build/tests/testShadedVoronoi.o build/tests/ShadedVoronoi.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testMandelbrot: build/tests/testMandelbrot.o build/tests/Mandelbrot/Mandelbrot.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

bin/testGradientMandelbrot: build/tests/testGradientMandelbrot.o build/tests/Mandelbrot/Mandelbrot.o build/tests/Mandelbrot/GradientMandelbrot.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

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
.SECONDARY: ${OBJS} build/tests.o $(OBJS:%.o=%.d)

