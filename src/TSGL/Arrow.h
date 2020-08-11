/*
 * Arrow.h extends ConcavePolygon and provides a class for drawing an Arrow shape to a Canvas.
 */

#ifndef ARROW_H_
#define ARROW_H_

#include "ConcavePolygon.h"  // For extending our ConcavePolygon object

namespace tsgl {

/*! \class Arrow
 *  \brief Draw a simple Arrow.
 *  \details Arrow is a class for holding vertex data for a concave polygon shaped like an Arrow.
 */
class Arrow : public ConcavePolygon {
 private:
    bool isDoubleArrow;
    GLfloat myLength, myWidth;
    GLfloat myEndpointX1, myEndpointY1, myEndpointZ1, myEndpointX2, myEndpointY2, myEndpointZ2;
    
    void recalculateVertices();
 public:
    Arrow(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow = false);

    Arrow(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow = false);

    Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow = false);

    Arrow(float x, float y, float z, GLfloat length, GLfloat width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow = false);

    void setLength(GLfloat length);

    void changeLengthBy(GLfloat delta);

    /*!\brief Accessor for the length of the arrow from endpoint to endpoint.
     * \details Returns the value of the myLength instance variable.
     * \return myLength, containing the distance in 3D space between the two endpoints.
     */
    GLfloat getLength() { return myLength; }

    void setWidth(GLfloat width);

    void changeWidthBy(GLfloat delta);

    /*!\brief Accessor for the width of the arrow from widest point to widest point.
     * \details Returns the value of the myLength instance variable.
     * \return myLength, containing the distance in 3D space between the two endpoints.
     */
    GLfloat getWidth() { return myWidth; }

    void setFirstEndpoint(GLfloat x, GLfloat y, GLfloat z);

    void setSecondEndpoint(GLfloat x, GLfloat y, GLfloat z);

    GLfloat getFirstEndpointX();

    GLfloat getFirstEndpointY();

    GLfloat getFirstEndpointZ();

    GLfloat getSecondEndpointX();

    GLfloat getSecondEndpointY();

    GLfloat getSecondEndpointZ();

    virtual void setColor(ColorFloat c) { Shape::setColor(c); }

    virtual void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
};

}

#endif /* LINE_H_ */