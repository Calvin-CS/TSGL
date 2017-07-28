/*
 * ConcavePolygon.h extends Polygon and provides a class for drawing a Concave polygon to a Canvas.
 */

#ifndef CONCAVEPOLYGON_H_
#define CONCAVEPOLYGON_H_

#include "Polygon.h"       // For extending our Polygon object
#include "TsglAssert.h"  // For unit testing purposes
#include <queue>         // std::queue
#include <iostream>      // DEBUGGING

namespace tsgl {

/*! \class ConcavePolygon
 *  \brief Draw an arbitrary Concave polygon with colored vertices.
 *  \details ConcavePolygon is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 */
class ConcavePolygon : public Polygon {
 private:
    bool dirty;         // Whether the new vertex buffer is dirty
    float* tarray;      // Buffer for recomputed vertex data
    int tsize;          // Number of floating point numbers in tarray
    void cleanup();     // Prepares the vertices for drawing

    static bool testIntersects();     // Unit test for intersects()
    static bool testPointInTriangle(); // Unit test for pointInTriangle()

 public:

    /*!
     * \brief Explicitly constructs a new ConcavePolygon.
     * \details Explicit constructor for a ConcavePolygon object.
     *   \param numVertices The number of vertices the complete ConcavePolygon will have.
     *   \param color The reference variable of the color of the Polygon.
     *   \param outlineColor The reference variable of the color of the Polygon's outline.
     * \warning An invariant is held where if numVertices is less than 3 then an error message is given.
     * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
     */
    ConcavePolygon(int numVertices, const ColorFloat& color, const ColorFloat& outlineColor = BLACK);

    /*!
     * \brief Destroys a ConcavePolygon object.
     * \details Destructor for a ConcavePolygon object.
     * \details Frees up memory that was allocated to a ConcavePolygon object.
     */
    ~ConcavePolygon();


    /**
     * \brief Returns a pointer to the vertices array for renderer.
     * \details Vertices specifies x and y coordinates for the ConcavePolygon.
     * \return Pointer to vertices.
     */
    virtual GLfloat* getPointerToVerticesArray();

    /**
     * \brief Returns the number of vertices in the Shape for renderer.
     * \return An int specifying the number of vertices.
     */
    virtual int getNumberOfVertices();

    /*!
     * \brief Determines if two lines intersect.
     * \details Simulates two lines inside of a ConcavePolygon object and determines whether
     * those two lines intersect.
     *      \param p0_x The x coordinate of the first point of the first line.
     *      \param p0_y The y coordinate of the first point of the first line.
     *      \param p1_x The x coordinate of the second point of the first line.
     *      \param p1_y The y coordinate of the second point of the first line.
     *      \param p2_x The x coordinate of the first point of the second line.
     *      \param p2_y The y coordinate of the first point of the second line.
     *      \param p3_x The x coordinate of the second point of the second line.
     *      \param p3_y The y coordinate of the second point of the second line.
     * \returns true if the lines do intersect, false if otherwise.
     */
    bool intersects(float p0_x, float p0_y, float p1_x, float p1_y,
                    float p2_x, float p2_y, float p3_x, float p3_y);

    /*!
     * \brief Determines whether a point resides inside of a Triangle.
     * \details Simulates a Triangle and point inside of a ConcavePolygon object and determines whether the point resides inside of
     * the Triangle.
     *   \param px The x coordinate of the point.
     *   \param py The y coordinate of the point.
     *   \param x1 The x coordinate of the first vertex of the Triangle.
     *   \param y1 The y coordinate of the first vertex of the Triangle.
     *   \param x2 The x coordinate of the second vertex of the Triangle.
     *   \param y2 The y coordinate of the second vertex of the Triangle.
     *   \param x3 The x coordinate of the third vertex of the Triangle.
     *   \param y3 The y coordinate of the third vertex of the Triangle.
     * \returns true if the point does reside in the Triangle, false if otherwise.
     */
    bool pointInTriangle (float px, float py, float x1, float y1, float x2, float y2, float x3, float y3);

    /*!
     * \brief Adds another vertex to a ConcavePolygon.
     * \details This function initializes the next vertex in the Polyline and adds it to a ConcavePolygon buffer.
     *      \param x The x position of the vertex.
     *      \param y The y position of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating that the vertex buffer is full.
     */
    void addVertex(float x, float y);

    /**
     * \brief Returns the geometry type for drawing.
     */
    virtual GLenum getGeometryType() { return GL_TRIANGLES; }

    /**
     * \brief Moves the ConcavePolygon to new coordinates.
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     */
    virtual void setCenter(float x, float y);

    /*!
     * \brief Runs the Unit tests.
     * \details Runs the Unit tests for the ConcavePolygon class. intersects() and pointInTriangle() are tested.
     */
    static void runTests();
};

}

#endif /* COLOREDPOLYGON_H_ */
