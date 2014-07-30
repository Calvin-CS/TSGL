/*
 * Triangle.h extends Shape and provides a class for drawing a three-sided polygon to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"  // For extending our Shape object

/*! \class Triangle
    \brief Draw a simple Triangle.
    \details Triangle is a class for holding vertex data for a simple triangle.
*/
class Triangle : public Shape {
 private:
    float vertices[18];
 public:
    /*!
     * \brief Explicitly constructs a new Triangle.
     * details This is the constructor for the Triangle class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param x3 The x coordinate of the second endpoint.
     *      \param y3 The y coordinate of the second endpoint.
     *      \param color The color of the Triangle.
     *      \return A new Triangle with the specified vertices and color.
     */
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
        vertices[0] = x1;
        vertices[1] = y1;
        vertices[6] = x2;
        vertices[7] = y2;
        vertices[12] = x3;
        vertices[13] = y3;
        vertices[2] = vertices[8] = vertices[14] = color.R;
        vertices[3] = vertices[9] = vertices[15] = color.G;
        vertices[4] = vertices[10] = vertices[16] = color.B;
        vertices[5] = vertices[11] = vertices[17] = color.A;
    }

    /*!
     *  \brief Draw the Triangle.
     *  \details This function actually draws the Triangle to the Canvas.
     */
    void draw() {
        glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }
};

#endif /* TRIANGLE_H_ */
