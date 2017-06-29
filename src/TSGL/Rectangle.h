/*
 * Rectangle.h extends ConvexPolygon and provides a class for drawing a rectangle to a Canvas.
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "ConvexPolygon.h"  // For extending our ConvexPolygon object

namespace tsgl {

/*! \class Rectangle
 *  \brief Draw a simple Rectangle.
 *  \details Rectangle is a class for holding vertex data for a simple rectangle.
 */
class Rectangle : public ConvexPolygon {
 public:

   GLfloat testVerts[8] = {0};
   ColorFloat testColor;

    /*!
     * \brief Explicitly constructs a Rectangle.
     * \details This is the constructor for the Rectangle class.
     *   \param x The x coordinate of the Rectangle's left edge.
     *   \param y The y coordinate of the Rectangle's top edge.
     *   \param width The width of the Rectangle.
     *   \param height The height of the Rectangle.
     *   \param color The color of the Rectangle.
     *   \param outlineColor The color of the Rectangle's outline (defaults to BLACK).
     * \return A new Rectangle with the specified top left corner, dimensions, and color.
     */
    Rectangle(int x, int y, int width, int height, const ColorFloat &color, const ColorFloat &outlineColor = BLACK);


    GLfloat* getPointerToVerticesArray() {return testVerts;}
    int getNumberOfVertices() { return 4; }
    GLenum getGeometryType() { return GL_QUADS; }
    ColorFloat* getObjectColor() { return &testColor; }

};

}

#endif /* RECTANGLE_H_ */
