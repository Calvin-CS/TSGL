/*
* Ellipse.h extends ConvexPolygon and provides a class for drawing a ellipse to a Canvas.
*/

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "ConvexPolygon.h" // For extending our Shape object

namespace tsgl {

  /*! \class Ellipse
  *  \brief Draw a simple ellipse.
  *  \details Ellipse is a class for holding vertex data for an ellipse.
  */
class Ellipse : public ConvexPolygon {
 private:

 public:

    /*!
    * \brief Explicitly constructs a new Ellipse.
    * \details This function draws a ellipse with the given center, radius, xRadius, yRadius, and color.
    *   \param x The x coordinate of the ellipse's center.
    *   \param y The y coordinate of the ellipse's center.
    *   \param xRadius The horizontal radius of the ellipse in pixels.
    *   \param yRadius The vertical radius of the ellipse in pixels.
    *   \param color The color of the ellipse (set to BLACK by default).
    */
    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat& color, bool filled = true, bool outlined = false);

};

}

#endif /* ELLIPSE_H_ */