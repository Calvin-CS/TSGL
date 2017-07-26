#Directories
SRCDIR = src
TSGLSRCDIR = src/TSGL
VISPRJDIR = visualizations
TESTDIR = tests
BINDIR = bin
BLDDIR = build

#Header Files
HEADERS := $(wildcard $(SRCDIR)/*.h) $(wildcard $(TSGLSRCDIR)/*.h)
# TODO depend on headers!!!

#Library Files
TSGLSRC := $(wildcard $(TSGLSRCDIR)/*.cpp)  $(SRCDIR)/glad.cpp
TSGLOBJ := $(addprefix build/, $(TSGLSRC:.cpp=.o))
TSGLDEP := $(TSGLOBJ:.o=.d)

#Tester Program Files
PRGMSRC := $(wildcard $(SRCDIR)/main.cpp)
PRGMOBJ := $(addprefix build/, $(PRGMSRC:.cpp=.o))
PRGMDEP := $(PRGMOBJ:.o=.d)

#Reader/Writer Program Files
RWSRC := $(wildcard $(VISPRJDIR)/ReaderWriter/*.cpp)
RWOBJ := $(addprefix build/, $(RWSRC:.cpp=.o))
RWDEP := $(RWOBJ:.o=.d)

#Dining Philosophers Program Files
DPHSRC := $(wildcard $(VISPRJDIR)/DiningPhilosophers/*.cpp)
DPHOBJ := $(addprefix build/, $(DPHSRC:.cpp=.o))
DPHDEP := $(DPHOBJ:.o=.d)

#Producer/Consumer Program Files
PCSRC := $(wildcard $(VISPRJDIR)/ProducerConsumer/*.cpp)
PCOBJ := $(addprefix build/, $(PCSRC:.cpp=.o))
PCDEP := $(PCOBJ:.o=.d)

#Tests subfolders
# TESTFOLDERS := $(notdir $(shell find $(TESTDIR)/ -maxdepth 1 -type d))

#Tests
TESTPRGMS := $(basename $(notdir $(wildcard $(TESTDIR)/*.cpp)))
TESTSRC := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ := $(addprefix build/, $(TESTSRC:.cpp=.o))
TESTDEP := $(TESTOBJ:.o=.d)


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
	-Iinclude/ \
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
	-Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fpermissive -fPIC -pthread




all: tester ReaderWriter DiningPhilosophers ProducerConsumer sftests

#Test Program
tester: $(PRGMOBJ) $(TSGLOBJ) $(HEADERS)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Tester Program ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(PRGMOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Reader Writer
ReaderWriter: $(RWOBJ) $(TSGLOBJ)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Reader/Writer Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(RWOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Dining Philosophers
DiningPhilosophers: $(DPHOBJ) $(TSGLOBJ)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Dining Philosophers Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(DPHOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Producer Consumer
ProducerConsumer: $(PCOBJ) $(TSGLOBJ)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Producer/Consumer Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(PCOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))



#Build all single-file tests
sftests: $(TSGLOBJ) $(TESTPRGMS)

#Single-file test programs
$(TESTPRGMS): %: $(BLDDIR)/tests/%.o $(TSGLOBJ)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Test Program: $(@) ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(BLDDIR)/tests/$(@).o $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))




# Include header dependencies
-include $(TSGLDEP)
-include $(PRGMDEP)
-include $(RWDEP)
-include $(DPHDEP)
-include $(PCDEP)
-include $(TESTDEP)

#Building rule for cpp to o
$(BLDDIR)/%.o: %.cpp
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Building $@ +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) $< -o $@


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
