#include "Square.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Square with monocolored fill.
 * \details This function draws a Square with the given center, sidelength, rotation, and color.
 *   \param x The x coordinate of the Square's center.
 *   \param y The y coordinate of the Square's center.
 *   \param z The z coordinate of the Square's center.
 *   \param sideLength The side length of the Square in pixels.
 *   \param yaw The Square's yaw in 3D space.
 *   \param pitch The Square's pitch in 3D space.
 *   \param roll The Square's roll in 3D space.
 *   \param color The color of the Square.
 */
Square::Square(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,4,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = mySideLength = sideLength;
    myZScale = 1;
    attribMutex.unlock();
    addVertex(-0.5, 0.5, 0, color);
    addVertex(-0.5, -0.5, 0, color);
    addVertex(0.5, -0.5, 0, color);
    addVertex(0.5, 0.5, 0, color);
}

/*!
 * \brief Explicitly constructs a new Square with multicolored fill.
 * \details This function draws a Square with the given center, sidelength, rotation, and color.
 *   \param x The x coordinate of the Square's center.
 *   \param y The y coordinate of the Square's center.
 *   \param z The z coordinate of the Square's center.
 *   \param sideLength The side length of the Square in pixels.
 *   \param yaw The Square's yaw in 3D space.
 *   \param pitch The Square's pitch in 3D space.
 *   \param roll The Square's roll in 3D space.
 *   \param color An array of colors for the Square's vertices.
 */
Square::Square(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,4,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myYScale = mySideLength = sideLength;
    myZScale = 1;
    attribMutex.unlock();
    addVertex(-0.5, 0.5, 0, color[0]);
    addVertex(-0.5, -0.5, 0, color[1]);
    addVertex(0.5, -0.5, 0, color[2]);
    addVertex(0.5, 0.5, 0, color[3]);
}

/**
 * \brief Mutates the distance between opposite sides of the Square.
 * \param sideLength The Square's new side length.
 */
void Square::setSideLength(GLfloat sideLength) {
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Square with sideLength less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    mySideLength = sideLength;
    myXScale = myYScale = sideLength;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance between opposite sides of the Square by the parameter amount.
 * \param delta The amount by which to change the side length of the Square.
 */
void Square::changeSideLengthBy(GLfloat delta) {
    if (mySideLength + delta <= 0) {
        TsglDebug("Cannot have a Square with sideLength less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    mySideLength += delta;
    myXScale += delta;
    myYScale += delta;
    attribMutex.unlock();
}
}