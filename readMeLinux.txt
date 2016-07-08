---------------------------------------------------------------------
| TSGL INSTALLATION INSTRUCTIONS FOR LINUX (Last updated: 07/08/16) |
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
	
	It *MUST* be at least 4.8 or greater in order to take advantage of C++-11 features.

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
1). Open up a terminal, and cd into the TSGL-master folder (or TSGL if cloned).

2). Execute the "install-fedora.sh" script from the folder.
	
	The first step is to determine the g++ version.

	It *MUST* be at least 4.8 or greater in order to take advantage of C++-11 features.
	
	If it is missing, it will be installed. 
    
    However, if you already have g++ installed, and the version is less than 4.8, the installer will ask if you would like to install g++-4.8.
	
	If you agree to do so, the installer will upgrade your g++ version. If not, the installer will abort.

	After the g++ version is sufficient, extra dependencies are installed for GLFW and TSGL.
	
	Then, the cmake version is determined.

	Cmake is used to compile GLFW from source, and it needs to be at least 2.8.11. 
	
	If it is not installed, the installer will install it.
	
	If it is installed, but the version is less than 2.8.11, the installer will ask if you would like to install a greater version.
	
	If you agree, the installer will proceed to install a greater version of Cmake.
	
	If not, the installer aborts. 
	
	After the cmake version is sufficient, the installer proceeds if the OpenGL is sufficient (>= 3.2). 
    
	It will alert you if it is NOT sufficient, and will abort.
	
	If, however, you have a sufficient version of OpenGL, the installer will ask if you have updated your graphics drivers. 
	
	Enter yes if you have, as a symlink is needed in order to determine the correct libGL.so file on your machine. 

	If not, you may say no. 
	
	If, however, TSGL fails to work after installation, re-run the install script and enter yes in order for the symlink to be made.

	After the OpenGL version is sufficient, GLFW is compiled from source and installed.
	
	FreeType is then compiled from source and installed.
	
	When all dependencies have been resolved, TSGL is compiled.
	
	It takes a few minutes to compile the entire library, so please be patient!

	Once the library has been built, it is installed on your machine.
	
	That conclude the installation process!
	

------------------------
| CentOS 7 Installation |
------------------------
1). Open up a terminal, and cd into the TSGL-master folder (TSGL folder if cloned).

2). Execute the "install-centos" script from the folder.
	
	The first step is to determine the g++ version.

	It *MUST* be at least 4.8 or greater in order to take advantage of C++-11 features.

	If it is missing, it will be installed. 
    
    However, if you already have g++ installed, and the version is less than 4.8, the installer will ask if you would like to install g++-4.8.
	
	If you agree to do so, the installer will upgrade your g++ version. If not, the installer will abort.

	After the g++ version is sufficient, extra dependencies are installed for GLFW and TSGL.

	Then, the cmake version is determined.

	Cmake is used to compile GLFW from source, and it needs to be at least 2.8.11. 
	
	If it is not installed, the installer will compile it from source and install it.
	
	If it is, but the version is less than 2.8.11, the installer will ask if you would like a greater version installed.
	
	If you say yes, the installer will proceed to install a greater version.
	
	If not, the installer will abort.

	After the cmake version is sufficient, the installer proceeds if the OpenGL is sufficient (>= 3.2). 
    
	It will alert you if it is NOT sufficient, and will abort.
	
	If, however, you have a sufficient version of OpenGL, the installer will ask if you have updated your graphics drivers. 
	
	Enter yes if you have, as a symlink is needed in order to determine the correct libGL.so file on your machine. 

	If not, you may say no. 
	
	If, however, TSGL fails to work after installation, re-run the install script and enter yes in order for the symlink to be made.

	After the OpenGL version is sufficient, GLFW is compiled from source and installed.
	
	FreeType is then compiled from source and installed.
	
	When all dependencies have been resolved, TSGL is compiled.
	
	It takes a few minutes to compile the entire library, so please be patient!

	Once the library has been built, it is installed on your machine.
	
	That conclude the installation process!
	
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
07/08/2016: Added Fedora documentation, CentOS documentation.
06/30/16: Renamed the install-linux.sh script to install-ubuntu.sh, updated installation documentation, created uninstall scripts for Fedora and CentOS 7.
	      Renamed uninstall-linux to uninstall-ubuntu.
06/29/16: Created a Fedora installer (install-fedora). (Developed on Fedora 24).
06/22/16 - 06/27/16: Created a CentOS 7 installer (install-centos). (CentOS 7).
05/27/16: install-linux.sh now resolves missing GLFW and GLEW dependencies. 

