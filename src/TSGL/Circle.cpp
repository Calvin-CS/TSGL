#include "Circle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new monocolored filled or outlined Circle.
 * \details This function draws a circle with the given center, radius, color, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param color The color of the circle's fill or outline
 *   \param filled Whether the circle should be filled
 *     (set to true by default).
 */
Circle::Circle(float x, float y, float radius, const ColorFloat color, bool filled)
: Ellipse(x,y,radius,radius,color,filled) { } //Create an Ellipse with equal x and y radii

/*!
 * \brief Explicitly constructs a new multicolored filled or outlined Circle.
 * \details This function draws a circle with the given center, radius, color, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param color An array of colors for the Circle's fill or outline
 *   \param filled Whether the circle should be filled
 *     (set to true by default).
 */
Circle::Circle(float x, float y, float radius, const ColorFloat color[], bool filled)
: Ellipse(x,y,radius,radius,color,filled) { } //Create an Ellipse with equal x and y radii

/*!
 * \brief Explicitly constructs a new Circle with different monocolored fill and outline.
 * \details This function draws a circle with the given center, radius, fillColor, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param fillColor The color of the circle's fill
 *   \param outlineColor The color of the circle's outline
 */
Circle::Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor)
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

/*!
 * \brief Explicitly constructs a new Circle with multicolored fill and monocolored outline.
 * \details This function draws a circle with the given center, radius, fillColor, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param fillColor An array of colors for the Circle's fill
 *   \param outlineColor The color of the circle's outline
 */
Circle::Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor)
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

/*!
 * \brief Explicitly constructs a new Circle with monocolored fill and multicolored outline.
 * \details This function draws a circle with the given center, radius, fillColor, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param fillColor The color of the circle's fill
 *   \param outlineColor An array of colors for the Circle's outline
 */
Circle::Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor[])
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

/*!
 * \brief Explicitly constructs a new Circle with different multicolored fill and outline.
 * \details This function draws a circle with the given center, radius, fillColor, and outline color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param fillColor An array of colors for the Circle's fill
 *   \param outlineColor An array of colors for the Circle's outline
 */
Circle::Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor[])
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

}