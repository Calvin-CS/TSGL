/*
 * Polygon.h extends Shape and provides a class for drawing a polygon.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Polygon
 *  \brief Draw an arbitrary Convex polygon with colored vertices.
 *  \details Polygon is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class Polygon : public Shape {
public:

    /*!
     * \brief Explicitly constructs a new Polygon.
     * \details Explicit constructor for a Convex Polygon object.
     *   \param numVertices the number of vertices the complete Polygon will have.
     *   \param c The color of the Polygon.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new Polygon with a buffer for storing the specified numbered of vertices.
     */
    Polygon(int numVertices, const ColorFloat& c);
};

}

#endif /* POLYGON_H_ */
