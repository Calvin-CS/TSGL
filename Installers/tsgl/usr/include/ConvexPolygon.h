/*
 * ConvexPolygon.h extends Shape and provides a class for drawing a Convex polygon.
 */

#ifndef CONVEXPOLYGON_H_
#define CONVEXPOLYGON_H_

#include "Shape.h"       // For extending our Shape object
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
class ConvexPolygon : public Shape {
 protected:
    ConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float yaw, float pitch, float roll);
 public:
    ConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorFloat color);

    ConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorFloat color[]);
};

}

#endif /* CONVEXPOLYGON_H_ */
