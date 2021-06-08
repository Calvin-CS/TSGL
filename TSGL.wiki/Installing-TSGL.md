### Introduction

As of summer 2016, there are two main ways that you can install TSGL:

* via a binary package ***(recommended)***.
* directly from source.

In essence, the binary packages depend on a laundry list of dependencies.

The same is true when you install from source.

However, the three main dependencies are:

* [FreeType](https://www.freetype.org/download.html)

* [GLFW](https://www.glfw.org/download.html)

* [GLEW](https://downloads.sourceforge.net/project/glew/glew/1.12.0/glew-1.12.0.zip)

These, along with the other dependencies (```doxygen```, development header files, ...), are installed along with TSGL regardless of which method you choose.

When installing TSGL using a binary package on Ubuntu & Fedora, the following is true:

* Debian/Ubuntu:

    * The Debian packages install ```libfreetype6``` and ```libfreetype6-dev```. This resolves the FreeType dependency.

    * The Debian packages install ```libglew-dev```. This resolves the GLEW dependency.

    * A packaged version of the ```GLFW``` library is installed along with the TSGL Debian packages. This resolves the GLFW dependency.

    * Please be sure to have ***at least OpenGL 3.0 or greater*** on your machine. You can update your graphics drivers by typing the following command: ```sudo ubuntu-drivers autoinstall```. If that does not work, there are plenty of online tutorials which should help you.

* Fedora/RPM:

    * The RPM package installs ```freetype``` and ```freetype-devel```. This resolves the FreeType dependency.

    * The RPM package installs ```glew``` and ```glew-devel```. This resolves the GLEW dependency.

    * A packaged version of the ```GLFW``` library (named ```glfwTSGL```) is also installed along with TSGL. This resolves the GLFW dependency.

    * As with the Debian packages, please be sure to have ***at least OpenGL 3.0 or greater*** on your machine!

Given this, let's take a closer look at the ways you can install TSGL.

### Install TSGL From Binary Package
Please see the respective page for you OS:

* Linux
    + [Debian/Ubuntu](https://github.com/Calvin-CS/TSGL/wiki/Debian-%28Aptitude%29)

    + [Fedora/RPM](https://github.com/Calvin-CS/TSGL/wiki/RPM)

* MacOS X
    + [MacOS X](https://github.com/Calvin-CS/TSGL/wiki/MacOS-X)

* [Windows](https://github.com/Calvin-CS/TSGL/blob/master/readMeWindows.txt)

### Install TSGL From Source

Use the appropriate script for your OS:

* Linux
    + Ubuntu: [install-ubuntu.sh](https://github.com/Calvin-CS/TSGL/blob/master/install-ubuntu.sh)

    + Fedora: [install-fedora.sh](https://github.com/Calvin-CS/TSGL/blob/master/install-fedora.sh)

    + CentOS: [install-centos.sh](https://github.com/Calvin-CS/TSGL/blob/master/install-centos.sh)

* MacOS: [install-mac](https://github.com/Calvin-CS/TSGL/blob/master/install-mac)

(There are currently ***NO*** source install scripts for Windows).
