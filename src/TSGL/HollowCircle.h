/*
 * HollowCircle.h extends Polyline and provides a class for drawing a circle to a Canvas.
 */

#ifndef HollowCircle_H_
#define HollowCircle_H_

#include "Shape.h"  // For extending our Polyline object
#include "Polyline.h"

namespace tsgl {

/*! \class HollowCircle
 *  \brief Draw a HollowCircle.
 *  \details HollowCircle is a class for holding Polyline data for a HollowCircle.
 */
class HollowCircle : public Polyline {
 private:
    int myX, myY, myRadius, mySides;
    bool isFilled;
 public:

    /*!
     * \brief Explicitly constructs a new HollowCircle.
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
    HollowCircle(int x, int y, int radius, int sides, ColorFloat color = BLACK);
};

}

#endif /* HollowCircle_H_ */
