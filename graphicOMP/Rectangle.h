/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/10/2014
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <FL/fl_draw.H>		// For the actual fl_line drawing function
#include "Shape.h"			// For extending our Shape object

class Rectangle : public Shape {
private:
	int myX, myY, myW, myH;										// Positioning data for our Rectangle
public:
	Rectangle(int x, int y, int w, int h);						// Default constructor
	Rectangle(int x, int y, int w, int h, int r, int g, int b);	// Explicit constructor
    void draw();												// Overridden draw method
    bool getIsPoint() { return false; }
};

/*
 * Explicit constructor for the Rectangle class (calls the base constructor)
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * Returns: a new Rectangle with the specified topleft corner and dimensions
 */
Rectangle::Rectangle(int x, int y, int w, int h) : Shape() {
	myX = x;
	myY = y;
	myW = w;
	myH = h;
}

/*
 * Explicit constructor for the Rectangle class
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * Returns: a new Rectangle with the specified topleft corner, dimensions, and color
 */
Rectangle::Rectangle(int x, int y, int w, int h, int r, int g, int b) : Shape(r, g, b) {
	myX = x;
	myY = y;
	myW = w;
	myH = h;
}

// draw() actually draws the Rectangle to the canvas
void Rectangle::draw() {
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(myX, myY);
	glVertex2f(myX, myY+myH);
	glVertex2f(myX+myW, myY);
	glVertex2f(myX+myW, myY+myH);
	glEnd();
}

#endif /* RECTANGLE_H_ */
