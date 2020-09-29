#include "Ellipse.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new monocolored Ellipse.
 * \details This function draws a Ellipse with the given center, radii, rotation, and color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param z The z coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param yaw The Ellipse's yaw in 3D space.
 *   \param pitch The Ellipse's pitch in 3D space.
 *   \param roll The Ellipse's roll in 3D space.
 *   \param color The color of the Ellipse's fill.
 */
Ellipse::Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color) : ConvexPolygon(x,y,z,(xRadius + yRadius) / 2 + 5 + 1,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myXRadius = xRadius;
    myYScale = myYRadius = yRadius;
    myZScale = 1;
    verticesPerColor = ((xRadius + yRadius) / 2 + 6) / 8;
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
 * \brief Explicitly constructs a new multicolored Ellipse.
 * \details This function draws a Ellipse with the given center, radii, rotation, and color.
 *   \param x The x coordinate of the Ellipse's center.
 *   \param y The y coordinate of the Ellipse's center.
 *   \param z The z coordinate of the Ellipse's center.
 *   \param xRadius The horizontal radius of the Ellipse in pixels.
 *   \param yRadius The vertical radius of the Ellipse in pixels.
 *   \param yaw The Ellipse's yaw in 3D space.
 *   \param pitch The Ellipse's pitch in 3D space.
 *   \param roll The Ellipse's roll in 3D space.
 *   \param color An array of colors for the Ellipse's fill.
 */
Ellipse::Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(x,y,z,(xRadius + yRadius) / 2 + 5 + 1,yaw,pitch,roll) {
    attribMutex.lock();
    myXScale = myXRadius = xRadius;
    myYScale = myYRadius = yRadius;
    myZScale = 1;
    verticesPerColor = ((xRadius + yRadius) / 2 + 6) / 8;
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

/**
 * \brief Sets the Ellipse to a new array of colors.
 * \param c An array of the new ColorFloats.
 */
void Ellipse::setColor(ColorFloat c[]) {
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

/**
 * \brief Accessor for Ellipse's colors.
 * \details Populates the reference parameter vector with a ColorFloat for each section of Ellipse.
 * \param colorVec A vector of ColorFloats to which the ColorFloats associated with Ellipse will be pushed.
 * \note Overrides Shape::getColors().
 */
void Ellipse::getColors(std::vector<ColorFloat> &colorVec) { 
    attribMutex.lock();
    for (int i = 0; i < numberOfVertices; i+=verticesPerColor) {
        colorVec.push_back(ColorFloat(vertices[i*7+3],vertices[i*7+4],vertices[i*7+5],vertices[i*7+6]));
    }
    attribMutex.unlock();
}

}