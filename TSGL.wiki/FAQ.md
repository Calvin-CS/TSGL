Welcome to the FAQ page! In here, we try to answer the most common questions that users may have in regards to TSGL.

#### Categories: ####

[Installing TSGL](#installing-tsgl)

[Errors and Debugging](#errors-and-debugging)

[How do I...?](#using-tsgl)

### Errors and Debugging ###

***Q:*** A test keeps crashing with the error "the font file could not be opened and read"! What do I do?

***A:*** Are you in the bin folder of the tsgl folder? Some tests won't work unless you are in the bin folder. Specifically:

```testBlurImage```, ```testCalcPi```, ```testCosineIntegral```, ```testGetPixels```, ```testGreyscale```,  ```testImage```, ```testImageCart```, ```testInverter```, ```testMandelbrot```, ```testPhilosophers```, ```testProgressBar```, ```testSpectrogram```, ```testText```, ```testTextCart```, ```testTextTwo```, ```testUnits```

If the test that keeps crashing is in the list above, go into the bin folder and execute it from there.

***Q:*** Does TSGL have Debug messages in case things break?

***A:*** In fact it does. Check out the [Error.h](http://calvin-cs.github.io/TSGL/html/_error_8h_source.html) file and you will see a commented out line:

~~~{.cpp}
//#define TSGL_DEBUG
~~~

Uncommenting out this line will turn on Debug messages for the TSGL library in case things break (font not loading, image not found, etc...).

***Q:*** What if I want to tell you about a different problem?

***A:*** You can bring up an issue in the [issues tab](Isssues and Reviews).

### Installing TSGL ###

***Q:*** With which operating systems does TSGL work?

***A:*** Good question! TSGL has been successfully tested on:

#### Linux

* Ubuntu 14.04 (Trusty Tahr)
* Ubuntu 16.04 (Xenial Xerus)
* Arch Linux

(With g++-4.8, g++-4.9, g++-5.1).

#### Red Hat

* CentOS 7
* Fedora 24

(With g++-4.8).

#### Windows

* Windows 7
* Windows 10

(With Visual Studio 2012, Visual Studio 2013, and Visual Studio 2015).

#### Mac OS X

* MacOS 10.9.5 (Wrangler)
* MacOS 10.10.x (Yosemite)
* MacOS 10.11.x (El Capitan)

(With g++-4.8, g++4.9, g++-5).

***Q:*** What is the maximum number of threads I can use in Windows?

***A:*** You can only have up to 64 threads at a time in Windows.

***Q:*** Why won't my tsgl Solution build in Windows?

***A:*** Have you built the tsgl project first? You must build that first in order to create the lib file for TSGL. Then, you can build the entire Solution.

***Q:*** I'm an Ubuntu user. Is there a Debian package out there for TSGL?

***A:*** Indeed there is! As of summer 2016, TSGL has a Debian package for easy installation. See the [[Debian (Aptitude)]] page.

***Q:*** What about Red Hat users? Is there an RPM package?

***A:*** Of course! See the [[RPM]] page!

***Q:*** So, if TSGL has a Debian package, and an RPM package, how do I create new ones?

***A:*** Take a look at our [[Creating New Deb Packages]] and [[Creating New RPM Packages]] pages. However, it is not recommended, as the process for creating these packages has already been automated.

***Q:*** A new version of Visual Studio has come out, but there's no TSGL Solution made! How do I create one?

***A:*** Check out our [[New Visual Studio Versions]] page for detailed instructions on how to make a new Visual Studio Solution.

***Q:*** What about a binary installer for MacOS X? If I recall correctly, it was especially difficult to install TSGL on a Mac...

***A:*** Yes, the process was a tad bit difficult for MacOS X users. Luckily, we have made a new MacOS X .pkg Installer which automates the entire installation process. Take a look at the [[MacOS X]] page. (There is also a refactored [install-mac](https://github.com/Calvin-CS/TSGL/blob/master/install-mac) script, if you wish to install TSGL from source agan).

***Q:*** What about creating new MacOS X .pkg Installers?

***A:*** Excellent question! See the [[Creating New MacOS X Packages]] page.

### Using TSGL ###

***Q:*** How do I update the Doxygen pages? Or the Wiki pages? I found that a change needs to be made.

***A:*** I'm glad you asked! See the [[Doxygen and Wiki]] page.

***Q:*** How do I know which Canvas I should be using?

***A:*** We have four different Canvases, and you can know which one to use by asking two questions about your project. First, what kind of coordinates do you want? If you want standard coordinates based on pixels, then you want the standard Canvas. If you want to graph a function or use Cartesian coordinates, then you should be using one of the Cartesian Canvases. Second, how do you want the Canvas to update? If you want it to draw everything every frame, which keeps up for hundreds of items, then use a standard Canvas. (See ReaderWriter, ProducerConsumer, or DiningPhilosophers.) This way allows you to move shapes around or take them off the Canvas. If you want it to draw once and then never again, use the Raster version of the Canvas. This is necessary if you want to use drawPoint() and can keep up if you want to draw a point on every pixel of the Canvas. (See Mandelbrot.)
