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
 private:
    bool init;          // Whether the Polyline has been initialized completely
    float* vertices;    // Buffer for vertex data
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)
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
     * \brief Destroys a Polyline object.
     * \details Destructor for a Polyline object.
     * \details Frees up memory allocated to a Polyline object.
     */
    ~Polyline();

    /*!
     * \brief Adds another vertex to a Polyline.
     * \details This function initializes the next vertex in a Polyline and adds it to the Polyline's buffer.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param color The reference variable to the color of the vertex (set to BLACK by default).
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    void addNextVertex(int x, int y, const ColorFloat &color = BLACK);

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
};

}

#endif /* POLYLINE_H_ */
