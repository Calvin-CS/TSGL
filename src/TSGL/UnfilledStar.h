/*
 * UnfilledStar.h extends UnfilledShape and provides a class for drawing an unfilled star to a Canvas.
 */

#ifndef UNFILLED_STAR_H_
#define UNFILLED_STAR_H_

#include "UnfilledShape.h"

namespace tsgl {

/*! \class UnfilledStar
 *  \brief Draw a star
 *  \details UnfilledStar extends UnfilledShape
 */
class UnfilledStar : public UnfilledShape {
private:
  int myRadius, myPoints;
public:

      /*!
       * \brief Explicitly constructs a new UnfilledStar.
       * \details This function draws a star with the given center,
       *   radius, points, and color.
       *   \param x The x coordinate of the star's center.
       *   \param y The y coordinate of the star's center.
       *   \param radius The radius of the star in pixels.
       *   \param points The number of points on the star.
       *   \param color The color of the star
       *     (set to BLACK by default).
       *   \param ninja The ninja setting of the star, making the star points twisted differently if true
       *     (set to false by default).
       */
      UnfilledStar(int x, int y, int radius, int points, ColorFloat color = BLACK, bool ninja = false);

};

}

#endif /* UNFILLED_STAR_H_ */
