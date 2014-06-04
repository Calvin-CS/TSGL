//
// Basic Point class build on top of FLTK library.
// UNFINISHED
//
// Last Modified: Patrick Crain, 6/4/2014
//

#ifndef POINT_H_
#define POINT_H_

#include <FL/fl_draw.H>
#include "Shape.h"
#include <iostream>

class Point : public Shape {
private:
	int myX, myY;
public:
    inline Point(int x, int y);
    inline Point(int x, int y, int r, int g, int b);
    inline virtual ~Point();
    inline void draw();
};

Point::Point(int x, int y) : Shape() {
	myX = x;
	myY = y;
}

Point::Point(int x, int y, int r, int g, int b) : Shape(r, g, b) {
	myX = x;
	myY = y;
}

Point::~Point() {}

void Point::draw() {
	fl_point(myX,myY);
}

#endif /* POINT_H_ */
