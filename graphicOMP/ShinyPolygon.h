/*
 * ShinyPolygon.h extends Shape and provides a class for drawing a polygon
 * 	with colored vertices to a Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/16/2014
 */

#ifndef SHINYPOLYGON_H_
#define SHINYPOLYGON_H_

#include "Shape.h"			// For extending our Shape object

class ShinyPolygon : public Shape {
private:
	bool init;					// Whether the vertex has been initialized completely
	int size, current, length;	// Max / current numbe of floats; number of vertices
	float* vertices;			// Buffer for vertex data
public:
	ShinyPolygon(int v);									// Default constructor
	~ShinyPolygon();
	void addVertex(int x, int y, RGBfloatType color);  			// Method for adding vertices to buffer
    void draw();												// Overridden draw method
};

/*
 * Explicit constructor for the ShinyPolygon class (calls the base constructor)
 * Parameters:
 * 		vertices, the number of vertices the complete polygon will have
 * Returns: a new ShinyPolygon with the specified numbered vertices
 */
ShinyPolygon::ShinyPolygon(int v = 3) : Shape(BLACK) {
	if (v < 3)
		throw std::out_of_range("Cannot have a polygon with fewer than 3 vertices.");
	length = v;
	size = length * 6;
	current = 0;
	vertices = new float[size];
	init = false;
}

ShinyPolygon::~ShinyPolygon() {
	delete vertices;
}

/*
 * addVertex allows you to add a vertex to the ShinyPolygon
 *  The function does nothing if the vertex buffer is already full
 * Parameters:
 * 		x, the x position of the vertex
 * 		y, the y position of the vertex
 * 		r, the red component of the vertex
 * 		g, the green component of the vertex
 * 		b, the blue component of the vertex
 * 		a, the alpha component of the vertex
 */
void ShinyPolygon::addVertex(int x, int y, RGBfloatType color = BLACK) {
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
void ShinyPolygon::draw() {
	if (!init)
		return;
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
}

#endif /* SHINYPOLYGON_H_ */
