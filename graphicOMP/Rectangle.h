/*
 * Rectangle.h extends Shape and provides a class for drawing a rectangle to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"													// For extending our Shape object

class Rectangle : public Shape {
private:
	float vertices[24];
public:
	/*
	 * Explicit constructor for the Rectangle class
	 * Parameters:
	 * 		x, the x coordinate of the Rectangle's left edge
	 *		y, the y coordinate of the Rectangle's top edge
	 * 		w, the width of the Rectangle
	 *		h, the height of the Rectangle
	 * 		color, the color
	 * Returns: a new Rectangle with the specified top left corner, dimensions, and color
	 */
	Rectangle(int x, int y, int w, int h, RGBfloatType color) {
//		isTextured = true;	// Let the Canvas know we're a textured object
		vertices[0]  = x;
		vertices[1]  = y;
		vertices[6]  = x+w;
		vertices[7]  = y;
		vertices[12] = x;
		vertices[13] = y+h;
		vertices[18] = x+w;
		vertices[19] = y+h;
		vertices[2]  = vertices[8]  = vertices[14] = vertices[20] = color.R;
		vertices[3]  = vertices[9]  = vertices[15] = vertices[21] = color.G;
		vertices[4]  = vertices[10] = vertices[16] = vertices[22] = color.B;
		vertices[5]  = vertices[11] = vertices[17] = vertices[23] = color.A;
//		vertices[6] = vertices[7] = 0.0f;
//		vertices[14] = 1.0f, vertices[15] = 0.0f;
//		vertices[22] = 0.0f, vertices[23] = 1.0f;
//		vertices[30] = vertices[31] = 1.0f;
	}

	// draw() actually draws the Rectangle to the canvas
	void draw() {
//		std::cout << "drawing @ " << vertices[24] << "," << vertices[25] << std::endl;
		glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};

#endif /* RECTANGLE_H_ */
