/*
 * Cuboid.h extends Prism and provides a class for drawing a Cuboid.
 */

#ifndef CUBOID_H_
#define CUBOID_H_

#include "Prism.h"       // For extending our Prism object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cuboid
 *  \brief Draw an arbitrary Cuboid with colored vertices.
 *  \details Cuboid is a class for holding vertex data for a Cuboid.
 *  \details Cuboid is a subclass of Prism with all rectangular faces.
 */
class Cuboid : public Prism {
protected:
    float myLength, myWidth, myHeight;
public:
    Cuboid(float x, float y, float z, float length, float width, float height, float yaw, float pitch, float roll, ColorFloat c);

    Cuboid(float x, float y, float z, float length, float width, float height, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setLength(float length);

    virtual void changeLengthBy(float delta);

    virtual void setWidth(float width);

    virtual void changeWidthBy(float delta);

    virtual void setHeight(float height);

    virtual void changeHeightBy(float delta);

    virtual ~Cuboid();
};

}

#endif /* CUBOID_H_ */