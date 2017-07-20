/*
 * Triangle.h extends ConvexPolygon and provides a class for drawing a three-sided polygon to a Canvas.
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "ConvexPolygon.h"  // For extending our ConvexPolygon object

namespace tsgl {

/*! \class Triangle
 *  \brief Draw a simple Triangle.
 *  \details Triangle is a class for holding vertex data for a simple triangle.
 */
class Triangle : public ConvexPolygon {
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
     *      \param color The color of the Triangle. (Defaults to BLACK.)
     *      \param outlineColor The color of the Triangle's outline. (Defaults to BLACK.)
     * \return A new Triangle with the specified vertices and color.
     */
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, const ColorFloat &color = BLACK, const ColorFloat& outlineColor = BLACK);
};

}

#endif /* TRIANGLE_H_ */
