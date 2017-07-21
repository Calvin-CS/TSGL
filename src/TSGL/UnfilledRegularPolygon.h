/*
 * UnfilledRegularPolygon.h extends UnfilledShape and provides a class for drawing a regular polygon to a Canvas.
 */

#ifndef UNFILLED_REGULAR_POLYGON_H_
#define UNFILLED_REGULAR_POLYGON_H_

#include "UnfilledShape.h" // For extending our UnfilledShape object

namespace tsgl {

/*! \class UnfilledRegularPolygon
 *  \brief Draw a UnfilledRegularPolygon.
 *  \details UnfilledRegularPolygon is a class for holding UnfilledShape data for a UnfilledRegularPolygon.
 */
class UnfilledRegularPolygon : public UnfilledShape {
 private:
    float myX, myY, myRadius;
    int mySides;
 public:

    /*!
     * \brief Explicitly constructs a new UnfilledRegularPolygon.
     * \details This function draws a regular polygon with the given center, radius, resolution
     *   (number of sides), color.
     *   \param x The x coordinate of the polygon's center.
     *   \param y The y coordinate of the polygon's center.
     *   \param radius The radius of the polygon in pixels.
     *   \param sides The number of sides to use in the polygon.
     *   \param color The color of the polygon (set to BLACK by default).
     */
    UnfilledRegularPolygon(float x, float y, float radius, int sides, ColorFloat color = BLACK);
};

}

#endif /* UNFILLED_REGULAR_POLYGON_H_ */
