#include "Arrow.h"

namespace tsgl {

/*!\brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x1 The x coordinate of the first endpoint of the arrow.
 *      \param y1 The y coordinate of the first endpoint of the arrow.
 *      \param z1 The z coordinate of the first endpoint of the arrow.
 *      \param x2 The x coordinate of the second endpoint of the arrow.
 *      \param y2 The y coordinate of the second endpoint of the arrow.
 *      \param z2 The z coordinate of the second endpoint of the arrow.
 *      \param width The width of the arrow at its widest points.
 *      \param yaw The yaw of the arrow.
 *      \param pitch The pitch of the arrow.
 *      \param roll The roll of the arrow.
 *      \param color A ColorFloat for the color of the Arrow.
 * \return A new Arrow with the specified endpoints and color.
 */
Arrow::Arrow(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow) : ConcavePolygon((x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2, (doubleArrow)? 10 : 7, yaw, pitch, roll)  {
    attribMutex.lock();
    myEndpointX1 = x1; myEndpointY1 = y1; myEndpointZ1 = z1;
    myEndpointX2 = x2; myEndpointY2 = y2; myEndpointZ2 = z2;
    myWidth = width;
    myLength = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2)); 
    isDoubleArrow = doubleArrow;
    isOutlined = false;

    // general gist of this overly complicated vertex-adding process (thanks, 3D):
    // 1. get the vector between the two endpoints (v1)
    // 2. get the z-axis in a vector (v2)
    // 3. get the normalized cross product of v1 and v2 (perpendicular to both) then scale it by width
    // 4. get vectors containing the endpoints shifted to be centered around zero (e1 and e2)
    // 5. get vectors containing the zero-centered endpoints shifted 10% closer to each other (base1 and base2)
    // 6. Finally, add vertices based on the cross product and base1 and base2 to get the pointy parts of the Arrow
    glm::vec3 v1 = { myEndpointX2 - myEndpointX1, myEndpointY2 - myEndpointY1, myEndpointZ2 - myEndpointZ1 };
    glm::vec3 v2 = { 0,0,1 };
    glm::vec3 cross = glm::normalize(glm::cross(v1, v2));
    cross *= myWidth;
    glm::vec3 e1 = { myEndpointX1 - myCenterX, myEndpointY1 - myCenterY, myEndpointZ1 - myCenterZ };
    glm::vec3 e2 = { myEndpointX2 - myCenterX, myEndpointY2 - myCenterY, myEndpointZ2 - myCenterZ };

    glm::vec3 base1 = { e1.x + v1.x / 10, e1.y + v1.y / 10, e1.z + v1.z / 10 };
    glm::vec3 base2 = { e2.x - v1.x / 10, e2.y - v1.y / 10, e2.z - v1.z / 10 };

    attribMutex.unlock();
    addVertex(base1.x + cross.x * 0.2, base1.y + cross.y * 0.2, base1.z + cross.z * 0.2, color);
    addVertex(base1.x + cross.x * 0.5, base1.y + cross.y * 0.5, base1.z + cross.z * 0.5, color);
    addVertex(e1.x, e1.y, e1.z, color);
    addVertex(base1.x + cross.x * -0.5, base1.y + cross.y * -0.5, base1.z + cross.z * -0.5, color);
    addVertex(base1.x + cross.x * -0.2, base1.y + cross.y * -0.2, base1.z + cross.z * -0.2, color);

    addOutlineVertex(base1.x + cross.x * 0.2, base1.y + cross.y * 0.2, base1.z + cross.z * 0.2, GRAY);
    addOutlineVertex(base1.x + cross.x * 0.5, base1.y + cross.y * 0.5, base1.z + cross.z * 0.5, GRAY);
    addOutlineVertex(e1.x, e1.y, e1.z, GRAY);
    addOutlineVertex(base1.x + cross.x * -0.5, base1.y + cross.y * -0.5, base1.z + cross.z * -0.5, GRAY);
    addOutlineVertex(base1.x + cross.x * -0.2, base1.y + cross.y * -0.2, base1.z + cross.z * -0.2, GRAY);

