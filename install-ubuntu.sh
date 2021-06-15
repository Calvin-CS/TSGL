#!/bin/bash
#
# install-linux.sh is the installation script for TSGL on Linux.
# Last updated: 06/30/26
# 
# -SUBJECT TO CHANGE-
################################################################

echo "Installing TSGL..."

#Determine the OpenGL version (has to be 3.2 or higher)
#(Use glxinfo, available in the mesa-utils package)
sudo apt-get -y install mesa-utils

echo 
echo "Checking OpenGL version (must be 3.0 or higher)..."

sudo apt-get -y install mesa-utils

GLVersInfo=$(glxinfo | grep OpenGL)

#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
#Get a string containing the version number.
GLVersString=$(echo "$GLVersInfo" | grep "OpenGL version string: ")

#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
#Get the version number from the version string.
GLVersNum=$(echo "$GLVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

#http://tldp.org/LDP/abs/html/comparison-ops.html
#Check if the version is less than the threshold
#if [ "$GLVersNum" \< "3.0" ]
#then
#	echo "Your version of GL is: $GLVersNum."
#	echo "You need at least OpenGL version 3.0 or greater."
#	echo "Please update your drivers in order to continue."
#	echo "Try the following command to update your drivers:"
#	echo
#	echo "sudo ubuntu-drivers autoinstall"
#	echo  
#	echo "Abort."
#	exit 1
#else
#	echo "OpenGL version is sufficient to continue."
#fi

echo "Checking for g++..."

echo

gVersCheck=$(g++ --version)

#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
#Get a string containing the version number.
gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
#Get the version number from the version string.
gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

#http://tldp.org/LDP/abs/html/comparison-ops.html
#Check if the version number is null...
if [ -z "$gVersNum" ]
then
	#Yep. g++ is NOT installed.
	echo "g++ not installed!"
	echo "Installing  g++..."
	sudo apt-get install g++
	
	#Update versioning info
	gVersCheck=$(g++ --version)

	gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

	#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
	#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
	gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

else

	echo "g++ already installed."

	#No. Check the version.
	if [ "$gVersNum" \< "4.8" ]
	then
		echo "The version of g++ is: $gVersNum."
		echo "You need at least g++ 4.8 or greater in order to continue."
		echo "I can install a greater version of g++ for you."
		echo "Would you like me to do that? (1 = Yes, 2 = No)"
		#Adapted from: http://stackoverflow.com/questions/226703/how-do-i-prompt-for-input-in-a-linux-shell-script
		#Get the choice from the user.
		select choice in "Yes" "No"; do		
		case $choice in
			Yes ) #Yes, so...
				echo "Installing a greater version of g++ (4.9)..."

				#Get g++-4.9 on the machine
				sudo add-apt-repository ppa:ubuntu-toolchain-r/test;
				sudo apt-get update;
				sudo apt-get install --yes --force-yes g++-4.9;		
				sudo unlink /usr/bin/g++;   #Take out any symlink made before...	
				sudo ln -s /usr/bin/g++-4.9 /usr/bin/g++;
				
				#Update version info
				gVersCheck=$(g++ --version)

				#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
				gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

				#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
				#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
				gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

				break;;
			No ) #No, use the current version		
				echo "Cannot continue without g++ 4.8 or greater."
				echo "Abort."
				exit 1
			esac
		done

	else
		#Version number is okay
		echo "g++ version is sufficient to continue."

		#Check if version number is NOT 4.9
		if [ "$gVersNum" \< "4.9" ]
		then
			echo "You have $gVersNum installed."
			echo "Would you like me to install g++-4.9? (1 = Yes, 2 = No)"		
			select choice in "Yes" "No"; do		
				case $choice in
					Yes ) #Yes, so...

						echo "Installing g++-4.9..."

						#Get g++-4.9 on the machine
						sudo add-apt-repository ppa:ubuntu-toolchain-r/test;
						sudo apt-get update;
						sudo apt-get install --yes --force-yes g++-4.9;		
						sudo unlink /usr/bin/g++;   #Take out any symlink made before...	
						sudo ln -s /usr/bin/g++-4.9 /usr/bin/g++;
						
						#Update version info
						gVersCheck=$(g++ --version)
						
						#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
						gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")
	
						#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
						#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
						gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

						break;;
					No ) #No, use the current version		
						echo "Proceeding with $gVersNum."; 
						break;;
				esac
			done 
		fi

	fi 
