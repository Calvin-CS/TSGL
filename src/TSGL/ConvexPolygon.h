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
 private:
    bool init;          // Whether the vertex has been initialized completely
    float* vertices;    // Buffer for vertex data
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)

    static bool testAddVertex();  // Unit test for addVertex()
 public:

    /*!
     * \brief Explicitly constructs a new ConvexPolygon.
     * \details Explicit constructor for a Convex Polygon object.
     *   \param numVertices the number of vertices the complete ConvexPolygon will have.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new ConvexPolygon with a buffer for storing the specified numbered of vertices.
     */
    ConvexPolygon(int numVertices);

    /*!
     * \brief Destroys a ConvexPolygon object.
     * \details Destructor for a ConvexPolygon.
     * \details Frees up memory that was allocated to a ConvexPolygon object.
     */
    ~ConvexPolygon();

    /*!
     * \brief Adds another vertex to a ConvexPolygon.
     * \details This function initializes the next vertex in the Polyline and adds it to a ConvexPolygon buffer.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param color The reference variable of the color of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    void addVertex(int x, int y, const ColorFloat &color);

    /*!
     * \brief Draw the ConvexPolygon.
     * \details This function actually draws the ConvexPolygon to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message is given indicating that the ConvexPolygon is *NOT* ready to be drawn yet (vertex buffer = not full).
     */
    void draw();

    /*!
     * \brief Runs the Unit tests.
     * \details Runs the Unit tests for the ConvexPolygon class. addVertex() is tested.
     */
    static void runTests();
};

}

#endif /* CONVEXPOLYGON_H_ */
