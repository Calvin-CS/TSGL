#!/bin/bash

cowsay "Now building ProducerConsumer..."

echo "Compiling..." && echo
g++ *.cpp ../../src/TSGL/* -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/include/Cellar/glfw3/3.1.1/include/ -Isrc/TSGL/ -I/usr/include/ -I/usr/local/include/ -I/usr/local/include/freetype2 -I/usr/local/include/freetype2/freetype -I/opt/AMDAPP/include/ -I/usr/include/c++/4.6/ -I/usr/include/c++/4.6/x86_64-linux-gnu/ -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -I./ -I../../src/TSGL/ -I../../ -I../ -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fopenmp -Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fpermissive -fPIC -Llib/ -L/usr/local/lib -L/usr/lib -L/usr/X11/lib/ -L/opt/AMDAPP/lib/x86_64/ -fopenmp

echo "Linking..." && echo
g++ -Llib/ -L/usr/local/lib -L/usr/lib -L/usr/X11/lib/ -L/opt/AMDAPP/lib/x86_64/ -fopenmp *.o -o "ProducerConsumer" -lfreetype -lGLEW -lglfw -lX11 -lGL -lXrandr

echo "Removing build files..." && echo
rm *.o
rm ../../src/TSGL/*.h.gch
