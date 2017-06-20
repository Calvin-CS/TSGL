/*
 * TextureHandler.h provides an interface for loading a variety of image formats and fonts into GL textures.
 */

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <iostream>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

// #include <GL/glew.h>     // Needed for GL function calls
#include "../glad/glad.h"      // New loader for GL function calls TODO: fix the path here

#ifdef _WIN32
  #include <GL/glut.h>
  #include <freetype/freetype.h>
  #include <freetype/ftglyph.h>
//#else
//  #include <freetype.h>
//  #include <ftglyph.h>
#endif

#include <GLFW/glfw3.h>  // For GL functions
#include <stdexcept>
#include <string>
#include <cstring>
#include <unordered_map>

#include "Error.h"
#include "TsglAssert.h"     // For unit testing purposes
#include "Util.h"           // For testing for the existence of files

typedef GLuint GLtexture;

namespace tsgl {

/*! \class TextureHandler
 *  \brief Handles saving, loading, and rendering of images and textures.
 *  \details TextureHandler provides an interface for saving, loading, and rendering images and text to Canvas
 *    and CartesianCanvas through the use of GLTextures.
 */
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

    bool saveToPNG(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;
    bool saveToBMP(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;

    static bool testLoadFont(TextureHandler& test);

 public:

    /*!
     * \brief Default TextureHandler constructor method.
     * \details This is the default constructor for theTextureHandler Canvas class.
     * \return A new TextureHandler instance.
     */
    TextureHandler();

    /*!
     * \brief TextureHandler destructor method.
     * \details This is the destructor for the TextureHandler class.
     * \details Frees up memory that was allocated to a TextureHandler instance.
     */
    ~TextureHandler();

    /*!
     * \brief Draws text.
     * \details Draws the text specified by its parameters onto a Canvas.
     *   \param text The UTF-8 encoded string of text to be drawn.
     *   \param font_size The size of the text in pixels.
     *   \param vertices An array of vertex data for the bonding box of the text.
     * \note <code>vertices</code> will be partially automatically set by drawText()
     *   itself in order to draw / kern the text properly, but the color, starting
     *   position, and texture coordinates will be left unchanged.
     * \note If no font is loaded before calling this function, TSGL will attempt to locate a
     *   default font at <i>../assets/freefont/FreeMono.ttf.</i>
     * \return True if successful, false otherwise.
     * \bug If the default font cannot be located, TSGL will crash.
     */
    bool drawText(std::wstring text, unsigned int font_size, float* vertices);

    /*!
     * \brief Loads a font.
     * \details Loads a font from the library given by <code>filename</code>.
     *   \param filename The file name of the font to be loaded.
     * \warning If the font cannot be found then an error message is printed out.
     * \warning If the font library is not correctly installed then an error message is printed out.
     * \warning If the font is not supported then an error message is printed out.
     * \return True if successful, false otherwise.
     */
    bool loadFont(const std::string& filename);

    /*!
     * \brief Gets the dimensions of an image
     * \details Loads the header of a .png, .jpeg, or .bmp image to read their dimensions.
     *   \param filename The file name of the picture.
     *   \param width A reference variable for holding the width of the picture.
     *   \param height A reference variable for holding the height of the picture.
     * \return The texture that created from the loaded image.
     */
    static void getDimensions(std::string filename, int &width, int &height);

    /*!
     * \brief Loads an image.
     * \details Loads a .png, .jpeg, or .bmp image from a file.
     *   \param filename The file name of the picture.
     *   \param width A reference variable for holding the width of the picture.
     *   \param height A reference variable for holding the height of the picture.
     *   \param texture A reference variable for holding the texture of the picture.
     *     (same as return value)
     * \return The texture that created from the loaded image.
     */
    GLtexture loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture);

    /*!
     * \brief Saves an Image.
     * \details Saves an Image to file that was captured from a Canvas object.
     *   \param filename The name of the file to save the Image to.
     *   \param pixels The pixel data for the Image.
     *   \param width The width of the Image.
     *   \param height The height of the Image.
     * \return True if successful, false otherwise.
     */
    bool saveImageToFile(std::string filename, GLubyte *pixels, unsigned int width, unsigned int height) const;

    /*!
     * \brief Runs the Unit tests for TextureHandler.
     */
    static void runTests();
};

}

#endif /* TEXTURELOADER_H_ */
