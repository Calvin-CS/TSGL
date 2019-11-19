/*
 * ConcavePolygon.h extends Shape and provides a class for drawing a Concave polygon to a Canvas.
 */

#ifndef CONCAVEPOLYGON_H_
#define CONCAVEPOLYGON_H_

#include "Polygon.h"       // For extending our Shape object
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
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class ConcavePolygon : public Polygon {
 private:
    bool dirty;         // Whether the new vertex buffer is dirty
    int size,           // Number of floating point numbers in vertices
        tsize,          // Number of floating point numbers in tarray
        length;         // Number of vertices in vertices (size / 6)

    void setup(int numVertices, bool filled, bool outlined = false);

    static bool testIntersects();     // Unit test for intersects()
    static bool testPointITriangle(); // Unit test for pointInTriangle()

 public:
    ConcavePolygon(int numVertices, bool filled = true, bool outlined = false);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat color, bool filled = true);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat color[], bool filled = true);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]);

    ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]);

    bool intersects(float p0_x, float p0_y, float p1_x, float p1_y,
                    float p2_x, float p2_y, float p3_x, float p3_y);

    bool pointInTriangle (float px, float py, float x1, float y1, float x2, float y2, float x3, float y3);

    void addVertex(float x, float y, const ColorFloat &color);

    void addVertex(float x, float y, const ColorFloat &fillColor, const ColorFloat &outlineColor);

    void preprocess();

    static void runTests();
};

}

#endif /* COLOREDPOLYGON_H_ */
