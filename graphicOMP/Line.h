/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/19/2014
 */

#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"													// For extending our Shape object

class Line : public Shape {
private:
	float vertices[12];
public:
    Line(int x1, int y1, int x2, int y2, RGBfloatType color);		// Explicit constructor
    void draw();													// Overridden draw method
    bool getIsPoint() { return false; }
};

/*
 * Explicit constructor for the Line class
 * Parameters:
 * 		x1, the first x coordinate
 *		y1, the first y coordinate
 * 		x2, the second x coordinate
 *		y2, the second y coordinate
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 		a, the alpha component
 * Returns: a new Line at the specified positions with the specified color
 */
Line::Line(int x1, int y1, int x2, int y2, RGBfloatType color) : Shape(color) {
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[6] = x2;
	vertices[7] = y2;
	vertices[2] = vertices[8] = color.R;
	vertices[3] = vertices[9] = color.G;
	vertices[4] = vertices[10] = color.B;
	vertices[5] = vertices[11] = color.A;
}

// draw() actually draws the Line to the canvas
void Line::draw() {
	glBufferData(GL_ARRAY_BUFFER, 2*sizeof(ColoredVertex), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, 2);
}

#endif /* LINE_H_ */
