/*
 * Triangle.h extends Shape and provides a class for drawing a three-sided polygon to a Canvas.
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"  // For extending our Shape object

namespace tsgl {

/*! \class Triangle
 *  \brief Draw a simple Triangle.
 *  \details Triangle is a class for holding vertex data for a simple triangle.
 */
class Triangle : public Shape {
 private:
    float vertices[18];
 public:

    /*!
     * \brief Explicitly constructs a new Triangle.
     * \details This is the constructor for the Triangle class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param x3 The x coordinate of the third endpoint.
     *      \param y3 The y coordinate of the third endpoint.
     *      \param color The color of the Triangle.
     * \return A new Triangle with the specified vertices and color.
     */
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color);

    /*!
     * \brief Draw the Triangle.
     * \details This function actually draws the Triangle to the Canvas.
     */
   //  void draw();
    int getNumberOfVertices();

    float* getVertices();

    GLenum getGeometryType();
};

}

#endif /* TRIANGLE_H_ */
