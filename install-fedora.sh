#!/bin/bash
# TSGL install script for Fedora. 
# Last modified: 06/29/16.
#
# -SUBJECT TO CHANGE-
#####################################

echo "Begin installation..."

#This determines whether or not we need to make a symlink
glSymlink=0

#Store the working directory
#(So that we can come back to the TSGL directory easily)
workingDir=$(pwd)

#First, check if g++ is even installed.
gVersCheck=$(g++ --version)

#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
#Get a string containing the version number.
gVersString=$(echo "$gVersCheck" | grep "g++ (GCC)")

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
	sudo yum install gcc-c++
	
	#Update versioning info
	gVersCheck=$(g++ --version)

	gVersString=$(echo "$gVersCheck" | grep "g++ (GCC)")

	#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
	#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
	gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

else
	echo "g++ already installed."
	#No. Check the version.
	#Check if it's below the threshold...
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
				echo "Installing g++ 4.8...."
				sudo yum install gcc-c++
				
				#Update version info
				gVersCheck=$(g++ --version)

				#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
				gVersString=$(echo "$gVersCheck" | grep "g++ (GCC)")

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
		#Version number is okay.
		echo "g++ version is sufficient to continue."
	fi 
fi

#Step 2: Install Cmake (because Cmake is up to 3.5.2).

#First, Get dependencies.
sudo yum install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel mesa-libGLU-devel libXmu-devel libXi-devel libGL-devel glew-devel freetype-devel doxygen 

#Now, check the version.
cmakeVersCheck=$(cmake --version)

#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
cmakeVersString=$(echo "$cmakeVersCheck" | grep "cmake version ")

#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
cmakeVersNum=$(echo "$cmakeVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

#Check if not installed.
if [ -z "$cmakeVersNum" ]
then
	echo "Cmake not installed!"
	echo "Installing Cmake..."
	
	sudo yum install cmake

	#Update versioning info.
	cmakeVersCheck=$(cmake --version)

	cmakeVersString=$(echo "$cmakeVersCheck" | grep "cmake version ")

	cmakeVersNum=$(echo "$cmakeVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

else
	echo "Cmake already installed."

	#http://tldp.org/LDP/abs/html/comparison-ops.html
	#Check if the version is less than the threshold
	if [ "$cmakeVersNum" \< "3.0" ]
	then
		echo "The version of Cmake is: $cmakeVersNum."
		echo "You need at least Cmake 3.0 or greater in order to continue."
		echo "I can install a greater version of Cmake for you."
		#Adapted from: http://stackoverflow.com/questions/226703/how-do-i-prompt-for-input-in-a-linux-shell-script
		#Get the choice from the user.
		echo "Would you like me to do that for you? (1 = Yes, 2 = No)"		
		select choice in "Yes" "No"; do		
		case $choice in
			Yes ) #Yes, so...
				echo "Installing Cmake..." 

				sudo yum install cmake 
	
				#Update the version info.
				cmakeVersCheck=$(cmake --version)

				cmakeVersString=$(echo "$cmakeVersCheck" | grep "cmake version ")

				cmakeVersNum=$(echo "$cmakeVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

				break;;
			No ) #No. Can't use the current version, so abort.	
				echo "Cannot continue without Cmake 3.0 or greater."
				echo "Abort."
				exit 1
			esac
		done
	else	#Version number is okay.
		echo "Cmake version is sufficient to continue."
	fi
fi

#GL version checking.
sudo yum install glxinfo

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
if [ "$GLVersNum" \< "3.2" ]
then
	echo "Your version of GL is: $GLVersNum."
	echo "You need at least OpenGL version 3.2 or greater."
	echo "Please update your drivers in order to continue."
	echo "If you have an Nvidia graphics card, you can follow this online tutorial"
	echo "in order to update your binary drivers:" 
	echo
	echo "http://rpmfusion.org/Howto/nVidia"
	echo
	echo "AMD users, here is a possible solution: "
	echo
	echo "https://bluehatrecord.wordpress.com/2015/09/17/installing-the-proprietary-amd-catalyst-15-9-fglrx-15-201-driver-on-fedora-22-with-linux-kernel-4-1-6/"
	echo 
	echo "We apologize for any inconvienence."
	echo "Abort."
	exit 1
else
	echo "OpenGL version is sufficient to continue."
	echo "Did you update your graphics card drivers before installing TSGL? (1 = Yes, 2 = No)"
	select choice in "Yes" "No"; do		
		case $choice in
		Yes ) #Yes, so, make a symlink
			echo "Making a note of that..."
			glSymlink=1
			echo "Thanks for telling me!"
			break;;
		No ) #No, don't have to make a symlink		
			echo "Thanks for telling me!" break;;
		esac
	done 
fi

#Step 3: Build GLFW from source
echo "Installing GLFW..."

git clone https://www.github.com/glfw/glfw.git

cd glfw

cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DBUILD_SHARED_LIBS=ON

make

sudo make install

cd ../

sudo rm -rf glfw/

#Copy over the .so file into lib64/ (so it can be found...)
sudo cp /usr/local/lib/libglfw.so.3 /usr/lib64

echo "GLFW installed."

#Step 4: Build freetype from source
echo "Installing freetype..."

wget downloads.sourceforge.net/project/freetype/freetype2/2.6.3/freetype-2.6.3.tar.bz2

tar vxfj freetype-2.6.3.tar.bz2

cd freetype-2.6.3

./configure

make 

sudo make install

#Go back to the TSGL directory
cd $workingDir
rm -rf freetype*

echo "Freetype installed."

#Make a symlink to GL.so file (so it can be found...)
#(Apparently, after updating the drivers, TSGL CANNOT find -lGL).
#(After doing a bit of research, I found that it's because of a broken symlink...)
#Check if we even need to do a symlink first!
if [ $glSymlink == 1 ]
then
	sudo ln -sfn /usr/lib64/libGL.so.1 /usr/lib64/libGL.so
fi

#Edit the LD_LIBRARY_PATH variable
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/include:/usr/local/lib:/usr/lib:/usr/lib64

#Now, make and install TSGL!
echo "Installing TSGL..."

cd $workingDir

sudo rm -rf /usr/local/include/TSGL
sudo rm -rf /usr/local/lib/libtsgl.*

mkdir -p lib bin

make 

sudo make install

sudo rm -rf /usr/local/include/TSGL/*.cpp

sudo ldconfig

echo "Installation complete! Execute the runtests bash script to verify that everything works."

echo

