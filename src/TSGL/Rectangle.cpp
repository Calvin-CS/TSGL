#include "Rectangle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a Rectangle with monocolored fill or outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param color The color of the Rectangle.
 *   \param filled Whether the Rectangle should be filled
 *     (set to true by default).
 * \return A new Rectangle with the specified top left corner, dimensions, and color.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat color, bool filled) : ConvexPolygon(4, filled, !filled) {
    addVertex(x, y, color);
    addVertex(x + width, y, color);
    addVertex(x + width, y + height, color);
    addVertex(x, y + height, color);
}

/*!
 * \brief Explicitly constructs a Rectangle with multicolored fill or outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param color An array of colors for the vertices of the Rectangle.
 *   \param filled Whether the Rectangle should be filled
 *     (set to true by default).
 * \return A new Rectangle with the specified top left corner, dimensions, and colors.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat color[], bool filled) : ConvexPolygon(4, filled, !filled) {
    addVertex(x, y, color[0]);
    addVertex(x + width, y, color[1]);
    addVertex(x + width, y + height, color[2]);
    addVertex(x, y + height, color[3]);
}

/*!
 * \brief Explicitly constructs a Rectangle with different monocolored fill and outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param fillColor The color of the Rectangle's fill
 *   \param outlineColor The color of the Rectangle's outline
 * \return A new Rectangle with the specified top left corner, dimensions, and coloring.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor, outlineColor);
    addVertex(x + width, y, fillColor, outlineColor);
    addVertex(x + width, y + height, fillColor, outlineColor);
    addVertex(x, y + height, fillColor, outlineColor);
}

/*!
 * \brief Explicitly constructs a Rectangle with multicolored fill and monocolored outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param fillColor An array of colors for the Rectangle's fill
 *   \param outlineColor The color of the Rectangle's outline
 * \return A new Rectangle with the specified top left corner, dimensions, and coloring.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor[0], outlineColor);
    addVertex(x + width, y, fillColor[1], outlineColor);
    addVertex(x + width, y + height, fillColor[2], outlineColor);
    addVertex(x, y + height, fillColor[3], outlineColor);
}

/*!
 * \brief Explicitly constructs a Rectangle with monocolored fill and multicolored outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param fillColor The color of the Rectangle's fill
 *   \param outlineColor An array of colors for the Rectangle's outline
 * \return A new Rectangle with the specified top left corner, dimensions, and coloring.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor, outlineColor[0]);
    addVertex(x + width, y, fillColor, outlineColor[1]);
    addVertex(x + width, y + height, fillColor, outlineColor[2]);
    addVertex(x, y + height, fillColor, outlineColor[3]);
}

/*!
 * \brief Explicitly constructs a Rectangle with different multicolored fill and outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param fillColor An array of colors for the Circle's fill
 *   \param outlineColor An array of colors for the Rectangle's outline
 * \return A new Rectangle with the specified top left corner, dimensions, and coloring.
 */
Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor[0], outlineColor[0]);
    addVertex(x + width, y, fillColor[1], outlineColor[1]);
    addVertex(x + width, y + height, fillColor[2], outlineColor[2]);
    addVertex(x, y + height, fillColor[3], outlineColor[3]);
}

}
