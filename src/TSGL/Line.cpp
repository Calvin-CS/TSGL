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
    myEndpointX1 = -length/2 + x;
    myEndpointY1 = y;
    myEndpointZ1 = z;
    myEndpointX2 = -length/2 + x;
    myEndpointY2 = y;
    myEndpointZ2 = z;
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
    myEndpointX1 = -length/2 + x;
    myEndpointY1 = y;
    myEndpointZ1 = z;
    myEndpointX2 = -length/2 + x;
    myEndpointY2 = y;
    myEndpointZ2 = z;
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
    myEndpointX1 = x1;
    myEndpointY1 = y1;
    myEndpointZ1 = z1;
    myEndpointX2 = x2;
    myEndpointY2 = y2;
    myEndpointZ2 = z2; 
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
    myEndpointX1 = x1;
    myEndpointY1 = y1;
    myEndpointZ1 = z1;
    myEndpointX2 = x2;
    myEndpointY2 = y2;
    myEndpointZ2 = z2;
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
    myEndpointX1 = x; myEndpointY1 = y; myEndpointZ1 = z;
    myLength = sqrt(pow(myEndpointX2 - myEndpointX1, 2) + pow(myEndpointY2 - myEndpointY1, 2) + pow(myEndpointZ2 - myEndpointZ1, 2));
    if (centerMatchesRotationPoint()) {
        myRotationPointX = (myEndpointX2 + myEndpointX1) / 2;
        myRotationPointY = (myEndpointY2 + myEndpointY1) / 2;
        myRotationPointZ = (myEndpointZ2 + myEndpointZ1) / 2;
        myCenterX = myRotationPointX;
        myCenterY = myRotationPointY;
        myCenterZ = myRotationPointZ;
    } else {
        myCenterX = (myEndpointX2 + myEndpointX1) / 2;
        myCenterY = (myEndpointY2 + myEndpointY1) / 2;
        myCenterZ = (myEndpointZ2 + myEndpointZ1) / 2;
    }
    myCurrentYaw = 0;
    myCurrentPitch = 0;
    myCurrentRoll = 0;
    vertices[0] = myEndpointX1 - myCenterX;
    vertices[1] = myEndpointY1 - myCenterY;
    vertices[2] = myEndpointZ1 - myCenterZ;
    vertices[7] = myEndpointX2 - myCenterX;
    vertices[8] = myEndpointY2 - myCenterY;
    vertices[9] = myEndpointZ2 - myCenterZ;
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
    myEndpointX2 = x; myEndpointY2 = y; myEndpointZ2 = z;
    myLength = sqrt(pow(myEndpointX2 - myEndpointX1, 2) + pow(myEndpointY2 - myEndpointY1, 2) + pow(myEndpointZ2 - myEndpointZ1, 2));
    if (centerMatchesRotationPoint()) {
        myRotationPointX = (myEndpointX2 + myEndpointX1) / 2;
        myRotationPointY = (myEndpointY2 + myEndpointY1) / 2;
        myRotationPointZ = (myEndpointZ2 + myEndpointZ1) / 2;
        myCenterX = myRotationPointX;
        myCenterY = myRotationPointY;
        myCenterZ = myRotationPointZ;
    } else {
        myCenterX = (myEndpointX2 + myEndpointX1) / 2;
        myCenterY = (myEndpointY2 + myEndpointY1) / 2;
        myCenterZ = (myEndpointZ2 + myEndpointZ1) / 2;
    }
    myCurrentYaw = 0;
    myCurrentPitch = 0;
    myCurrentRoll = 0;
    vertices[0] = myEndpointX1 - myCenterX;
    vertices[1] = myEndpointY1 - myCenterY;
    vertices[2] = myEndpointZ1 - myCenterZ;
    vertices[7] = myEndpointX2 - myCenterX;
    vertices[8] = myEndpointY2 - myCenterY;
    vertices[9] = myEndpointZ2 - myCenterZ;
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

/**
 * \brief Returns the x-coordinate of the line's first endpoint.
 * \details Returns the value of the endpointX1 instance variable.
 * \return x-coordinate of the Line's first endpoint.
 */
