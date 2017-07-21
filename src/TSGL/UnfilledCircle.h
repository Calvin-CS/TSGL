/*
 * UnfilledCircle.h extends UnfilledRegularPolygon and provides a class for drawing a circle to a Canvas.
 */

#ifndef UNFILLED_CIRCLE_H_
#define UNFILLED_CIRCLE_H_

#include "UnfilledRegularPolygon.h" // For extending our UnfilledRegularPolygon object

namespace tsgl {

/*! \class UnfilledCircle
 *  \brief Draw a UnfilledCircle.
 *  \details UnfilledCircle is a class for holding UnfilledRegularPolygon data for a UnfilledCircle.
 */
class UnfilledCircle : public UnfilledRegularPolygon {
public:

    /*!
     * \brief Explicitly constructs a new UnfilledCircle.
     * \details This function draws a circle with the given center, radius, and color.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param color The color of the circle (set to BLACK by default).
     */
    UnfilledCircle(float x, float y, float radius, ColorFloat color = BLACK);
};

}

#endif /* UNFILLED_CIRCLE_H_ */
