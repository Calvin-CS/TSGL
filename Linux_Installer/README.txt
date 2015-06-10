TSGL INSTALLATION INSTRUCTIONS
----------------------------------------------------------------------------------------------------------
Hello, and thank you for downloading the TSGL Linux Package.

The installation process is very fast and should only take but a few minutes.

--------------------------
NOTES
--------------------------
+ You should have the following libraries already installed: libGLEW, libfreetype, libglfw, and libGL. If you have one or more of these libraries missing, please download them before
proceeding. (To check whether you have them on most Unix systems open a terminal and type "ldconfig -p | grep LIBRARY_NAME" where LIBRARY_NAME is GLEW, freetype, glfw, 
or GL. If nothing shows up, then you don't have the library installed.)  
+ If any of these libraries were installed in a different place besides their default destination, when you type ./configure in step 3 be sure to hit SPACE and type: 
LDFLAGS="$LDFLAGS -L/the/library/path/that/has/the/library", replacing /the/library/path/that/has/the/library with the library path then hit ENTER. 

---------------------------
STEPS
---------------------------

1). The very first thing you should do is place the "configure" script wherever you want to have TSGL installed.
(Example: empty folder in your home directory named "TSGL").

2). Next, open up a terminal and cd into the folder that contains the "configure" script.

3). Finally, type ./configure in the terminal and hit enter. The TSGL library should now be installed and should take a few minutes to install. 
(Please be patient as the library is being built as it takes a few minutes to build!).

4). Once steps 1-3 have been completed, cd into workspace, then cd into TSGL. Type ./runtests in the terminal and hit enter. You should see a window pop up with an animation. 
Hit the ESC key to go through each test, where upon hitting the ESC key the current window should close and a new one should open with another animation. 
If you have gone through all of the tests without any trouble, then congratulations! TSGL has been successfully installed on your Linux computer!