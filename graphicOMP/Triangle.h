/*
 * Triangle.h extends Shape and provides a class for drawing a three-sided polygon to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/2/2014
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"																	// For extending our Shape object

class Triangle : public Shape {
private:
	float vertices[18];
public:
	/*
	 * Explicit constructor for the Triangle class
	 * Parameters:
	 * 		x1, the first x coordinate
	 *		y1, the first y coordinate
	 * 		x2, the second x coordinate
	 *		y2, the second y coordinate
	 *		x3, the third x coordinate
	 *		y3, the third y coordinate
	 * 		color, the color
	 * Returns: a new Triangle with the specified vertices and color
	 */
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color) {
		vertices[0] = x1;
		vertices[1] = y1;
		vertices[6] = x2;
		vertices[7] = y2;
		vertices[12] = x3;
		vertices[13] = y3;
		vertices[2] = vertices[8]  = vertices[14] = color.R;
		vertices[3] = vertices[9]  = vertices[15] = color.G;
		vertices[4] = vertices[10] = vertices[16] = color.B;
		vertices[5] = vertices[11] = vertices[17] = color.A;
	}

	// draw() actually draws the Line to the canvas
	void draw() {
		glBufferData(GL_ARRAY_BUFFER, 3*sizeof(ColoredVertex), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	}
};

#endif /* TRIANGLE_H_ */
