#include "Triangle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Triangle with monocolored fill.
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
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3, 3,yaw,pitch,roll) {
    float xAverage = (x1 + x2 + x3) / 3;
    float yAverage = (y1 + y2 + y3) / 3;
    float zAverage = (z1 + z2 + z3) / 3;
    addVertex(x1 - xAverage, y1 - yAverage, z1 - zAverage, color);
    addVertex(x2 - xAverage, y2 - yAverage, z2 - zAverage, color);
    addVertex(x3 - xAverage, y3 - yAverage, z3 - zAverage, color);
    addOutlineVertex(x1 - xAverage, y1 - yAverage, z1 - zAverage, GRAY);
    addOutlineVertex(x2 - xAverage, y2 - yAverage, z2 - zAverage, GRAY);
    addOutlineVertex(x3 - xAverage, y3 - yAverage, z3 - zAverage, GRAY);
    geometryType = GL_TRIANGLES;
}

/*!
 * \brief Explicitly constructs a new Triangle with multicolored fill.
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
Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll,  ColorFloat color[]) : ConvexPolygon((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3, 3,yaw,pitch,roll) {
    float xAverage = (x1 + x2 + x3) / 3;
    float yAverage = (y1 + y2 + y3) / 3;
    float zAverage = (z1 + z2 + z3) / 3;
    addVertex(x1 - xAverage, y1 - yAverage, z1 - zAverage, color[0]);
    addVertex(x2 - xAverage, y2 - yAverage, z2 - zAverage, color[1]);
    addVertex(x3 - xAverage, y3 - yAverage, z3 - zAverage, color[2]);
    addOutlineVertex(x1 - xAverage, y1 - yAverage, z1 - zAverage, GRAY);
    addOutlineVertex(x2 - xAverage, y2 - yAverage, z2 - zAverage, GRAY);
    addOutlineVertex(x3 - xAverage, y3 - yAverage, z3 - zAverage, GRAY);
    geometryType = GL_TRIANGLES;
}
}