fi

echo 

echo "Checking for necessary dependencies..."
echo

#Alright, now get glfw and GLEW (freetype can be gained through the wiki as well as OpenGL).
echo "Getting other dependencies (or updating if all found)..."

#Get the necessary header files as well as doxygen, git
sudo apt-get install --yes --force-yes build-essential libtool cmake xorg-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev libglu1-mesa-dev git libglew-dev doxygen 

echo 

#Get the glfw library
glfwFile=/usr/lib/libglfw.so

if [ -f "$glfwFile" ]
then
	echo "glfw dependency found"
else
	echo "Resolving missing glfw dependency..."
	git clone https://github.com/glfw/glfw.git || exit 1
	
	cd glfw

	#Build shared lib from source
	cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBUILD_SHARED_LIBS=ON

	#Make and install
	make

	sudo make install

	cd ..

	sudo rm -rf glfw
fi

#Check if we have to install freetype from source...
freetypeFile=/usr/lib/libfreetype.so

if [ -f "$freetypeFile" ]
then
	echo "freetype dependency found"
else
	echo "Resolving missing freetype dependency..."
	
	#We do, so get the freetype source
	wget downloads.sourceforge.net/project/freetype/freetype2/2.6.3/freetype-2.6.3.tar.bz2

	#Untar and unzip, configure, make, and install. 
	tar vxfj freetype-2.6.3.tar.bz2

	cd freetype-2.6.3

	./configure --prefix=/usr

	make 

	sudo make install
	
	cd ../
	
	#Remove the freetype folders from the TSGL folder
	rm -rf freetype*
fi

#Check if we have to clone cxxopts ...
cxxoptsFile=/usr/include/cxxopts.hpp

if [ -f "$cxxoptsFile" ]
then
	echo "cxxopts dependency found"
else
	echo "Resolving missing cxxopts dependency..."
	
	#We do, so clone the repository
	git clone https://github.com/jarro2783/cxxopts.git || exit 1

	#copy the necessary file 
	cd cxxopts/include

	sudo cp cxxopts.hpp /usr/include

	cd ../..
	
	#Remove the freetype folders from the TSGL folder
	rm -rf cxxopts*
fi

#check if we have to clone glm...
glmFile=/usr/include/glm/glm.hpp

if  [ -f "$glmFile" ]
then
        echo "glm dependency found"
else
        echo "Resolving missing glm dependency..."

        #clone the repository
        git clone https://github.com/g-truc/glm.git

        #copy the folder to usr/include
        cd glm

        sudo cp -r glm /usr/include

        cd ..

        rm -rf glm
fi

echo 
echo

#Dependencies were installed! (GLEW and glfw, as well as g++)
echo "All dependencies resolved!"

###################################################################################

#change the call for assets and LFLAGS directory
cd src/examples
grep -rli '/usr/local/include/TSGL/assets' * | xargs -i@ sed -i's|/usr/local/include/TSGL/assets|/usr/include/TSGL/assets|g' @

grep -rli 'L/usr/local/lib' * | xargs -i@ sed -i 's|L/usr/local/lib|L/usr/lib|g' @
cd -

###################################################################################

echo 

echo "Begin installation of TSGL..."

echo

#Clean install = remove the TSGL folder and lib files if they already exist
sudo rm -rf /usr/include/TSGL
sudo rm -rf /usr/lib/libtsgl.*

#Create the following directories (Since they aren't included in github but are needed)
mkdir -p lib bin

#Add TSGL and stb dependency
sudo cp -r TSGL/src/TSGL /usr/include
sudo cp -r TSGL/stb /usr/include

#Make the library
make

#Install it
sudo make install

#Take out the .cpp files from the TSGL library package folder
sudo rm -rf /usr/include/TSGL/*.cpp

#Final step (.so file won't be found unless I do this...)
sudo ldconfig

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo
echo
echo
#checking update
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

