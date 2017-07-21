/*
* Circle.h extends RegularPolygon and provides a class for drawing a circle to a Canvas.
*/

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "RegularPolygon.h" // For extending our RegularPolygon object

namespace tsgl {

  /*! \class Circle
  *  \brief Draw a circle.
  *  \details Circle is a class for holding Shape data for a circle.
  */
  class Circle : public RegularPolygon {
  public:

    /*!
    * \brief Explicitly constructs a new Circle.
    * \details This function draws a circle with the given center, radius, color, and outline color.
    *   \param x The x coordinate of the circle's center.
    *   \param y The y coordinate of the circle's center.
    *   \param radius The radius of the circle in pixels.
    *   \param color The color of the circle (set to BLACK by default).
    *   \param outlineColor The color of the circle's outline (set to BLACK by default).
    */
    Circle(float x, float y, float radius, const ColorFloat& color = BLACK, const ColorFloat& outlineColor = BLACK);

  };

}

#endif /* CIRCLE_H_ */
