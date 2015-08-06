TARGET = main
OBJS = $(TARGET).o

CC = g++
RM = rm -f

CXXFLAGS=-c -O3 -g3 \
    -Wall -Wextra -pedantic-errors \
    -D__GXX_EXPERIMENTAL_CXX0X__ \
    -I/usr/local/include/TSGL/ \
    -I/usr/local/include/ \
    -I/opt/AMDAPP/include/ \
    -I/usr/local/include/freetype2/ \
    -std=c++0x -fopenmp

LFLAGS=-o $(TARGET) \
    -L/usr/local/lib/TSGL/ \
    -L/opt/AMDAPP/lib/x86_64/ \
    -ltsgl -lfreetype \
    -Wl,-rpath=/usr/local/lib/ \
    -lGLEW -lglfw \
    -lX11 -lGL -lXrandr \
    -fopenmp

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo -e '\nLinking...'
	$(CC) $(OBJS) $(LFLAGS)

.cpp.o:
	@echo -e '\nCompiling...'
	$(CC) $(CXXFLAGS) $< 

$(TARGET).o:

clean:
	$(RM) $(TARGET) $(OBJS) 