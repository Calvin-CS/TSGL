
OBJS=build/Canvas.o build/CartesianCanvas.o build/Color.o build/ColoredPolygon.o build/Image.o \
	build/Line.o build/Polyline.o build/Rectangle.o build/Text.o build/TextureHandler.o \
	build/Timer.o build/Triangle.o

HEADERS=src/TSGL/Canvas.h src/TSGL/CartesianCanvas.h src/TSGL/Color.h src/TSGL/ColoredPolygon.h src/TSGL/Image.h \
	src/TSGL/Line.h src/TSGL/Polyline.h src/TSGL/Rectangle.h src/TSGL/Text.h src/TSGL/TextureHandler.h \
	src/TSGL/Timer.h src/TSGL/Triangle.h

CC=g++

CFLAGS=-Wall -D__GXX_EXPERIMENTAL_CXX0X__ -c \
	-Isrc/TSGL/ \
	-I/usr/include/ \
	-I/opt/AMDAPP/include/ \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-Ilibraries/include/ \
	-Ilibraries/include/freetype2/ \
        -std=c++0x -fopenmp

LFLAGS=-o bin/testTSGL -LTSGL/ -ltsgl \
	-Llib/ \
	-L/opt/local/lib \
	-L/usr/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-Llibraries/lib/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	-ltsgl -lfreetype -lpng -ljpeg \
	-lGLEW -lglfw \
	-lX11 -lGL -lXrandr \
	-fopenmp

all: tsgl tests docs

debug: tsgl tests

tsgl: lib/libtsgl.a

lib/libtsgl.a: $(OBJS) 
	ar -r "lib/libtsgl.a" $(OBJS)

build/%.o: src/TSGL/%.cpp
	echo "Compiling objs"
	$(CC) $(CFLAGS) -o "$@" "$<"

tests: bin/testTSGL

bin/testTSGL: build/tests.o lib/libtsgl.a
	$(CC) build/tests.o lib/libtsgl.a $(LFLAGS)

build/tests.o: src/tests/tests.cpp
	$(CC) $(CFLAGS) -o "$@" src/tests/tests.cpp

docs: html/index.html

html/index.html: $(HEADERS) Doxyfile
	doxygen

clean:
	rm -rf bin/* build/*.o docs/html/* lib/*.a lib/*.so*  *~ *# *.tmp

.PHONY: all debug clean tsgl tests docs
.SECONDARY: $(OBJS) build/tests.o

