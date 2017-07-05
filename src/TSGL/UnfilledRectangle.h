/*
 * UnfilledRectangle.h extends UnfilledShape and provides a class for drawing a rectangle to a Canvas.
 */

#ifndef UNFILLED_RECTANGLE_H_
#define UNFILLED_RECTANGLE_H_

#include "UnfilledShape.h"  // For extending our UnfilledShape object

namespace tsgl {

/*! \class UnfilledRectangle
 *  \brief Draw a simple UnfilledRectangle.
 *  \details UnfilledRectangle is a class for holding vertex data for a simple rectangle.
 */
class UnfilledRectangle : public UnfilledShape {
 public:

    /*!
     * \brief Explicitly constructs a UnfilledRectangle.
     * \details This is the constructor for the Rectangle class.
     *   \param x The x coordinate of the Rectangle's left edge.
     *   \param y The y coordinate of the Rectangle's top edge.
     *   \param width The width of the Rectangle.
     *   \param height The height of the Rectangle.
     *   \param color The color of the Rectangle. (Defaults to BLACK.)
     * \return A new UnfilledRectangle with the specified top left corner, dimensions, and color.
     */
    UnfilledRectangle(int x, int y, int width, int height, const ColorFloat &color = BLACK);
};

}

#endif /* UNFILLED_RECTANGLE_H_ */
