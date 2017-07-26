#Target
TARGET = Langton
all: $(TARGET)

#Directories
SRCDIR = ../../src
TSGLSRCDIR = $(SRCDIR)/TSGL
BINDIR = ../../bin
BLDDIR = ../../build/tests/$(TARGET)
GLAD = ../../build/src/glad.o

#Library Files
TSGLSRC := $(wildcard $(TSGLSRCDIR)/*.cpp)
TSGLOBJ := $(addprefix ../../build/src/TSGL/, $(notdir $(TSGLSRC:.cpp=.o)))
TSGLDEP := $(TSGLOBJ:.o=.d)

#Target Program Files
SRC := $(wildcard *.cpp)
OBJ := $(addprefix $(BLDDIR)/, $(SRC:.cpp=.o))
DEP := $(OBJ:.o=.d)

# OS Check
UNAME := $(shell uname)



# Commands
ifeq ($(UNAME), Linux)
CC = g++
RM = rm -f
endif

ifeq ($(UNAME), Darwin)
CC = g++-7
RM = rm -f
endif


#Linker flags
# Linux
ifeq ($(UNAME), Linux)
LFLAGS = \
	-Llib/ \
	-L/usr/local/lib \
	-L/usr/lib \
	-L/usr/X11/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-fopenmp -lfreetype -ldl -lm -lGLEW -lglfw -lX11 -lGL -lXrandr
endif


# MacOS
ifeq ($(UNAME), Darwin)
LFLAGS = \
	-Llib/ -L/usr/local/lib  -L/usr/X11/lib/ \
	-lfreetype -lGLEW -lglfw -lX11  -lXrandr -fopenmp \
	-framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
endif

#Compiler flags
CFLAGS = \
	-std=c++0x \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.1.1/include/ \
	-I../../include/ \
	-I../../src/ \
	-I../../src/TSGL/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/usr/local/include/freetype2 \
	-I/usr/local/include/freetype2/freetype \
	-I/opt/AMDAPP/include/ \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-I/usr/include/freetype2 \
	-I/usr/include/freetype2/freetype \
	-I./ \
	-I../include/TSGL/ \
	-O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fopenmp \
	-Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fpermissive -fPIC



#Link from obj files
$(TARGET): $(OBJ) $(TSGLOBJ)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking $@ Example ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(OBJ) $(GLAD) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

# Include header dependencies
-include $(TSGLDEP)
-include $(DEP)

#Building rule for cpp to o
$(BLDDIR)/%.o: %.cpp
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Building $@ +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) $< -o $@

clean:
		$(RM) -r bin/* build/* lib/* tutorial/docs/html/* *~ *# *.tmp


#Linking
