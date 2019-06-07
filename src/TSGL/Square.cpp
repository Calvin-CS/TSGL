#include "Square.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Square with monocolored fill or outline.
 * \details This function draws a Square with the given upper left corner, sidelength, and color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param color The color of the Square.
 *   \param filled Whether the Square should be filled
 *     (set to true by default).
 */
Square::Square(float x, float y, float sideLength, const ColorFloat color, bool filled)
: Rectangle(x,y,sideLength,sideLength,color,filled) { } //Create an Rectangle with equal width and height

/*!
 * \brief Explicitly constructs a new Square with multicolored fill or outline.
 * \details This function draws a Square with the given upper left corner, sidelength, and color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param color An array of colors for the Square's vertices.
 *   \param filled Whether the Square should be filled
 *     (set to true by default).
 */
Square::Square(float x, float y, float sideLength, const ColorFloat color[], bool filled)
: Rectangle(x,y,sideLength,sideLength,color,filled) { } //Create an Rectangle with equal width and height

/*!
 * \brief Explicitly constructs a new Square with different monocolored fill and outline.
 * \details This function draws a Square with the given upper left corner, sidelength, fillColor, and outline color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param fillColor The color of the Square's fill.
 *   \param outlineColor The color of the Square's outline.
 */
Square::Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor)
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

/*!
 * \brief Explicitly constructs a new Square with multicolored fill and monocolored outline.
 * \details This function draws a square with the given upper left corner, sidelength, fillColor, and outline color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param fillColor An array of colors for the Square's fill.
 *   \param outlineColor The color of the Square's outline.
 */
Square::Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor)
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

/*!
 * \brief Explicitly constructs a new Square with monocolored fill and multicolored outline.
 * \details This function draws a Square with the given upper left corner, sidelength, fillColor, and outline color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param fillColor The color of the Square's fill.
 *   \param outlineColor An array of colors for the Square's outline.
 */
Square::Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor[])
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

/*!
 * \brief Explicitly constructs a new Square with different multicolored fill and outline.
 * \details This function draws a Square with the given upper left corner, sidelength, fillColor, and outline color.
 *   \param x The x coordinate of the Square's left edge.
 *   \param y The y coordinate of the Square's top edge.
 *   \param sideLength The side length of the Square in pixels.
 *   \param fillColor An array of colors for the Square's fill.
 *   \param outlineColor An array of colors for the Square's outline.
 */
Square::Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor[])
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

}