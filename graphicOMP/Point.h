/*
 * Point.h extends Shape and provides a class for drawing a single point to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/6/2014
 */

#ifndef POINT_H_
#define POINT_H_

#include <FL/fl_draw.H>		//For the actual fl_point drawing function
#include "Shape.h"			//For extending our Shape object

class Point : public Shape {
private:
	int myX, myY;										//Positioning data for our Point
public:
    inline Point(int x, int y);							//Default constructor
    inline Point(int x, int y, int r, int g, int b);	//Explicit constructor
    inline void draw();									//Overridden draw method
};

/*
 * Explicit constructor for the Point class (calls the base constructor)
 * Parameters:
 * 		x, the x coordinate
 *		y, the y coordinate
 * Returns: a new Point at the specified position
 */
Point::Point(int x, int y) : Shape() {
	myX = x;
	myY = y;
}

/*
 * Explicit constructor for the Point class (calls the base constructor)
 * Parameters:
 * 		x, the x coordinate
 *		y, the y coordinate
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * Returns: a new Point at the specified position with the specified color
 */
Point::Point(int x, int y, int r, int g, int b) : Shape(r, g, b) {
	myX = x;
	myY = y;
}

// draw actually draws the Point to the canvas
void Point::draw() {
	fl_point(myX,myY);	//Call to the FLTK fl_point drawing method
}

#endif /* POINT_H_ */
