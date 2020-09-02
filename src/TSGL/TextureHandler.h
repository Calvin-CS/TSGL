// /*
//  * TextureHandler.h provides an interface for loading a variety of image formats and fonts into GL textures.
//  */

// #ifndef TEXTURELOADER_H_
// #define TEXTURELOADER_H_

// #include <iostream>
// #include <ft2build.h>
// #include <stb/stb_image.h>
// #include <stb/stb_image_write.h>

// #include FT_FREETYPE_H
// #include FT_GLYPH_H

// #include <GL/glew.h>     // Needed for GL function calls

// #ifdef _WIN32
//   #include <GL/glut.h>
//   #include <freetype/freetype.h>
//   #include <freetype/ftglyph.h>
// //#else
// //  #include <freetype.h>
// //  #include <ftglyph.h>
// #endif

// #include <GLFW/glfw3.h>  // For GL functions
// #include <stdexcept>
// #include <string>
// #include <cstring>
// #include <unordered_map>
// #include <cassert>

// #include "Error.h"
// #include "TsglAssert.h"     // For unit testing purposes
// #include "Util.h"           // For testing for the existence of files

// typedef GLuint GLtexture;

// namespace tsgl {

// /*! \class TextureHandler
//  *  \brief Handles saving, loading, and rendering of images and textures.
//  *  \details TextureHandler provides an interface for saving, loading, and rendering images and text to Canvas
//  *    and CartesianCanvas through the use of GLTextures.
//  */
// class TextureHandler {
//  private:
//     typedef std::unordered_map<std::string, GLtexture> TextureMap;
//     typedef std::unordered_map<std::string, FT_Face> FontMap;

//     TextureMap loadedTextures;
//     FontMap loadedFonts;
//     FT_Library fontLibrary;
//     FT_Face fontFace;

//     static void createGLtextureFromBuffer(GLtexture &texture, unsigned char* buffer, const unsigned int &width,
//                                           const unsigned int &height, int glMode);
                                          
//     GLtexture loadTextureFromBMP(const char* filename, unsigned int &width, unsigned int &height,
//                                  GLtexture &texture) const;
//     GLtexture loadTextureFromJPG(const char* filename, unsigned int &width, unsigned int &height,
//                                  GLtexture &texture) const;
//     GLtexture loadTextureFromPNG(const char* filename, unsigned int &width, unsigned int &height,
//                                  GLtexture &texture) const;

//     bool saveToPNG(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;
//     bool saveToBMP(const char* filename, GLubyte *pixels, unsigned int w, unsigned int h) const;

//     static bool testLoadFont(TextureHandler& test);

//  public:
//     TextureHandler();

//     ~TextureHandler();

//     bool drawText(std::string text, unsigned int font_size, float* vertices);

//     bool loadFont(const std::string& filename);

//     void calculateTextDimensions(std::string text, unsigned int font_size, float& width, float& height);

//     static void getDimensions(std::string filename, int &width, int &height);

//     GLtexture loadPicture(std::string filename, unsigned int &width, unsigned int &height, GLtexture &texture);

//     bool saveImageToFile(std::string filename, GLubyte *pixels, unsigned int width, unsigned int height) const;


//     void drawGLtextureFromBuffer(GLubyte* buffer, int x, int y, unsigned int width, unsigned int height, int glMode);

//     static void runTests();
// };

// }

// #endif /* TEXTURELOADER_H_ */
