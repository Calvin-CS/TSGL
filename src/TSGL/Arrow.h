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
    float headX, headY, tailX, tailY;
    bool isDoubleArrow;
    GLfloat myLength;
    ColorGLfloat headColor;
    ColorGLfloat tailColor;
    void makeArrowHead(float x, float y, float deltaX, float deltaY, const ColorGLfloat color); // Helps constructor by calculating the Arrow Head's coordinates
    void generateVertices();
 public:

    Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color, bool doubleArrow = false);

    Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[], bool doubleArrow = false);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    GLfloat getLength() { return myLength; }
};

}

#endif /* LINE_H_ */