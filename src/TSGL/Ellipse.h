/*
* Ellipse.h extends ConvexPolygon and provides a class for drawing a ellipse to a Canvas.
*/

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "ConvexPolygon.h" // For extending our Shape object

namespace tsgl {

  /*! \class Ellipse
  *  \brief Draw a simple ellipse.
  *  \details Ellipse is a class for holding vertex data for an ellipse.
  */
class Ellipse : public ConvexPolygon {
 private:
    GLfloat myXRadius, myYRadius;
    GLfloat verticesPerColor;
 public:
    Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color);

    Ellipse(float x, float y, float z, GLfloat xRadius, GLfloat yRadius, float yaw, float pitch, float roll, ColorFloat color[]);

    void setXRadius(GLfloat xRadius);

    void setYRadius(GLfloat yRadius);

    void changeXRadiusBy(GLfloat delta);

    void changeYRadiusBy(GLfloat delta);

    /*!
    * \brief Accessor for the x-radius of the Ellipse.
    * \details Returns the value of the myXRadius private variable, a GLfloat.
    */
    GLfloat getXRadius() { return myXRadius; }

    /*!
    * \brief Accessor for the y-radius of the Ellipse.
    * \details Returns the value of the myYRadius private variable, a GLfloat.
    */
    GLfloat getYRadius() { return myYRadius; }
    
    void setColor(ColorFloat c) { Shape::setColor(c); }

    void setColor(ColorFloat c[]);

    virtual void getColors(std::vector<ColorFloat> &colorVec);
};

}

#endif /* ELLIPSE_H_ */