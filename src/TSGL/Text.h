/*
 * Text.h extends Shape and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Drawable.h"          // For extending our Shape object
#include "TextureHandler.h"

namespace tsgl {

/*! \class Text
 *  \brief Draw a string of text.
 *  \details Text is a class for holding the data necessary for rendering a string of text.
 *  \note Text is aligned by the upper-left corner.
 *  \note Fonts supported by FreeType are also supported.
 */
class Text : public Drawable {
 private:
    ColorFloat myColor;
    unsigned int myFontSize;
    TextureHandler* myLoader;
    float * vertices;
    std::wstring myString;
    int myX, myY;
    float myRotation;
 public:
    Text(std::wstring text, int x, int y, unsigned int fontsize, const ColorFloat &color);

    virtual void draw();

    virtual void setText(std::wstring text);

    virtual void setFontSize(int fontsize);

    virtual void setFont(std::string filename);

    virtual void setBottomLeftCorner(int x, int y);

    virtual void setCenter(int x, int y);

    virtual void moveTextBy(int deltaX, int deltaY);

    virtual void setRotation(float radians);

    virtual void setColor(const ColorFloat& color);

    ~Text();
};

}

#endif /* TEXT_H_ */