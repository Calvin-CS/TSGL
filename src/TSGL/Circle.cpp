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
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,radius * 30,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    attribMutex.unlock();
    float delta = 2.0f / numberOfVertices * PI;
    for (int i = 0; i < numberOfVertices; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color);
    }
}

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
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,radius * 30,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    attribMutex.unlock();
    float delta = 2.0f / numberOfVertices * PI;
    for (int i = 0; i < numberOfVertices; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color[i]);
    }
}

/**
 * \brief Mutates the radius of the Circle.
 * \param radius The Circle's new radius.
 */
void Circle::setRadius(GLfloat radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = myYScale = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the radius of the Circle by the parameter amount.
 * \param delta The amount by which to change the radius of the Circle.
 */
void Circle::changeRadiusBy(GLfloat delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius += delta;
    myXScale += delta;
    myYScale += delta;
    attribMutex.unlock();
}

}