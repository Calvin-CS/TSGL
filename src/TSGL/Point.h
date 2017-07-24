/*
 * Point.h extends Shape and provides a class for drawing a Point with colored vertices to a Canvas.
 */

#ifndef POINT_H_
#define POINT_H_

#include "Shape.h"  // For extending our Shape object

namespace tsgl {

/*! \class Point
 *  \brief Draw multiple lines chained together.
 *  \details Point is a class for holding vertex data for multiple lines whose endpoints are connected.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual
 *   Line instances.
 */
class Point : public Shape {
public:

    /*!
     * \brief Explicitly constructs a new Point.
     * \details Explicit constructor for a new Point object.
     *   \param x The x coordinate for the Point.
     *   \param y The y coordiante for the Point.
     *   \param color The reference variable of the color of the Point. (Defaults to BLACK).
     * \return A new Point with x and y coordiantes.
     */
    Point(float x, float y, const ColorFloat& color = BLACK);

    /**
     * \brief Returns the geometry type for drawing.
     */
    virtual GLenum getGeometryType() { return GL_POINTS; }
};

}

#endif /* POINT_H_ */
