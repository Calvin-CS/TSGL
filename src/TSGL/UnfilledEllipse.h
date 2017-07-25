/*
* UnfilledEllipse.h extends UnfilledShape and provides a class for drawing a ellipse outline to a Canvas.
*/

#ifndef UNFILLED_ELLIPSE_H_
#define UNFILLED_ELLIPSE_H_

#include "UnfilledShape.h" // For extending our UnfilledShape object

namespace tsgl {

  /*! \class UnfilledEllipse
  *  \brief Draw a ellipse.
  *  \details UnfilledEllipse is a class for holding UnfilledShape data for a ellipse.
  */
  class UnfilledEllipse : public UnfilledShape {
  public:

    /*!
    * \brief Explicitly constructs a new UnfilledEllipse.
    * \details This function draws a ellipse with the given center, radius, xRadius, yRadius, and color.
    *   \param x The x coordinate of the ellipse's center.
    *   \param y The y coordinate of the ellipse's center.
    *   \param xRadius The horizontal radius of the ellipse in pixels.
    *   \param yRadius The vertical radius of the ellipse in pixels.
    *   \param color The color of the ellipse (set to BLACK by default).
    */
    UnfilledEllipse(float x, float y, float xRadius, float yRadius, const ColorFloat& color = BLACK);

  };

}

#endif /* UNFILLED_ELLIPSE_H_ */
