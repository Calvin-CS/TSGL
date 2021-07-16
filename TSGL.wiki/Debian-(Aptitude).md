***NOTE:*** This page is for Ubuntu/Debian users. If you are a Fedora/RedHat user, please see the [[RPM]] page!

***NOTE:*** Please make SURE that you have ***at least OpenGL 3.0 or greater*** on your machine. 

As of Summer 2016, an aptitude/debian package has been made for TSGL for ease of installation. Packages have been made for the following Ubuntu distros:

* Ubuntu 14.04 (Trusty Tahr)
* Ubuntu 16.04 (Xenial Xerus)
 
If you have one of those distros, you can use ```apt-get``` to install the TSGL library.

Follow these steps in order to get TSGL on your machine:

1). Type ```sudo add-apt-repository ppa:tsgl-test/tsgl-dev```. When prompted, hit ```ENTER```.

2). Type ```sudo apt-get update```. 

3). Type ```sudo apt-get install tsgl```. 

TSGL will be installed along with a packaged version of the GLFW library.

The generic ```Makefile``` for creating programs (as seen in the tutorials, i.e. [[Building Programs]]) is located in the ```TSGL_GENERIC_MAKEFILE``` directory under ```/usr/include/```.

Go into that directory, and copy over the ```Makefile``` into your project directory in order to start building applications that use TSGL.  