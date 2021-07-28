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
    GLfloat myEndpointX1, myEndpointY1, myEndpointZ1, myEndpointX2, myEndpointY2, myEndpointZ2;
 public:
    Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorFloat color);

    Line(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorFloat color[]);

    Line(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, float yaw, float pitch, float roll, ColorFloat color);

    Line(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, float yaw, float pitch, float roll, ColorFloat color[]);

    void setFirstEndpoint(float x, float y, float z);

    void setSecondEndpoint(float x, float y, float z);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    /**
     * \brief Returns the length of the line.
     * \details Returns the value of the myLength instance variable.
     * \return Length of the Line.
     */
    GLfloat getLength() { return myLength; }

    GLfloat getFirstEndpointX();

    GLfloat getFirstEndpointY();

    GLfloat getFirstEndpointZ();

    GLfloat getSecondEndpointX();

    GLfloat getSecondEndpointY();

    GLfloat getSecondEndpointZ();
};

}

#endif /* LINE_H_ */
