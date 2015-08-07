#Target to make and objects needed
TARGET = program
OBJS = $(TARGET).o

#Compiler, remove command, and OS we're working on
CC = g++
RM = rm -f
UNAME := $(shell uname)

#Check if we're on a Mac or a Linux machine

#Linux
ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_INCLUDE := -I/usr/local/include/freetype2/
	OS_LDIRS := -L/opt/AMDAPP/lib/x86_64/ 
	OS_GLFW := glfw
	OS_GL := -lGL
endif

#Mac
ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_INCLUDE := -I/opt/local/include/freetype2/
	OS_LDIRS := -L/opt/local/lib/ 
	OS_GLFW := glfw3
	OS_GL :=
endif

#Compiler flags
CXXFLAGS=-c -O3 -g3 \
    -Wall -Wextra -pedantic-errors \
    -D__GXX_EXPERIMENTAL_CXX0X__ \
    -I/usr/local/include/TSGL/ \
    -I/usr/local/include/ \
    -I/opt/AMDAPP/include/ \
    ${OS_INCLUDE} \
    -std=c++0x -fopenmp

#Linking flags
LFLAGS=-o $(TARGET) \
    -L/usr/local/lib/TSGL/ \
    -ltsgl -lfreetype \
    -lGLEW -l${OS_GLFW} \
    -lX11 ${OS_GL} -lXrandr \
    -Wl,-rpath=/usr/local/lib/ \
    -fopenmp \
    ${OS_LFLAGS} \
    ${OS_LDIRS} \

.SUFFIXES: .cpp .o

#all command
all: $(TARGET)

#Linking
$(TARGET): $(OBJS)
	@echo -e '\nLinking...'
	$(CC) $(OBJS) $(LFLAGS)

#Compiling
.cpp.o:
	@echo -e '\nCompiling...'
	$(CC) $(CXXFLAGS) $< 

#Indirect dependencies (Multiple file/classes only)
$(TARGET).o:

#Clean command
clean:
	$(RM) $(TARGET) $(OBJS) 
