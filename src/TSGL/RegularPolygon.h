/*
* RegularPolygon.h extends ConvexPolygon and provides a class for drawing a regular polygon to a Canvas.
*/

#ifndef REGULAR_POLYGON_H_
#define REGULAR_POLYGON_H_

#include "ConvexPolygon.h" // For extending our ConvexPolygon object

namespace tsgl {

  /*! \class RegularPolygon
  *  \brief Draw a regular polygon.
  *  \details RegularPolygon is a class for holding ConvexPolygon data for a regular polygon.
  */
  class RegularPolygon : public ConvexPolygon {
  public:
    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color, bool filled = true);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color[], bool filled = true);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor[]);
    

  };

}

#endif /* REGULAR_POLYGON_H_ */