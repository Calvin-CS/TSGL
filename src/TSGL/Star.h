/*
 * Star.h extends Shape and provides a class for drawing a star to a Canvas.
 */

#ifndef STAR_H_
#define STAR_H_

#include "Shape.h"  // For extending our Shape object
#include "ConcavePolygon.h"

namespace tsgl {

/*! \class Star
 *  \brief Draw a star
 *  \details Star is a class for holding other Shape data to draw a star
 */
class Star : public Shape {
private:
  int myX, myY, myRadius, myPoints;
  ColorFloat myColor;
  ConcavePolygon * s;
public:

      /*!
       * \brief Explicitly constructs a new Star.
       * \details This function draws a star with the given center,
       *   radius, points, and color.
       *   \param x The x coordinate of the star's center.
       *   \param y The y coordinate of the star's center.
       *   \param radius The radius of the star in pixels.
       *   \param points The number of points to use in the star.
       *   \param color The color of the star
       *     (set to BLACK by default).
       */
      Star(int x, int y, int radius, int points, ColorFloat color = BLACK, bool ninja = true);

      /*!
       * \brief Draw the Star.
       * \details This function actually draws the Star to the Canvas.
       */
      void draw();

      int getX() { return myX; }
      int getY() { return myY; }
      ColorFloat getColor() { return myColor; }

      //TODO: comment this
      float* getVerticesPointerForRenderer();

};

}

#endif /* STAR_H_ */
