/*
 * ImageLoader.h provides an interface for loading a variety of image formats into GL textures
 *
 * Author: Patrick Crain
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

//#ifdef _WIN32
//#include <GL/glut.h>
//#else
//#include <GLUT/glut.h>
//#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdexcept>

#include <png.h>			// For loading PNG files
#include <GL/glew.h>		// For GL drawing calls
#include <GL/gl.h>			// For GL functions

class ImageLoader {
public:
	static GLuint loadTextureFromBMP(const char* filename, int &width, int &height, GLuint &texture);
	static GLuint loadTextureFromPNG(const char* filename, int &width, int &height, GLuint &texture);
};

#endif /* IMAGELOADER_H_ */
