/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
//TODO move the includes up in the stack so they can be used by the canvas??

#include "Drawable.h"          // For extending our Drawable object
#include "TextureHandler.h"

namespace tsgl {

class Text : public Drawable {
 private:

    FT_Library    library;
    FT_Face       face;

    FT_GlyphSlot  slot;
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;

    int base_x;
    int base_y;

    ColorFloat color;
    unsigned int fontsize;
    int space_size;

    bool useKerning = true;

    struct character_object {
      char character;
      bool isSpace = false;
      int width;
      int height;
      int advance_x;
      int advance_y;
      int bearing;
      unsigned long int buffer_len;
      char* bitmap_buffer;
    };

    std::vector<character_object*> char_vec; // Hold pointers to the buffers for the character objects

    int maxBearing;

    const char*         filename;
    const char*         text;

    double angle;
    int target_height;
    int n, num_chars;

    GLuint texID = 0;

 public:


    Text(std::string t, int x, int y, unsigned int font_size, const ColorFloat &c, std::string fname = "assets/freefont/FreeSans.ttf");
    void Text::generateTextBitmaps();
    void render();

    int getStringWidth();

    void draw();
};

}

#endif /* TEXT_H_ */
