#!/bin/bash

# install-linux.sh is the installation script for TSGL on Linux.
# -SUBJECT TO CHANGE-
#####

echo "Installing TSGL..."
echo

echo "Checking for necessary dependencies..."
echo

#Check for the following libraries: 
glfw=0   #glfw
GL=0  #OpenGL
freetype=0  #Freetype
GLEW=0  #And GLEW

#To do so, use the ldconfig command and pipe it to grep with the following keywords:
ldconfig -p | grep glfw > glfw.txt   #'glfw'
ldconfig -p | grep GL > opengl.txt  #'GL'
ldconfig -p | grep freetype > freetype.txt  #'freetype'
ldconfig -p | grep GLEW > glew.txt  #and 'GLEW'

#Based off of the piping above, if any of the keywords were found, then the corresponding text files will have 
#information about the libraries currently installed. 
#If they aren't found, then the text files will be blank.
#If that is the case, then the library shouldn't be on the machine. (A missing dependency).

#Check the text files to see if there are any missing libraries.

#If the text file exists, then that just means the check went through with no problems.
if [ -e glfw.txt ]
then
	#Checking for dependencies now...
	#If the text file contains the name of the library that we are looking 
	#for, then it must be installed. 
	if grep "libglfw.so" glfw.txt > holder.txt
	then
	#Which means, it's not missing. 
	glfw=1
	echo
	fi
fi

#Continue to do that for the next three libraries

#GL
if [ -e opengl.txt ]
then
	if grep "libGL.so" opengl.txt > holder.txt
	then
		GL=1
		echo
	fi	
fi

#freetype
if [ -e freetype.txt ]
then
	if grep "libfreetype.so" freetype.txt > holder.txt
	then
		freetype=1
		echo
	fi	
fi

#GLEW
if [ -e glew.txt ]
then
	if grep "libGLEW.so" glew.txt > holder.txt
	then
		GLEW=1
		echo
	fi	
fi

#Alright, we're done checking. 
#Clean up the text files, we no longer need them.
rm glfw.txt
rm glew.txt
rm opengl.txt
rm freetype.txt
rm holder.txt

#Now, determine if any of the dependencies are missing.
if test $glfw == 0
then
	echo "glfw not found!" #Even if it's not installed, it will be with the install script.
elif test $GL == 0
then
	echo "GL not found! Please see the 'Library Versions' section of our wiki pages for a link to download and install this library."
	exit 1 
elif test $freetype == 0
then
	echo "Freetype not found! Please see the 'Library Versions' section of our wiki pages for a link to download and install this library."
	exit 1
elif test $GLEW == 0
then
	echo "GLEW not found!"  #Same with GLEW
fi

#Alright, now get glfw and GLEW (freetype can be gained through the wiki as well as OpenGL).
echo "Getting dependencies (or updating if all found)..."

#Get the necessary dependencies needed for glfw as well as for Doxygen and git
sudo apt-get install --yes --force-yes build-essential devscripts libtool cmake xorg-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev libglu1-mesa-dev git libglew-dev doxygen

echo 

#Get the glfw library
if test $glfw == 0
then
	git clone https://github.com/glfw/glfw.git || exit 1
	#go into it
	cd glfw

	#cmake command
	cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DBUILD_SHARED_LIBS=ON

	#Make and install
	make

	sudo make install

	cd ..

	#Take it out 
	sudo rm -rf glfw
fi

#Check for g++-4.9
g49=0
g48=0
g47=0
g46=0
glate=0

echo "Checking for g++..."

echo

#Check the version (Same as the check for the libraries above, but with g++ --version)
g++ --version > version.txt

g++-4.9 --version | grep 4.9.* > version4.9.txt

g++-4.8 --version | grep 4.8.* > version4.8.txt

g++-4.7 --version | grep 4.7.* > version4.7.txt

g++-4.6 --version | grep 4.6.* > version4.6.txt

echo

#Check if any of the files were empty (or exist)...
#g++-4.9
if [ -e version4.9.txt ]
then
	if [ -s version4.9.txt ]
	then
	echo "g++-4.9 found."
	g49=1
	echo
	fi
else
echo "g++-4.9 not found. Checking for 4.8..."
echo	
fi

