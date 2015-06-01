/*
 * TextureHandler.h provides an interface for loading a variety of image formats and fonts into GL textures
 *
 * Author: Patrick Crain
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <iostream>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <GL/glew.h>     // Needed for GL function calls

#ifdef _WIN32
  #include <GL/glut.h>
#else
  #include <freetype.h>
  #include <ftglyph.h>
  #include <png.h>
  #include <jpeglib.h>
#endif

#include <GLFW/glfw3.h>  // For GL functions
#include <stdexcept>
#include <string>
#include <unordered_map>

typedef GLuint GLtexture;

class TextureHandler {
 private:
    typedef std::unordered_map<std::string, GLtexture> TextureMap;

    TextureMap loadedTextures;

#ifndef _WIN32
    typedef std::unordered_map<std::string, FT_Face> FontMap;
    FontMap loadedFonts;
    FT_Library fontLibrary;
    FT_Face fontFace;

    static void my_error_exit(j_common_ptr cinfo);
#endif

    static void createGLtextureFromBuffer(GLtexture &texture, unsigned char* buffer, const unsigned int &width,
                                          const unsigned int &height, int glMode);

    GLtexture loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;
    GLtexture loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;
    GLtexture loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;

    bool saveToPNG(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;
    bool saveToBMP(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;
 public:
    TextureHandler();
    ~TextureHandler();

    bool drawText(std::wstring text, unsigned int font_size, float* vertices);
    bool loadFont(const std::string& filename);
    GLtexture loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture);
    bool saveImageToFile(std::string filename, GLubyte *pixels, unsigned int w, unsigned int h) const;
};

#endif /* TEXTURELOADER_H_ */
