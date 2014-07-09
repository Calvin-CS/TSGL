/*
 * ImageLoader.h
 *
 *  Created on: Jul 8, 2014
 *      Author: pretzel
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
#include <png.h>
#include <string>
#include <iostream>			// DEBUGGING

#include <GL/glew.h>		// For GL drawing calls
#include <GL/gl.h>			// For GL functions

namespace ImageLoader {
	GLuint loadTexture(std::string filename, int &width, int &height, GLuint &texture);
	GLuint loadTextureFromJPG(std::string filename, int &width, int &height, GLuint &texture);
}

#endif /* IMAGELOADER_H_ */
