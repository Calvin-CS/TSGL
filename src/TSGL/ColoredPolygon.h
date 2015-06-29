/*
 * ColoredPolygon.h extends Shape and provides a class for drawing a polygon
 *     with colored vertices to a Canvas.
 *
 * Authors: Patrick Crain, Mark Vander Stel, Chris Dilley.
 * Last Modified: Patrick Crain, 7/20/2014
 */

#ifndef COLOREDPOLYGON_H_
#define COLOREDPOLYGON_H_

#include "Shape.h"  // For extending our Shape object

namespace tsgl {

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
class ColoredPolygon : public Shape {
 private:
    bool init;          // Whether the vertex has been initialized completely
    float* vertices;    // Buffer for vertex data
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)
 public:
    /*!
     * \brief Explicitly construct a new ColoredPolygon.
     * \details Explicit constructor for a ColoredPolygon object.
     *   \param numVertices The number of vertices the complete ColoredPolygon will have.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new ColoredPolygon with a buffer for storing the specified numbered of vertices.
     */
    ColoredPolygon(int numVertices);

    /*!
     * \brief Destroys a ColoredPolygon object
     * \details Destructor for a ColoredPolygon object.
     * \details Frees up memory that has been allocated to a ColoredPolygon object.
     */
    ~ColoredPolygon();

    /*!
     * \brief Adds another vertex to a ColoredPolygon.
     * \details This function initializes the next vertex in the Polyline and adds it to a ColoredPolygon buffer.
     *      \param x The x position of the vertex.
     *      \param y The y position of the vertex.
     *      \param color The reference variable to a color of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message will be given to show when the vertex buffer is full.
     */
    void addVertex(int x, int y, const ColorFloat &color);

    /*!
     * \brief Draw the ColoredPolygon.
     * \details This function actually draws the ColoredPolygon to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message will be given to show if the ColoredPolygon is *NOT* ready to be drawn (vertex buffer = not full).
     * \note Implemented inherited abstract method from Shape class.
     */
    void draw();
};

}

#endif /* COLOREDPOLYGON_H_ */
