/*
 * ImageLoader.h provides an interface for loading a variety of image formats into GL textures
 *
 * Author: Patrick Crain
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#ifdef _WIN32
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <jpeglib.h>
#include <png.h>			// For loading PNG files

#include <iostream>			// DEBUGGING
#include <fstream>
#include <string>
#include <stdexcept>

#include <GL/glew.h>		// For GL drawing calls
#include <GL/gl.h>			// For GL functions

namespace ImageLoader {
	GLuint loadTextureFromBMP(std::string filename, int &width, int &height, GLuint &texture);
	GLuint loadTextureFromJPG(std::string filename, int &width, int &height, GLuint &texture);
	GLuint loadTextureFromPNG(std::string filename, int &width, int &height, GLuint &texture);
}

#endif /* IMAGELOADER_H_ */
