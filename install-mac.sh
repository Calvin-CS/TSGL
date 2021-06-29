#!/bin/bash
# Easy install script for TSGL on OS X
# Last updated: 08/10/16
#
# -SUBJECT TO CHANGE-
###############################

if [[ $1 ]]
then
	PREFIX=$1
	echo Install location $PREFIX
else
	echo Install location /usr/local
	PREFIX=/usr/local
fi

#Function for checking command availability
has () {
	(command -v "$1" >/dev/null 2>&1 && echo 1) || echo 0
}

#Install XQuartz
if [ $(ls /Applications/Utilities | grep XQuartz.app | wc -l) -eq 0 ]; then
	echo "Installing XQuartz..."
	URL=$(curl -s https://api.github.com/repos/XQuartz/XQuartz/releases | grep browser_download_url | head -n 1 | cut -d '"' -f 4)
	curl -L -O $URL 
	open -W XQuartz*.dmg
 	echo "Please continue the installation process by double-clicking XQuartz.pkg and following the instructions on screen."
	echo "Please enter to continue once XQuartz has finished installing."
	read
fi

if grep -s . XQuartz*.dmg
then
	rm XQuartz*.dmg
fi

#Install Xcode command line tools (will do nothing if already installed)
if [ ! -e "/Library/Developer/CommandLineTools" ]; then
	cmd="xcode-select --install"
	$cmd # Fix for vim syntax highlighting bugginess
	echo "Please select \"Install\" from the confirmation window that appears, and Agree to the license terms."
	echo "Press enter to continue once Xcode command line tools has finished installing"
	read
fi

#Install brew
if [ $(has brew) = 0 ]; then
	echo "Installing Homebrew..."
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
	#export PATH=/usr/bin:$PATH
else
	echo "Homebrew detected."
	#export PATH=/usr/bin:$PATH
fi

echo

echo "Using Homebrew to install dependencies..."

echo

brew update

#Install gcc
echo "Installing gcc..."
brew install gcc

#Install glfw3 (will do nothing if already installed)
echo "Installing GLFW..."
brew install glfw3

#GLEW
echo "Installing GLEW..."
brew install glew

#glm
echo "Installing glm..."
brew install glm

#Freetype
echo "Installing Freetype2..."
brew install freetype

#Doxygen
echo "Installing Doxygen..."
brew install doxygen

#libomp
echo "Installing libomp..."
brew install libomp

#cxxopts
echo "Installing cxxopts..."
brew install cxxopts

####################################################################################

#redefine path variable and append to bashrc
tsglFile=/usr/local/lib/libtsgl.so

if [ -f "$tsglFile" ]
then
	echo
	echo "Environment variable is already updated!" 
	source ~/.bashrc	
	
else
	echo "export PATH=/usr/local/bin:$PATH" >> ~/.bashrc
	echo "export TSGL_HOME=/usr/local" >> ~/.bashrc
	echo "export TSGL_DEFAULT_FONT=/include/TSGL/assets/freefont/FreeSansBold.ttf" >> ~/.bashrc

	source ~/.bashrc	
fi

#move stb to $TSGL_HOME/include
sudo cp -r stb $TSGL_HOME/include 

####################################################################################

#check the latest version of gcc
gccLatest=$(brew list --versions gcc | cut -f 2 -d ' ')
gcc_version_header=$(brew list --versions gcc | cut -f 2 -d ' ' | cut -f 1 -d .)

echo Latest gcc version ${gccLatest}

#First, check if we need to move an existing g++ / gcc compiler (in case we want to add code to do this)

#Create symbolic links for g++ and gcc
if brew list | grep -q 'gcc*'
then
	#check if gcc/g++ is installed in the right spot and create a sym links
	if [ -e /usr/local/bin/g++ ] && [ -e /usr/local/bin/gcc ]
	then
		echo "Updating g++ and gcc links!"
		rm /usr/local/bin/gcc
		rm /usr/local/bin/g++
	fi

	#Create the g++ and gcc links
	ln -s /usr/local/Cellar/gcc/${gccLatest}/bin/gcc-${gcc_version_header}* /usr/local/bin/gcc
	ln -s /usr/local/Cellar/gcc/${gccLatest}/bin/g++-${gcc_version_header}* /usr/local/bin/g++
fi

#Symlinks
echo "Fixing missing symlinks..."
if [ ! -e /usr/X11/lib/libX11.dylib ]; then
	sudo ln -s /usr/X11/lib/libX11.6.dylib /usr/X11/lib/libX11.dylib
fi

if [ ! -e /usr/X11/lib/libXrandr.dylib ]; then
	sudo ln -s /usr/X11/lib/libXrandr.2.dylib /usr/X11/lib/libXrandr.dylib
fi

#Remove any broken symlinks before installing
sudo rm -f $TSGL_HOME/libtsgl.* 
sudo rm -rf $TSGL_HOME/include/TSGL

#Build TSGL and install
echo "Building TSGL..."

echo

make prefix=$PREFIX

echo "Installing..."

echo

sudo make install prefix=$PREFIX

echo

echo "Installation complete!"

echo

#Checking update
echo "Checking for updates..."

TSGL_VERSION=$(git describe --tags --abbrev=0)
TSGL_LATEST_VERSION=$(git describe --tags $(git rev-list --tags --max-count=1))

if [[ $TSGL_VERSION < $TSGL_LATEST_VERSION ]]
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
	echo "Latest update already installed!"
fi

echo

echo "Run \"./runtests\" to verify everything is working."
