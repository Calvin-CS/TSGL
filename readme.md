TSGL
====

Thread Safe Graphics Library

You can generate Doxygen locally using 'make docs', or view the [TSGL API here](http://calvin-cs.github.io/TSGL/html/index.html).

------------
Description
------------
TSGL is a thread-safe graphics library perfect for drawing graphics. You can do a wide variety of things with TSGL, including: image manipulation and rendering (.bmp, .jpeg, and .png image formats supported), 2D polygon drawing (rectangles, circles, triangles, etc.), text rendering, animations with keyboard and/or mouse events, and much more. All drawing and rendering is done with threads and in parallel. This library is currently supported on Windows, Mac OS, and Linux. 3D graphics are currently not supported by this library.

If you would like TSGL in your local git repository, use the following command:

git clone https://github.com/Calvin-CS/TSGL.git

Otherwise, click the "Download zip" button to download a zipped up version.

------------
How to Get Started
------------
1. [Install the library](https://github.com/Calvin-CS/TSGL/wiki/Installing-TSGL).
2. Learn how to use the library, starting with [Using Canvas](https://github.com/Calvin-CS/TSGL/wiki/Using-Canvas).
3. Play with some visualizations or examples in TSGL/visualizations and TSGL/tests.
4. See the [API](http://calvin-cs.github.io/TSGL/html/index.html) for details beyond the tutorials.
4. Create something new!

------------
Goals
------------
The main goal of this library is to provide a thread-safe graphics library for 2D graphics. Other goals include: Helping beginning programming students learn about the complex process of parallelization by giving them hands-on tools to use in order to learn about parallelization without having them to worry about the problems associated with parallelization such as race conditions, mutexes, and more. It also helps educators teach programming students about parallelization through simple visualizations.

------------
docs and docs-wiki
------------
docs and docs-wiki are both submodules located inside of the TSGL root directory. docs contains the documentation for TSGL classes and docs-wiki contains the wiki pages. See the READMEMISC.txt file for information on how to initialize and update these submodules in your local git repository.
