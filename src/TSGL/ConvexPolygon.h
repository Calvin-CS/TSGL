/*
 * ConvexPolygon.h extends Polygon and provides a class for drawing a Convex polygon.
 */

#ifndef CONVEXPOLYGON_H_
#define CONVEXPOLYGON_H_

#include "Polygon.h"       // For extending our Polygon object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class ConvexPolygon
 *  \brief Draw an arbitrary Convex polygon with colored vertices.
 *  \details ConvexPolygon is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class ConvexPolygon : public Polygon {
public:

    /*!
     * \brief Explicitly constructs a new ConvexPolygon.
     * \details Explicit constructor for a ConvexPolygon object.
     *   \param numVertices the number of vertices the complete ConvexPolygon will have.
     *   \param color The color of the Polygon.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new ConvexPolygon with a buffer for storing the specified numbered of vertices.
     */
    ConvexPolygon(int numVertices, const ColorFloat& color);
};

}

#endif /* CONVEXPOLYGON_H_ */