    if( isDoubleArrow ) {
        addVertex(base2.x + cross.x * -0.2, base2.y + cross.y * -0.2, base2.z + cross.z * -0.2, color);
        addVertex(base2.x + cross.x * -0.5, base2.y + cross.y * -0.5, base2.z + cross.z * -0.5, color);
        addVertex(e2.x, e2.y, e2.z, color);
        addVertex(base2.x + cross.x * 0.5, base2.y + cross.y * 0.5, base2.z + cross.z * 0.5, color);
        addVertex(base2.x + cross.x * 0.2, base2.y + cross.y * 0.2, base2.z + cross.z * 0.2, color);

        addOutlineVertex(base2.x + cross.x * -0.2, base2.y + cross.y * -0.2, base2.z + cross.z * -0.2, GRAY);
        addOutlineVertex(base2.x + cross.x * -0.5, base2.y + cross.y * -0.5, base2.z + cross.z * -0.5, GRAY);
        addOutlineVertex(e2.x, e2.y, e2.z, GRAY);
        addOutlineVertex(base2.x + cross.x * 0.5, base2.y + cross.y * 0.5, base2.z + cross.z * 0.5, GRAY);
        addOutlineVertex(base2.x + cross.x * 0.2, base2.y + cross.y * 0.2, base2.z + cross.z * 0.2, GRAY);
    } else {
        addVertex(e2.x + cross.x * -0.2, e2.y + cross.y * -0.2, e2.z + cross.z * -0.2, color);
        addVertex(e2.x + cross.x *  0.2, e2.y + cross.y *  0.2, e2.z + cross.z *  0.2, color);
    
        addOutlineVertex(e2.x + cross.x * -0.2, e2.y + cross.y * -0.2, e2.z + cross.z * -0.2, GRAY);
        addOutlineVertex(e2.x + cross.x *  0.2, e2.y + cross.y *  0.2, e2.z + cross.z *  0.2, GRAY);
    }
}

/*!\brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x1 The x coordinate of the first endpoint of the arrow.
 *      \param y1 The y coordinate of the first endpoint of the arrow.
 *      \param z1 The z coordinate of the first endpoint of the arrow.
 *      \param x2 The x coordinate of the second endpoint of the arrow.
 *      \param y2 The y coordinate of the second endpoint of the arrow.
 *      \param z2 The z coordinate of the second endpoint of the arrow.
 *      \param width The width of the arrow at its widest points.
 *      \param yaw The yaw of the arrow.
 *      \param pitch The pitch of the arrow.
 *      \param roll The roll of the arrow.
 *      \param color An array of ColorFloats for the colors of the Arrow.
 * \return A new Arrow with the specified endpoints and color.
 */
