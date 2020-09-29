/*
 * Cuboid.h extends Prism and provides a class for drawing a Cuboid.
 */

#ifndef CUBOID_H_
#define CUBOID_H_

#include "Shape.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cuboid
 *  \brief Draw an arbitrary Cuboid with colored vertices.
 *  \details Cuboid is a class for holding vertex data for a Cuboid.
 *  \details Cuboid is a 6-sided subclass of Shape with all rectangular faces.
 */
class Cuboid : public Shape {
protected:
    GLfloat myLength, myWidth, myHeight;
public:
    Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorFloat c);

    Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setLength(GLfloat length);

    virtual void changeLengthBy(GLfloat delta);

    virtual void setWidth(GLfloat width);

    virtual void changeWidthBy(GLfloat delta);

    virtual void setHeight(GLfloat height);

    virtual void changeHeightBy(GLfloat delta);

    /*!
    * \brief Accessor for the length of the Prism.
    * \details Returns the value of the myLength private variable, a GLfloat.
    */
    virtual GLfloat getLength() { return myLength; }

    /*!
    * \brief Accessor for the height of the Prism.
    * \details Returns the value of the myHeight private variable, a GLfloat.
    */
    virtual GLfloat getHeight() { return myHeight; }

    /*!
    * \brief Accessor for the width of the Prism.
    * \details Returns the value of the myWidth private variable, a GLfloat.
    */
    virtual GLfloat getWidth() { return myWidth; }

    virtual void setColor(ColorFloat c) { Shape::setColor(c); }

    virtual void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
};

}

#endif /* CUBOID_H_ */