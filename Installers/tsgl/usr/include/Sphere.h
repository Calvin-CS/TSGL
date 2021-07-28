/*
 * Sphere.h extends Shape and provides a class for drawing a sphere.
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Sphere
 *  \brief Draw an arbitrary Sphere with colored vertices.
 *  \details Sphere is a class for holding vertex data for a Sphere with non-negative radius.
 */
class Sphere : public Shape {
protected:
    GLfloat myRadius;
    int horizontalSections, verticalSections;
public:
    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c);

    Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual GLfloat getRadius() { return myRadius; }

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
};

}

#endif /* SPHERE_H_ */