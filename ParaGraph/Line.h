/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/2/2014
 */

#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"  // For extending our Shape object

/*! \class Line
    \brief Draw a simple line.
    \details Line is a class for holding vertex data for a simple line.
*/
class Line : public Shape {
 private:
    float vertices[12];
 public:
    /*!
     * \brief Explicitly constructs a new Line.
     * details This is the constructor for the Line class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param color The color of the Line.
     *      \return A new Line with the specified endpoints and color.
     */
    Line(int x1, int y1, int x2, int y2, Color color) {
        vertices[0] = x1;
        vertices[1] = y1;
        vertices[6] = x2;
        vertices[7] = y2;
        vertices[2] = vertices[8] = color.R;
        vertices[3] = vertices[9] = color.G;
        vertices[4] = vertices[10] = color.B;
        vertices[5] = vertices[11] = color.A;
    }

    /*!
     *  \brief Draw the Line.
     *  \details This function actually draws the Line to the Canvas.
     */
    void draw() {
        glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);
    }
};

#endif /* LINE_H_ */
