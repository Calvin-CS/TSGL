#include "Ellipse.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new monocolored filled or outlined Ellipse.
 * \details This function draws a Ellipse with the given center, radii, color, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param color The color of the Ellipse's fill or outline
 *   \param filled Whether the Ellipse should be filled
 *     (set to true by default).
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color, bool filled) : ConvexPolygon((xRadius + yRadius) / 2, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), color);
  }
}

/*!
 * \brief Explicitly constructs a new multicolored filled or outlined Ellipse.
 * \details This function draws a Ellipse with the given center, radii, color, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param color An array of colors for the Ellipse's fill or outline
 *   \param filled Whether the Ellipse should be filled
 *     (set to true by default).
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color[], bool filled) : ConvexPolygon((xRadius + yRadius) / 2, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), color[i]);
  }
}

/*!
 * \brief Explicitly constructs a new Ellipse with different monocolored fill and outline.
 * \details This function draws a Ellipse with the given center, radii, fillColor, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param fillColor The color of the Ellipse's fill
 *   \param outlineColor The color of the Ellipse's outline
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon((xRadius + yRadius) / 2, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor, outlineColor);
  }
}

/*!
 * \brief Explicitly constructs a new Ellipse with multicolored fill and monocolored outline.
 * \details This function draws a Ellipse with the given center, radii, fillColor, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param fillColor An array of colors for the Ellipse's fill
 *   \param outlineColor The color of the Ellipse's outline
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon((xRadius + yRadius) / 2, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor[i], outlineColor);
  }
}

/*!
 * \brief Explicitly constructs a new Ellipse with monocolored fill and multicolored outline.
 * \details This function draws a Ellipse with the given center, radii, fillColor, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param fillColor The color of the Ellipse's fill
 *   \param outlineColor An array of colors for the Ellipse's outline
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon((xRadius + yRadius) / 2, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor, outlineColor[i]);
  }
}

/*!
 * \brief Explicitly constructs a new Ellipse with different multicolored fill and outline.
 * \details This function draws a Ellipse with the given center, radius, fillColor, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param fillColor An array of colors for the Ellipse's fill
 *   \param outlineColor An array of colors for the Ellipse's outline
 */
Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon((xRadius + yRadius) / 2, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor[i], outlineColor[i]);
  }
}


}