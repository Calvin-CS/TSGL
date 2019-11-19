/*
 * Ellipsoid.h extends Object3D and provides a class for drawing a ellipsoid.
 */

#ifndef ELLIPSOID_H_
#define ELLIPSOID_H_

#include "Object3D.h"       // For extending our Object3D object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Ellipsoid
 *  \brief Draw an arbitrary Ellipsoid with colored vertices.
 *  \details Ellipsoid is a class for holding vertex data for an Ellipsoid.
 */
class Ellipsoid : public Object3D {
protected:
    float myXRadius, myYRadius, myZRadius;
public:
    Ellipsoid(float x, float y, float z, float xRadius, float yRadius, float zRadius, float yaw, float pitch, float roll, ColorFloat c);

    Ellipsoid(float x, float y, float z, float xRadius, float yRadius, float zRadius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual ~Ellipsoid();

    virtual void draw();

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    virtual void setXRadius(float radiusX);

    virtual void changeXRadiusBy(float delta);

    virtual void setYRadius(float radiusY);

    virtual void changeYRadiusBy(float delta);

    virtual void setZRadius(float radiusZ);

    virtual void changeZRadiusBy(float delta);

    virtual ColorFloat* getColor();
};

}

#endif /* ELLIPSOID_H_ */