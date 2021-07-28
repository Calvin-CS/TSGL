/*
 * Pyramid.h extends Shape and provides a class for drawing a pyramid.
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Pyramid
 *  \brief Draw an arbitrary Pyramid with colored vertices.
 *  \details Pyramid is a class for holding vertex data for a pyramid with a base with at least 3 sides.
 */
class Pyramid : public Shape {
protected:
    GLfloat myHeight;
    GLfloat myRadius;
    int mySides;
public:
    Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c);

    Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c[]);

    virtual void setRadius(float radius);

    virtual void changeRadiusBy(float delta);

    virtual void setHeight(float height);

    virtual void changeHeightBy(float delta);

   /*!
    * \brief Accessor for the height of the Prism.
    * \details Returns the value of the myHeight private variable, a GLfloat.
    */
    virtual GLfloat getHeight() { return myHeight; }

    /*!
    * \brief Accessor for the radius of the Prism.
    * \details Returns the value of the myRadius private variable, a GLfloat.
    */
    virtual GLfloat getRadius() { return myRadius; }    

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
};

}

#endif /* PYRAMID_H_ */