/*
 * Prism.h extends Object3D and provides a class for drawing a prism.
 */

#ifndef PRISM_H_
#define PRISM_H_

#include "Object3D.h"       // For extending our Object3D object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Prism
 *  \brief Draw an arbitrary Prism with colored vertices.
 *  \details Prism is a class for holding vertex data for a Prism with a base with at least 3 sides.
 */
class Prism : public Object3D {
protected:
    GLfloat myHeight;
    GLfloat myRadius;
    int mySides;
public:
    Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c);

    Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[]);

    virtual ~Prism();

    virtual void setHeight(GLfloat height);

    virtual void changeHeightBy(GLfloat delta);

    virtual void setRadius(GLfloat radius);

    virtual void changeRadiusBy(GLfloat delta);

    /*!
    * \brief Accessor for the radius of the Prism.
    * \details Returns the value of the myRadius private variable, a GLfloat.
    */
    virtual GLfloat getRadius() { return myRadius; }

    /*!
    * \brief Accessor for the height of the Prism.
    * \details Returns the value of the myHeight private variable, a GLfloat.
    */
    virtual GLfloat getHeight() { return myHeight; }

    virtual void setColor(ColorGLfloat c) { Object3D::setColor(c); }

    virtual void setColor(ColorGLfloat c[]);
};

}

#endif /* PRISM_H_ */