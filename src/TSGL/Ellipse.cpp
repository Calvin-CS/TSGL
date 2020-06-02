#include "Ellipse.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new monocolored filled or outlined Ellipse.
 * \details This function draws a Ellipse with the given center, radii, color, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param color The color of the Ellipse's fill or outline
 *   \param filled Whether the Ellipse should be filled
 *     (set to true by default).
 */
Ellipse::Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,(xRadius + yRadius) / 2 * 30,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myXRadius = xRadius;
    myYScale = myYRadius = yRadius;
    myZScale = 1;
    attribMutex.unlock();
    float delta = 2.0f / numberOfVertices * PI;
    for (int i = 0; i < numberOfVertices; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color);
    }
}

/*!
 * \brief Explicitly constructs a new multicolored filled or outlined Ellipse.
 * \details This function draws a Ellipse with the given center, radii, color, and outline color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param color An array of colors for the Ellipse's fill or outline
 *   \param filled Whether the Ellipse should be filled
 *     (set to true by default).
 */
Ellipse::Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,(xRadius + yRadius) / 2 * 30,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myXRadius = xRadius;
    myYScale = myYRadius = yRadius;
    myZScale = 1;
    attribMutex.unlock();
    float delta = 2.0f / numberOfVertices * PI;
    for (int i = 0; i < numberOfVertices; ++i) {
        addVertex(cos(i*delta), sin(i*delta), 0, color[i]);
    }
}

/**
 * \brief Mutates the horizontal radius of the Ellipse.
 * \param xRadius The Ellipse's new x-radius.
 */
void Ellipse::setXRadius(GLfloat xRadius) {
    if (xRadius <= 0) {
        TsglDebug("Cannot have a Ellipse with x-radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXRadius = xRadius;
    myXScale = xRadius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the horizontal radius of the Ellipse by the parameter amount.
 * \param delta The amount by which to change the x-radius of the Ellipse.
 */
void Ellipse::changeXRadiusBy(GLfloat delta) {
    if (myXRadius + delta <= 0) {
        TsglDebug("Cannot have a Ellipse with x-radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXRadius += delta;
    myXScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the vertical radius of the Ellipse.
 * \param YRadius The Ellipse's new y-radius.
 */
void Ellipse::setYRadius(GLfloat yRadius) {
    if (yRadius <= 0) {
        TsglDebug("Cannot have a Ellipse with y-radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myYRadius = yRadius;
    myYScale = yRadius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the vertical radius of the Ellipse by the parameter amount.
 * \param delta The amount by which to change the y-radius of the Ellipse.
 */
void Ellipse::changeYRadiusBy(GLfloat delta) {
    if (myYRadius + delta <= 0) {
        TsglDebug("Cannot have a Ellipse with y-radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myYRadius += delta;
    myYScale += delta;
    attribMutex.unlock();
}


}