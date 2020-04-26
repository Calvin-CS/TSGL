#include "Line.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x The x coordinate of the center of the line.
 *      \param y The y coordinate of the center of the line.
 *      \param z The z coordinate of the center of the line.
 *      \param length The length of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color The reference variable to the color of the Line.
 * \return A new Line with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the line will be drawn directly parallel to the x-axis.
 */
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color) : Polyline(x,y,z,2,yaw,pitch,roll) {
    if (length <= 0)
        TsglDebug("Cannot have a line with length less than or equal to 0.");
    attribMutex.lock();
    myXScale = length;
    myLength = length; 
    attribMutex.unlock();
    addVertex(-0.5, 0, 0, color);
    addVertex(0.5, 0, 0, color);
}

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x The x coordinate of the center of the line.
 *      \param y The y coordinate of the center of the line.
 *      \param z The z coordinate of the center of the line.
 *      \param length The length of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color The reference variable to the colors of the Line.
 * \return A new Line with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the line will be drawn directly parallel to the x-axis.
 */
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[]) : Polyline(x,y,z,2,yaw,pitch,roll) {
    if (length <= 0)
        TsglDebug("Cannot have a line with length less than or equal to 0.");
    attribMutex.lock();
    myXScale = length;
    myLength = length; 
    attribMutex.unlock();
    addVertex(-0.5, 0, 0, color[0]);
    addVertex(0.5, 0, 0, color[1]);
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
void Line::changeLengthBy(GLfloat delta) {
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
