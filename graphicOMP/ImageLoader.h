/*
 * ImageLoader.h
 *
 *  Created on: Jul 8, 2014
 *      Author: pretzel
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
#include <png.h>
#include <iostream>
#include <string.h>

#include <GL/glew.h>		// For GL drawing calls
#include <GL/gl.h>			// For GL functions

class ImageLoader {
private:
//	GLubyte *textureImage;

public:
	static GLuint loadTexture(const char* filename, int &width, int &height, GLuint &texture);
};

#endif /* IMAGELOADER_H_ */
