/*
* RegularPolygon.h extends ConvexPolygon and provides a class for drawing a regular polygon to a Canvas.
*/

#ifndef REGULAR_POLYGON_H_
#define REGULAR_POLYGON_H_

#include "ConvexPolygon.h" // For extending our ConvexPolygon object

namespace tsgl {

/*! \class RegularPolygon
*  \brief Draw a regular polygon.
*  \details RegularPolygon is a class for holding ConvexPolygon data for a regular polygon.
*/
class RegularPolygon : public ConvexPolygon {
protected:
    GLfloat myRadius;
    int mySides;
public:
    RegularPolygon(float x, float y, float z, GLfloat radius, int sides, float yaw, float pitch, float roll, ColorFloat color);

    RegularPolygon(float x, float y, float z, GLfloat radius, int sides, float yaw, float pitch, float roll, ColorFloat color[]);

    /*!
    * \brief Accessor for the number of sides of the RegularPolygon.
    * \details Returns the value of the mySides private variable, an int.
    */
    int getSides() { return mySides; }

    /*!
    * \brief Accessor for the radius of the RegularPolygon.
    * \details Returns the value of the myRadius private variable, a GLfloat.
    */
    GLfloat getRadius() { return myRadius; }

    void setRadius(GLfloat radius);

    void changeRadiusBy(GLfloat delta);
};

}

#endif /* REGULAR_POLYGON_H_ */