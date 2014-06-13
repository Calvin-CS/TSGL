/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/12/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Function.h"

class CartesianCanvas : public Canvas {
	typedef long double Type;						//Define the variable type to use for coordinates
	typedef void (*cfcall)(CartesianCanvas* const);	//Define a type for our callback function pointer
private:
	Type minX, maxX, minY, maxY;					//Bounding Cartesian coordinates for the window
	Type cartWidth, cartHeight;						//maxX-minX, maxY-minY
	Type pixelWidth, pixelHeight;					//cartWidth/window.w(), cartHeight/window.h()
	cfcall cartesianUpdateFunc;						//Function pointer to the user's update function
	Type xError, yError;							//Variables to hold rounding errors for rendering
public:
	CartesianCanvas(cfcall c, unsigned int b);							//Default constructor for our CartesianCanvas
	CartesianCanvas(cfcall c, int xx, int yy, int w, int h, Type xMin,
			Type yMin, Type xMax, Type yMax, unsigned int b, char *t);	//Explicit constructor for our CartesianCanvas
	inline void callUpdate();									//Actually calls updateFunc (needed to avoid typing errors)
	void getScreenCoordinates(Type cartX, Type cartY,
			int &screenX, int &screenY);						//Returns the equivalent screen coordinates for the specified Cartesian ones
	void getCartesianCoordinates(int screenX, int screenY,
			Type &cartX, Type &cartY);							//Returns the equivalent Cartesian coordinates for the specified screen ones
	Type getPixelWidth() { return pixelWidth; }					//Accessor for pixelWidth
	Type getPixelHeight() { return pixelHeight; }				//Accessor for pixelHeight
	Type getMinX() { return minX; }								//Accessor for minX
	Type getMaxX() { return maxX; }								//Accessor for maxX
	Type getMinY() { return minY; }								//Accessor for minY
	Type getMaxY() { return maxY; }								//Accessor for maxY
	Point drawPoint(Type x, Type y);							//Draws a point at the given coordinates
	Point drawPointColor(Type x, Type y, int r, int g, int b);	//Draws a point at the given coordinates with the given color
	Line drawLine(Type x1, Type y1, Type x2, Type y2);			//Draws a line at the given coordinates
	Line drawLineColor(Type x1, Type y1, Type x2,
						Type y2, int r, int g, int b);			//Draws a line at the given coordinates with the given color
	Rectangle drawRectangle(Type x, Type y, Type w, Type h);	//Draws a rectangle at the given coordinates with the given dimensions
	Rectangle drawRectangleColor(Type x, Type y, Type w,
			Type h, int r, int g, int b);						//Draws a rectangle at the given coordinates with the given dimensions and color
	Triangle drawTriangle(int x1, int y1, int x2, int y2,
			int x3, int y3);									//Draws a triangle with the given vertices
	Triangle drawTriangleColor(int x1, int y1, int x2, int y2,
			int x3, int y3,	int r, int g, int b);				//Draws a triangle with the given vertices and color
	const Function* drawFunction(const Function* f);
};

/*
 * Default constructor for the CartesianCanvas class
 * Parameter:
 * 		c, a callback to the user's own draw function
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 CartesianCanvas with 1-1 pixel correspondence and central origin
 */
CartesianCanvas::CartesianCanvas(cfcall c, unsigned int b) : Canvas(NULL, b) {
	if (c == NULL) {
		cartesianUpdateFunc = [](CartesianCanvas* c){};					//Empty lambda function that does nothing
	} else
		cartesianUpdateFunc = c;										//Adds a callback to the user's own draw function
	minX = -400;
	maxX = 400;
	minY = -300;
	maxY = 300;
	xError = 1.0f/monitorWidth;
	yError = 1.0f/monitorHeight;
	cartWidth = maxX-minX;
	cartHeight = maxY-minY;
	pixelWidth = (cartWidth-xError) / monitorWidth;
	pixelHeight = (cartHeight-yError) / monitorHeight;
}

