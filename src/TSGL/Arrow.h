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
    GLfloat myLength;
 public:

    Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color, bool doubleArrow = false);

    Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[], bool doubleArrow = false);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    GLfloat getLength() { return myLength; }

    void setColor(ColorGLfloat c) { Shape::setColor(c); }

    void setColor(ColorGLfloat c[]);
};

}

#endif /* LINE_H_ */