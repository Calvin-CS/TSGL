/*
 * Text.h extends Shape and provides a class for drawing strings of text to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/20/2014
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <FL/fl_draw.H>		// For the actual fl_draw drawing function
#include "Shape.h"			// For extending our Shape object
#include <iostream>
#include <string>

class Text : public Shape {
private:
	int myX, myY;									// Positioning data for our Text object
	const char * myString;								// String data for our Text object
public:
	Text(const char * s, int x, int y);								// Default constructor
	Text(const char * s, int x, int y, RGBfloatType color);			// Explicit constructor
    void draw();									// Overridden draw method
    bool getIsPoint() { return false; }
};

/*
 * Explicit constructor for the Text class (calls the base constructor)
 * Parameters:
 *		s, the string to display
 * 		x, the x coordinate
 *		y, the y coordinate
 * Returns: a new Text object at the specified position displaying the specified string
 */
Text::Text(const char * s, int x, int y) : Shape() {
	myX = x;
	myY = y;
	myString = s;
}

/*
 * Explicit constructor for the Point class (calls the base constructor)
 * Parameters:
 *		s, the string to display
 * 		x, the x coordinate
 *		y, the y coordinate
 * 		color, the color in which to draw the string
 * Returns: a new Point at the specified position with the specified color
 */
Text::Text(const char * s, int x, int y, RGBfloatType color) : Shape(color) {
	myX = x;
	myY = y;
	myString = s;
}

// draw actually draws the Text to the canvas
void Text::draw() {
	gl_draw(myString, myX, myY);
}

#endif /* TEXT_H_ */
