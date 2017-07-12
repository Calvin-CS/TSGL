/*
 * UnfilledShape.h extends Polyline and provides a class for drawing an unfilled shape to a Canvas.
 */

#ifndef UNFILLED_SHAPE_H_
#define UNFILLED_SHAPE_H_

#include "Polyline.h"  // For extending our Polyline object

namespace tsgl {

/*! \class UnfilledShape
 *  \brief Draw a simple UnfilledShape.
 *  \details UnfilledShape is a class for holding vertex data for an unfilled shape.
 */
class UnfilledShape : public Polyline {
 public:

    /*!
     * \brief Explicitly constructs a UnfilledShape.
     * \details This is the constructor for the UnfilledShape class.
     * \details UnfilledShape is quite similar to Polyline, but draws vertices in a line loop rather than simply segments.
     *   \param vertices The number of vertices in the unfilled shape.
     *   \param color The color of the Rectangle. (Defaults to BLACK).
     * \return A new UnfilledShape with the specified top left corner, dimensions, and color.
     */
    UnfilledShape(int vertices, const ColorFloat &color = BLACK);

    /**
     * \brief Returns the geometry type for drawing
     */
    virtual GLenum getGeometryType() { return GL_LINE_LOOP; }

};

}

#endif /* UNFILLED_SHAPE_H_ */
