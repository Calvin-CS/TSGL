/*
 * ShinyPolygon.h extends Shape and provides a class for drawing a polygon
 * 	with colored vertices to a Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/16/2014
 */

#ifndef SHINYPOLYGON_H_
#define SHINYPOLYGON_H_

#include <FL/fl_draw.H>		// For the actual fl_polygon drawing function
#include "Shape.h"			// For extending our Shape object
#include <iostream>

class ShinyPolygon : public Shape {
private:
	struct VertexData {
		int x;
		int y;
		float r;
		float g;
		float b;
		float a;
	};
	bool init;
	int size, current;
	VertexData* myVertex;
public:
	ShinyPolygon(int vertices);				// Default constructor
	void addVertex(int x, int y, int r, int g, int b, int a);
    void draw();							// Overridden draw method
    bool getIsPoint() { return false; }
};

/*
 * Explicit constructor for the Triangle class (calls the base constructor)
 * Parameters:
 * 		x1, the first x coordinate
 *		y1, the first y coordinate
 * 		x2, the second x coordinate
 *		y2, the second y coordinate
 *		x3, the third x coordinate
 *		y3, the third y coordinate
 * Returns: a new Triangle with the specified vertices
 */
ShinyPolygon::ShinyPolygon(int vertices = 3) : Shape() {
	size = (vertices > 3) ? vertices : 3;
	current = 0;
	myVertex = new VertexData[size];
	init = false;
}

void ShinyPolygon::addVertex(int x, int y, int r, int g, int b, int a) {
	if (init)
		return;
	myVertex[current].x = x;
	myVertex[current].y = y;
	myVertex[current].r = r/255.0f;
	myVertex[current].g = g/255.0f;
	myVertex[current].b = b/255.0f;
	myVertex[current].a = a/255.0f;
	current++;
	if (current == size)
		init = true;
}

// draw() actually draws the Line to the canvas
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
//	fl_polygon(myX1,myY1,myX2,myY2,myX3,myY3);	//Call to the FLTK fl_polygon drawing method
}

#endif /* SHINYPOLYGON_H_ */
