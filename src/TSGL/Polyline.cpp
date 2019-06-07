#include "Polyline.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param numVertices The number of vertices the complete Polyline will have.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(int numVertices) : Shape() {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
}

 /*!
  * \brief Explicitly constructs a new monocolored Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param numVertices The number of vertices the complete Polyline will have.
  *   \param x Array of x positions for the Polyline vertices.
  *   \param y Array of y positions for the Polyline vertices.
  *   \param color Color of the Polyline.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(int numVertices, int x[], int y[], ColorFloat color) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

 /*!
  * \brief Explicitly constructs a new multicolored Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param numVertices The number of vertices the complete Polyline will have.
  *   \param x Array of x positions for the Polyline vertices.
  *   \param y Array of y positions for the Polyline vertices.
  *   \param color Color of the Polyline.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(int numVertices, int x[], int y[], ColorFloat color[]) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

/*!
 *  \brief Overrides isProcessed() in Shape.h
 *  \details Overrides Shape::isProcessed() to include invariant.
 */
bool Polyline::isProcessed() {
    if (numberOfVertices < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return false;
    }
    return init;
}

}
