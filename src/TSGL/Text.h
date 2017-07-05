/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

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

    const char*         filename = "assets/freefont/FreeMono.ttf";
    // char*         filename;
    const char*         text = "Wazzup world?";
    // char*         text;

    double angle;
    int target_height;
    int n, num_chars;

    void error_check(int num = -1);

 public:


    Text(std::wstring text, TextureHandler &loader, int x, int y, unsigned int fontsize, const ColorFloat &color);
    Text();
    void testRender();

    void draw();
};

}

#endif /* TEXT_H_ */
