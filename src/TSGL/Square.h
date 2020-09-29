/*
* Square.h extends ConvexPolygon and provides a class for drawing a square to a Canvas.
*/

#ifndef SQUARE_H_
#define SQUARE_H_

#include "ConvexPolygon.h" // For extending our Rectangle object

namespace tsgl {

/*! \class Square
*  \brief Draw a square.
*  \details Square is a class for holding Shape data for a square.
*/
class Square : public ConvexPolygon {
protected:
    GLfloat mySideLength;
public:
    Square(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat color);

    Square(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat color[]);

    /*!
    * \brief Accessor for the side length of the Square.
    * \details Returns the value of the mySideLength private variable, a GLfloat.
    */
    GLfloat getSideLength() { return mySideLength; }

    void setSideLength(GLfloat sideLength);
    
    void changeSideLengthBy(GLfloat delta);

  };

}

#endif /* SQUARE_H_ */