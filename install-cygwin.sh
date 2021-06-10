#!/bin/bash
#
# install-cygwin.sh is the installation script for TSGL on Cygwin.
# Last updated: 07/14/2020
#
# -SUBJECT TO CHANGE-
################################################################

#install apt-cyg using lynx to download packages
lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
install apt-cyg /bin

#install apt-cyg using curl
#curl https://raw.githubusercontent.com/transcode-open/apt-cyg/master/apt-cyg > /cygdrive/c/cygwin64/bin/apt-cyg
#cd /cygdrive/c/cygwin64/bin
#chmod +x apt-cyg

#install necessary packages
echo "*******************************************************************************"
echo "Installing necessary packages..."
echo
echo

apt-cyg install make cmake gcc-g++ libXinerama-devel libXcursor-devel libXi-devel libfreetype-devel libfreetype-doc libfreetype6 libGL-devel libGL1 xorg-server xinit xeyes glm-devel libglut-devel libglut3 openmpi glew libGLEW-devel libXrandr-devel mingw64-i686-freetype2 mingw64-x86_64-freetype2 libopenmpi-devel libopenmpi12 libopenmpicxx1 doxygen-doxywizard dos2unix

echo
echo
echo "All necessary packages installed"
echo "*******************************************************************************"

echo "Checking for dependencies..."
glfwFile=/usr/lib/libglfw.dll.a

if [ -f "$glfwFile" ]
then
        echo "glfw dependency resolved"
else
        #clone the repository and install glfw
        git clone https://github.com/glfw/glfw.git

        cd glfw

        cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBUILD_SHARED_LIBS=ON

        make

        make install

        cd ..

        rm -rf glfw

        cd /usr/lib

        ln -s /usr/lib/libglfw.dll.a

        echo "glfw dependecy resolved"
fi
echo "All necessary dependencies resolved"

echo "Installing TSGL..."
echo

#Clean install = remove the TSGL folder and lib files if they already exist
rm -rf /usr/include/TSGL
rm -rf /usr/lib/libtsgl.*

#Create the following directories (Since they aren't included in github but are needed)
mkdir -p lib bin

#Make the library
make

#Install it
make install

#Take out the .cpp files from the TSGL library package folder
rm -rf /usr/include/TSGL/*.cpp

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo
echo
#check for update and install if available
echo "Checking for updates..."

TSGL_VERSION=$(git describe --tags --abbrev=0)
TSGL_LATEST_VERSION=$(git describe --tags $(git rev-list --tags --max-count=1))

if [ $TSGL_VERSION < $TSGL_LATEST_VERSION ]
then
        echo "Latest version $TSGL_LATEST_VERSION found. WARNING, If you have changed anything in the TSGL folder it may be overwritten during update. To keep your changes, please commit them before updating."
        read -p "Do you want to install the update? This will replace all the files with the updated ones (y/n): " INPUT
	if [ $INPUT == y ] || [ $INPUT == Y ]
        then
                echo "Updating TSGL..."
                git remote add tsgl https://github.com/Calvin-CS/TSGL.git
                git pull tsgl master
        fi
else
        echo "Latest version already installed"
fi