/*
 * Explicit constructor for the CartesianCanvas class
 * Parameters:
 * 		c, a callback to the user's own draw function
 * 		xx, the x position of the CartesianCanvas window
 * 		yy, the y position of the CartesianCanvas window
 * 		w, the x dimension of the CartesianCanvas window
 * 		h, the y dimension of the CartesianCanvas window
 * 		xMin, the real number corresponding to the left edge of the CartesianCanvas
 * 		YMin, the real number corresponding to the top edge of the CartesianCanvas
 * 		xMax, the real number corresponding to the right edge of the CartesianCanvas
 * 		xMax, the real number corresponding to the bottom edge of the CartesianCanvas
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * 		t, the title of the window
 * Returns: a new CartesianCanvas with the specified positional/scaling data and title
 */
CartesianCanvas::CartesianCanvas(cfcall c, int xx, int yy, int w, int h,
			Type xMin, Type yMin, Type xMax, Type yMax, unsigned int b, char* t = 0) :
			Canvas(NULL, xx, yy, w, h, b, t) {
	if (c == NULL) {
		cartesianUpdateFunc = [](CartesianCanvas* c){};					//Empty lambda function that does nothing
	} else {
		cartesianUpdateFunc = c;										//Adds a callback to the user's own draw function
	}
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	xError = 1.0f/monitorWidth;
	yError = 1.0f/monitorHeight;
	cartWidth = (maxX-minX)-xError;
	cartHeight = (maxY-minY)-yError;
	pixelWidth = (cartWidth) / (monitorWidth+xError);
	pixelHeight = (cartHeight) / (monitorHeight+yError);
}

/*
 * callUpdate simply calls the update function for the CartesianCanvas class
 */
void CartesianCanvas::callUpdate() {
	cartesianUpdateFunc(this);		//Call the user's callback to do work on the Canvas
}

/*
 * getScreenCoordinates takes a pair of Cartesian coordinates and translates them to on-screen coordinates
 * Parameters:
 * 		cartX, the Cartesian x coordinate
 * 		cartY, the Cartesian y coordinate
 * 		screenX, a reference variable to be filled with cartX's window position
 * 		screenY, a reference variable to be filled with cartY's window position
 */
void CartesianCanvas::getScreenCoordinates(Type cartX, Type cartY, int &screenX, int &screenY) {
	screenX = (cartX-minX)/cartWidth*monitorWidth;
	screenY = window->h() - (cartY-minY)/cartHeight*monitorHeight;
}

/*
 * getCartesianCoordinates takes a pair of on-screen coordinates and translates them to Cartesian coordinates
 * Parameters:
 * 		screenX, the window's x coordinate
 * 		screenY, the window's y coordinate
 * 		cartX, a reference variable to be filled with screenX's Cartesian position
 * 		cartY, a reference variable to be filled with screenY's Cartesian position
 */
