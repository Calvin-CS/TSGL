/*
 * Triangle.h extends Shape and provides a class for drawing a three-sided polygon to a Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/12/2014
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <FL/fl_draw.H>		// For the actual fl_polygon drawing function
#include "Shape.h"			// For extending our Shape object

class Triangle : public Shape {
private:
	int myX1, myY1, myX2, myY2, myX3, myY3;						// Positioning data for our Triangle
public:
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3);						// Default constructor
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);	// Explicit constructor
    void draw();																	// Overridden draw method
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
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3) : Shape() {
	myX1 = x1;
	myY1 = y1;
	myX2 = x2;
	myY2 = y2;
	myX3 = x3;
	myY3 = y3;
}

/*
 * Explicit constructor for the Triangle class
 * Parameters:
 * 		x1, the first x coordinate
 *		y1, the first y coordinate
 * 		x2, the second x coordinate
 *		y2, the second y coordinate
 *		x3, the third x coordinate
 *		y3, the third y coordinate
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 		a, the alpha component
 * Returns: a new Triangle with the specified vertices and color
 */
Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a) : Shape(r, g, b, a) {
	myX1 = x1;
	myY1 = y1;
	myX2 = x2;
	myY2 = y2;
	myX3 = x3;
	myY3 = y3;
}

// draw() actually draws the Line to the canvas
void Triangle::draw() {
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(myX1, myY1);
	glVertex2f(myX2, myY2);
	glVertex2f(myX3, myY3);
	glEnd();
//	fl_polygon(myX1,myY1,myX2,myY2,myX3,myY3);	//Call to the FLTK fl_polygon drawing method
}

#endif /* TRIANGLE_H_ */
