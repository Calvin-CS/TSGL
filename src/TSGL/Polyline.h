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
 *  \note The addVertex() method must be called the same number of times as specified in the constructor, unless a float array is also passed.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling Drawable::draw() before all vertices have been added will do nothing.
 */
class Polyline : public Shape {
 private:

 public:

    Polyline(float x, float y, float z, int numVertices, float yaw, float pitch, float roll);

    Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorGLfloat color);

    Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorGLfloat color[]);

    bool isProcessed();
};

}

#endif /* POLYLINE_H_ */
