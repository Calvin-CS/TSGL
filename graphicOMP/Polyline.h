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

class Polyline : public Shape {
 private:
    bool init;          // Whether the Polyline has been initialized completely
    float* vertices;    // Buffer for vertex data
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)
 public:
    /*
     * Explicit constructor for the Polyline class (calls the base constructor)
     * Parameters:
     *      vertices, the number of vertices the complete polyline will have
     * Returns: a new Polyline with the specified numbered vertices
     */
    Polyline(int v) {
        if (v < 2) throw std::out_of_range("Cannot have a line with fewer than 2 vertices.");
        length = v;
        size = length * 6;
        current = 0;
        vertices = new float[size];
        init = false;
    }

    ~Polyline() {
        delete vertices;
    }

    /*
     * addVertex allows you to add a vertex to the Polyline
     *  The function does nothing if the vertex buffer is already full
     * Parameters:
     *      x, the x position of the vertex
     *      y, the y position of the vertex
     *      color, the color of the vertex
     */
    void addVertex(int x, int y, RGBfloatType color = BLACK) {
        if (init) return;
        vertices[current] = x;
        vertices[current + 1] = y;
        vertices[current + 2] = color.R;
        vertices[current + 3] = color.G;
        vertices[current + 4] = color.B;
        vertices[current + 5] = color.A;
        current += 6;
        if (current == size) init = true;
    }

    // draw() actually draws the Polyline to the canvas
    void draw() {
        if (!init) return;
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, length);
    }
};

#endif /* POLYLINE_H_ */
