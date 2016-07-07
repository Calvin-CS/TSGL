---------------------------------------------------------------------
| TSGL INSTALLATION INSTRUCTIONS FOR LINUX (Last updated: 06/30/16) |
---------------------------------------------------------------------
Hello, and thank you for downloading TSGL.

The installation process is very fast and should only take but a few minutes.

This process affects the following Linux distributions: Ubuntu, Fedora, CentOS 7.

Make sure that you are installing TSGL on one of those operating systems during this process!

-------------------------------
| NOTES (PLEASE READ FIRST!!) |
-------------------------------
+ TSGL has many dependencies, but the four main ones are: GLEW, glfw, freetype, and GL.
  The installer scripts will attempt to resolve missing dependencies, such as GLEW, glfw, and freetype.
  However, if you have a version of OpenGL that is less than 3.2, the installer scripts will alert you and direct you 
  to online tutorials/commands that will help you to update your version. 

+ Make sure that you follow the correct section! (e.g "Ubuntu Installation" for Ubuntu OSs, "Fedora Installation" for Fedora, etc.)

+ To un-install TSGL, simply run the "uninstall-*" script from a terminal, replacing "*" with whatever OS you are working in (Fedora, Ubuntu, or CentOS 7).

+ You *MUST* have sudo permissions on the computer that you are using!

+ You should have g++ installed on your machine. If not, the installer scripts should resolve the missing dependency and install it for you.
  The g++ version *MUST* be 4.8, and if it is not, the installer will alert you and ask if you would like to update the version.
  
-----------------------
| Ubuntu Installation |
-----------------------

1). Open up a terminal, and cd into the TSGL-master folder (or TSGL folder if cloned). 
2). Execute the "install-ubuntu.sh" script from the folder.
    
    The first step of the script is to determine the OpenGL version.
    
    The installer proceeds if the OpenGL is sufficient (>= 3.2). 
    
    It will alert you if it is NOT sufficient, and will abort.
    
    With a sufficient version of OpenGL, the installer proceeds to check the g++ version.
    If it is missing, it will be installed. 
    
    However, if you already have g++-4.8 installed, the installer will also ask if you would like to install g++-4.9.

    With a sufficient version of g++, the installer then proceeds to check which of the four main dependencies are alredy installed on your machine.
    
    If GLEW, GLFW, and/or freetype are not found, the installer will attempt to resolve the missing dependency.
    
    Afterwards, additional dependencies (development header files, Cmake, doxygen...) are installed.
    
    After that, missing dependencies are resolved (GLFW, GLEW, and/or freetype). 
    
    Once all dependencies are resolved and installed, TSGL is built.
    
    The library takes a few minutes to build, so please be patient!
    
    After the library has been built, it is installed. 
    
    That concludes the installation process! 

-----------------------
| Fedora Installation |
-----------------------

------------------------
| CentOS 7 Installation |
------------------------

----------------------
| MISSING LIBRARIES? |
----------------------
Missing some of the necessary libraries? Don't fret! Here are some links to follow so you can download them:

GLEW: http://glew.sourceforge.net/
glfw: http://www.glfw.org/download.html
freetype: http://www.freetype.org/download.html
GL: See GLEW link.

(These links are also availible on our "Library Versions" wiki page on GitHub)

------------------
| RECENT UPDATES |
------------------
06/30/16: Renamed the install-linux.sh script to install-ubuntu.sh, updated installation documentation, created uninstall scripts for Fedora and CentOS 7.
	      Renamed uninstall-linux to uninstall-ubuntu.
06/29/16: Created a Fedora installer (install-fedora). (Developed on Fedora 24).
06/22/16 - 06/27/16: Created a CentOS 7 installer (install-centos). (CentOS 7).
05/27/16: install-linux.sh now resolves missing GLFW and GLEW dependencies. 