Arrow::Arrow(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow) : ConcavePolygon((x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    attribMutex.lock();
    myEndpointX1 = x1; myEndpointY1 = y1; myEndpointZ1 = z1;
    myEndpointX2 = x2; myEndpointY2 = y2; myEndpointZ2 = z2;
    myWidth = width;
    myLength = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2)); 
    isDoubleArrow = doubleArrow;
    isOutlined = false;

    // general gist of this overly complicated vertex-adding process (thanks, 3D):
    // 1. get the vector between the two endpoints (v1)
    // 2. get the z-axis in a vector (v2)
    // 3. get the normalized cross product of v1 and v2 (perpendicular to both) then scale it by width
    // 4. get vectors containing the endpoints shifted to be centered around zero (e1 and e2)
    // 5. get vectors containing the zero-centered endpoints shifted 10% closer to each other (base1 and base2)
    // 6. Finally, add vertices based on the cross product and base1 and base2 to get the pointy parts of the Arrow
    glm::vec3 v1 = { myEndpointX2 - myEndpointX1, myEndpointY2 - myEndpointY1, myEndpointZ2 - myEndpointZ1 };
    glm::vec3 v2 = { 0,0,1 };
    glm::vec3 cross = glm::normalize(glm::cross(v1, v2));
    cross *= myWidth;
    glm::vec3 e1 = { myEndpointX1 - myCenterX, myEndpointY1 - myCenterY, myEndpointZ1 - myCenterZ };
    glm::vec3 e2 = { myEndpointX2 - myCenterX, myEndpointY2 - myCenterY, myEndpointZ2 - myCenterZ };

    glm::vec3 base1 = { e1.x + v1.x / 10, e1.y + v1.y / 10, e1.z + v1.z / 10 };
    glm::vec3 base2 = { e2.x - v1.x / 10, e2.y - v1.y / 10, e2.z - v1.z / 10 };

    attribMutex.unlock();
    addVertex(base1.x + cross.x * 0.2, base1.y + cross.y * 0.2, base1.z + cross.z * 0.2, color[0]);
    addVertex(base1.x + cross.x * 0.5, base1.y + cross.y * 0.5, base1.z + cross.z * 0.5, color[0]);
    addVertex(e1.x, e1.y, e1.z, color[0]);
    addVertex(base1.x + cross.x * -0.5, base1.y + cross.y * -0.5, base1.z + cross.z * -0.5, color[0]);
    addVertex(base1.x + cross.x * -0.2, base1.y + cross.y * -0.2, base1.z + cross.z * -0.2, color[0]);

    addOutlineVertex(base1.x + cross.x * 0.2, base1.y + cross.y * 0.2, base1.z + cross.z * 0.2, GRAY);
    addOutlineVertex(base1.x + cross.x * 0.5, base1.y + cross.y * 0.5, base1.z + cross.z * 0.5, GRAY);
    addOutlineVertex(e1.x, e1.y, e1.z, GRAY);
    addOutlineVertex(base1.x + cross.x * -0.5, base1.y + cross.y * -0.5, base1.z + cross.z * -0.5, GRAY);
    addOutlineVertex(base1.x + cross.x * -0.2, base1.y + cross.y * -0.2, base1.z + cross.z * -0.2, GRAY);

    if( isDoubleArrow ) {
        addVertex(base2.x + cross.x * -0.2, base2.y + cross.y * -0.2, base2.z + cross.z * -0.2, color[1]);
        addVertex(base2.x + cross.x * -0.5, base2.y + cross.y * -0.5, base2.z + cross.z * -0.5, color[1]);
        addVertex(e2.x, e2.y, e2.z, color[1]);
        addVertex(base2.x + cross.x * 0.5, base2.y + cross.y * 0.5, base2.z + cross.z * 0.5, color[1]);
        addVertex(base2.x + cross.x * 0.2, base2.y + cross.y * 0.2, base2.z + cross.z * 0.2, color[1]);

        addOutlineVertex(base2.x + cross.x * -0.2, base2.y + cross.y * -0.2, base2.z + cross.z * -0.2, GRAY);
        addOutlineVertex(base2.x + cross.x * -0.5, base2.y + cross.y * -0.5, base2.z + cross.z * -0.5, GRAY);
        addOutlineVertex(e2.x, e2.y, e2.z, GRAY);
        addOutlineVertex(base2.x + cross.x * 0.5, base2.y + cross.y * 0.5, base2.z + cross.z * 0.5, GRAY);
        addOutlineVertex(base2.x + cross.x * 0.2, base2.y + cross.y * 0.2, base2.z + cross.z * 0.2, GRAY);
    } else {
        addVertex(e2.x + cross.x * -0.2, e2.y + cross.y * -0.2, e2.z + cross.z * -0.2, color[1]);
        addVertex(e2.x + cross.x *  0.2, e2.y + cross.y *  0.2, e2.z + cross.z *  0.2, color[1]);
    
        addOutlineVertex(e2.x + cross.x * -0.2, e2.y + cross.y * -0.2, e2.z + cross.z * -0.2, GRAY);
        addOutlineVertex(e2.x + cross.x *  0.2, e2.y + cross.y *  0.2, e2.z + cross.z *  0.2, GRAY);
    }
}

