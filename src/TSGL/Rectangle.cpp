#include "Rectangle.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a Rectangle with monocolored fill or outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param color The color of the Rectangle.
 *   \param filled Whether the Rectangle should be filled
 *     (set to true by default).
 * \return A new Rectangle with the specified top left corner, dimensions, and color.
 */
Rectangle::Rectangle(float x, float y, float z, GLfloat width, GLfloat height, float yaw, float pitch, float roll, ColorGLfloat color) : ConvexPolygon(x,y,z,4,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_QUADS;
    myXScale = myWidth = width;
    myYScale = myHeight = height;
    myZScale = 1;
    attribMutex.unlock();
    addVertex(-0.5, 0.5, 0, color);
    addVertex(-0.5, -0.5, 0, color);
    addVertex(0.5, -0.5, 0, color);
    addVertex(0.5, 0.5, 0, color);
}

/*!
 * \brief Explicitly constructs a Rectangle with multicolored fill or outline.
 * \details This is the constructor for the Rectangle class.
 *   \param x The x coordinate of the Rectangle's left edge.
 *   \param y The y coordinate of the Rectangle's top edge.
 *   \param width The width of the Rectangle.
 *   \param height The height of the Rectangle.
 *   \param color An array of colors for the vertices of the Rectangle.
 *   \param filled Whether the Rectangle should be filled
 *     (set to true by default).
 * \return A new Rectangle with the specified top left corner, dimensions, and colors.
 */
Rectangle::Rectangle(float x, float y, float z, GLfloat width, GLfloat height, float yaw, float pitch, float roll, ColorGLfloat color[]) : ConvexPolygon(x,y,z,4,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_QUADS;
    myXScale = myWidth = width;
    myYScale = myHeight = height;
    myZScale = 1;
    attribMutex.unlock();
    addVertex(-0.5, 0.5, 0, color[0]);
    addVertex(-0.5, -0.5, 0, color[1]);
    addVertex(0.5, -0.5, 0, color[2]);
    addVertex(0.5, 0.5, 0, color[3]);
}

/**
 * \brief Mutates the distance from the left side of the Rectangle base to its right side.
 * \param width The Rectangle's new width.
 */
void Rectangle::setWidth(GLfloat width) {
    if (width <= 0) {
        TsglDebug("Cannot have a Rectangle with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = width;
    myXScale = width;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the left side of the Rectangle base to its right side by the parameter amount.
 * \param delta The amount by which to change the width of the Rectangle.
 */
void Rectangle::changeWidthBy(GLfloat delta) {
    if (myWidth + delta <= 0) {
        TsglDebug("Cannot have a Rectangle with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth += delta;
    myXScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the top side of the Rectangle base to its bottom side.
 * \param height The Rectangle's new height.
 */
void Rectangle::setHeight(GLfloat height) {
    if (height <= 0) {
        TsglDebug("Cannot have a Rectangle with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = height;
    myYScale = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the top side of the Rectangle base to its bottom side by the parameter amount.
 * \param delta The amount by which to change the height of the Rectangle.
 */
void Rectangle::changeHeightBy(GLfloat delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Rectangle with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    myYScale += delta;
    attribMutex.unlock();
}

}
