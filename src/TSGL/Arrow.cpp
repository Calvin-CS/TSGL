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
 *      \param color A ColorGLfloats for the color of the Arrow.
 * \return A new Arrow with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the arrow will be drawn directly parallel to the x-axis as a plane perpindicular to the z-axis.
 */
Arrow::Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color, bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    attribMutex.lock();
    myXScale = length;
    myLength = length;
    isDoubleArrow = doubleArrow;
    attribMutex.unlock();
    addVertex(-0.4, 0.02, 0, color);
    addVertex(-0.4, 0.05, 0, color);
    addVertex(-0.5, 0, 0, color);
    addVertex(-0.4, -0.05, 0, color);
    addVertex(-0.4, -0.02, 0, color);

    if( isDoubleArrow ) {
        addVertex(0.4, -0.02, 0, color);
        addVertex(0.4, -0.05, 0, color);
        addVertex(0.5, 0, 0, color);
        addVertex(0.4, 0.05, 0, color);
        addVertex(0.4, 0.02, 0, color);
    } else {
        addVertex(0.5, -.02, 0, color);
        addVertex(0.5, .02, 0, color);
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
 *      \param color An array of ColorGLfloats for the colors of the Arrow.
 * \return A new Arrow with the specified length and color.
 * \note At 0,0,0 yaw,pitch,roll, the arrow will be drawn directly parallel to the x-axis as a plane perpindicular to the z-axis.
 */
Arrow::Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[], bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    attribMutex.lock();
    myXScale = length;
    myLength = length;
    isDoubleArrow = doubleArrow;
    attribMutex.unlock();
    addVertex(-0.4, 0.02, 0, color[0]);
    addVertex(-0.4, 0.05, 0, color[0]);
    addVertex(-0.5, 0, 0, color[0]);
    addVertex(-0.4, -0.05, 0, color[0]);
    addVertex(-0.4, -0.02, 0, color[0]);

    if( isDoubleArrow ) {
        addVertex(0.4, -0.02, 0, color[1]);
        addVertex(0.4, -0.05, 0, color[1]);
        addVertex(0.5, 0, 0, color[1]);
        addVertex(0.4, 0.05, 0, color[1]);
        addVertex(0.4, 0.02, 0, color[1]);
    } else {
        addVertex(0.5, -0.02, 0, color[1]);
        addVertex(0.5, 0.02, 0, color[1]);
    }
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

/**
 * \brief Sets the Arrow to a new color.
 * \param c The new array of ColorGLfloats.
 * \note Overrides Shape::setColor(ColorGLfloat c[]).
 */
void Arrow::setColor(ColorGLfloat c[]) {
    for(int i = 0; i < 7; i++) {
        colors[i*4] = c[i/5].R;
        colors[i*4 + 1] = c[i/5].G;
        colors[i*4 + 2] = c[i/5].B;
        colors[i*4 + 3] = c[i/5].A;
    }
    if (isDoubleArrow) {
        for(int i = 7; i < 10; i++) {
            colors[i*4] = c[1].R;
            colors[i*4 + 1] = c[1].G;
            colors[i*4 + 2] = c[1].B;
            colors[i*4 + 3] = c[1].A;
        }
    }
}

}