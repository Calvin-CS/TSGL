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
    GLfloat endpointX1, endpointY1, endpointZ1, endpointX2, endpointY2, endpointZ2;
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

    /**
     * \brief Returns the x-coordinate of the line's first endpoint.
     * \details Returns the value of the endpointX1 instance variable.
     * \return x-coordinate of the Line's first endpoint.
     */
    GLfloat getFirstEndpointX() { return endpointX1; }

    /**
     * \brief Returns the y-coordinate of the line's first endpoint.
     * \details Returns the value of the endpointY1 instance variable.
     * \return y-coordinate of the Line's first endpoint.
     */
    GLfloat getFirstEndpointY() { return endpointY1; }

    /**
     * \brief Returns the z-coordinate of the line's first endpoint.
     * \details Returns the value of the endpointZ1 instance variable.
     * \return z-coordinate of the Line's first endpoint.
     */
    GLfloat getFirstEndpointZ() { return endpointZ1; }

    /**
     * \brief Returns the x-coordinate of the line's second endpoint.
     * \details Returns the value of the endpointX2 instance variable.
     * \return x-coordinate of the Line's second endpoint.
     */
    GLfloat getSecondEndpointX() { return endpointX2; }

    /**
     * \brief Returns the y-coordinate of the line's second endpoint.
     * \details Returns the value of the endpointY2 instance variable.
     * \return y-coordinate of the Line's second endpoint.
     */
    GLfloat getSecondEndpointY() { return endpointY2; }

    /**
     * \brief Returns the z-coordinate of the line's second endpoint.
     * \details Returns the value of the endpointZ2 instance variable.
     * \return z-coordinate of the Line's second endpoint.
     */
    GLfloat getSecondEndpointZ() { return endpointZ2; }
};

}

#endif /* LINE_H_ */