/*!
 * \brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x The x coordinate of the center of the arrow.
 *      \param y The y coordinate of the center of the arrow.
 *      \param z The z coordinate of the center of the arrow.
 *      \param length The length of the arrow (from endpoint to endpoint).
 *      \param width The width of the arrow (between its widest points).
 *      \param yaw The yaw of the arrow.
 *      \param pitch The pitch of the arrow.
 *      \param roll The roll of the arrow.
 *      \param color A ColorFloat for the color of the Arrow.
 * \return A new Arrow with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the arrow will be drawn directly parallel to the x-axis as a plane perpindicular to the z-axis.
 */
Arrow::Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    if (width <= 0 || length <= 0) {
        TsglDebug("Cannot have an Arrow with length or width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myLength = length;
    myWidth = width;
    isDoubleArrow = doubleArrow;
    isOutlined = false;
    myEndpointX1 = -myLength/2 + x;
    myEndpointX2 = myLength/2 + x;
    myEndpointY1 = myEndpointY2 = y;
    myEndpointZ1 = myEndpointZ2 = z;
    attribMutex.unlock();
    addVertex(-0.4 * myLength, 0.2 * myWidth, 0, color);
    addVertex(-0.4 * myLength, 0.5 * myWidth, 0, color);
    addVertex(-0.5 * myLength, 0, 0, color);
    addVertex(-0.4 * myLength, -0.5 * myWidth, 0, color);
    addVertex(-0.4 * myLength, -0.2 * myWidth, 0, color);

    addOutlineVertex(-0.4 * myLength, 0.2 * myWidth, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, 0.5 * myWidth, 0, GRAY);
    addOutlineVertex(-0.5 * myLength, 0, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, -0.5 * myWidth, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, -0.2 * myWidth, 0, GRAY);

    if( isDoubleArrow ) {
        addVertex(0.4 * myLength, -0.2 * myWidth, 0, color);
        addVertex(0.4 * myLength, -0.5 * myWidth, 0, color);
        addVertex(0.5 * myLength, 0, 0, color);
        addVertex(0.4 * myLength, 0.5 * myWidth, 0, color);
        addVertex(0.4 * myLength, 0.2 * myWidth, 0, color);

        addOutlineVertex(0.4 * myLength, -0.2 * myWidth, 0, GRAY);
        addOutlineVertex(0.4 * myLength, -0.5 * myWidth, 0, GRAY);
        addOutlineVertex(0.5 * myLength, 0, 0, GRAY);
        addOutlineVertex(0.4 * myLength, 0.5 * myWidth, 0, GRAY);
        addOutlineVertex(0.4 * myLength, 0.2 * myWidth, 0, GRAY);
    } else {
        addVertex(0.5 * myLength, -.2 * myWidth, 0, color);
        addVertex(0.5 * myLength, .2 * myWidth, 0, color);
    
        addOutlineVertex(0.5 * myLength, -.2 * myWidth, 0, GRAY);
        addOutlineVertex(0.5 * myLength, .2 * myWidth, 0, GRAY);
    }
}

/*!
 * \brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x The x coordinate of the center of the arrow.
 *      \param y The y coordinate of the center of the arrow.
 *      \param z The z coordinate of the center of the arrow.
 *      \param length The length of the arrow (from endpoint to endpoint).
 *      \param width The width of the arrow (between its widest points).
 *      \param yaw The yaw of the arrow.
 *      \param pitch The pitch of the arrow.
 *      \param roll The roll of the arrow.
 *      \param color An array of ColorFloats for the colors of the Arrow.
 * \return A new Arrow with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the arrow will be drawn directly parallel to the x-axis as a plane perpindicular to the z-axis.
 */
Arrow::Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    if (width <= 0 || length <= 0) {
        TsglDebug("Cannot have an Arrow with length or width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myLength = length;
    myWidth = width;
    isDoubleArrow = doubleArrow;
    isOutlined = false;
    myEndpointX1 = -myLength/2 + x;
    myEndpointX2 = myLength/2 + x;
    myEndpointY1 = myEndpointY2 = y;
    myEndpointZ1 = myEndpointZ2 = z;
    attribMutex.unlock();
    addVertex(-0.4 * myLength, 0.2 * myWidth, 0, color[0]);
    addVertex(-0.4 * myLength, 0.5 * myWidth, 0, color[0]);
    addVertex(-0.5 * myLength, 0, 0, color[0]);
    addVertex(-0.4 * myLength, -0.5 * myWidth, 0, color[0]);
    addVertex(-0.4 * myLength, -0.2 * myWidth, 0, color[0]);

    addOutlineVertex(-0.4 * myLength, 0.2 * myWidth, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, 0.5 * myWidth, 0, GRAY);
    addOutlineVertex(-0.5 * myLength, 0, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, -0.5 * myWidth, 0, GRAY);
    addOutlineVertex(-0.4 * myLength, -0.2 * myWidth, 0, GRAY);

    if( isDoubleArrow ) {
        addVertex(0.4 * myLength, -0.2 * myWidth, 0, color[1]);
        addVertex(0.4 * myLength, -0.5 * myWidth, 0, color[1]);
        addVertex(0.5 * myLength, 0, 0, color[1]);
        addVertex(0.4 * myLength, 0.5 * myWidth, 0, color[1]);
        addVertex(0.4 * myLength, 0.2 * myWidth, 0, color[1]);

        addOutlineVertex(0.4 * myLength, -0.2 * myWidth, 0, GRAY);
        addOutlineVertex(0.4 * myLength, -0.5 * myWidth, 0, GRAY);
        addOutlineVertex(0.5 * myLength, 0, 0, GRAY);
        addOutlineVertex(0.4 * myLength, 0.5 * myWidth, 0, GRAY);
        addOutlineVertex(0.4 * myLength, 0.2 * myWidth, 0, GRAY);
    } else {
        addVertex(0.5 * myLength, -0.2 * myWidth, 0, color[1]);
        addVertex(0.5 * myLength, 0.2 * myWidth, 0, color[1]);

        addOutlineVertex(0.5 * myLength, -.2 * myWidth, 0, GRAY);
        addOutlineVertex(0.5 * myLength, .2 * myWidth, 0, GRAY);
    }
    attribMutex.lock();
    myAlpha = (color[0].A + color[1].A) / 2;
    attribMutex.unlock();
}

/*!\brief Replacement mutator for the length of the Arrow.
 * \details Sets the value of the myLength instance variable to the parameter, and modifies the vertices array accordingly.
 * \param length New length of the Arrow, a float.
 */
void Arrow::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    float ratio = length / myLength;
    myLength = length;
    myEndpointX1 = (myEndpointX1 - myCenterX) * ratio + myCenterX;
    myEndpointY1 = (myEndpointY1 - myCenterY) * ratio + myCenterY;
    myEndpointZ1 = (myEndpointZ1 - myCenterZ) * ratio + myCenterZ;
    myEndpointX2 = (myEndpointX2 - myCenterX) * ratio + myCenterX;
    myEndpointY2 = (myEndpointY2 - myCenterY) * ratio + myCenterY;
    myEndpointZ2 = (myEndpointZ2 - myCenterZ) * ratio + myCenterZ;
    attribMutex.unlock();
    recalculateVertices();
}

/*!\brief Alteration mutator for the length of the Arrow.
 * \details Alters the value of the myLength instance variable by delta and modifies the vertices array accordingly.
 * \param delta The difference between the old and new lengths of the Arrow.
 */
void Arrow::changeLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    float ratio = (myLength + delta) / myLength;
    myLength += delta;
    myEndpointX1 = (myEndpointX1 - myCenterX) * ratio + myCenterX;
    myEndpointY1 = (myEndpointY1 - myCenterY) * ratio + myCenterY;
    myEndpointZ1 = (myEndpointZ1 - myCenterZ) * ratio + myCenterZ;
    myEndpointX2 = (myEndpointX2 - myCenterX) * ratio + myCenterX;
    myEndpointY2 = (myEndpointY2 - myCenterY) * ratio + myCenterY;
    myEndpointZ2 = (myEndpointZ2 - myCenterZ) * ratio + myCenterZ;
    attribMutex.unlock();
    recalculateVertices();
}

