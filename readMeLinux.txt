TSGL INSTALLATION INSTRUCTIONS
----------------------------------------------------------------------------------------------------------
Hello, and thank you for downloading the TSGL Linux Package.

The installation process is very fast and should only take but a few minutes.

--------------------------
NOTES (PLEASE READ FIRST!)
--------------------------
+ You should have the following libraries already installed: GLEW, freetype, glfw, and GL. If you have one or more of these libraries missing, please download them before
proceeding. (To check whether you have them on most Unix systems open a terminal and type "ldconfig -p". A list of the currently installed libraries should show up.
To find out if you have these libraries already installed, you can either: search for them manually in the list (scroll up the list until you see the names libGLEW,
libglfw, libfreetype, and/or libGL), or pipe it to grep (type "ldconfig -p | grep LIBRARY_NAME" where LIBRARY_NAME is GLEW, glfw, GL, or freetype). If you cannot 
find one or more of these libraries, please see below for links on where you can find and download them.)
 
+ If any of these libraries were installed in a different place besides their default destination, when you type ./install-linux in step 1 be sure to hit SPACE and type: 
LDFLAGS="$LDFLAGS -L/the/library/path/that/has/the/library", replacing /the/library/path/that/has/the/library with the library path then hit ENTER. 
(To determine where your libraries are, type "ldconfig -p | grep LIBRARY_NAME" where LIBRARY_NAME is GLEW, glfw, GL, or freetype.).
---------------------------
STEPS
---------------------------

1). The very first thing you should do is execute the "install-linux". Do so from a terminal (./install-linux).
The TSGL library should now be installed and should take a few minutes to install. 
(Please be patient as the library is being built as it takes a few minutes to build!)

2). Once the installation is complete, execute the "runtests" script (./runtests). 
Hit the ESC key to go through each test, where upon hitting the ESC key the current window should close and a new one should open with another animation. 
If you have gone through all of the tests without any trouble, then congratulations! TSGL has been successfully installed on your Linux computer!

----------------------------
MISSING LIBRARIES?
----------------------------
Missing some of the necessary libraries? Don't fret! Here are some links to follow so you can download them:

GLEW: http://glew.sourceforge.net/
glfw: http://www.glfw.org/download.html
freetype: http://www.freetype.org/download.html
GL: See GLEW link.
