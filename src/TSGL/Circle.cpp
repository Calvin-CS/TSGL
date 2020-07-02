#include "Circle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new monocolored filled Circle.
 * \details This function draws a circle with the given center, radius, 3D rotation, color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param z The z coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param yaw The circle's yaw in 3D space.
 *   \param pitch The circle's pitch in 3D space.
 *   \param roll The circle's roll in 3D space.
 *   \param color The color of the circle's fill
 */
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,(radius + 5) + 1,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    verticesPerColor = (myRadius + 6) / 8;
    numberOfOutlineVertices = numberOfVertices - 1;
    attribMutex.unlock();
    addVertex(0,0,0,color);
    float delta = 2.0f / (numberOfVertices - 2) * PI;
    for (int i = 0; i < numberOfVertices - 1; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color);
        addOutlineVertex(cos(i*delta), sin(i*delta), 0, GRAY);
    }
}

/*!
 * \brief Explicitly constructs a new multicolored filled Circle.
 * \details This function draws a circle with the given center, radius, roation, and fill color.
 *   \param x The x coordinate of the circle's center.
 *   \param y The y coordinate of the circle's center.
 *   \param z The z coordinate of the circle's center.
 *   \param radius The radius of the circle in pixels.
 *   \param yaw The circle's yaw in 3D space.
 *   \param pitch The circle's pitch in 3D space.
 *   \param roll The circle's roll in 3D space.
 *   \param color An array of colors for the Circle's fill
 */
Circle::Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,(GLint) (radius + 5) + 1,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Circle with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    verticesPerColor = (myRadius + 6) / 8;
    numberOfOutlineVertices = numberOfVertices - 1;
    attribMutex.unlock();
    addVertex(0,0,0,color[0]);
    float delta = 2.0f / (numberOfVertices - 2) * PI;
    for (int i = 0; i < numberOfVertices - 1; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color[(int) ((float) i / verticesPerColor + 1)]);
        addOutlineVertex(cos(i*delta), sin(i*delta), 0, GRAY);
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
    attribMutex.lock();
    myAlpha = 0.0f;
    vertices[3] = c[0].R;
    vertices[4] = c[0].G;
    vertices[5] = c[0].B;
    vertices[6] = c[0].A;
    myAlpha += c[0].A;
    int colorIndex;
    for (int i = 1; i < numberOfVertices; ++i) {
        colorIndex = (int) ((float) (i - 1) / verticesPerColor + 1);
        vertices[i*7 + 3] = c[colorIndex].R;
        vertices[i*7 + 4] = c[colorIndex].G;
        vertices[i*7 + 5] = c[colorIndex].B;
        vertices[i*7 + 6] = c[colorIndex].A;
        myAlpha += c[colorIndex].A;
    }
    myAlpha /= numberOfVertices;
    attribMutex.unlock();
}

}