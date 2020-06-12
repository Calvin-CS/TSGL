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
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,(radius + 5) + 1,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    edgesOutlined = false;
    verticesPerColor = (myRadius + 6) / 8;
    attribMutex.unlock();
    addVertex(0,0,0,color);
    float delta = 2.0f / (numberOfVertices - 2) * PI;
    for (int i = 0; i < numberOfVertices - 1; ++i) {
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
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,(GLint) (radius + 5) + 1,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    edgesOutlined = false;
    verticesPerColor = (myRadius + 6) / 8;
    attribMutex.unlock();
    addVertex(0,0,0,color[0]);
    float delta = 2.0f / (numberOfVertices - 2) * PI;
    for (int i = 0; i < numberOfVertices - 1; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color[(int) ((float) i / verticesPerColor + 1)]);
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

/**
 * \brief Sets the Circle to a new array of colors.
 * \param c An array of the new ColorFloats.
 */
void Circle::setColor(ColorFloat c[]) {
    colors[0] = c[0].R;
    colors[1] = c[0].G;
    colors[2] = c[0].B;
    colors[3] = c[0].A;
    int colorIndex;
    for (int i = 1; i < numberOfVertices; ++i) {
        colorIndex = (int) ((float) (i - 1) / verticesPerColor + 1);
        colors[i*4] = c[colorIndex].R;
        colors[i*4 + 1] = c[colorIndex].G;
        colors[i*4 + 2] = c[colorIndex].B;
        colors[i*4 + 3] = c[colorIndex].A;
    }
}

}