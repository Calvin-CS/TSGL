/*
 * Cube.h extends Prism and provides a class for drawing a Cube.
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Prism.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cube
 *  \brief Draw an arbitrary Cube with colored vertices.
 *  \details Cube is a class for holding vertex data for a Cube.
 *  \details Cube is a subclass of Prism with all square faces.
 */
class Cube : public Prism {
protected:
    GLfloat mySideLength;
public:
    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c);

    Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c[]);

    virtual void setSideLength(float length);

    virtual void changeSideLengthBy(float delta);

    virtual void setRotation(float radians);

    virtual ~Cube();
};

}

#endif /* CUBE_H_ */