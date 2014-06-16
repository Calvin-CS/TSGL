/*
 * Polyline.h extends Shape and provides a class for drawing a polyline
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/16/2014
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"			// For extending our Shape object
#include <stdexcept>		// Needed for exceptions

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
	if (vertices < 2) {
		std::cerr << "Cannot have a line with less than 2 vertices." << std::endl;
		throw std::out_of_range("Cannot have a line with less than 2 vertices.");
	}
	size = vertices;
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
	if (init) {
		std::cerr << "Polyline has all vertices already initialized." << std::endl;
		throw std::out_of_range("Polyline has all vertices already initialized.");
	}
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
		throw std::out_of_range("Polyline not initialized.");
	}
	glBegin(GL_LINE_STRIP);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	for (int i = 0; i < size; i++) {
		glVertex2f(myVertex[i].x,myVertex[i].y);
	}
	glEnd();
}

#endif /* SHINYPOLYGON_H_ */
