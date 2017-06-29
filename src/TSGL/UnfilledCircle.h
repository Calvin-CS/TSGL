/*
 * UnfilledCircle.h extends Polyline and provides a class for drawing a circle to a Canvas.
 */

#ifndef UNFILLEDCIRCLE_H_
#define UNFILLEDCIRCLE_H_

#include "Polyline.h" // For extending our Polyline object

namespace tsgl {

/*! \class UnfilledCircle
 *  \brief Draw a UnfilledCircle.
 *  \details UnfilledCircle is a class for holding Polyline data for a UnfilledCircle.
 */
class UnfilledCircle : public Polyline {
 private:
    int myX, myY, myRadius, mySides;
    bool isFilled;
 public:

    /*!
     * \brief Explicitly constructs a new UnfilledCircle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param sides The number of sides to use in the circle.
     *   \param color The color of the circle
     *     (set to BLACK by default).
     *   \param filled Whether the circle should be filled
     *     (set to true by default).
     */
    UnfilledCircle(int x, int y, int radius, int sides, ColorFloat color = BLACK);
};

}

#endif /* UNFILLEDCIRCLE_H_ */
