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

    /*!
     * \brief Explicitly constructs a Rectangle.
     * \details This is the constructor for the Rectangle class.
     *   \param x The x coordinate of the Rectangle's left edge.
     *   \param y The y coordinate of the Rectangle's top edge.
     *   \param width The width of the Rectangle.
     *   \param height The height of the Rectangle.
     *   \param color The color of the Rectangle. (Defaults to BLACK)
     *   \param outlineColor The color of the Rectangle's outline. (Defaults to BLACK)
     * \return A new Rectangle with the specified top left corner, dimensions, and color.
     */
    Rectangle(float x, float y, float width, float height, const ColorFloat &color = BLACK, const ColorFloat& outlineColor = BLACK);

};

}

#endif /* RECTANGLE_H_ */
