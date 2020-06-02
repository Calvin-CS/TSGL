/*
 * Sphere.h extends Drawable and provides a class for drawing a sphere.
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Drawable.h"       // For extending our Drawable object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Sphere
 *  \brief Draw an arbitrary Sphere with colored vertices.
 *  \details Sphere is a class for holding vertex data for a Sphere with non-negative radius.
 */
class Sphere : public Drawable {
protected:
    GLfloat myRadius;
    int horizontalSections, verticalSections;
public:
    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c);

    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual ~Sphere();

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual GLfloat getRadius() { return myRadius; }

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);
};

}

#endif /* SPHERE_H_ */