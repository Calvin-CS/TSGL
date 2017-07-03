TARGET = "libtester"

#Directories
SRCDIR = src
TSGLSRCDIR = src/TSGL
VISPRJDIR = visualizations
BINDIR = bin
BLDDIR = build

#Files
# HEADERS := $(wildcard $(HDRDIR)*.h)
# TODO depend on headers!!!

#Library Files
TSGLSRC := $(wildcard $(TSGLSRCDIR)/*.cpp)
TSGLOBJ := $(addprefix build/, $(TSGLSRC:.cpp=.o))

#Tester Program Files
PRGMSRC := $(wildcard $(SRCDIR)/*.cpp)
PRGMOBJ := $(addprefix build/, $(PRGMSRC:.cpp=.o))

#Reader/Writer Program Files
RWSRC := $(wildcard $(VISPRJDIR)/ReaderWriter/*.cpp) $(SRCDIR)/glad.cpp
RWOBJ := $(addprefix build/, $(RWSRC:.cpp=.o))

#Dining Philosophers Program Files
DPHSRC := $(wildcard $(VISPRJDIR)/DiningPhilosophers/*.cpp) $(SRCDIR)/glad.cpp
DPHOBJ := $(addprefix build/, $(DPHSRC:.cpp=.o))

#Producer/Consumer Program Files
PCSRC := $(wildcard $(VISPRJDIR)/ProducerConsumer/*.cpp) $(SRCDIR)/glad.cpp
PCOBJ := $(addprefix build/, $(PCSRC:.cpp=.o))





# Commands
CC = g++
RM = rm -f



#Linker flags
LFLAGS = \
	-Llib/ \
	-L"/home/christiaan/workspace/TSGL/src/glad" \
	-L/usr/local/lib \
	-L/usr/lib \
	-L/usr/X11/lib/ \
	-L/opt/AMDAPP/lib/x86_64/ \
	-fopenmp -lfreetype -ldl -lm -lGLEW -lglfw -lX11 -lGL -lXrandr

#Compiler flags
CFLAGS = \
	-std=c++0x \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.1.1/include/ \
	-Isrc/ \
	-Isrc/TSGL/ \
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




all: tester ReaderWriter DiningPhilosophers ProducerConsumer

#Test Program
tester: $(PRGMOBJ) $(TSGLOBJ)
	@echo ""
	@echo "//////////////////// Building Tester Program ////////////////////"
	@echo ""
	$(CC) $(PRGMOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Reader Writer
ReaderWriter: $(RWOBJ) $(TSGLOBJ)
	@echo ""
	@echo "//////////////////// Building Reader/Writer Visualization ////////////////////"
	@echo ""
	$(CC) $(RWOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Dining Philosophers
DiningPhilosophers: $(DPHOBJ) $(TSGLOBJ)
	@echo ""
	@echo "//////////////////// Building Dining Philosophers Visualization ////////////////////"
	@echo ""
	$(CC) $(DPHOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Producer Consumer
ProducerConsumer: $(PCOBJ) $(TSGLOBJ)
	@echo ""
	@echo "//////////////////// Building Producer/Consumer Visualization ////////////////////"
	@echo ""
	$(CC) $(PCOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))



#Building rule for cpp to o
$(BLDDIR)/%.o: %.cpp
	@echo "//////////////////// Building $@ ////////////////////"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

# $(EXECUTABLE): $(OBJECTS)
# 	echo "--------------Linking Files--------------"
# 	$(CC) $(OBJECTS) $(LFLAGS)
#
# .cpp.o:
# 	echo $@
# 	$(CC) $(CFLAGS) $< -o build/$(notdir $@)
#

clean:
		$(RM) -r bin/* build/* lib/* tutorial/docs/html/* *~ *# *.tmp


#Linking
