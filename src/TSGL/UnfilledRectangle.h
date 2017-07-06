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
     * \details This is the constructor for the UnfilledRectangle class.
     *   \param x The x coordinate of the UnfilledRectangle's left edge.
     *   \param y The y coordinate of the UnfilledRectangle's top edge.
     *   \param width The width of the UnfilledRectangle.
     *   \param height The height of the UnfilledRectangle.
     *   \param color The color of the UnfilledRectangle. (Defaults to BLACK.)
     * \return A new UnfilledRectangle with the specified top left corner, dimensions, and color.
     */
    UnfilledRectangle(int x, int y, int width, int height, const ColorFloat &color = BLACK);
};

}

#endif /* UNFILLED_RECTANGLE_H_ */
