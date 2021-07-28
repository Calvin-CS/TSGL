/*
 * Cube.h extends Shape and provides a class for drawing a Cube.
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cube
 *  \brief Draw an arbitrary Cube with colored vertices.
 *  \details Cube is a class for holding vertex data for a Cube.
 *  \details Cube is a 6-sided subclass of Shape with all square faces.
 */
class Cube : public Shape {
protected:
    GLfloat mySideLength;
public:
    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat c);

    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setSideLength(float length);

    virtual void changeSideLengthBy(float delta);

    /*!
    * \brief Accessor for the side length of the Cube.
    * \details Returns the value of the mySideLength private variable, a GLfloat.
    */
    virtual GLfloat getSideLength() { return mySideLength; }

    virtual void setColor(ColorFloat c) { Shape::setColor(c); }

    virtual void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);

    /*!
     * \brief Destructor for the Cube.
     */
    virtual ~Cube() {}
};

}

#endif /* CUBE_H_ */