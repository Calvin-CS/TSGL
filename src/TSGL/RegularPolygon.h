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

    /*!
    * \brief Explicitly constructs a new RegularPolygon.
    * \details This function draws a regular polygon with the given center, radius, resolution
    *   (number of sides), and color.
    *   \param x The x coordinate of the regular polygon's center.
    *   \param y The y coordinate of the regular polygon's center.
    *   \param radius The radius of the regular polygon in pixels.
    *   \param sides The number of sides to use in the regular polygon.
    *   \param color The color of the regular polygon (set to BLACK by default).
    *   \param outlineColor The color of the regular polygon's outline (set to BLACK by default).
    */
    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color = BLACK, bool filled = true, bool outlined = false);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color[], bool filled = true, bool outlined = false);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor, bool filled = true, bool outlined = false);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor, bool filled = true, bool outlined = false);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor[], bool filled = true, bool outlined = false);

    RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor[], bool filled = true, bool outlined = false);
    

  };

}

#endif /* REGULAR_POLYGON_H_ */