#g++-4.8
if [ -e version4.8.txt ]
then
	if [ -s version4.8.txt ]
	then	
	echo "g++-4.8 found."
	g48=1
	echo
	fi
else 
echo "g++-4.8 not found. Checking for 4.7..."
echo
fi

#g++-4.7
if [ -e version4.7.txt ]
then		
	if [ -s version4.7.txt ]
	then
	echo "g++-4.7 found."
	g47=1
	echo
	fi	
else
echo "g++-4.7 not found. Checking for 4.6..."
echo
fi

#g++-4.6
if [ -e version4.6.txt ]
then

	if [ -s version4.6.txt ]
	then
	echo "g++-4.6 found."
	g46=1
	echo
	fi
else
echo "g++-4.6 not found. Checking for later versions..."
echo
fi

#Earlier versions of g++ than 4.6
if [ -e version.txt ]
then
	if [ -s version.txt ]
	then
	echo "g++ found."
	glate=1
	echo
	fi	
else
echo "Symlink not found."
fi

#Take out the version check files
rm version*

#If g++-4.9 isn't found...
if test $g49 == 0 
then
	#Check whether g++-4.8 or g++4.7 were found.
	if test $g48 == 1 || $g47 == 1
	then
		#If so, then prompt the user.
		#Adapted from: http://stackoverflow.com/questions/226703/how-do-i-prompt-for-input-in-a-linux-shell-script
		#Get the choice from the user.
		echo "Would you like to install g++-4.9? (Enter 1 for Yes or 2 for No)"		
		select choice in "Yes" "No"; do		
		case $choice in
			Yes ) #Yes, so...
				#Get g++-4.9 on the machine
				sudo add-apt-repository ppa:ubuntu-toolchain-r/test;
				sudo apt-get update;
				sudo apt-get install --yes --force-yes g++-4.9;		
				sudo unlink /usr/bin/g++;   #Take out any symlink made before...	
				sudo ln -s /usr/bin/g++-4.9 /usr/bin/g++;
				break;;
			No ) #No, use the current version		
				echo "Proceeding with the current g++ version."; break;;
		esac
	done 
	echo
	#Check whether g++-4.6 or earlier were found if neither g++-4.8 nor g++-4.7 were found.
	elif test $g46 == 1 || $glate == 1
	then
		#If so, same as above. Prompt the user, get the choice.
		#Adapted from: http://stackoverflow.com/questions/226703/how-do-i-prompt-for-input-in-a-linux-shell-script
		echo "Would you like to install g++-4.9? (Enter 1 for Yes or 2 for No)"		
		select choice in "Yes" "No"; do		
		case $choice in
			Yes ) #Yes, so.. 
				#Get g++-4.9 on the machine
				sudo add-apt-repository ppa:ubuntu-toolchain-r/test;
				sudo apt-get update;
				sudo apt-get install --yes --force-yes g++-4.9;
				sudo unlink /usr/bin/g++;  #Take out any symlink made before...
				sudo ln -s /usr/bin/g++-4.9 /usr/bin/g++;  #Create a new one to g++-4.9
				break;;
			No ) #No, use the current version
				echo "Proceeding with the current g++ version."; break;;
		esac
	done 	
	echo
	fi
fi

#Dependencies were installed! (GLEW and glfw, as well as g++)
echo "Dependencies installed!"

echo 

#Clean install = Remove the TSGL folder and lib files if they already exist
sudo rm -rf /usr/local/include/TSGL
sudo rm -rf /usr/local/lib/libtsgl.*

#A weird bug occurs whenever I install ONLY glfw. I have to install and uninstall libglfw-dev in order for it to work.
sudo apt-get --yes --force-yes install libglfw-dev
sudo apt-get --yes --force-yes remove libglfw-dev

#Create the following directories (Since they aren't included in github but are needed)
mkdir lib bin

#Make the library
make

#Install it
sudo make install

#Take out the .cpp files from the TSGL library package folder
sudo rm -rf /usr/local/include/TSGL/*.cpp

#Take out the config.log file that is made when you run the install script
sudo rm -f config.log

#Final step (.so file won't be found unless I do this...)
sudo ldconfig

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo

