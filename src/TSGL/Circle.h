/*
* Circle.h extends ConvexPolygon and provides a class for drawing a circle to a Canvas.
*/

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "ConvexPolygon.h" // For extending our ConvexPolygon object

namespace tsgl {

/*! \class Circle
*  \brief Draw a circle.
*  \details Circle is a class for holding Shape data for a circle.
*/
class Circle : public ConvexPolygon {
protected:
	GLfloat myRadius;
    GLfloat verticesPerColor;
public:
    Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color);

    Circle(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorFloat color[]);

    void setRadius(GLfloat radius);

    void changeRadiusBy(GLfloat delta);

    /*!
    * \brief Accessor for the radius of the Circle.
    * \details Returns the value of the myRadius private variable, a GLfloat.
    */
    GLfloat getRadius() { return myRadius; }

    void setColor(ColorFloat c) { Shape::setColor(c); }

    void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
  };

}

#endif /* CIRCLE_H_ */