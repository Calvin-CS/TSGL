/*
* Circle.h extends ConvexPolygon and provides a class for drawing a circle to a Canvas.
*/

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "ConvexPolygon.h" // For extending our ConvexPolygon object

namespace tsgl {

  /*! \class Circle
  *  \brief Draw a circle.
  *  \details Circle is a class for holding ConvexPolygon data for a circle.
  */
  class Circle : public ConvexPolygon {
  public:

    /*!
    * \brief Explicitly constructs a new Circle.
    * \details This function draws a circle with the given center, radius, resolution
    *   (number of sides), color, and fill status.
    *   \param x The x coordinate of the circle's center.
    *   \param y The y coordinate of the circle's center.
    *   \param radius The radius of the circle in pixels.
    *   \param sides The number of sides to use in the circle.
    *   \param color The color of the circle (set to BLACK by default).
    *   \param outlineColor The color of the circle's outline (set to BLACK by default).
    */
    Circle(float x, float y, float radius, int sides, const ColorFloat& color = BLACK, const ColorFloat& outlineColor = BLACK);

  };

}

#endif /* CIRCLE_H_ */
