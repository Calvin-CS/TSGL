/*
 * TriangleStrip.h extends Shape and provides a class for drawing a triangle strip
 *     with colored vertices to a Canvas.
 */

#ifndef TRIANGLESTRIP_H_
#define TRIANGLESTRIP_H_

#include "ConvexPolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class TriangleStrip
 *  \brief Draw an arbitrary triangle strip with colored vertices.
 *  \details TriangleStrip is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class TriangleStrip : public ConvexPolygon {
 private:

 public:

    /*!
     * \brief Explicitly construct a new TriangleStrip.
     * \details Explicit constructor for a TriangleStrip object.
     *   \param numVertices The number of vertices the complete TriangleStrip will have.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new TriangleStrip with a buffer for storing the specified numbered of vertices.
     */
    TriangleStrip(int numVertices, bool filled = true, bool outlined = false);
};

}

#endif /* TRIANGLESTRIP_H_ */
