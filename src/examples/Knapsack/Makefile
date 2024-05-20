#Target to make and objects needed
TARGET := Knapsack
OBJS := $(TARGET).o

#Compiler, remove command, and OS we're working on
CC = g++-11
RM = rm -f
UNAME := $(shell uname)

#Check if we're on a Mac or a Linux machine

#Linux
ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_INCLUDE :=
	OS_LDIRS := -L/opt/AMDAPP/lib/x86_64/
	OS_EXTRA_LIB := -L$(TSGL_HOME)/lib -ltsgl
	OS_GLFW := glfw
	OS_GL := -lGL
	OS_EXTRA_LINK := -Wl,-rpath=$(TSGL_HOME)/lib/
endif

#Mac
ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_INCLUDE :=  
	OS_LDIRS := 
	OS_EXTRA_LIB := 
	OS_GLFW := glfw.3
	OS_GL :=
	OS_EXTRA_LINK :=
endif

#Compiler flags
CXXFLAGS=-c -O3 -g3 -ggdb3 \
    -I$(TSGL_HOME)/include \
    -I$(TSGL_HOME)/include/TSGL \
    -I$(TSGL_HOME)/include/freetype2 \
    -std=c++11 -fopenmp \

#Linking flags
LFLAGS=-o $(TARGET) \
    -L$(TSGL_HOME)/lib \
    $(TSGL_HOME)/lib/libtsgl.a \
	${OS_EXTRA_LIB} \
    -L/opt/X11/lib \
    ${OS_LDIRS} \
    ${OS_EXTRA_LINK} \
    -lfreetype \
    -lGLEW -l${OS_GLFW} \
    -lX11 ${OS_GL} -lXrandr \
    -fopenmp \
    ${OS_LFLAGS} \

.SUFFIXES: .cpp .o

#all command
all: $(TARGET)

#Linking
$(TARGET): $(OBJS)
	@echo "\nLinking $(TARGET)..."
	$(CC) $(OBJS) $(LFLAGS)
	@echo
	
#Compiling
.cpp.o:
	@echo "\nCompiling $<..."
	$(CC) $(CXXFLAGS) $< 

#Indirect dependencies (Multiple file/classes only)
#$(TARGET).o:

#Clean command
clean:
	$(RM) $(TARGET) $(OBJS) *~
