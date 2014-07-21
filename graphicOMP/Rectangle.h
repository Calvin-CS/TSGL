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
    \brief Draw a simple Rectangle.
    \details Rectangle is a class for holding vertex data for a simple rectangle.
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
    Rectangle(int x, int y, int w, int h, RGBfloatType color) {
        vertices[0] = x;
        vertices[1] = y;
        vertices[6] = x + w;
        vertices[7] = y;
        vertices[12] = x;
        vertices[13] = y + h;
        vertices[18] = x + w;
        vertices[19] = y + h;
        vertices[2] = vertices[8] = vertices[14] = vertices[20] = color.R;  // Color of the coords
        vertices[3] = vertices[9] = vertices[15] = vertices[21] = color.G;
        vertices[4] = vertices[10] = vertices[16] = vertices[22] = color.B;
        vertices[5] = vertices[11] = vertices[17] = vertices[23] = color.A;
    }

    /*!
     * \brief Draw the Rectangle.
     * \details This function actually draws the Rectangle to the Canvas.
     */
    void draw() {
        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};

#endif /* RECTANGLE_H_ */
