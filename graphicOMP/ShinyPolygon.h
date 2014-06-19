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
	struct VertexData {		// Data for each of the polygon's vertices
		int x;				// Vertex x position
		int y;				// Vertex y position
		float r;			// Vertex red component
		float g;			// Vertex green component
		float b;			// Vertex blue component
		float a;			// Vertex alpha component
	};
	bool init;				// Whether the vertex has been initialized completely
	int size, current;		// Ultimate and current number of vertices
	VertexData* myVertex;	// Buffer for vertex data
public:
	ShinyPolygon(int vertices);									// Default constructor
	void addVertex(int x, int y, RGBfloatType color);  			// Method for adding vertices to buffer
    void draw();												// Overridden draw method
    bool getIsPoint() { return false; }							// We are not a single point
};

/*
 * Explicit constructor for the ShinyPolygon class (calls the base constructor)
 * Parameters:
 * 		vertices, the number of vertices the complete polygon will have
 * Returns: a new ShinyPolygon with the specified numbered vertices
 */
ShinyPolygon::ShinyPolygon(int vertices = 3) : Shape() {
	size = (vertices > 3) ? vertices : 3;
	current = 0;
	myVertex = new VertexData[size];
	init = false;
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
void ShinyPolygon::addVertex(int x, int y, RGBfloatType color) {
	if (init)
		return;
	myVertex[current].x = x;
	myVertex[current].y = y;
	myVertex[current].r = color.R;
	myVertex[current].g = color.G;
	myVertex[current].b = color.B;
	myVertex[current].a = color.A;
	current++;
	if (current == size)
		init = true;
}

// draw() actually draws the Polygon to the canvas
void ShinyPolygon::draw() {
	if (!init)
		return;
	VertexData v;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < size; i++) {
		v = myVertex[i];
		glColor4f(v.r,v.g,v.b,v.a);
		glVertex2f(v.x,v.y);
	}
	glEnd();
}

#endif /* SHINYPOLYGON_H_ */
