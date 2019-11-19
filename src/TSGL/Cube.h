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
    float mySideLength;
public:
    Cube(float x, float y, float z, float sideLength, float yaw, float pitch, float roll, ColorFloat c);

    Cube(float x, float y, float z, float sideLength, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setSideLength(float length);

    virtual void changeSideLengthBy(float delta);

    virtual ~Cube();
};

}

#endif /* CUBE_H_ */