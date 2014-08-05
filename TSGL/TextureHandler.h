/*
 * TextureHandler.h provides an interface for loading a variety of image formats and fonts into GL textures
 *
 * Author: Patrick Crain
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#ifdef _WIN32
#include <GL/glut.h>
#endif

#include <string>
#include <stdexcept>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <jpeglib.h>
#include <png.h>
#include <GLFW/glfw3.h>  // For GL functions

typedef GLuint GLtexture;

struct Character {
    GLtexture texture;
    int width;
    int height;
    int left;
    int up;
    int dx;
    int dy;
};

class TextureHandler {
 private:
    typedef std::unordered_map<std::string, GLtexture> TextureMap;
    typedef std::unordered_map<std::string, FT_Face> FontMap;

    TextureMap loadedTextures;
    FontMap loadedFonts;

    FT_Library fontLibrary;
    FT_Face fontFace;

    static void createGLtextureFromBuffer(GLtexture &texture, unsigned char* buffer, const unsigned int &width,
                                          const unsigned int &height, int glMode);

    GLtexture loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;
    GLtexture loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;
    GLtexture loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
                                 GLtexture &texture) const;

    static void my_error_exit(j_common_ptr cinfo);

    bool saveToPNG(const char* filename, GLubyte *pixels, int w, int h) const;
 public:
    TextureHandler();
    ~TextureHandler();

    bool loadFont(const std::string& filename);
    Character loadChar(const char character, unsigned int font_size);

    GLtexture loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture);
    bool saveImageToFile(std::string filename, GLubyte *pixels, int w, int h) const;
};

#endif /* TEXTURELOADER_H_ */
