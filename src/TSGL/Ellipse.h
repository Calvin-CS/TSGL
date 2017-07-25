/*
* Ellipse.h extends ConvexPolygon and provides a class for drawing a ellipse to a Canvas.
*/

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "ConvexPolygon.h" // For extending our ConvexPolygon object

namespace tsgl {

  /*! \class Ellipse
  *  \brief Draw a ellipse.
  *  \details Ellipse is a class for holding ConvexPolygon data for a ellipse.
  */
  class Ellipse : public ConvexPolygon {
  public:

    /*!
    * \brief Explicitly constructs a new Ellipse.
    * \details This function draws a ellipse with the given center, radius, xRadius, yRadius, and color.
    *   \param x The x coordinate of the ellipse's center.
    *   \param y The y coordinate of the ellipse's center.
    *   \param xRadius The horizontal radius of the ellipse in pixels.
    *   \param yRadius The vertical radius of the ellipse in pixels.
    *   \param color The color of the ellipse (set to BLACK by default).
    *   \param outlineColor The color of the ellipse's outline (set to BLACK by default).
    */
    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat& color = BLACK, const ColorFloat& outlineColor = BLACK);

  };

}

#endif /* ELLIPSE_H_ */
