/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/6/2014
 */

#ifndef LINE_H_
#define LINE_H_

#include <FL/fl_draw.H>		//For the actual fl_line drawing function
#include "Shape.h"			//For extending our Shape object

class Line : public Shape {
private:
	int myX1, myY1, myX2, myY2;											//Positioning data for our Line
public:
    inline Line(int x1, int y1, int x2, int y2);						//Default constructor
    inline Line(int x1, int y1, int x2, int y2, int r, int g, int b);	//Explicit constructor
    inline virtual ~Line();												//Default destructor
    inline void draw();													//Overridden draw method
};

/*
 * Explicit constructor for the Line class (calls the base constructor)
 * Parameters:
 * 		x1, the first x coordinate
 *		y1, the first y coordinate
 * 		x2, the second x coordinate
 *		y2, the second y coordinate
 * Returns: a new Line at the specified positions
 */
Line::Line(int x1, int y1, int x2, int y2) : Shape() {
	myX1 = x1;
	myY1 = y1;
	myX2 = x2;
	myY2 = y2;
}

/*
 * Explicit constructor for the Line class (calls the base constructor)
 * Parameters:
 * 		x1, the first x coordinate
 *		y1, the first y coordinate
 * 		x2, the second x coordinate
 *		y2, the second y coordinate
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * Returns: a new Line at the specified positions with the specified color
 */
Line::Line(int x1, int y1, int x2, int y2, int r, int g, int b) : Shape(r, g, b) {
	myX1 = x1;
	myY1 = y1;
	myX2 = x2;
	myY2 = y2;
}

// Destructor for the Line class
Line::~Line() {}

// draw actually draws the Line to the canvas
void Line::draw() {
	fl_line(myX1,myY1,myX2,myY2);	//Call to the FLTK fl_point drawing method
}

#endif /* LINE_H_ */
