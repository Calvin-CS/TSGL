TSGL INSTALLATION INSTRUCTIONS (Last updated: 05/27/16)
----------------------------------------------------------------------------------------------------------
Hello, and thank you for downloading the TSGL Linux Package.

The installation process is very fast and should only take but a few minutes.

--------------------------
NOTES (PLEASE READ FIRST!!)
--------------------------
+ You should have the following libraries already installed: GLEW, freetype, glfw, and GL. 

+ Follow the pre-installation steps first, then proceed to the installation.

+ To un-install TSGL, simply run the "uninstall-linux" script from a terminal.

---------------------------
PRE-INSTALL STEPS
---------------------------
(***NOTE:*** You must have sudo permissions in order to download and install the dependencies needed).

1). Download the Freetype library (see the "MISSING LIBRARIES?" section for a link to the download page).

2). Open up a terminal, cd into your Downloads folder; Unzip and cd into the downloaded freetype folder. (Open up the file explorer, go into Downloads, right click on freetype-2.6.tar.bz2, click "Extract Here" and cd from a terminal into the extracted folder).

3). Type "./configure" and hit enter.

4). Type "make" and hit enter.

5). Type "sudo -s" and hit enter.

6). Type "make install" and hit enter.

7). Type "exit".

---------------------------
INSTALL STEPS
---------------------------

(UPDATE: As of 05/27/16, the install script now resolves missing dependencies. Specifically, if you do not have GLFW and/or GLEW, the install script will download them 
 and install them for you automatically.)

1). The very first thing you should do is execute the "install-linux.sh" script in the TSGL-master folder. Do so from a terminal (./install-linux.sh). The libraries should be found (if not, see "PRE-INSTALL STEPS" and "MISSING LIBRARIES?" sections).

The TSGL library should now be installed and should take a few minutes to install. 
(Please be patient as the library is being built as it takes a few minutes to build!)

If you have g++-4.8 or earlier installed, then the script will ask whether you would like g++-4.9 installed instead.
(If you already have g++-4.9 installed, then the script will skip this step).

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

(These links are also availible on our "Library Versions" wiki page on GitHub)
