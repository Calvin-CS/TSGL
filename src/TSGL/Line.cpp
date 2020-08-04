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
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorFloat color) : Polyline(x,y,z,2,yaw,pitch,roll) {
    if (length <= 0)
        TsglDebug("Cannot have a line with length less than or equal to 0.");
    attribMutex.lock();
    myLength = length;
    endpointX1 = -length/2 + x;
    endpointY1 = y;
    endpointZ1 = z;
    endpointX2 = -length/2 + x;
    endpointY2 = y;
    endpointZ2 = z;
    attribMutex.unlock();
    addVertex(-length/2, 0, 0, color);
    addVertex(length/2, 0, 0, color);
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
Line::Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorFloat color[]) : Polyline(x,y,z,2,yaw,pitch,roll) {
    if (length <= 0)
        TsglDebug("Cannot have a line with length less than or equal to 0.");
    attribMutex.lock();
    myLength = length;
    endpointX1 = -length/2 + x;
    endpointY1 = y;
    endpointZ1 = z;
    endpointX2 = -length/2 + x;
    endpointY2 = y;
    endpointZ2 = z;
    attribMutex.unlock();
    addVertex(-length/2, 0, 0, color[0]);
    addVertex(length/2, 0, 0, color[1]);
}

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint of the line.
 *      \param y1 The y coordinate of the first endpoint of the line.
 *      \param z1 The z coordinate of the first endpoint of the line.
 *      \param x2 The x coordinate of the second endpoint of the line.
 *      \param y2 The y coordinate of the second endpoint of the line.
 *      \param z2 The z coordinate of the second endpoint of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color The reference variable to the color of the Line.
 * \return A new Line with the specified length and color.
 */
Line::Line(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, float yaw, float pitch, float roll, ColorFloat color) : Polyline((x2 + x1) / 2, (y2 + y1) / 2, (z2 + z1) / 2, 2, yaw, pitch, roll) {
    attribMutex.lock();
    myLength = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));   
    endpointX1 = x1;
    endpointY1 = y1;
    endpointZ1 = z1;
    endpointX2 = x2;
    endpointY2 = y2;
    endpointZ2 = z2; 
    attribMutex.unlock();
    addVertex(x1 - myCenterX, y1 - myCenterY, z1 - myCenterZ, color);
    addVertex(x2 - myCenterX, y2 - myCenterY, z2 - myCenterZ, color);
}

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint of the line.
 *      \param y1 The y coordinate of the first endpoint of the line.
 *      \param z1 The z coordinate of the first endpoint of the line.
 *      \param x2 The x coordinate of the second endpoint of the line.
 *      \param y2 The y coordinate of the second endpoint of the line.
 *      \param z2 The z coordinate of the second endpoint of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color The reference variable to the colors of the Line.
 * \return A new Line with the specified length and color.
 */
Line::Line(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, float yaw, float pitch, float roll, ColorFloat color[]) : Polyline((x2 + x1) / 2, (y2 + y1) / 2, (z2 + z1) / 2, 2, yaw, pitch, roll) {
    attribMutex.lock();
    myLength = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));  
    endpointX1 = x1;
    endpointY1 = y1;
    endpointZ1 = z1;
    endpointX2 = x2;
    endpointY2 = y2;
    endpointZ2 = z2;
    attribMutex.unlock();
    addVertex(x1 - myCenterX, y1 - myCenterY, z1 - myCenterZ, color[0]);
    addVertex(x2 - myCenterX, y2 - myCenterY, z2 - myCenterZ, color[1]);
}

/**
 * \brief Moves the line's first endpoint to the parameter coordinates.
 * \param x The Line's first endpoint's new x-coordinate.
 * \param y The Line's first endpoint's new y-coordinate.
 * \param z The Line's first endpoint's new z-coordinate.
 * \warning Upon altering an endpoint, center coordinates will be altered accordingly, 
 *  yaw, pitch, and roll will be reset to 0, 
 *  and rotation point coordinates will be adjusted if they previously matched the center of the line.
 */
