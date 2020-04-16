/*
 * Sphere.h extends Object3D and provides a class for drawing a sphere.
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object3D.h"       // For extending our Object3D object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Sphere
 *  \brief Draw an arbitrary Sphere with colored vertices.
 *  \details Sphere is a class for holding vertex data for a Sphere with non-negative radius.
 */
class Sphere : public Object3D {
protected:
    GLfloat myRadius;
    int horizontalSections, verticalSections;
public:
    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c);

    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[]);

    virtual ~Sphere();

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual GLfloat getRadius() { return myRadius; }

    virtual void setColor(ColorGLfloat c);

    virtual void setColor(ColorGLfloat c[]);
};

}

#endif /* SPHERE_H_ */