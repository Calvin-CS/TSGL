#include "Arrow.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x The x coordinate of the center of the arrow.
 *      \param y The y coordinate of the center of the arrow.
 *      \param z The z coordinate of the center of the arrow.
 *      \param length The length of the arrow.
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
    myXScale = length;
    myLength = length;
    myYScale = width;
    myWidth = width;
    isDoubleArrow = doubleArrow;
    isOutlined = false;
    attribMutex.unlock();
    addVertex(-0.4, 0.4, 0, color);
    addVertex(-0.4, 1, 0, color);
    addVertex(-0.5, 0, 0, color);
    addVertex(-0.4, -1, 0, color);
    addVertex(-0.4, -0.4, 0, color);

    addOutlineVertex(-0.4, 0.4, 0, GRAY);
    addOutlineVertex(-0.4, 1, 0, GRAY);
    addOutlineVertex(-0.5, 0, 0, GRAY);
    addOutlineVertex(-0.4, -1, 0, GRAY);
    addOutlineVertex(-0.4, -0.4, 0, GRAY);

    if( isDoubleArrow ) {
        addVertex(0.4, -0.4, 0, color);
        addVertex(0.4, -1, 0, color);
        addVertex(0.5, 0, 0, color);
        addVertex(0.4, 1, 0, color);
        addVertex(0.4, 0.4, 0, color);

        addOutlineVertex(0.4, -0.4, 0, GRAY);
        addOutlineVertex(0.4, -1, 0, GRAY);
        addOutlineVertex(0.5, 0, 0, GRAY);
        addOutlineVertex(0.4, 1, 0, GRAY);
        addOutlineVertex(0.4, 0.4, 0, GRAY);
    } else {
        addVertex(0.5, -.4, 0, color);
        addVertex(0.5, .4, 0, color);
    
        addOutlineVertex(0.5, -.4, 0, GRAY);
        addOutlineVertex(0.5, .4, 0, GRAY);
    }
}

/*!
 * \brief Explicitly constructs a new Arrow.
 * \details This is the constructor for the Arrow class.
 *      \param x The x coordinate of the center of the arrow.
 *      \param y The y coordinate of the center of the arrow.
 *      \param z The z coordinate of the center of the arrow.
 *      \param length The length of the arrow.
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
    myXScale = length;
    myLength = length;
    myYScale = width;
    myWidth = width;
    isDoubleArrow = doubleArrow;
    isOutlined = false;
    attribMutex.unlock();
    addVertex(-0.4, 0.4, 0, color[0]);
    addVertex(-0.4, 1, 0, color[0]);
    addVertex(-0.5, 0, 0, color[0]);
    addVertex(-0.4, -1, 0, color[0]);
    addVertex(-0.4, -0.4, 0, color[0]);

    addOutlineVertex(-0.4, 0.4, 0, GRAY);
    addOutlineVertex(-0.4, 1, 0, GRAY);
    addOutlineVertex(-0.5, 0, 0, GRAY);
    addOutlineVertex(-0.4, -1, 0, GRAY);
    addOutlineVertex(-0.4, -0.4, 0, GRAY);

    if( isDoubleArrow ) {
        addVertex(0.4, -0.4, 0, color[1]);
        addVertex(0.4, -1, 0, color[1]);
        addVertex(0.5, 0, 0, color[1]);
        addVertex(0.4, 1, 0, color[1]);
        addVertex(0.4, 0.4, 0, color[1]);

        addOutlineVertex(0.4, -0.4, 0, GRAY);
        addOutlineVertex(0.4, -1, 0, GRAY);
        addOutlineVertex(0.5, 0, 0, GRAY);
        addOutlineVertex(0.4, 1, 0, GRAY);
        addOutlineVertex(0.4, 0.4, 0, GRAY);
    } else {
        addVertex(0.5, -0.4, 0, color[1]);
        addVertex(0.5, 0.4, 0, color[1]);

        addOutlineVertex(0.5, -.4, 0, GRAY);
        addOutlineVertex(0.5, .4, 0, GRAY);
    }
    attribMutex.lock();
    myAlpha = (color[0].A + color[1].A) / 2;
    attribMutex.unlock();
}

void Arrow::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = length;
    myLength = length;
    attribMutex.unlock();
}

void Arrow::changeLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale += delta;
    myLength += delta;
    attribMutex.unlock();
}

void Arrow::setWidth(GLfloat width) {
    if (width <= 0) {
        TsglDebug("Cannot have an Arrow with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myYScale = width;
    myWidth = width;
    attribMutex.unlock();
}

void Arrow::changeWidthBy(GLfloat delta) {
    if (myWidth + delta <= 0) {
        TsglDebug("Cannot have an Arrow with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myYScale += delta;
    myWidth += delta;
    attribMutex.unlock();
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

}