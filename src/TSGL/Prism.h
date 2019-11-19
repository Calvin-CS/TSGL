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
    // float myHeight;
public:
    Prism(float x, float y, float z, float vertices, int sides, float yaw, float pitch, float roll, ColorFloat c);

    Prism(float x, float y, float z, float vertices, int sides, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual ~Prism();

    virtual void draw();

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    // virtual void setHeight(float height);

    // virtual void changeHeightBy(float delta);

    virtual ColorFloat* getColor();
};

}

#endif /* PRISM_H_ */