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

#include <iostream>        // DEBUGGING
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <jpeglib.h>
#include <png.h>

#include <GL/glew.h>    // For GL drawing calls
#include <GL/gl.h>      // For GL functions

class ImageLoader {
private:
    typedef std::unordered_map<std::string,GLuint> TextureMap;

    TextureMap loadedTextures;

    GLuint loadTextureFromBMP(const char* filename, int &width, int &height, GLuint &texture) const;
    GLuint loadTextureFromJPG(const char* filename, int &width, int &height, GLuint &texture) const;
    GLuint loadTextureFromPNG(const char* filename, int &width, int &height, GLuint &texture) const;

    static bool saveToPNG(const char *filename, GLubyte *pixels, int w, int h);
    static void my_error_exit (j_common_ptr cinfo);
public:
    ImageLoader() {}
    ~ImageLoader();

    GLuint loadTexture(std::string filename, int &width, int &height, GLuint &texture);
    static bool saveImageToFile(std::string filename, GLubyte *pixels, int w, int h);
};

#endif /* IMAGELOADER_H_ */
