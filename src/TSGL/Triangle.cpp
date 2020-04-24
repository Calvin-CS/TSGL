#include "Triangle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Triangle with monocolored fill or outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param z1 The z coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param z2 The z coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param z3 The z coordinate of the third endpoint.
 *      \param yaw The yaw of the triangle's rotation.
 *      \param pitch The pitch of the triangle's rotation.
 *      \param roll The roll of the triangle's rotation.
 *      \param color The color of the Triangle.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorGLfloat color) : ConvexPolygon((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3, 3,yaw,pitch,roll) {
    addVertex(x1, y1, z1, color);
    addVertex(x2, y2, z2, color);
    addVertex(x3, y3, z3, color);
}

/*!
 * \brief Explicitly constructs a new Triangle with multicolored fill or outline.
 * \details This is the constructor for the Triangle class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param z1 The z coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param z2 The z coordinate of the second endpoint.
 *      \param x3 The x coordinate of the third endpoint.
 *      \param y3 The y coordinate of the third endpoint.
 *      \param z3 The z coordinate of the third endpoint.
 *      \param yaw The yaw of the triangle's rotation.
 *      \param pitch The pitch of the triangle's rotation.
 *      \param roll The roll of the triangle's rotation.
 *      \param color An array of colors for the Triangle's vertices.
 * \return A new Triangle with the specified vertices and color.
 */
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll,  ColorGLfloat color[]) : ConvexPolygon((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3, 3,yaw,pitch,roll) {
    addVertex(x1, y1, z1, color[0]);
    addVertex(x2, y2, z2, color[1]);
    addVertex(x3, y3, z3, color[2]);
}
}
