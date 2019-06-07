#include "RegularPolygon.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new RegularPolygon with monocolored fill or outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and color.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param color The color of the regular polygon.
 *   \param filled Whether the regular polygon should be filled
 *     (set to true by default).
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color, bool filled) : ConvexPolygon(sides, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), color);
  }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with multicolored fill or outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param color An array of colors for the regular polygon's vertices.
 *   \param filled Whether the regular polygon should be filled
 *     (set to true by default).
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color[], bool filled) : ConvexPolygon(sides, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), color[i]);
  }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with different monocolored fill and outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param fillColor The color of the regular polygon's fill.
 *   \param outlineColor The color of the regular polygon's outline.
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor, outlineColor);
  }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with multicolored fill and monocolored outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param color The color of the regular polygon.
 *   \param fillColor An array of colors for the regular polygon's fill.
 *   \param outlineColor The color of the regular polygon's outline.
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor[i], outlineColor);
  }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with monocolored fill and multicolored outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param color The color of the regular polygon.
 *   \param fillColor The color of the regular polygon's fill.
 *   \param outlineColor An array of colors for the regular polygon's outline.
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor, outlineColor[i]);
  }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with different multicolored fill and outline.
 * \details This function draws a regular polygon with the given center, radius, resolution
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the regular polygon's center.
 *   \param y The y coordinate of the regular polygon's center.
 *   \param radius The radius of the regular polygon in pixels.
 *   \param sides The number of sides to use in the regular polygon.
 *   \param color The color of the regular polygon.
 *   \param fillColor An array of colors for the regular polygon's fill.
 *   \param outlineColor An array of colors for the regular polygon's outline.
 */
RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor[i], outlineColor[i]);
  }
}

} 