void Line::setFirstEndpoint(float x, float y, float z) {
    attribMutex.lock();
    endpointX1 = x; endpointY1 = y; endpointZ1 = z;
    myLength = sqrt(pow(endpointX2 - endpointX1, 2) + pow(endpointY2 - endpointY1, 2) + pow(endpointZ2 - endpointZ1, 2));
    if (centerMatchesRotationPoint()) {
        myRotationPointX = (endpointX2 + endpointX1) / 2;
        myRotationPointY = (endpointY2 + endpointY1) / 2;
        myRotationPointZ = (endpointZ2 + endpointZ1) / 2;
        myCenterX = myRotationPointX;
        myCenterY = myRotationPointY;
        myCenterZ = myRotationPointZ;
    } else {
        myCenterX = (endpointX2 + endpointX1) / 2;
        myCenterY = (endpointY2 + endpointY1) / 2;
        myCenterZ = (endpointZ2 + endpointZ1) / 2;
    }
    myCurrentYaw = 0;
    myCurrentPitch = 0;
    myCurrentRoll = 0;
    vertices[0] = endpointX1 - myCenterX;
    vertices[1] = endpointY1 - myCenterY;
    vertices[2] = endpointZ1 - myCenterZ;
    vertices[7] = endpointX2 - myCenterX;
    vertices[8] = endpointY2 - myCenterY;
    vertices[9] = endpointZ2 - myCenterZ;
    attribMutex.unlock();
}

/**
 * \brief Moves the line's second endpoint to the parameter coordinates.
 * \param x The Line's second endpoint's new x-coordinate.
 * \param y The Line's second endpoint's new y-coordinate.
 * \param z The Line's second endpoint's new z-coordinate.
 * \warning Upon altering an endpoint, center coordinates will be altered accordingly, 
 *  yaw, pitch, and roll will be reset to 0, 
 *  and rotation point coordinates will be adjusted if they previously matched the center of the line.
 */
void Line::setSecondEndpoint(float x, float y, float z) {
    attribMutex.lock();
    endpointX2 = x; endpointY2 = y; endpointZ2 = z;
    myLength = sqrt(pow(endpointX2 - endpointX1, 2) + pow(endpointY2 - endpointY1, 2) + pow(endpointZ2 - endpointZ1, 2));
    if (centerMatchesRotationPoint()) {
        myRotationPointX = (endpointX2 + endpointX1) / 2;
        myRotationPointY = (endpointY2 + endpointY1) / 2;
        myRotationPointZ = (endpointZ2 + endpointZ1) / 2;
        myCenterX = myRotationPointX;
        myCenterY = myRotationPointY;
        myCenterZ = myRotationPointZ;
    } else {
        myCenterX = (endpointX2 + endpointX1) / 2;
        myCenterY = (endpointY2 + endpointY1) / 2;
        myCenterZ = (endpointZ2 + endpointZ1) / 2;
    }
    myCurrentYaw = 0;
    myCurrentPitch = 0;
    myCurrentRoll = 0;
    vertices[0] = endpointX1 - myCenterX;
    vertices[1] = endpointY1 - myCenterY;
    vertices[2] = endpointZ1 - myCenterZ;
    vertices[7] = endpointX2 - myCenterX;
    vertices[8] = endpointY2 - myCenterY;
    vertices[9] = endpointZ2 - myCenterZ;
    attribMutex.unlock();
}

/**
 * \brief Mutates the line's length to the new parameter value.
 * \param length The Line's new length.
 */
void Line::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have a Line with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    float ratio = length / myLength;
    myLength = length; 
    vertices[0] *= ratio;
    vertices[1] *= ratio;
    vertices[2] *= ratio;
    vertices[7] *= ratio;
    vertices[8] *= ratio;
    vertices[9] *= ratio;
    attribMutex.unlock();
}

/**
 * \brief Mutates the line's length by the parameter value.
 * \param delta The difference between the new and old line lengths.
 */
void Line::changeLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have a Line with length less than or equal to 0.");
        printf("failure\n");
        return;
    }
    attribMutex.lock();
    float ratio = (myLength + delta) / myLength;
    myLength += delta; 
    vertices[0] *= ratio;
    vertices[1] *= ratio;
    vertices[2] *= ratio;
    vertices[7] *= ratio;
    vertices[8] *= ratio;
    vertices[9] *= ratio;
    attribMutex.unlock();
}

}
