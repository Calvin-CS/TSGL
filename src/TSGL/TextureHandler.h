/*
 * TextureHandler.h provides an interface for loading a variety of image formats and fonts into GL textures.
 *
 * Author: Patrick Crain
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include "Assert.h"     // For unit testing purposes

#include <iostream>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <GL/glew.h>     // Needed for GL function calls

#ifdef _WIN32
  #include <GL/glut.h>
  #include <freetype/freetype.h>
  #include <freetype/ftglyph.h>
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
    typedef std::unordered_map<std::string, FT_Face> FontMap;

    TextureMap loadedTextures;
    FontMap loadedFonts;
    FT_Library fontLibrary;
    FT_Face fontFace;

#ifndef _WIN32
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
    /*!
     * \brief Constructs a TextureHandler object.
     * \details Default constructor for a TextureHandler object.
     * \return A new TextureHandler object with default values.
     */
    TextureHandler();

    /*!
     * \brief Destroys a TextureHandler object.
     * \details Destructor for a TextureHandler object.
     * \details Frees up memory that was allocated to a TextureHandler object.
     */
    ~TextureHandler();

    /*!
     * \brief Draw text.
     * \details Draws the text specified by its parameters onto a Canvas.
     *      \param text The string that contains the text to be drawn.
     *      \param font_size The size of the text.
     *      \param vertices The number of vertices that the shape of the text has.
     * \warning If no font has been specified then a default font will be loaded.
     * \return True if successful, false otherwise.
     */
    bool drawText(std::wstring text, unsigned int font_size, float* vertices);

    /*!
     * \brief Load a font.
     * \details Loads a font from a specified library given a specific file name.
     *      \param filename The file name that contains the font to be loaded.
     * \warning If the font cannot be found then an error message is printed out.
     * \warning If the font library is not correctly installed then an error message is printed out.
     * \warning If the font is not supported then an error message is printed out.
     * \return True if successful, false otherwise.
     */
    bool loadFont(const std::string& filename);

    /*!
     * \brief Load an image.
     * \details Loads a .png, .jpeg, or .bmp image from a file.
     *      \param filename The file name that contains the picture.
     *      \param width The width of the picture.
     *      \param height The height of the picture.
     *      \param texture The texture that contains the picture.
     * \return The texture that was found from opening the file that contains the image.
     */
    GLtexture loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture);

    /*!
     * \brief Saves an Image.
     * \details Saves an Image to file that was captured from a Canvas object.
     *      \param filename The name of the image file.
     *      \param pixels The number of pixels in the Image.
     *      \param w The width of the Image.
     *      \param h The height of the Image.
     * \return True if successful, false otherwise.
     */
    bool saveImageToFile(std::string filename, GLubyte *pixels, unsigned int w, unsigned int h) const;

    /*!
     *
     */
    static void runTests();
};

#endif /* TEXTURELOADER_H_ */
