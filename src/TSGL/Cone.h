/*
 * Cone.h extends Pyramid and provides a class for drawing a cone.
 */

#ifndef CONE_H_
#define CONE_H_

#include "Pyramid.h"       // For extending our Pyramid object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Cone
 *  \brief Draw an arbitrary Cone with colored vertices.
 *  \details Cone is a class for holding vertex data for a cone.
 *  \details Cone is a subclass of Pyramid with a base of (radius * 3) sides, minimum of 3.
 */
class Cone : public Pyramid {
public:
    Cone(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c);

    Cone(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual ~Cone();
};

}

#endif /* CONE_H_ */