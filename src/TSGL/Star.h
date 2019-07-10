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
  float myRadius;
  int myPoints;
public:
  Star(float x, float y, float radius, int points, ColorFloat color, bool ninja = false, bool filled = true);

  Star(float x, float y, float radius, int points, ColorFloat color[], bool ninja = false, bool filled = true);

  Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor, bool ninja = false);

  Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor, bool ninja = false);
  
  Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor[], bool ninja = false);

  Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor[], bool ninja = false);

};

}

#endif /* STAR_H_ */