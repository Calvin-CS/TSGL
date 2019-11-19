/*
 * Pyramid.h extends Object3D and provides a class for drawing a pyramid.
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "Object3D.h"       // For extending our Object3D object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Pyramid
 *  \brief Draw an arbitrary Pyramid with colored vertices.
 *  \details Pyramid is a class for holding vertex data for a pyramid with a base with at least 3 sides.
 */
class Pyramid : public Object3D {
protected:
    float myHeight;
    float myRadius;
public:
    Pyramid(float x, float y, float z, float height, float radius, int sides, float yaw, float pitch, float roll, ColorFloat c);

    Pyramid(float x, float y, float z, float height, float radius, int sides, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual ~Pyramid();

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual void setHeight(float height);

    virtual void changeHeightBy(float delta);

    virtual ColorFloat* getColor();
};

}

#endif /* PYRAMID_H_ */