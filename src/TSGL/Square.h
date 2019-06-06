/*
* Square.h extends Rectangle and provides a class for drawing a square to a Canvas.
*/

#ifndef SQUARE_H_
#define SQUARE_H_

#include "Rectangle.h" // For extending our Ellipse object

namespace tsgl {

  /*! \class Square
  *  \brief Draw a square.
  *  \details Square is a class for holding Shape data for a square.
  */
  class Square : public Rectangle {
  public:

    /*!
    * \brief Explicitly constructs a new Square.
    * \details This function draws a square with the given upper left corner, sidelength, color, and outline color.
    *   \param x The x coordinate of the square's center.
    *   \param y The y coordinate of the square's center.
    *   \param sideLength The side length of the square in pixels.
    *   \param color The color of the circle (set to BLACK by default).
    *   \param outlineColor The color of the circle's outline (set to BLACK by default).
    */
    Square(float x, float y, float sideLength, const ColorFloat color = BLACK, bool filled = true);

    Square(float x, float y, float sideLength, const ColorFloat color[], bool filled = true);

    Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor);

    Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor[]);

  };

}

#endif /* SQUARE_H_ */