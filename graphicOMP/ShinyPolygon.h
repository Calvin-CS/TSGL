/*
 * ShinyPolygon.h extends Shape and provides a class for drawing a polygon
 * 	with colored vertices to a Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 7/2/2014
 */

#ifndef SHINYPOLYGON_H_
#define SHINYPOLYGON_H_

#include "Shape.h"				// For extending our Shape object

class ShinyPolygon : public Shape {
private:
	bool init;					// Whether the vertex has been initialized completely
	float* vertices;			// Buffer for vertex data
	int size,					// Number of floating point numbers in vertices
		current,				// Current number of floating point numbers in vertices
		length;					// Number of vertices in vertices (size / 6)
public:
	/*
	 * Explicit constructor for the ShinyPolygon class (calls the base constructor)
	 * Parameters:
	 * 		v, the number of vertices the complete polygon will have
	 * Returns: a new ShinyPolygon with the specified numbered vertices
	 */
	ShinyPolygon(int v = 3) {
		if (v < 3)
			throw std::out_of_range("Cannot have a polygon with fewer than 3 vertices.");
		length = v;
		size = length * 6;
		current = 0;
		vertices = new float[size];
		init = false;
	}

	~ShinyPolygon() { delete vertices; }

	/*
	 * addVertex allows you to add a vertex to the ShinyPolygon
	 *  The function does nothing if the vertex buffer is already full
	 * Parameters:
	 * 		x, the x position of the vertex
	 * 		y, the y position of the vertex
	 * 		color, the color of the vertex
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

	// draw() actually draws the Polygon to the canvas
	void draw() {
		if (!init)	return;
		glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
	}
};



#endif /* SHINYPOLYGON_H_ */
