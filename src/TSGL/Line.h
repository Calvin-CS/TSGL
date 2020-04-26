/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef LINE_H_
#define LINE_H_

#include "Polyline.h"  // For extending our Shape object

namespace tsgl {

/*! \class Line
 *  \brief Draw a simple line.
 *  \details Line is a class for holding vertex data for a simple line.
 */
class Line : public Polyline {
 private:
    GLfloat myLength;
 public:
    Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color);

    Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[]);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    /**
     * \brief Returns the length of the line.
     * \details Returns the value of the myLength instance variable.
     * \return Length of the Line.
     */
    GLfloat getLength() { return myLength; }
};

}

#endif /* LINE_H_ */
