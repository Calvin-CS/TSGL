/*
 * Polyline.h extends Shape and provides a class for drawing a Polyline with colored vertices to a Canvas.
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"  // For extending our Shape object

namespace tsgl {

/*! \class Polyline
 *  \brief Draw multiple lines chained together.
 *  \details Polyline is a class for holding vertex data for multiple lines whose endpoints are connected.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual
 *   Line instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class Polyline : public Shape {
public:

    /*!
     * \brief Explicitly constructs a new Polyline.
     * \details Explicit constructor for a new Polyline object.
     *   \param numVertices The number of vertices the complete Polyline will have.
     * \warning An invariant is held where if v is less than 2 then an error message is given.
     * \return A new Polyline with a buffer for storing the specified numbered of vertices.
     */
    Polyline(int numVertices);

    /*!
     * \brief Draw the Polyline.
     * \details This function actually draws the Polyline to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message indicating that the Polyline cannot be drawn yet will be given
     *   if the above condition is met (vertex buffer = not full).
     */
    void draw();

    //TODO: comment this, implement
    float* getVerticesPointerForRenderer();

    /**
     * \brief Gets the current color of the Polyline
     * \return The ColorFloat of the Polyline.
     */
    virtual ColorFloat getColor() { return ColorFloat(vertices[2], vertices[3], vertices[4], vertices[5]); }
};

}

#endif /* POLYLINE_H_ */
