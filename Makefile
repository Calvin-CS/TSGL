CC=g++

TARGET=shapes
FILE=shapes.cpp
OBJECT=shapes.o

ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_LDIRS := -L/opt/AMDAPP/lib/x86_64/
	OS_GLFW := glfw
	OS_GL := -lGL
endif

ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_LDIRS :=
	OS_GLFW := glfw3
	OS_GL :=
endif

LFLAGS=-Llib/ \
	-L/opt/local/lib/ \
	-L/usr/lib/ \
	-L/usr/local/lib/ \
	-L/usr/X11/lib/ \
	-ltsgl -lfreetype -lGLEW -l${OS_GLFW} \
	-lX11 ${OS_GL} -lXrandr -fopenmp \
	${OS_LDIRS} ${OS_LFLAGS}

$(TARGET): $(OBJECT)
		   $(CC) -o $@ $(OBJECT) $(LFLAGS)

$(OBJECT): $(FILE)
           $(CC) -c -o $@ $(FILE) 

 