void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Type &cartX, Type &cartY) {
	cartX = (screenX*cartWidth)/monitorWidth + minX;
	cartY = window->h() + (screenY*cartHeight)/monitorHeight + minY;
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the Cartesian-adjusted position
 */
Point CartesianCanvas::drawPoint(Type x, Type y) {
	int actualX, actualY;
	getScreenCoordinates(x,y,actualX,actualY);
	Point* p = new Point(actualX,actualY);	//Creates the Point with the specified coordinates
	myShapes->push(p);			//Push it onto our drawing queue
	return *p;					//Return a pointer to our new Point
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new point at the Cartesian-adjusted position with the specified color
 */
Point CartesianCanvas::drawPointColor(Type x, Type y, int r, int g, int b) {
	int actualX, actualY;
	getScreenCoordinates(x,y,actualX,actualY);
	Point* p = new Point(actualX,actualY,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(p);					//Push it onto our drawing queue
	return *p;							//Return a pointer to our new Point
}

/*
 * drawLine draws a line at the given coordinates
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 	Returns: a new line with Cartesian-adjusted coordinates
 */
Line CartesianCanvas::drawLine(Type x1, Type y1, Type x2, Type y2) {
	int actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2); //Creates the Line with the specified coordinates
	myShapes->push(l);				//Push it onto our drawing queue
	return *l;						//Return a pointer to our new Line
}

/*
 * drawLineColor draws a line at the given coordinates with the given color
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new line with Cartesian-adjusted coordinates and the specified color
 */
Line CartesianCanvas::drawLineColor(Type x1, Type y1, Type x2, Type y2, int r, int g, int b) {
	int actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2,r,g,b);	//Creates the Line with the specified coordinates and color
	myShapes->push(l);						//Push it onto our drawing queue
	return *l;								//Return a pointer to our new Line
}

/*
 * drawRectangle draws a rectangle with the given coordinates and dimensions
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 	Returns: a new rectangle with the given Cartesian-adjusted coordinates and dimensions
 */
Rectangle CartesianCanvas::drawRectangle(Type x, Type y, Type w, Type h) {
	int actualX, actualY, actualW, actualH;
	getScreenCoordinates(x,y,actualX,actualY);
	getScreenCoordinates(w,h,actualW,actualH);
	Rectangle* rec = new Rectangle(x,y,w,h);	//Creates the Rectangle with the specified coordinates
	myShapes->push(rec);						//Push it onto our drawing queue
	return *rec;								//Return a pointer to our new Rectangle
}

/*
 * drawRectangleColor draws a rectangle with the given coordinates, dimensions, and color
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 	Returns: a new rectangle with the given Cartesian-adjusted coordinates, dimensions, and color
 */
Rectangle CartesianCanvas::drawRectangleColor(Type x, Type y, Type w, Type h, int r, int g, int b) {
	int actualX, actualY, actualW, actualH;
	getScreenCoordinates(x,y,actualX,actualY);
	getScreenCoordinates(w,h,actualW,actualH);
	Rectangle* rec = new Rectangle(x,y,w,h,r,g,b);		//Creates the Rectangle with the specified coordinates and color
	myShapes->push(rec);								//Push it onto our drawing queue
	return *rec;										//Return a pointer to our new Rectangle
}

/*
 * drawTriangle draws a Triangle with the given vertices
 * Parameters:
 * 		x1, the x position of the first vertex of the triangle
 * 		y1, the y position of the first vertex of the triangle
 *		x2, the x position of the second vertex of the triangle
 * 		y2, the y position of the second vertex of the triangle
 * 		x3, the x position of the third vertex of the triangle
 * 		y3, the y position of the third vertex of the triangle
 * 	Returns: a new triangle with the given Cartesian-adjusted vertices
 */
Triangle CartesianCanvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	getScreenCoordinates(x3,y3,actualX3,actualY3);
	Triangle* t = new Triangle(actualX1,actualY1,actualX2,
			actualY2,actualX3,actualY3);	// Creates the Triangle with the specified vertices
	myShapes->push(t);						// Push it onto our drawing queue
	return *t;								// Return a pointer to our new Triangle
}

/*
 * drawTriangle draws a Triangle with the given vertices
 * Parameters:
 * 		x1, the x position of the first vertex of the triangle
 * 		y1, the y position of the first vertex of the triangle
 *		x2, the x position of the second vertex of the triangle
 * 		y2, the y position of the second vertex of the triangle
 * 		x3, the x position of the third vertex of the triangle
 * 		y3, the y position of the third vertex of the triangle
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 	Returns: a new triangle with the given Cartesian-adjusted vertices and color
 */
Triangle CartesianCanvas::drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3,
		int r, int g, int b) {
	int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	getScreenCoordinates(x3,y3,actualX3,actualY3);
	Triangle* t = new Triangle(actualX1,actualY1,actualX2,actualY2,
			actualX3,actualY3,r,g,b);	// Creates the Triangle with the specified vertices and color
	myShapes->push(t);					// Push it onto our drawing queue
	return *t;							// Return a pointer to our new Triangle
}

const Function* CartesianCanvas::drawFunction(const Function* f) {
	fl_color(0,0,0);

	int lastX = 0, lastY = 0, screenX = 0, screenY = 0;

	fl_begin_line();
	fl_line_style(0,2,0);
	bool drawNext = false;
	for (Type x = minX; x <= maxX; x += pixelWidth) {
		lastX = screenX, lastY = screenY;
		getScreenCoordinates(x, f->valueAt(x), screenX, screenY);
		if (screenX < 0 || screenY < 0 || screenX > window->w() || screenY > window->h()) {
			if (drawNext)
				fl_vertex(screenX,screenY);
			drawNext = false;
		}
		else {
			if (!drawNext && lastX != screenX)
				fl_vertex(lastX,lastY);
			fl_vertex(screenX,screenY);
			drawNext =true;
		}
	}
	fl_end_line();

	fl_color(colorR, colorG, colorB);
	return f;
}

#endif /* CARTESIANCANVAS_H_- */
