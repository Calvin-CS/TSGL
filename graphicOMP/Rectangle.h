/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/10/2014
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"													// For extending our Shape object

class Rectangle : public Shape {
private:
	float vertices[24];
public:
	Rectangle(int x, int y, int w, int h, RGBfloatType color);		// Explicit constructor
    void draw();													// Overridden draw method
};

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
 * 		a, the alpha component
 * Returns: a new Rectangle with the specified topleft corner, dimensions, and color
 */
Rectangle::Rectangle(int x, int y, int w, int h, RGBfloatType color) : Shape(color) {
	vertices[0] = x;
	vertices[1] = y;
	vertices[6] = x+w;
	vertices[7] = y;
	vertices[12] = x;
	vertices[13] = y+h;
	vertices[18] = x+w;
	vertices[19] = y+h;
	vertices[2] = vertices[8] = vertices[14] =vertices[20] =color.R;
	vertices[3] = vertices[9] = vertices[15] =vertices[21] =color.G;
	vertices[4] = vertices[10] = vertices[16] =vertices[22] =color.B;
	vertices[5] = vertices[11] = vertices[17] =vertices[23] =color.A;
}

// draw() actually draws the Rectangle to the canvas
void Rectangle::draw() {
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(ColoredVertex), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

#endif /* RECTANGLE_H_ */
