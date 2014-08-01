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

#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <iostream>

#include <jpeglib.h>
#include <png.h>

#include <GL/gl.h>      // For GL functions
class ImageHandler {
 private:
    typedef std::unordered_map<std::string, GLuint> TextureMap;

    TextureMap loadedTextures;

    static void createGLTextureFromBuffer(GLuint &texture, unsigned char* buffer, unsigned int &width,
                                          unsigned int &height, int components);

    GLuint loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
                              GLuint &texture) const;
    GLuint loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
                              GLuint &texture) const;
    GLuint loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
                              GLuint &texture) const;

    static void my_error_exit(j_common_ptr cinfo);

    bool saveToPNG(const char* filename, GLubyte *pixels, int w, int h) const;
 public:
    ImageHandler() {
    }
    ~ImageHandler();

    GLuint loadTexture(std::string filename, unsigned int &width, unsigned int &height, GLuint &texture);
    bool saveImageToFile(std::string filename, GLubyte *pixels, int w, int h) const;
};

#endif /* IMAGELOADER_H_ */
