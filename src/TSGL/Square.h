/*
* Square.h extends Rectangle and provides a class for drawing a square to a Canvas.
*/

#ifndef SQUARE_H_
#define SQUARE_H_

#include "Rectangle.h" // For extending our Rectangle object

namespace tsgl {

  /*! \class Square
  *  \brief Draw a square.
  *  \details Square is a class for holding Shape data for a square.
  */
  class Square : public Rectangle {
  public:
    Square(float x, float y, float sideLength, const ColorFloat color, bool filled = true);

    Square(float x, float y, float sideLength, const ColorFloat color[], bool filled = true);

    Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor);

    Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor[]);

  };

}

#endif /* SQUARE_H_ */