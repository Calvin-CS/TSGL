/*
 * UnfilledTriangle.h extends UnfilledShape and provides a class for drawing a three-sided polyline to a Canvas.
 */

#ifndef UNFILLED_TRIANGLE_H_
#define UNFILLED_TRIANGLE_H_

#include "UnfilledShape.h"  // For extending our UnfilledShape object

namespace tsgl {

/*! \class UnfilledTriangle
 *  \brief Draw a simple UnfilledTriangle.
 *  \details UnfilledTriangle is a class for holding vertex data for a simple triangle.
 */
class UnfilledTriangle : public UnfilledShape {
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
     *      \param color The color of the UnfilledTriangle. (Defaults to BLACK).
     * \return A new UnfilledTriangle with the specified vertices and color.
     */
    UnfilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const ColorFloat &color = BLACK);
};

}

#endif /* UNFILLED_TRIANGLE_H_ */