/*!\brief Replacement mutator for the width of the Arrow.
 * \details Sets the value of the myWidth instance variable to the parameter, and modifies the vertices array accordingly.
 * \param width New width of the Arrow, a float.
 */
void Arrow::setWidth(GLfloat width) {
    if (width <= 0) {
        TsglDebug("Cannot have an Arrow with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = width;
    attribMutex.unlock();
    recalculateVertices();
}

/*!\brief Alteration mutator for the width of the Arrow.
 * \details Alters the value of the myWidth instance variable by delta and modifies the vertices array accordingly.
 * \param delta The difference between the old and new widths of the Arrow.
 */
void Arrow::changeWidthBy(GLfloat delta) {
    if (myWidth + delta <= 0) {
        TsglDebug("Cannot have an Arrow with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth += delta;
    attribMutex.unlock();
    recalculateVertices();
}

/*!\brief Mutator for the first endpoint coordinates of the Arrow.
 * \details Sets the values of myEndpoint_1 instance variables by the parameters and modifies the vertices array accordingly.
 * \param x The new x-coordinate of the first endpoint of the Arrow.
 * \param y The new y-coordinate of the first endpoint of the Arrow.
 * \param z The new z-coordinate of the first endpoint of the Arrow.
 */
void Arrow::setFirstEndpoint(GLfloat x, GLfloat y, GLfloat z) {
    attribMutex.lock();
    myEndpointX1 = x; myEndpointY1 = y; myEndpointZ1 = z;
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
    myCurrentYaw = myCurrentPitch = myCurrentRoll = 0;
    myLength = sqrt(pow(myEndpointX2 - myEndpointX1, 2) + pow(myEndpointY2 - myEndpointY1, 2) + pow(myEndpointZ2 - myEndpointZ1, 2));
    attribMutex.unlock(); 
    recalculateVertices();
}

/*!\brief Mutator for the second endpoint coordinates of the Arrow.
 * \details Sets the values of myEndpoint_2 instance variables by the parameters and modifies the vertices array accordingly.
 * \param x The new x-coordinate of the second endpoint of the Arrow.
 * \param y The new y-coordinate of the second endpoint of the Arrow.
 * \param z The new z-coordinate of the second endpoint of the Arrow.
 */
void Arrow::setSecondEndpoint(GLfloat x, GLfloat y, GLfloat z) {
    attribMutex.lock();
    myEndpointX2 = x; myEndpointY2 = y; myEndpointZ2 = z;
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
    myCurrentYaw = myCurrentPitch = myCurrentRoll = 0;
    myLength = sqrt(pow(myEndpointX2 - myEndpointX1, 2) + pow(myEndpointY2 - myEndpointY1, 2) + pow(myEndpointZ2 - myEndpointZ1, 2)); 
    attribMutex.unlock();
    recalculateVertices();
}

/*!\brief Accessor for the x-coordinate of the Arrow's first endpoint.
 * \details Returns the value of the myEndpointX1 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointX1, containing the value of the Arrow's first endpoint's x-coordinate.
 * \note See Drawable::getCenterX() documentation for more info.
 */
float Arrow::getFirstEndpointX() {
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

/*!\brief Accessor for the y-coordinate of the Arrow's first endpoint.
 * \details Returns the value of the myEndpointY1 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointY1, containing the value of the Arrow's first endpoint's y-coordinate.
 * \note See Drawable::getCenterY() documentation for more info.
 */
float Arrow::getFirstEndpointY() {
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

/*!\brief Accessor for the z-coordinate of the Arrow's first endpoint.
 * \details Returns the value of the myEndpointZ1 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointZ1, containing the value of the Arrow's first endpoint's z-coordinate.
 * \note See Drawable::getCenterZ() documentation for more info.
 */
float Arrow::getFirstEndpointZ() {
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
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ez1 = -sinPitch * (myEndpointX1 - myRotationPointX) + cosPitch * sinRoll * (myEndpointY1 - myRotationPointY) + cosPitch * cosRoll * (myEndpointZ1 - myRotationPointZ) + myRotationPointZ;
    attribMutex.unlock();
    return ez1;
}

/*!\brief Accessor for the x-coordinate of the Arrow's second endpoint.
 * \details Returns the value of the myEndpointX2 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointX2, containing the value of the Arrow's second endpoint's x-coordinate.
 * \note See Drawable::getCenterX() documentation for more info.
 */
float Arrow::getSecondEndpointX() {
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

/*!\brief Accessor for the y-coordinate of the Arrow's second endpoint.
 * \details Returns the value of the myEndpointY2 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointY2, containing the value of the Arrow's second endpoint's y-coordinate.
 * \note See Drawable::getCenterY() documentation for more info.
 */
float Arrow::getSecondEndpointY() {
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

/*!\brief Accessor for the z-coordinate of the Arrow's second endpoint.
 * \details Returns the value of the myEndpointZ2 instance variable, rotated around myRotationPoint by myCurrentYawPitchRoll.
 * \return myEndpointZ2, containing the value of the Arrow's second endpoint's z-coordinate.
 * \note See Drawable::getCenterZ() documentation for more info.
 */
float Arrow::getSecondEndpointZ() {
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
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    ez2 = -sinPitch * (myEndpointX2 - myRotationPointX) + cosPitch * sinRoll * (myEndpointY2 - myRotationPointY) + cosPitch * cosRoll * (myEndpointZ2 - myRotationPointZ) + myRotationPointZ;
    attribMutex.unlock();
    return ez2;
}

/**
 * \brief Sets the Arrow to a new color.
 * \param c The new array of ColorFloats.
 * \note Overrides Shape::setColor(ColorFloat c[]).
 */
void Arrow::setColor(ColorFloat c[]) {
    attribMutex.lock();
    myAlpha = (c[0].A + c[1].A) / 2;
    for(int i = 0; i < 7; i++) {
        vertices[i*7 + 3] = c[i/5].R;
        vertices[i*7 + 4] = c[i/5].G;
        vertices[i*7 + 5] = c[i/5].B;
        vertices[i*7 + 6] = c[i/5].A;
    }
    if (isDoubleArrow) {
        for(int i = 7; i < 10; i++) {
            vertices[i*7 + 3] = c[1].R;
            vertices[i*7 + 4] = c[1].G;
            vertices[i*7 + 5] = c[1].B;
            vertices[i*7 + 6] = c[1].A;
        }
    }
    attribMutex.unlock();
}

/*! \brief Private helper method that recalculates vertices array whenever endpoints are altered.
 *  \details Assigns new values to coordinates section of vertices[], principally based on myEndpoint and myWidth.
 */
void Arrow::recalculateVertices() {
    attribMutex.lock();
    glm::vec3 v1 = { myEndpointX2 - myEndpointX1, myEndpointY2 - myEndpointY1, myEndpointZ2 - myEndpointZ1 };
    glm::vec3 v2 = { 0,0,1 };
    glm::vec3 cross = glm::normalize(glm::cross(v1, v2));
    cross *= myWidth;
    glm::vec3 e1 = { myEndpointX1 - myCenterX, myEndpointY1 - myCenterY, myEndpointZ1 - myCenterZ };
    glm::vec3 e2 = { myEndpointX2 - myCenterX, myEndpointY2 - myCenterY, myEndpointZ2 - myCenterZ };

    glm::vec3 base1 = { e1.x + v1.x / 10, e1.y + v1.y / 10, e1.z + v1.z / 10 };
    glm::vec3 base2 = { e2.x - v1.x / 10, e2.y - v1.y / 10, e2.z - v1.z / 10 };

    vertices[0] = base1.x + cross.x * 0.2; vertices[1] = base1.y + cross.y * 0.2; vertices[2] = base1.z + cross.z * 0.2;
    vertices[7] = base1.x + cross.x * 0.5; vertices[8] = base1.y + cross.y * 0.5; vertices[9] = base1.z + cross.z * 0.5;
    vertices[14] = e1.x;                   vertices[15] = e1.y;                   vertices[16] = e1.z;
    vertices[21] = base1.x + cross.x * -0.5; vertices[22] = base1.y + cross.y * -0.5; vertices[23] = base1.z + cross.z * -0.5;
    vertices[28] = base1.x + cross.x * -0.2; vertices[29] = base1.y + cross.y * -0.2; vertices[30] = base1.z + cross.z * -0.2;

    outlineVertices[0] = base1.x + cross.x * 0.2; outlineVertices[1] = base1.y + cross.y * 0.2; outlineVertices[2] = base1.z + cross.z * 0.2;
    outlineVertices[7] = base1.x + cross.x * 0.5; outlineVertices[8] = base1.y + cross.y * 0.5; outlineVertices[9] = base1.z + cross.z * 0.5;
    outlineVertices[14] = e1.x;                   outlineVertices[15] = e1.y;                   outlineVertices[16] = e1.z;
    outlineVertices[21] = base1.x + cross.x * -0.5; outlineVertices[22] = base1.y + cross.y * -0.5; outlineVertices[23] = base1.z + cross.z * -0.5;
    outlineVertices[28] = base1.x + cross.x * -0.2; outlineVertices[29] = base1.y + cross.y * -0.2; outlineVertices[30] = base1.z + cross.z * -0.2;

    if( isDoubleArrow ) {
        vertices[35] = base2.x + cross.x * -0.2; vertices[36] = base2.y + cross.y * -0.2; vertices[37] = base2.z + cross.z * -0.2;
        vertices[42] = base2.x + cross.x * -0.5; vertices[43] = base2.y + cross.y * -0.5; vertices[44] = base2.z + cross.z * -0.5;
        vertices[49] = e2.x;                   vertices[50] = e2.y;                   vertices[51] = e2.z;
        vertices[56] = base2.x + cross.x * 0.5; vertices[57] = base2.y + cross.y * 0.5; vertices[58] = base2.z + cross.z * 0.5;
        vertices[63] = base2.x + cross.x * 0.2; vertices[64] = base2.y + cross.y * 0.2; vertices[65] = base2.z + cross.z * 0.2;

        outlineVertices[35] = base2.x + cross.x * -0.2; outlineVertices[36] = base2.y + cross.y * -0.2; outlineVertices[37] = base2.z + cross.z * -0.2;
        outlineVertices[42] = base2.x + cross.x * -0.5; outlineVertices[43] = base2.y + cross.y * -0.5; outlineVertices[44] = base2.z + cross.z * -0.5;
        outlineVertices[49] = e2.x;                     outlineVertices[50] = e2.y;                   outlineVertices[51] = e2.z;
        outlineVertices[56] = base2.x + cross.x * 0.5; outlineVertices[57] = base2.y + cross.y * 0.5; outlineVertices[58] = base2.z + cross.z * 0.5;
        outlineVertices[63] = base2.x + cross.x * 0.2; outlineVertices[64] = base2.y + cross.y * 0.2; outlineVertices[65] = base2.z + cross.z * 0.2;
    } else {
        vertices[35] = e2.x + cross.x * -0.2; vertices[36] = e2.y + cross.y * -0.2; vertices[37] = e2.z + cross.z * -0.2;
        vertices[42] = e2.x + cross.x *  0.2; vertices[43] = e2.y + cross.y *  0.2; vertices[44] = e2.z + cross.z *  0.2;
    
        outlineVertices[35] = e2.x + cross.x * -0.2; outlineVertices[36] = e2.y + cross.y * -0.2; outlineVertices[37] = e2.z + cross.z * -0.2;
        outlineVertices[42] = e2.x + cross.x *  0.2; outlineVertices[43] = e2.y + cross.y *  0.2; outlineVertices[44] = e2.z + cross.z *  0.2;
    }
    attribMutex.unlock();
}

}