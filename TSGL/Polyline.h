/*
 * Polyline.h extends Shape and provides a class for drawing a polyline
 *     with colored vertices to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"  // For extending our Shape object

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
     * \brief Explicitly constructor a new Polyline.
     *      \param v, the number of vertices the complete Polyline will have.
     * \return a new Polyline with a buffer for storing the specified numbered of vertices.
     */
    Polyline(int v);

    ~Polyline();

    /*!
     * \brief Add another vertex to the Polyline.
     * \details This function initalizes the next vertex in the Polyline and adds it to the Polyline's buffer.
     *      \param x The x position of the vertex.
     *      \param y The y position of the vertex.
     *      \param color The color of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     */
    void addNextVertex(int x, int y, const Color &color = BLACK);

    /*!
     * \brief Draw the Polyline.
     * \details This function actually draws the Polyline to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     */
    void draw();
};

#endif /* POLYLINE_H_ */
