/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Drawable.h"          // For extending our Shape object
#include <iostream>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

namespace tsgl {

/*! \class Text
 *  \brief Draw a string of text.
 *  \details Text is a class for holding the data necessary for rendering a string of text.
 *  \note Text is aligned by the upper-left corner.
 *  \note Fonts supported by FreeType are also supported.
 */
class Text : public Drawable {
 private:
    std::wstring myString;
    float mySize;
    std::string myFont;
    ColorFloat myColor;

    GLfloat myWidth;
    GLfloat myHeight;

    FT_Face face;
    FT_Library ft;

    struct Character {
        FT_Bitmap    Bitmap;
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::map<wchar_t, Character> Characters;

    void populateCharacters();
 public:
    Text(float x, float y, float z, std::wstring text, std::string fontFilename, float size, float yaw, float pitch, float roll, const ColorFloat &color);

    virtual void draw(Shader * shader);

    virtual void setText(std::wstring text);

    virtual void setSize(float size);

    virtual void setFont(std::string filename);

    virtual void setColor(const ColorFloat& color);

    std::wstring getText() { return myString; }

    float getSize() { return mySize; }

    GLfloat getWidth() { return myWidth; }
    GLfloat getHeight() { return myHeight; }

    ColorFloat getColor() { return myColor; }

    ~Text();
};

}

#endif /* TEXT_H_ */