#!/bin/bash

workingDir=$(pwd)

#Step 1: Install g++
sudo yum install gcc-c++

echo "g++ installed."

#Step 2: Build cmake from source 

#First, get the dependencies for cmake

#echo "Checking for cmake 3.0 or greater..."

sudo yum install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel mesa-libGLU-devel libXmu-devel libXi-devel libGL-devel 

wget www.cmake.org/files/v3.0/cmake-3.0.0.tar.gz

tar zxf cmake-3.0.0.tar.gz 

cd cmake-3.0.0

./bootstrap --prefix=/usr

make

sudo make install

echo "Cmake installed."

#Step 3: Build GLFW from source
echo "Installing GLFW..."

git clone https://www.github.com/glfw/glfw.git

cd glfw

cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DBUILD_SHARED_LIBS=ON

make

sudo make install

cd ../

sudo rm -rf glfw/

sudo cp /usr/local/lib/libglfw.so.3 /usr/lib64

echo "GLFW installed."

#Step 4: Build freetype from source
echo "Installing freetype..."

wget downloads.sourceforge.net/project/freetype/freetype2/2.6.3/freetype-2.6.3.tar.bz2

tar vxj freetype-2.6.3.tar.bz2

cd freetype-2.6.3

./configure

make 

sudo make install

#Here comes the fun part...
#Copy over files so TSGL can find most recent freetype files

cd /usr/local/include/

sudo cp -r freetype2/ ../../include/freetype2/

cd ../../include/freetype2/freetype2/freetype/

sudo cp -r * ../../freetype/

cd ../../freetype2/

sudo cp ft2build.h ../

cd ../

sudo rm -rf freetype2/

echo "Freetype installed."

#Make a symlink to GL.so file
sudo ln -s /usr/lib64/libGL.so.1 /usr/local/lib/libGl.so

#Edit the LD_LIBRARY_PATH variable
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/include:/usr/local/lib:/usr/lib:/usr/lib64

#Now, make and install TSGL!
echo "Installing TSGL..."

cd $workingDir

sudo rm -rf /usr/local/include/TSGL
sudo rm -rf /usr/local/lib/libtsgl.*

mkdir lib bin

make 

sudo make install

sudo rm -rf /usr/local/include/TSGL/*.cpp

sudo ldconfig

echo "Installation complete! Execute the runtests bash script to verify that everything works."

echo

