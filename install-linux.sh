#!/bin/bash
#
# install-linux.sh is the installation script for TSGL on Linux.
# Last updated: 05/27/26
# 
# -SUBJECT TO CHANGE-
################################################################

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
	if grep "libglfw.so.3" glfw.txt > holder.txt
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
if [ $glfw == 0 ]
then
	echo "glfw not found! (Will be resolved shortly)" #Even if it's not installed, it will be with the install script.
elif [ $GL == 0 ]
then
	echo "GL not found! Please see the 'Library Versions' section of our wiki pages for a link to download and install this library."
	echo "(You may also have to update your drivers!)"
	exit 1 
elif [ $freetype == 0 ]
then
	echo "Freetype not found! Please see the 'Library Versions' section of our wiki pages for a link to download and install this library."
	exit 1
elif [ $GLEW == 0 ]
then
	echo "GLEW not found! (Will be resolved shortly)"  #Same with GLEW
fi

#Alright, now get glfw and GLEW (freetype can be gained through the wiki as well as OpenGL).
echo "Getting other dependencies (or updating if all found)..."

#Get the necessary header files as well as doxygen, git
sudo apt-get install --yes --force-yes build-essential libtool cmake xorg-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev libglu1-mesa-dev git libglew-dev doxygen 

echo 

#Get the glfw library
if [ $glfw == 0 ]
then
	echo "Resolving missing glfw dependency..."
	git clone https://github.com/glfw/glfw.git || exit 1
	
	cd glfw

	#Build shared lib from source
	cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DBUILD_SHARED_LIBS=ON

	#Make and install
	make

	sudo make install

	cd ..

	sudo rm -rf glfw
fi

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


#Dependencies were installed! (GLEW and glfw, as well as g++)
echo "All dependencies resolved!"

echo 

echo "Begin installation of TSGL..."

echo

#Clean install = remove the TSGL folder and lib files if they already exist
sudo rm -rf /usr/local/include/TSGL
sudo rm -rf /usr/local/lib/libtsgl.*

#Create the following directories (Since they aren't included in github but are needed)
mkdir -p lib bin

#Make the library
make

#Install it
sudo make install

#Take out the .cpp files from the TSGL library package folder
sudo rm -rf /usr/local/include/TSGL/*.cpp

#Final step (.so file won't be found unless I do this...)
sudo ldconfig

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo

