/*
 * Polyline.h extends Shape and provides a class for drawing a polyline
 * 	with colored vertices to a Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 7/2/2014
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"			// For extending our Shape object

class Polyline : public Shape {
private:
	bool init;					// Whether the vertex has been initialized completely
	int size,					// Max / current number of floats; number of vertices
		current,
		length;
	float* vertices;			// Buffer for vertex data
public:
	/*
	 * Explicit constructor for the Polyline class (calls the base constructor)
	 * Parameters:
	 * 		vertices, the number of vertices the complete polyline will have
	 * Returns: a new Polyline with the specified numbered vertices
	 */
	Polyline(int v = 3) {
		if (v < 2)
			throw std::out_of_range("Cannot have a line with fewer than 2 vertices.");
		length = v;
		size = length * 6;
		current = 0;
		vertices = new float[size];
		init = false;
	}

	~Polyline() { delete vertices; }

	/*
	 * addVertex allows you to add a vertex to the Polyline
	 *  The function does nothing if the vertex buffer is already full
	 * Parameters:
	 * 		x, the x position of the vertex
	 * 		y, the y position of the vertex
	 * 		r, the red component of the vertex
	 * 		g, the green component of the vertex
	 * 		b, the blue component of the vertex
	 * 		a, the alpha component of the vertex
	 */
	void addVertex(int x, int y, RGBfloatType color = BLACK) {
		if (init)
			return;
		vertices[current] = x;
		vertices[current+1] = y;
		vertices[current+2] = color.R;
		vertices[current+3] = color.G;
		vertices[current+4] = color.B;
		vertices[current+5] = color.A;
		current+=6;
		if (current == size)
			init = true;
	}

	// draw() actually draws the Polyline to the canvas
	void draw() {
		if (!init)
			return;
		glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 0, length);
	}
};

#endif /* POLYLINE_H_ */
