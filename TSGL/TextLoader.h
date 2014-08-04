/*
 * TextLoader.h
 *
 *  Created on: Aug 4, 2014
 *      Author: mbv26
 */

#ifndef TEXTLOADER_H_
#define TEXTLOADER_H_

#include <string>
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include "ImageHandler.h"
#include <GLFW/glfw3.h>

class TextLoader {
 private:
    FT_Library library;
    FT_Face    face;

 public:
    TextLoader();
    virtual ~TextLoader();
    bool load_font(const std::string& filename);
    GLuint load_char(const char filename, uint32_t font_size, unsigned int &width, unsigned int &height);
};

#endif /* TEXTLOADER_H_ */