GLfloat Line::getFirstEndpointX() { 
    attribMutex.lock();
    float ex1;
    if (myEndpointX1 == myRotationPointX && myEndpointY1 == myRotationPointY && myEndpointZ1 == myRotationPointZ) {
        ex1 = myEndpointX1;
        attribMutex.unlock();
        return ex1;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ex1 = myEndpointX1;
        attribMutex.unlock();
        return ex1;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ex1 = cosYaw * cosPitch * (myEndpointX1 - myRotationPointX) + (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) * (myEndpointY1 - myRotationPointY) + (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll) * (myEndpointZ1 - myRotationPointZ) + myRotationPointX;
    attribMutex.unlock();
    return ex1;
}

/**
 * \brief Returns the y-coordinate of the line's first endpoint.
 * \details Returns the value of the endpointY1 instance variable.
 * \return y-coordinate of the Line's first endpoint.
 */
GLfloat Line::getFirstEndpointY() { 
    attribMutex.lock();
    float ey1;
    if (myEndpointX1 == myRotationPointX && myEndpointY1 == myRotationPointY && myEndpointZ1 == myRotationPointZ) {
        ey1 = myEndpointY1;
        attribMutex.unlock();
        return ey1;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ey1 = myEndpointY1;
        attribMutex.unlock();
        return ey1;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ey1 = sinYaw * cosPitch * (myEndpointX1 - myRotationPointX) + (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) * (myEndpointY1 - myRotationPointY) + (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll) * (myEndpointZ1 - myRotationPointZ)  + myRotationPointY;
    attribMutex.unlock();
    return ey1;
}

/**
 * \brief Returns the z-coordinate of the line's first endpoint.
 * \details Returns the value of the endpointZ1 instance variable.
 * \return z-coordinate of the Line's first endpoint.
 */
GLfloat Line::getFirstEndpointZ() { 
    attribMutex.lock();
    float ez1;
    if (myEndpointX1 == myRotationPointX && myEndpointY1 == myRotationPointY && myEndpointZ1 == myRotationPointZ) {
        ez1 = myEndpointZ1;
        attribMutex.unlock();
        return ez1;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ez1 = myEndpointZ1;
        attribMutex.unlock();
        return ez1;
    }
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ez1 = -sinPitch * (myEndpointX1 - myRotationPointX) + cosPitch * sinRoll * (myEndpointY1 - myRotationPointY) + cosPitch * cosRoll * (myEndpointZ1 - myRotationPointZ) + myRotationPointZ;
    attribMutex.unlock();
    return ez1; 
}

/**
 * \brief Returns the x-coordinate of the line's second endpoint.
 * \details Returns the value of the endpointX2 instance variable.
 * \return x-coordinate of the Line's second endpoint.
 */
GLfloat Line::getSecondEndpointX() { 
    attribMutex.lock();
    float ex2;
    if (myEndpointX2 == myRotationPointX && myEndpointY2 == myRotationPointY && myEndpointZ2 == myRotationPointZ) {
        ex2 = myEndpointX2;
        attribMutex.unlock();
        return ex2;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ex2 = myEndpointX2;
        attribMutex.unlock();
        return ex2;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ex2 = cosYaw * cosPitch * (myEndpointX2 - myRotationPointX) + (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) * (myEndpointY2 - myRotationPointY) + (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll) * (myEndpointZ2 - myRotationPointZ) + myRotationPointX;
    attribMutex.unlock();
    return ex2; 
}

/**
 * \brief Returns the y-coordinate of the line's second endpoint.
 * \details Returns the value of the endpointY2 instance variable.
 * \return y-coordinate of the Line's second endpoint.
 */
GLfloat Line::getSecondEndpointY() { 
    attribMutex.lock();
    float ey2;
    if (myEndpointX2 == myRotationPointX && myEndpointY2 == myRotationPointY && myEndpointZ2 == myRotationPointZ) {
        ey2 = myEndpointY2;
        attribMutex.unlock();
        return ey2;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ey2 = myEndpointY2;
        attribMutex.unlock();
        return ey2;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ey2 = sinYaw * cosPitch * (myEndpointX2 - myRotationPointX) + (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) * (myEndpointY2 - myRotationPointY) + (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll) * (myEndpointZ2 - myRotationPointZ)  + myRotationPointY;
    attribMutex.unlock();
    return ey2; 
}

/**
 * \brief Returns the z-coordinate of the line's second endpoint.
 * \details Returns the value of the endpointZ2 instance variable.
 * \return z-coordinate of the Line's second endpoint.
 */
GLfloat Line::getSecondEndpointZ() { 
    attribMutex.lock();
    float ez2;
    if (myEndpointX2 == myRotationPointX && myEndpointY2 == myRotationPointY && myEndpointZ2 == myRotationPointZ) {
        ez2 = myEndpointZ2;
        attribMutex.unlock();
        return ez2;
    }
    if (myCurrentYaw == 0 && myCurrentPitch == 0 && myCurrentRoll == 0) {
        ez2 = myEndpointZ2;
        attribMutex.unlock();
        return ez2;
    }
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ez2 = -sinPitch * (myEndpointX2 - myRotationPointX) + cosPitch * sinRoll * (myEndpointY2 - myRotationPointY) + cosPitch * cosRoll * (myEndpointZ2 - myRotationPointZ) + myRotationPointZ;
    attribMutex.unlock();
    return ez2;
}

}
