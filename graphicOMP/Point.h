/*
 * Point.h extends Shape and provides a class for drawing a single point to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/19/2014
 */

#ifndef POINT_H_
#define POINT_H_

#include "Shape.h"											// For extending our Shape object

class Point : public Shape {
public:
	float vertices[6];
	Point(int x, int y, RGBfloatType color);				// Explicit constructor
	void draw();											// Overridden draw method
    bool getIsPoint() { return true; }
};

/*
 * Explicit constructor for the Point class (calls the base constructor)
 * Parameters:
 * 		x, the x coordinate
 *		y, the y coordinate
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 		a, the alpha component
 * Returns: a new Point at the specified position with the specified color
 */
Point::Point(int x, int y, RGBfloatType color = BLACK) : Shape(color) {
	vertices[0] = x;
	vertices[1] = y;
	vertices[2] = color.R;
	vertices[3] = color.G;
	vertices[4] = color.B;
	vertices[5] = color.A;
}

// draw actually draws the Point to the canvas
void Point::draw() {
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, 1);
}

#endif /* POINT_H_ */
