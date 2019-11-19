/*
 * Cylinder.h extends Prism and provides a class for drawing a Cylinder.
 */

#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Prism.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cylinder
 *  \brief Draw an arbitrary Cylinder with colored vertices.
 *  \details Cylinder is a class for holding vertex data for a Cylinder.
 *  \details Cylinder is a subclass of Prism with a circular base.
 */
class Cylinder : public Prism {
protected:
    float myRadius, myHeight;
public:
    Cylinder(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c);

    Cylinder(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual void setHeight(float height);

    virtual void changeHeightBy(float delta);

    virtual ~Cylinder();
};

}

#endif /* CYLINDER_H_ */