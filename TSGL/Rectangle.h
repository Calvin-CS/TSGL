/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"  // For extending our Shape object

/*! \class Rectangle
 *  \brief Draw a simple Rectangle.
 *  \details Rectangle is a class for holding vertex data for a simple rectangle.
 */
class Rectangle : public Shape {
 private:
    float vertices[24];
 public:
    /*!
     * \brief Explicitly construct a Rectangle.
     * \details This is the constructor for the Rectangle class.
     *      \param x The x coordinate of the Rectangle's left edge.
     *      \param y The y coordinate of the Rectangle's top edge.
     *      \param w The width of the Rectangle.
     *      \param h The height of the Rectangle.
     *      \param color The color of the Rectangle.
     * \return a new Rectangle with the specified top left corner, dimensions, and color.
     */
    Rectangle(int x, int y, int w, int h, const Color &color);

    /*!
     * \brief Draw the Rectangle.
     * \details This function actually draws the Rectangle to the Canvas.
     */
    void draw();
};

#endif /* RECTANGLE_H_ */
