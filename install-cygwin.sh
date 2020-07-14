#!/bin/bash
#
# install-cygwin.sh is the installation script for TSGL on Cygwin.
# Last updated: 07/09/2020
# 
# -SUBJECT TO CHANGE-
################################################################

echo "Installing TSGL..."

# # Ensure all necessary packages are installed

# echo 
# echo "Checking dependencies..."

# DepsList="xorg-server xinit xeyes libGL1 libGL-devel
# 				glm-devel libglut-devel libglut3 libfreetype6 glew
# 				libGLEW-devel openmpi libfreetype-doc doxygen-doxywizard
# 				libopenmpi-devel libopenmpi12 libopenmpicxx1
# 				cmake libXrandr-devel libXinerama-devel libXcursor-devel
# 				libXi-devel"

# sort $DepsList

# for i in $DepsList; do
# 	echo "$i"
# done

# echo "Checking X server dependencies..."

# InstalledPackages=$(cygcheck -c xorg-server xinit xeyes | sed '^x.')

# for i in $InstalledPackages; do
# 	echo "$i"
# done

# UninstalledPackages=""

# echo "Packages required: "

# # Output needed packages
# for i in $UninstalledPackages; do
# 	echo "$i"
# done

# Get list of dependencies currently installed
# GCCVersInfo=$(cygcheck -c | grep "gcc-g++")
# GLVersString=$(echo "$GCCVersInfo" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# echo $GLVersString

# GLVersInfo=$(cygcheck -c | grep OpenGL)

# #http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
# #Get a string containing the version number.
# GLVersString=$(echo "$GLVersInfo" | grep "OpenGL version string: ")

# #http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
# #http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
# #Get the version number from the version string.
# GLVersNum=$(echo "$GLVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# #http://tldp.org/LDP/abs/html/comparison-ops.html
# #Check if the version is less than the threshold
# if [ "$GLVersNum" \< "3.0" ]
# then
# 	echo "Your version of GL is: $GLVersNum."
# 	echo "You need at least OpenGL version 3.0 or greater."
# 	echo "Please update your drivers in order to continue."
# 	echo "Try the following command to update your drivers:"
# 	echo
# 	echo "ubuntu-drivers autoinstall"
# 	echo  
# 	echo "Abort."
# 	exit 1
# else
# 	echo "OpenGL version is sufficient to continue."
# fi

# echo "Checking for g++..."

# echo

# gVersCheck=$(g++ --version)

# #http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
# #Get a string containing the version number.
# gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

# #http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
# #http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
# #Get the version number from the version string.
# gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# #http://tldp.org/LDP/abs/html/comparison-ops.html
# #Check if the version number is null...
# if [ -z "$gVersNum" ]
# then
# 	#Yep. g++ is NOT installed.
# 	echo "g++ not installed!"
# 	echo "Installing  g++..."
# 	apt-get install g++
	
# 	#Update versioning info
# 	gVersCheck=$(g++ --version)

# 	gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

# 	#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
# 	#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
# 	gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# else

# 	echo "g++ already installed."

# 	#No. Check the version.
# 	if [ "$gVersNum" \< "4.8" ]
# 	then
# 		echo "The version of g++ is: $gVersNum."
# 		echo "You need at least g++ 4.8 or greater in order to continue."
# 		echo "I can install a greater version of g++ for you."
# 		echo "Would you like me to do that? (1 = Yes, 2 = No)"
# 		#Adapted from: http://stackoverflow.com/questions/226703/how-do-i-prompt-for-input-in-a-linux-shell-script
# 		#Get the choice from the user.
# 		select choice in "Yes" "No"; do		
# 		case $choice in
# 			Yes ) #Yes, so...
# 				echo "Installing a greater version of g++ (4.9)..."

# 				#Get g++-4.9 on the machine
# 				add-apt-repository ppa:ubuntu-toolchain-r/test;
# 				apt-get update;
# 				apt-get install --yes --force-yes g++-4.9;		
# 				unlink /usr/bin/g++;   #Take out any symlink made before...	
# 				ln -s /usr/bin/g++-4.9 /usr/bin/g++;
				
# 				#Update version info
# 				gVersCheck=$(g++ --version)

# 				#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
# 				gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")

# 				#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
# 				#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
# 				gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# 				break;;
# 			No ) #No, use the current version		
# 				echo "Cannot continue without g++ 4.8 or greater."
# 				echo "Abort."
# 				exit 1
# 			esac
# 		done

# 	else
# 		#Version number is okay
# 		echo "g++ version is sufficient to continue."

# 		#Check if version number is NOT 4.9
# 		if [ "$gVersNum" \< "4.9" ]
# 		then
# 			echo "You have $gVersNum installed."
# 			echo "Would you like me to install g++-4.9? (1 = Yes, 2 = No)"		
# 			select choice in "Yes" "No"; do		
# 				case $choice in
# 					Yes ) #Yes, so...

# 						echo "Installing g++-4.9..."

# 						#Get g++-4.9 on the machine
# 						add-apt-repository ppa:ubuntu-toolchain-r/test;
# 						apt-get update;
# 						apt-get install --yes --force-yes g++-4.9;		
# 						unlink /usr/bin/g++;   #Take out any symlink made before...	
# 						ln -s /usr/bin/g++-4.9 /usr/bin/g++;
						
# 						#Update version info
# 						gVersCheck=$(g++ --version)
						
# 						#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
# 						gVersString=$(echo "$gVersCheck" | grep "g++ (Ubuntu *")
	
# 						#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
# 						#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
# 						gVersNum=$(echo "$gVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

# 						break;;
# 					No ) #No, use the current version		
# 						echo "Proceeding with $gVersNum."; 
# 						break;;
# 				esac
# 			done 
# 		fi

# 	fi 
# fi

# echo 







#Dependencies were installed! (GLEW and glfw, as well as g++)
echo "All dependencies resolved!"

echo 

echo "Begin installation of TSGL..."

echo

#Clean install = remove the TSGL folder and lib files if they already exist
rm -rf /usr/local/include/TSGL
rm -rf /usr/local/lib/libtsgl.*

#Create the following directories (Since they aren't included in github but are needed)
mkdir -p lib bin

#Make the library
make

#Install it
make install

#Take out the .cpp files from the TSGL library package folder
rm -rf /usr/local/include/TSGL/*.cpp

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo

