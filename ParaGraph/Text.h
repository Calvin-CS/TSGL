/*
 * Text.h extends Shape and provides a class for drawing a string of text to the Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/11/2014
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Shape.h"          // For extending our Shape object
#include "ImageLoader.h"

/*! \class Text
    \brief Draw a string of text.
    \details Text is a class for holding the data necessary for rendering a string of text.
    \note Text is aligned by the upper-left corner.
    \note At the moment, only a single font is supported.
*/
class Text : public Shape {
 private:
    Color myColor;
    ImageLoader myLoader;
    std::string myString;
    int myX, myY;
 public:
    /*!
     * \brief Explicitly constructs a new Text instance.
     * details This is the constructor for the Text class.
     *      \param s The string to draw.
     *      \param x The x coordinate.
     *      \param y The y coordinate.
     *      \param color A color.
     *      \return A new Text instance with the specified string, position, and color.
     */
    Text(std::string s, ImageLoader& loader, int x, int y, const Color &color);

    /*!
     *  \brief Draw the Text.
     *  \details This function actually draws the Text to the Canvas.
     */
    void draw();
};

#endif /* TEXT_H_ */
