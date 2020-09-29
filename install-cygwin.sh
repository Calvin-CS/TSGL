#!/bin/bash
#
# install-cygwin.sh is the installation script for TSGL on Cygwin.
# Last updated: 07/14/2020
# 
# -SUBJECT TO CHANGE-
################################################################

echo "Installing TSGL..."

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

