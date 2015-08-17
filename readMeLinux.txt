TSGL INSTALLATION INSTRUCTIONS
----------------------------------------------------------------------------------------------------------
Hello, and thank you for downloading the TSGL Linux Package.

The installation process is very fast and should only take but a few minutes.

--------------------------
NOTES (PLEASE READ FIRST!)
--------------------------
+ You should have the following libraries already installed: GLEW, freetype, glfw, and GL. 

+ Follow the pre-installation steps to make sure that you have them installed.

---------------------------
PRE-INSTALL STEPS
---------------------------
(***NOTE:*** You must have sudo permissions in order to download and install the dependencies needed. Type "sudo -s" to see. If you are prompted for your password, enter it and you should be good to go!)

(If prompted, enter 'y' for y/n questions)

1). Open up a terminal.

2). Make sure you have g++ installed. (Type "sudo apt-get install g++" to be sure).

3). Next, download the freetype library (see the "MISSING LIBRARIES?" section for a link to the download page).

4). cd into your Downloads folder; Unzip and cd into the downloaded freetype folder. (Open up the file explorer, go into Downloads, right click on freetype-2.6.tar.bz2, click "Extract Here" and cd from a terminal into the extracted folder).

5). Type "./configure" and hit enter.

6). Type "make" and hit enter.

7). Type "sudo -s" and hit enter.

8). Type "make install" and hit enter.

9). Now, type "sudo apt-get install build-essential devscripts libtool checkinstall cmake xorg-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev libglu1-mesa-dev git libglew-dev doxygen" (yes, the whole thing) and hit enter.

10). Go into your Downloads folder and type "git clone https://github.com/glfw/glfw.git" and hit enter.

11). cd into the glfw folder, and type "cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DBUILD_SHARED_LIBS=ON" and hit enter.

12). Type "make" and hit enter.

13). Type "make install" and hit enter. 

14). Done.

---------------------------
INSTALL STEPS
---------------------------

1). The very first thing you should do is execute the "install-linux". Do so from a terminal (./install-linux). The libraries should be found (if not, see "PRE-INSTALL STEPS").

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
