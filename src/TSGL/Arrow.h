/*
 * Arrow.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef ARROW_H_
#define ARROW_H_

#include "ConcavePolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class Arrow
 *  \brief Draw a simple Arrow.
 *  \details Line is a class for holding vertex data for a simple Arrow.
 */
class Arrow : public ConcavePolygon {
 private:
    bool isDoubleArrow;
    GLfloat myLength, myWidth;
 public:

    Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow = false);

    Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow = false);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    GLfloat getLength() { return myLength; }

    void setWidth(GLfloat width);

    void changeWidthBy(GLfloat delta);

    GLfloat getWidth() { return myWidth; }

    void setColor(ColorFloat c) { Shape::setColor(c); }

    void setColor(ColorFloat c[]);
};

}

#endif /* LINE_H_ */