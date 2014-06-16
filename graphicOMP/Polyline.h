/*
 * Polyline.h extends Shape and provides a class for drawing a polyline
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/16/2014
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"			// For extending our Shape object

class Polyline : public Shape {
private:
	struct VertexData {		// Data for each of the polygon's vertices
		int x;				// Vertex x position
		int y;				// Vertex y position
	};
	bool init;				// Whether the vertex has been initialized completely
	int size, current;		// Ultimate and current number of vertices
	VertexData* myVertex;	// Buffer for vertex data
public:
	Polyline(int vertices);					// Default constructor
	void addVertex(int x, int y);  			// Method for adding vertices to buffer
    void draw();							// Overridden draw method
    bool getIsPoint() { return false; }		// We are not a single point
};

/*
 * Explicit constructor for the Polyline class (calls the base constructor)
 * Parameters:
 * 		vertices, the number of vertices the complete polyline will have
 * Returns: a new Polyline with the specified numbered vertices
 */
Polyline::Polyline(int vertices = 2) : Shape() {
	size = (vertices > 2) ? vertices : 2;
	current = 0;
	myVertex = new VertexData[size];
	init = false;
}

/*
 * addVertex allows you to add a vertex to the Polyline
 *  The function does nothing if the vertex buffer is already full
 * Parameters:
 * 		x, the x position of the vertex
 * 		y, the y position of the vertex
 */
void Polyline::addVertex(int x, int y) {
	if (init)
		return;
	myVertex[current].x = x;
	myVertex[current].y = y;
	current++;
	if (current == size)
		init = true;
}

// draw() actually draws the Polyline to the canvas
void Polyline::draw() {
	if (!init) {
		std::cerr << "Polyline not initialized." << std::endl;
		return;
	}
	std::cout << "Polyline initialized." << std::endl;
	glBegin(GL_LINE_STRIP);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	for (int i = 0; i < size; i++)
		glVertex2f(myVertex[i].x,myVertex[i].y);
	glEnd();
}

#endif /* SHINYPOLYGON_H_ */
