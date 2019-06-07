#include "Triangle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Triangle with monocolored fill or outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param color The color of the Triangle.
 *      \param filled Whether the Triangle should be filled
 *          (set to true by default).
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color, bool filled) : ConvexPolygon(3, filled, !filled) {
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
    addVertex(x3, y3, color);
}

/*!
 * \brief Explicitly constructs a new Triangle with multicolored fill or outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param color An array of colors for the Triangle's vertices.
 *      \param filled Whether the Triangle should be filled
 *          (set to true by default).
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color[], bool filled) : ConvexPolygon(3, filled, !filled) {
    addVertex(x1, y1, color[0]);
    addVertex(x2, y2, color[1]);
    addVertex(x3, y3, color[2]);
}

/*!
 * \brief Explicitly constructs a new Triangle with different monocolored fill and outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param fillColor The color of the Triangle's fill.
 *      \param outlineColor The color of the Triangle's outline.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon(3, true, true) {
    addVertex(x1, y1, fillColor, outlineColor);
    addVertex(x2, y2, fillColor, outlineColor);
    addVertex(x3, y3, fillColor, outlineColor);
}

/*!
 * \brief Explicitly constructs a new Triangle with multicolored fill and monocolored outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param fillColor An array of colors for the Triangle's fill.
 *      \param outlineColor The color of the Triangle's outline.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon(3, true, true) {
    addVertex(x1, y1, fillColor[0], outlineColor);
    addVertex(x2, y2, fillColor[1], outlineColor);
    addVertex(x3, y3, fillColor[2], outlineColor);
}

/*!
 * \brief Explicitly constructs a new Triangle with monocolored fill and multicolored outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param fillColor The color of the Triangle's fill.
 *      \param outlineColor An array of colors for the Triangle's outline.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon(3, true, true) {
    addVertex(x1, y1, fillColor, outlineColor[0]);
    addVertex(x2, y2, fillColor, outlineColor[1]);
    addVertex(x3, y3, fillColor, outlineColor[2]);
}

/*!
 * \brief Explicitly constructs a new Triangle with different multicolored fill and outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param fillColor An array of colors for the Triangle's fill.
 *      \param outlineColor An array of colors for the Triangle's outline.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon(3, true, true) {
    addVertex(x1, y1, fillColor[0], outlineColor[0]);
    addVertex(x2, y2, fillColor[1], outlineColor[1]);
    addVertex(x3, y3, fillColor[2], outlineColor[2]);
}
}
