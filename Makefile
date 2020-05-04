AR=ar
CC=g++
RM=rm -f
INSTALL=/usr/bin/install
PREFIX=/usr/local

SRC_PATH=src/TSGL/
TESTS_PATH=src/tests/
OBJ_PATH=build/
VPATH=SRC_PATH:TESTS_PATH:OBJ_PATH

HEADERS  := $(wildcard src/TSGL/*.h)
SOURCES  := $(wildcard src/TSGL/*.cpp)
TESTS    := $(wildcard src/tests/*.cpp)
OBJS     := $(patsubst src/TSGL/%.cpp,build/TSGL/%.o,${SOURCES})
TESTOBJS := $(patsubst src/tests/%.cpp,build/tests/%.o,${TESTS})
NOWARN   := -Wno-unused-parameter -Wno-unused-function -Wno-narrowing
UNAME    := $(shell uname)

ifeq ($(UNAME), Linux)
	OS_LFLAGS := -lpthread
	OS_LDIRS := -L/opt/AMDAPP/lib/x86_64/
	OS_EXTRA_LIB := -L/usr/lib
	OS_GLFW := glfw
	OS_GL := -lGL
endif

ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_LDIRS :=
	OS_EXTRA_LIB := 
	OS_GLFW := glfw
	OS_GL :=
endif

CXXFLAGS=-O3 -g3 -ggdb3 \
	-Wall -Wextra \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.3/include/ \
	-I${SRC_PATH}/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/usr/local/include/freetype2 \
	-I/usr/local/include/freetype2/freetype \
	-I/opt/AMDAPP/include/ \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/9/include/ \
	-I/usr/include/freetype2  \
	-I/usr/include/freetype2/freetype  \
	-I./ \
  -std=c++0x -fopenmp \
  ${NOWARN} -fpermissive
  # -pedantic-errors

LFLAGS=-Llib/ \
	-L/usr/local/lib \
	${OS_EXTRA_LIB} \
	-L/usr/X11/lib/ \
	${OS_LDIRS} \
	-ltsgl -lfreetype -lGLEW -lglfw -l${OS_GLFW} \
	-lX11 ${OS_GL} -lXrandr -Xpreprocessor -fopenmp -lomp -I"$(brew --prefix libomp)/include" \
	${OS_LFLAGS} 

DEPFLAGS=-MMD -MP

BINARIES= \
	bin/test3DRotation \
	bin/testArrows \
	bin/testBallroom \
	bin/testCone \
	bin/testConvexPolygon \
	bin/testConcavePolygon \
	bin/testCube \
	bin/testCuboid \
	bin/testCylinder \
	bin/testCircle \
	bin/testEllipse \
	bin/testEllipsoid \
	bin/testLines \
	bin/testPrism \
	bin/testPyramid \
	bin/testRectangle \
	bin/testRegularPolygon \
	bin/testSolarSystem \
	bin/testSphere \
	bin/testSquare \
	bin/testStar \
	bin/testTriangle \
	bin/testTriangleStrip \
#  bin/test_specs \
#	bin/testAlphaRectangle \
#	bin/testArrows \
#	bin/testAura \
#	bin/testBlurImage \
#	bin/testCalcPi \
#	bin/testColorPoints \
#	bin/testColorWheel \
#	bin/testConcavePolygon \
#	bin/testConstructors \
#	bin/testConway \
#	bin/testCosineIntegral \
#	bin/testDumbSort \
#	bin/testFireworks \
#	bin/testForestFire \
#	bin/testFunction \
#	bin/testGetPixels \
#	bin/testGradientWheel \
#	bin/testGraydient \
#	bin/testGreyscale \
#	bin/testHighData \
#	bin/testImage \
#	bin/testImageCart \
#	bin/testInverter \
#	bin/testLangton \
#	bin/testLineChain \
#	bin/testLineFan \
#	bin/testMandelbrot \
#	bin/testMouse \
#	bin/testNewtonPendulum \
#	bin/testPhilosophers \
#	bin/testProducerConsumer \
#	bin/testPong \
#	bin/testProgressBar \
#	bin/testProjectiles \
#	bin/testReaderWriter \
#	bin/testRotation \
#	bin/testScreenshot \
#	bin/testSeaUrchin \
#	bin/testSmartSort \
#	bin/testSpectrogram \
#	bin/testSpectrum \
#	bin/testStar \
#	bin/testText \
#	bin/testTextCart \
#	bin/testTextTwo \
#	bin/testUnits \
#	bin/testVoronoi

#Use make tsgl to make only the library files
all: dif tsgl tests docs tutorial

debug: dif tsgl tests

dif: build/build

#This may change (for the Mac installer)!
tsgl: lib/libtsgl.a lib/libtsgl.so

tests: ${BINARIES}

docs: docs/html/index.html

tutorial: tutorial/docs/html/index.html

cleanall: clean cleandocs

clean:
	$(RM) -r bin/* build/* lib/* tutorial/docs/html/* *~ *# *.tmp

cleandocs:
	$(RM) -r docs/html/*

# -include build/*.d

install:
	test -d $(PREFIX) || mkdir $(PREFIX)
	test -d $(PREFIX)/lib || mkdir $(PREFIX)
	test -d $(PREFIX)/include || mkdir $(PREFIX)
	install -m 0644 lib/libtsgl.a $(PREFIX)/lib
	install -m 0755 lib/libtsgl.so $(PREFIX)/lib
	cp -r src/TSGL $(PREFIX)/include

build/build: ${HEADERS} ${SOURCES} ${TESTS}
	@echo 'Files that changed:'
	@echo $(patsubst src/%,%,$?)

ifeq ($(UNAME), Linux)
lib/libtsgl.so: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	$(CC) -shared -o $@ $?
	@touch build/build
endif

ifeq ($(UNAME), Darwin)
lib/libtsgl.so: ${OBJS}
	@echo 'Building $(pathsubst lib/%,%,$@)'
	$(CC) -shared -undefined suppress -flat_namespace -o $@ $?
	@touch build/build
endif

lib/libtsgl.a: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	mkdir -p lib
	$(AR) -r $@ $?
	@touch build/build

#List additional dependencies for test binaries
bin/testConway: build/tests/Conway/LifeFarm.o
bin/testFireworks: build/tests/Fireworks/Arc.o \
		   build/tests/Fireworks/Dot.o \
		   build/tests/Fireworks/Firework.o
bin/testInverter: build/tests/ImageInverter/ImageInverter.o
bin/testLangton: build/tests/Langton/AntFarm.o build/tests/Langton/LangtonAnt.o
bin/testMandelbrot: build/tests/Mandelbrot/Mandelbrot.o \
	build/tests/Mandelbrot/GradientMandelbrot.o \
	build/tests/Mandelbrot/Buddhabrot.o \
	build/TSGL/VisualTaskQueue.o \
	build/tests/Mandelbrot/Julia.o \
	build/tests/Mandelbrot/Nova.o
bin/testPhilosophers: build/tests/DiningPhilosophers/Philosopher.o \
	build/tests/DiningPhilosophers/Table.o
bin/testPong: build/tests/Pong/Pong.o build/tests/Pong/Paddle.o build/tests/Pong/Ball.o
bin/testProducerConsumer: build/tests/ProducerConsumer/Producer.o \
	build/tests/ProducerConsumer/Consumer.o \
	build/tests/ProducerConsumer/Thread.o \
	build/tests/ProducerConsumer/PCThread.o
bin/testReaderWriter: build/tests/ReaderWriter/FLock.o \
	build/tests/ReaderWriter/Lock.o \
	build/tests/ReaderWriter/Reader.o \
	build/tests/ReaderWriter/RLock.o \
	build/tests/ReaderWriter/RWThread.o \
	build/tests/ReaderWriter/Thread.o \
	build/tests/ReaderWriter/WLock.o \
	build/tests/ReaderWriter/Writer.o
bin/testSeaUrchin: build/tests/SeaUrchin/SeaUrchin.o
bin/testVoronoi: build/tests/Voronoi/Voronoi.o build/tests/Voronoi/ShadedVoronoi.o

#General compilation recipes for test binaries (appended to earlier dependencies)
bin/test%: build/tests/test%.o lib/libtsgl.a
	mkdir -p bin
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ -o $@ $(LFLAGS)
	@touch build/build

build/%.o: src/%.cpp
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Building $@ +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	mkdir -p ${@D}
	$(CC) -c -fpic $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"



#Doxygen stuff
docs/html/index.html: ${HEADERS} doxyfile
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Generating Doxygen +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	mkdir -p docs
	@doxygen doxyfile

tutorial/docs/html/index.html: ${HEADERS} tutDoxyFile
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Generating Doxygen +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	mkdir -p tutorial/docs
	doxygen tutDoxyFile

.PHONY: all debug clean tsgl tests docs tutorial dif
.SECONDARY: ${OBJS} ${TESTOBJS} $(OBJS:%.o=%.d)
