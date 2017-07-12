/*
 * UnfilledCircle.h extends UnfilledShape and provides a class for drawing a circle to a Canvas.
 */

#ifndef UNFILLED_CIRCLE_H_
#define UNFILLED_CIRCLE_H_

#include "UnfilledShape.h" // For extending our UnfilledShape object

namespace tsgl {

/*! \class UnfilledCircle
 *  \brief Draw a UnfilledCircle.
 *  \details UnfilledCircle is a class for holding UnfilledShape data for a UnfilledCircle.
 */
class UnfilledCircle : public UnfilledShape {
 private:
    int myX, myY, myRadius, mySides;
 public:

    /*!
     * \brief Explicitly constructs a new UnfilledCircle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param sides The number of sides to use in the circle.
     *   \param color The color of the circle (set to BLACK by default).
     */
    UnfilledCircle(int x, int y, int radius, int sides, ColorFloat color = BLACK);
};

}

#endif /* UNFILLED_CIRCLE_H_ */
