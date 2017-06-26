/*
 * UnfilledTriangle.h extends Polyline and provides a class for drawing a three-sided polyline to a Canvas.
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Polyline.h"  // For extending our Polyline object

namespace tsgl {

/*! \class UnfilledTriangle
 *  \brief Draw a simple UnfilledTriangle.
 *  \details UnfilledTriangle is a class for holding vertex data for a simple triangle.
 */
class UnfilledTriangle : public Polyline {
 public:

    /*!
     * \brief Explicitly constructs a new UnfilledTriangle.
     * \details This is the constructor for the UnfilledTriangle class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param x3 The x coordinate of the third endpoint.
     *      \param y3 The y coordinate of the third endpoint.
     *      \param color The color of the Triangle.
     * \return A new UnfilledTriangle with the specified vertices and color.
     */
    UnfilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color);
};

}

#endif /* TRIANGLE_H_ */
