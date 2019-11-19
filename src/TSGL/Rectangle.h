/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas.
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "ConvexPolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class Rectangle
 *  \brief Draw a simple Rectangle.
 *  \details Rectangle is a class for holding vertex data for a simple rectangle.
 */
class Rectangle : public ConvexPolygon {
 private:
 
 public:

    Rectangle(float x, float y, float width, float height, const ColorFloat color, bool filled = true);

    Rectangle(float x, float y, float width, float height, const ColorFloat color[], bool filled = true);

    Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor);

    Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor[]);
};

}

#endif /* RECTANGLE_H_ */
