/*
 * Star.h extends ConcavePolygon and provides a class for drawing a star to a Canvas.
 */

#ifndef STAR_H_
#define STAR_H_

#include "ConcavePolygon.h"

namespace tsgl {

/*! \class Star
 *  \brief Draw a star
 *  \details Star extends ConcavePolygon
 */
class Star : public ConcavePolygon {
private:
  int myRadius, myPoints;
public:

      /*!
       * \brief Explicitly constructs a new Star.
       * \details This function draws a star with the given center,
       *   radius, points, and color.
       *   \param x The x coordinate of the star's center.
       *   \param y The y coordinate of the star's center.
       *   \param radius The radius of the star in pixels.
       *   \param points The number of points to use in the star.
       *   \param color The color of the star (set to BLACK by default).
       *   \param ninja The ninja setting of the star, making the star points spin differently if true
       *     (set to false by default).
       */
      Star(int x, int y, int radius, int points, ColorFloat color = BLACK, bool ninja = false);

};

}

#endif /* STAR_H_ */
