/*
 * ColoredPolygon.h extends Shape and provides a class for drawing a polygon
 *     with colored vertices to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/20/2014
 */

#ifndef CONCAVEPOLYGON_H_
#define CONCAVEPOLYGON_H_

#include "Shape.h"  // For extending our Shape object
#include <queue>          // std::queue
#include <iostream>         // DEBUGGING

/*! \class ColoredPolygon
 *  \brief Draw an arbitrary polygon with colored vertices.
 *  \details ColoredPolygon is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class ConcavePolygon : public Shape {
 private:
    bool init;          // Whether the vertex has been initialized completely
    bool dirty;         // Whether the new vertex buffer is dirty
    float* vertices;    // Buffer for vertex data
    float* tarray;      // Buffer for recomputed vertex data
    int size,           // Number of floating point numbers in vertices
        tsize,          // Number of floating point numbers in tarray
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)
 public:
    /*!
     * \brief Explicitly constructor a new ColoredPolygon.
     *      \param v, the number of vertices the complete ColoredPolygon will have.
     * \return a new ColoredPolygon with a buffer for storing the specified numbered of vertices.
     */
    ConcavePolygon(int v);

    ~ConcavePolygon();

    bool intersects(float p0_x, float p0_y, float p1_x, float p1_y,
                    float p2_x, float p2_y, float p3_x, float p3_y);

    bool pointInTriangle (float px, float py, float x1, float y1, float x2, float y2, float x3, float y3);

    /*!
     * \brief Add another vertex to the ConcavePolygon.
     * \details This function initalizes the next vertex in the Polyline and adds it to the ConcavePolygon buffer.
     *      \param x The x position of the vertex.
     *      \param y The y position of the vertex.
     *      \param color The color of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     */
    void addVertex(int x, int y, const ColorFloat &color);

    /*!
     * \brief Draw the ConcavePolygon.
     * \details This function actually draws the ColoredPolygon to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     */
    void draw();
};

#endif /* COLOREDPOLYGON_H_ */
