/*
 * Text.h extends Shape and provides a class for drawing a string of text to the Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/11/2014
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Shape.h"											// For extending our Shape object
#include "ImageLoader.h"

class Text : public Shape {
private:
	ImageLoader 	myLoader;
	std::string 	myString;
	int 			myX, myY;
public:
	/*
	 * Explicit constructor for the Text class (calls the base constructor)
	 * Parameters:
	 * 		s, the string to draw
	 * 		x, the x coordinate
	 *		y, the y coordinate
	 * 		color, a color
	 * Returns: a new Text object at the specified position with the specified string and color
	 */
	Text(std::string s, ImageLoader& loader, int x, int y, RGBfloatType color);

	// draw actually draws the Text to the canvas
	void draw();
};

#endif /* TEXT_H_ */
