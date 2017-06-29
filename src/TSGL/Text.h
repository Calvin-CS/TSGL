/*
 * Text.h extends Drawable and provides a class for drawing a string of text to the Canvas.
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Drawable.h"          // For extending our Drawable object
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
    std::wstring myString;
    int myX, myY;
 public:

    /*!
     * \brief Explicitly constructs a new Text instance.
     * \details This is the constructor for the Text class.
     *      \param text The string to draw.
     *      \param loader A reference pointer to the TextureHandler with which to load the font.
     *      \param x The x coordinate.
     *      \param y The y coordinate.
     *      \param fontsize The size of the text in pixels.
     *      \param color A reference to the ColorFloat to use.
     * \return A new Text instance with the specified string, position, and color.
     */
    Text(std::wstring text, TextureHandler &loader, int x, int y, unsigned int fontsize, const ColorFloat &color);

    /*!
     * \brief Draw the Text.
     * \details This function actually draws the Text to the Canvas.
     */
    void draw();
};

}

#endif /* TEXT_H_ */
