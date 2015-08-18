TSGL INSTALLATION INSTRUCTIONS
----------------------------------------------------------------------------------------------------------
Hello, and thank you for downloading the TSGL Linux Package.

The installation process is very fast and should only take but a few minutes.

--------------------------
NOTES (PLEASE READ FIRST!)
--------------------------
+ You should have the following libraries already installed: GLEW, freetype, glfw, and GL. 

+ Follow the pre-installation steps to make sure that you have them installed.

+ To un-install TSGL, simply run the "uninstall-linux" script from a terminal.

---------------------------
PRE-INSTALL STEPS
---------------------------
(***NOTE:*** You must have sudo permissions in order to download and install the dependencies needed).

1). Open up a terminal.

2). Run the "pre-install-linux" script ("./pre-install-linux"). 

3). Next, download the freetype library (see the "MISSING LIBRARIES?" section for a link to the download page).

4). cd into your Downloads folder; Unzip and cd into the downloaded freetype folder. (Open up the file explorer, go into Downloads, right click on freetype-2.6.tar.bz2, click "Extract Here" and cd from a terminal into the extracted folder).

5). Type "./configure" and hit enter.

6). Type "make" and hit enter.

7). Type "sudo -s" and hit enter.

8). Type "make install" and hit enter.

9). Done.

(***NOTE:*** You only have to run the "pre-install-linux" script ONCE. You don't have to again if you uninstall TSGL and want to re-install it.)

---------------------------
INSTALL STEPS
---------------------------

1). The very first thing you should do is execute the "install-linux". Do so from a terminal (./install-linux). The libraries should be found (if not, see "PRE-INSTALL STEPS").

The TSGL library should now be installed and should take a few minutes to install. 
(Please be patient as the library is being built as it takes a few minutes to build!)

2). Once the installation is complete, execute the "runtests" script (./runtests). 
Hit the ESC key to go through each test, where upon hitting the ESC key the current window should close and a new one should open with another animation. 

Congratulations! TSGL has been successfully installed on your Linux computer!

----------------------------
MISSING LIBRARIES?
----------------------------
Missing some of the necessary libraries? Don't fret! Here are some links to follow so you can download them:

GLEW: http://glew.sourceforge.net/
glfw: http://www.glfw.org/download.html
freetype: http://www.freetype.org/download.html
GL: See GLEW link.
