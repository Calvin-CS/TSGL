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
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color) : Polyline(x,y,z,2,yaw,pitch,roll) {
    addVertex(-0.5, 0, 0, color);
    addVertex(0.5, 0, 0, color);
    myXScale = length;
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
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[]) : Polyline(x,y,z,2,yaw,pitch,roll) {
    addVertex(-0.5, 0, 0, color[0]);
    addVertex(0.5, 0, 0, color[1]);
    myXScale = length;
}

/**
 * \brief Mutates the line's length to the new parameter value.
 * \param length The Prism's new length.
 */
void Line::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have a Line with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = length;
    myLength = length; 
    attribMutex.unlock();
}

/**
 * \brief Mutates the line's length by the parameter value.
 * \param delta The difference between the new and old line lengths.
 */
void Line::changeLineLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have a Line with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale += delta;
    myLength += delta; 
    attribMutex.unlock();
}

}
