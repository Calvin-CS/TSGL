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

LFLAGS=-o bin/testTSGL -LTSGL/ -ltsgl \
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

all: dif tsgl tests docs

debug: dif tsgl tests

dif: build/build

tsgl: lib/libtsgl.a

tests: bin/testTSGL

docs: docs/html/index.html

clean:
	$(RM) -r bin/* build/* docs/html/* lib/* *~ *# *.tmp

-include build/*.d

build/build: ${HEADERS} ${SOURCES} src/tests/tests.cpp
	@echo 'Files that changed:'
	@echo $(patsubst src/%,%,$?)

lib/libtsgl.a: ${OBJS}
	@echo 'Building $(patsubst lib/%,%,$@)'
	$(AR) -r $@ $?
	@touch build/build

bin/testTSGL: build/tests.o lib/libtsgl.a
	@echo 'Building $(patsubst bin/%,%,$@)'
	$(CC) $^ $(LFLAGS)
	@touch build/build

build/%.o: src/TSGL/%.cpp
	@echo 'Building $(patsubst src/TSGL/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

build/tests.o: src/tests/tests.cpp
	@echo 'Building $(patsubst src/tests/%,%,$<)'
	$(CC) -c $(CXXFLAGS) $(DEPFLAGS) -o "$@" "$<"

docs/html/index.html: ${HEADERS} Doxyfile
	@echo 'Generating Doxygen'
	@doxygen

.PHONY: all debug clean tsgl tests docs dif
.SECONDARY: ${OBJS} build/tests.o $(OBJS:%.o=%.d)

