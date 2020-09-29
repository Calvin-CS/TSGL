/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas.
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "ConvexPolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class Rectangle
 *  \brief Draw a simple Rectangle.
 *  \details Rectangle is a class for holding vertex data for a simple rectangle.
 */
class Rectangle : public ConvexPolygon {
 private:
    GLfloat myWidth, myHeight;
 public:
    Rectangle(float x, float y, float z, GLfloat width, GLfloat height, float yaw, float pitch, float roll, ColorFloat color);

    Rectangle(float x, float y, float z, GLfloat width, GLfloat height, float yaw, float pitch, float roll, ColorFloat color[]);

    /*!
    * \brief Accessor for the width of the Rectangle.
    * \details Returns the value of the myWidth private variable, a GLfloat.
    */
    GLfloat getWidth() { return myWidth; }

    /*!
    * \brief Accessor for the height of the Rectangle.
    * \details Returns the value of the myHeight private variable, a GLfloat.
    */
    GLfloat getHeight() { return myHeight; }

    void setWidth(GLfloat width);

    void setHeight(GLfloat height);

    void changeWidthBy(GLfloat delta);

    void changeHeightBy(GLfloat delta);
};

}

#endif /* RECTANGLE_H_ */
