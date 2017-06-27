/*
 * Polygon.h extends Shape and provides a class for drawing a polygon.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"       // For extending our Shape object
#include "Polyline.h"    // For outline
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
protected:
  Polyline * outline;
    static bool testAddVertex();  // Unit test for addVertex()
 public:

    /*!
     * \brief Explicitly constructs a new Polygon.
     * \details Explicit constructor for a Convex Polygon object.
     *   \param numVertices the number of vertices the complete Polygon will have.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new Polygon with a buffer for storing the specified numbered of vertices.
     */
    Polygon(int numVertices);

    /*!
     * \brief Draw the Polygon.
     * \details This function actually draws the Polygon to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message is given indicating that the Polygon is *NOT* ready to be drawn yet (vertex buffer = not full).
     */
    virtual void draw();

    /*!
     * \brief Adds another vertex to a Polygon.
     * \details This function initializes the next vertex in the Polygon and adds it to a Shape buffer.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param color The reference variable of the color of the vertex.
     *   \param outlineColor The reference variables of the color of the vertex's outline.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    virtual void addVertex(int x, int y, const ColorFloat& color, const ColorFloat& outlineColor = BLACK);

    /**
     * \brief Draws the Outline of the Polygon
     * TODO: comment, maybe make protected
     */
    virtual void drawOutline();

    //TODO: comment this, implement
    virtual float* getVerticesPointerForRenderer();

    /*!
     * \brief Runs the Unit tests.
     * \details Runs the Unit tests for the Polygon class. addVertex() is tested.
     */
    static void runTests();

    /**
     * \brief Gets the current color of the Polygon
     * \return The ColorFloat of the Polygon.
     */
    virtual ColorFloat getColor() { return ColorFloat(vertices[2], vertices[3], vertices[4], vertices[5]); }

    /**
     * \brief Moves the Polygon to new coordinates
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     */
    virtual void setCenter(int x, int y);
};

}

#endif /* POLYGON_H_ */
