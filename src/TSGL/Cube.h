/*
 * Cube.h extends Prism and provides a class for drawing a Cube.
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Drawable.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cube
 *  \brief Draw an arbitrary Cube with colored vertices.
 *  \details Cube is a class for holding vertex data for a Cube.
 *  \details Cube is a 6-sided subclass of Drawable with all square faces.
 */
class Cube : public Drawable {
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

    virtual void setColor(ColorGLfloat c) { Drawable::setColor(c); }

    virtual void setColor(ColorGLfloat c[]);

    virtual ~Cube();
};

}

#endif /* CUBE_H_ */