#!/bin/bash
# Easy install script for TSGL on OS X
# Last updated: 08/10/16
#
# -SUBJECT TO CHANGE-
###############################

VER_XQ="XQuartz-2.7.11"

#Function for checking command availability
has () {
  (command -v "$1" >/dev/null 2>&1 && echo 1) || echo 0
}

#FIXME: probably need to make it so X11 autoupdates. this currently seems super broken.
#Print X11 warning
if [ $(ls /Applications/Utilities | grep XQuartz.app | wc -l) -eq 0 ]; then
  echo "Installing X11"
  curl -L -O "http://xquartz.macosforge.org/downloads/SL/$VER_XQ.dmg"
  open -W "$VER_XQ.dmg"
  echo "Please continue the installation process by double-clicking XQuartz.pkg and following the instructions on screen."
  echo "Please enter to continue once XQuartz has finished installing."
  read
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
  ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
  export PATH=/usr/bin:$PATH
else
  echo "Homebrew detected."
  export PATH=/usr/bin:$PATH
fi

echo
echo "Using Homebrew to install dependencies..."
echo

brew update

#brew tap homebrew/versions

#Install glfw3 (will do nothing if already installed)
echo "Installing GLFW..."
brew install glfw3

#GLEW
echo "Installing GLEW..."
brew install glew

#Freetype
echo "Installing Freetype2..."
brew install freetype

#Doxygen
echo "Installing Doxygen..."
brew install doxygen

#cxxopts
echo "Installing cxxopts..."
brew install cxxopts

#glm
echo "Installing glm..."
brew install glm

####################################################################################

#redefine path variable and append to bashrc
tsglFile=/usr/local/lib/libtsgl.so

if [ -f "$tsglFile" ]
then
	echo "" 
	echo "export PATH=/usr/local/bin:$PATH" >> ~/.bashrc
	echo "export TSGL_HOME=/usr/local" >> ~/.bashrc
	echo "export TSGL_DEFAULT_FONT=/include/TSGL/assets/freefont/FreeSansBold.ttf" >> ~/.bashrc

	source ~/.bashrc	
else
	echo ""
fi

#change the call for assets and LFLAG directory
#cd src/examples
#grep -rli '/usr/include/TSGL/assets' * | xargs perl -i -pe's|/usr/include/TSGL/assets|/usr/local/include/TSGL/assets|g'

#grep -rli 'L/usr/lib' * | xargs perl -pi -e 's|L/usr/lib|L/usr/local/lib|g'
#cd -

#move stb to /usr/local/include
#sudo cp -r stb /usr/local/include 

####################################################################################

#g++
gVers=$(g++ -v 2>&1)
gVeres=$(g++ -v 2>&1 | grep "gcc version" | cut -f 3 -d ' ' | tr -d .)

echo "$gVers" | grep "clang" > check.txt

version=490

#First, check if we need to move an existing g++ compiler
if [ -e /usr/bin/g++ ] && [ ! -e /usr/Cellar/gcc49 ];
then
	echo ""
	#sudo mv /usr/bin/g++ /usr/bin/g++old
	#sudo mv /usr/bin/gcc /usr/bin/gccold
fi

#Now check if we need to install gcc10
if grep --quiet clang check.txt; then
	echo "Installing g++...this may take a while!"
	brew install gcc
	rm /usr/local/bin/gcc
	rm /usr/local/bin/g++
	ln -s /usr/local/Cellar/gcc/11.1.0_1/bin/gcc-11 /usr/local/bin/gcc
	ln -s /usr/local/Cellar/gcc/11.1.0_1/bin/g++-11 /usr/local/bin/g++
else
	echo "Installing g++...this may take a while!"
	brew install gcc
	#sudo rm /usr/bin/gcc
	#sudo rm /usr/bin/g++
	#sudo ln -s /usr/Cellar/gcc/10.2.0/bin/gcc-10 /usr/bin/gcc
	#sudo ln -s /usr/Cellar/gcc/10.2.0/bin/g++-10 /usr/bin/g++
fi

#delete check.txt
rm check.txt 

#Symlinks
echo "Fixing missing symlinks..."
if [ ! -e /usr/X11/lib/libX11.dylib ]; then
  sudo ln -s /usr/X11/lib/libX11.6.dylib /usr/X11/lib/libX11.dylib
fi

if [ ! -e /usr/X11/lib/libXrandr.dylib ]; then
  sudo ln -s /usr/X11/lib/libXrandr.2.dylib /usr/X11/lib/libXrandr.dylib
fi

#Remove any broken symlinks before installing
rm -f /usr/lib/libtsgl.* 
rm -rf /usr/include/TSGL

#Build TSGL and install
echo "Building TSGL..."
echo

make

echo "Installing..."
echo

sudo make install

echo

echo "Installation complete!"

echo
#checking update
echo "Checking for updates..."

TSGL_VERSION=$(git describe --tags --abbrev=0)
TSGL_LATEST_VERSION=$(git describe --tags $(git rev-list --tags --max-count=1))

if [ $TSGL_VERSION < $TSGL_LATEST_VERSION ]
then
	echo ""
else
         echo "Latest version $TSGL_LATEST_VERSION found. WARNING, If you have changed anything in the TSGL folder it may be overwritten during update. To keep your changes, please commit them before updating."
        read -p "Do you want to install the update? This will replace all the files with the updated ones (y/n): " INPUT
        if [ $INPUT == y ] || [ $INPUT == Y ]
        then
                echo "Updating TSGL..."
                git remote add tsgl https://github.com/Calvin-CS/TSGL.git
                git pull tsgl master
        fi
       echo "Latest version already installed"
fi

echo "Run \"./runtests\" to verify everything is working."
