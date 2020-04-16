/*
 * Cube.h extends Prism and provides a class for drawing a Cube.
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Object3D.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cube
 *  \brief Draw an arbitrary Cube with colored vertices.
 *  \details Cube is a class for holding vertex data for a Cube.
 *  \details Cube is a 6-sided subclass of Object3D with all square faces.
 */
class Cube : public Object3D {
protected:
    GLfloat mySideLength;
public:
    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c);

    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c[]);

    virtual void setSideLength(float length);

    virtual void changeSideLengthBy(float delta);

    /*!
    * \brief Accessor for the radius of the Prism.
    * \details Returns the value of the myRadius private variable, a GLfloat.
    */
    virtual GLfloat getSideLength() { return mySideLength; }

    virtual void setColor(ColorGLfloat c) { Object3D::setColor(c); }

    virtual void setColor(ColorGLfloat c[]);

    virtual ~Cube();
};

}

#endif /* CUBE_H_ */