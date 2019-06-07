#include "Line.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param color The reference variable to the color of the Line.
 * \return A new Line with the specified endpoints and color.
 */
Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color) : Polyline(2) {
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
}

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param color An array for the colors of the line endpoints.
 * \return A new Line with the specified endpoints and colors.
 */
Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color[]) : Polyline(2) {
    addVertex(x1, y1, color[0]);
    addVertex(x2, y2, color[1]);
}
}
