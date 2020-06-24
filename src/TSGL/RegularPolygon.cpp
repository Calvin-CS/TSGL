#include "RegularPolygon.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new RegularPolygon with monocolored fill.
 * \details This function draws a regular polygon with the given center, sides, radius, rotation, and color.
 *   (number of sides), and color.
 *   \param x The x coordinate of the RegularPolygon's center.
 *   \param y The y coordinate of the RegularPolygon's center.
 *   \param z The z coordinate of the RegularPolygon's center.
 *   \param radius The radius of the RegularPolygon in pixels.
 *   \param sides The number of sides to use in the RegularPolygon.
 *   \param yaw The RegularPolygon's yaw in 3D space.
 *   \param pitch The RegularPolygon's pitch in 3D space.
 *   \param roll The RegularPolygon's roll in 3D space.
 *   \param color The color of the RegularPolygon.
 */
RegularPolygon::RegularPolygon(float x, float y, float z, GLfloat radius, int sides, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,sides,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    attribMutex.unlock();
    float delta = 2.0f / sides * PI;
    for (int i = 0; i < sides; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color);
    }
}

/*!
 * \brief Explicitly constructs a new RegularPolygon with multicolored fill.
 * \details This function draws a regular polygon with the given center, sides, radius, rotation, and color.
 *   (number of sides), and coloring.
 *   \param x The x coordinate of the RegularPolygon's center.
 *   \param y The y coordinate of the RegularPolygon's center.
 *   \param z The z coordinate of the RegularPolygon's center.
 *   \param radius The radius of the RegularPolygon in pixels.
 *   \param sides The number of sides to use in the RegularPolygon.
 *   \param yaw The RegularPolygon's yaw in 3D space.
 *   \param pitch The RegularPolygon's pitch in 3D space.
 *   \param roll The RegularPolygon's roll in 3D space.
 *   \param color An array of colors for the regular polygon's vertices.
 */
RegularPolygon::RegularPolygon(float x, float y, float z, GLfloat radius, int sides, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,sides,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = myRadius = radius;
    myZScale = 1;
    mySides = sides;
    attribMutex.unlock();
    float delta = 2.0f / sides * PI;
    for (int i = 0; i < sides; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color[i % (mySides - 1)]);
    }
}

/**
 * \brief Mutates the distance from the center of the RegularPolygon base to its vertices.
 * \param radius The RegularPolygon's new radius.
 */
void RegularPolygon::setRadius(GLfloat radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a RegularPolygon with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the RegularPolygon to its vertices by the parameter amount.
 * \param delta The amount by which to change the radius of the RegularPolygon.
 */
void RegularPolygon::changeRadiusBy(GLfloat delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a RegularPolygon with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius += delta;
    myXScale += delta;
    myYScale += delta;
    attribMutex.unlock();
}

void RegularPolygon::setColor(ColorFloat c[]) {
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*7 + 3] = c[i % (mySides - 1)].R;
        vertices[i*7 + 4] = c[i % (mySides - 1)].G;
        vertices[i*7 + 5] = c[i % (mySides - 1)].B;
        vertices[i*7 + 6] = c[i % (mySides - 1)].A;
    }
}

} 