/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/12/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Function.h"								// For drawing math functions on the screen

class CartesianCanvas : public Canvas {
	typedef long double Type;						// Define the variable type to use for coordinates
private:
	Type minX, maxX, minY, maxY;					// Bounding Cartesian coordinates for the window
	Type cartWidth, cartHeight;						// maxX-minX, maxY-minY
	Type pixelWidth, pixelHeight;					// cartWidth/window.w(), cartHeight/window.h()
	Type xError, yError;							// Variables to hold rounding errors for rendering
public:
	CartesianCanvas(unsigned int b);				// Default constructor for our CartesianCanvas
	CartesianCanvas(int xx, int yy, int w, int h, Type xMin,
			Type yMin, Type xMax, Type yMax, unsigned int b, char *t);	// Explicit constructor for our CartesianCanvas
	void getScreenCoordinates(Type cartX, Type cartY,
			int &screenX, int &screenY);						// Returns the equivalent screen coordinates for the specified Cartesian ones
	void getCartesianCoordinates(int screenX, int screenY,
			Type &cartX, Type &cartY);							// Returns the equivalent Cartesian coordinates for the specified screen ones
	Type getPixelWidth() 	{ return pixelWidth; }				// Accessor for pixelWidth
	Type getPixelHeight() 	{ return pixelHeight; }				// Accessor for pixelHeight
	Type getMinX() 			{ return minX; }					// Accessor for minX
	Type getMaxX() 			{ return maxX; }					// Accessor for maxX
	Type getMinY() 			{ return minY; }					// Accessor for minY
	Type getMaxY() 			{ return maxY; }					// Accessor for maxY
	void drawPoint(Type x, Type y);							// Draws a point at the given coordinates
	void drawPointColor(Type x, Type y, int r, int g, int b);	// Draws a point at the given coordinates with the given color
	void drawLine(Type x1, Type y1, Type x2, Type y2);			// Draws a line at the given coordinates
	void drawLineColor(Type x1, Type y1, Type x2,
						Type y2, int r, int g, int b);			// Draws a line at the given coordinates with the given color
	void drawRectangle(Type x, Type y, Type w, Type h);	// Draws a rectangle at the given coordinates with the given dimensions
	void drawRectangleColor(Type x, Type y, Type w,
			Type h, int r, int g, int b);						// Draws a rectangle at the given coordinates with the given dimensions and color
	void drawTriangle(int x1, int y1, int x2, int y2,
			int x3, int y3);									// Draws a triangle with the given vertices
	void drawTriangleColor(int x1, int y1, int x2, int y2,
			int x3, int y3,	int r, int g, int b);				// Draws a triangle with the given vertices and color
	const Function* drawFunction(const Function* f);			// Draws the Function on the screen
};

/*
 * Default constructor for the CartesianCanvas class
 * Parameter:
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 CartesianCanvas with 1-1 pixel correspondence and central origin
 */
CartesianCanvas::CartesianCanvas(unsigned int b) : Canvas(b) {
	minX = -400;
	maxX = 400;
	minY = -300;
	maxY = 300;
	xError = 1.0f / monitorWidth;
	yError = 1.0f / monitorHeight;
	cartWidth = maxX - minX;
	cartHeight = maxY - minY;
	pixelWidth = (cartWidth - xError) / monitorWidth;
	pixelHeight = (cartHeight - yError) / monitorHeight;
}

/*
 * Explicit constructor for the CartesianCanvas class
 * Parameters:
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
CartesianCanvas::CartesianCanvas(int xx, int yy, int w, int h,
			Type xMin, Type yMin, Type xMax, Type yMax, unsigned int b, char* t = 0) :
			Canvas(xx, yy, w, h, b, t) {
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	xError = 1.0f / monitorWidth;
	yError = 1.0f / monitorHeight;
	cartWidth = (maxX - minX) - xError;
	cartHeight = (maxY - minY) - yError;
	pixelWidth = (cartWidth) / (monitorWidth + xError);
	pixelHeight = (cartHeight) / (monitorHeight + yError);
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
	std::unique_lock<std::mutex> mlock(mutex);
	screenX = (cartX - minX) / cartWidth * monitorWidth;
	screenY = window->h() - (cartY - minY) / cartHeight * monitorHeight;
	mlock.unlock();
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
	std::unique_lock<std::mutex> mlock(mutex);
	cartX = (screenX * cartWidth) / monitorWidth + minX;
	cartY = window->h() + (screenY * cartHeight) / monitorHeight + minY;
	mlock.unlock();
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the Cartesian-adjusted position
 */
void CartesianCanvas::drawPoint(Type x, Type y) {
	int actualX, actualY;
	getScreenCoordinates(x, y, actualX, actualY);
	Point* p = new Point(actualX, actualY);		// Creates the Point with the specified coordinates
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);							// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawPointColor(Type x, Type y, int r, int g, int b) {
	int actualX, actualY;
	getScreenCoordinates(x, y, actualX, actualY);
	Point* p = new Point(actualX, actualY, r, g, b);	// Creates the Point with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);									// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawLine(Type x1, Type y1, Type x2, Type y2) {
	int actualX1, actualY1, actualX2, actualY2;
	getScreenCoordinates(x1, y1, actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	Line* l = new Line(actualX1, actualY1, actualX2, actualY2); // Creates the Line with the specified coordinates
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(l);											// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawLineColor(Type x1, Type y1, Type x2, Type y2, int r, int g, int b) {
	int actualX1, actualY1, actualX2, actualY2;
	getScreenCoordinates(x1, y1,actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	Line* l = new Line(actualX1, actualY1, actualX2, actualY2, r, g, b);	// Creates the Line with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(l);														// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawRectangle(Type x, Type y, Type w, Type h) {
	int actualX, actualY, actualW, actualH;
	getScreenCoordinates(x, y, actualX, actualY);
	getScreenCoordinates(w, h, actualW, actualH);
	Rectangle* rec = new Rectangle(x, y, w, h);	// Creates the Rectangle with the specified coordinates
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(rec);						// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawRectangleColor(Type x, Type y, Type w, Type h, int r, int g, int b) {
	int actualX, actualY, actualW, actualH;
	getScreenCoordinates(x, y, actualX, actualY);
	getScreenCoordinates(w, h, actualW, actualH);
	Rectangle* rec = new Rectangle(x, y, w, h, r, g, b);	// Creates the Rectangle with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(rec);									// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
	getScreenCoordinates(x1, y1, actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	getScreenCoordinates(x3, y3, actualX3, actualY3);
	Triangle* t = new Triangle(actualX1, actualY1, actualX2,
			actualY2, actualX3, actualY3);	// Creates the Triangle with the specified vertices
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(t);						// Push it onto our drawing queue
	mlock.unlock();
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
void CartesianCanvas::drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3,
		int r, int g, int b) {
	int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
	getScreenCoordinates(x1, y1, actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	getScreenCoordinates(x3, y3, actualX3, actualY3);
	Triangle* t = new Triangle(actualX1, actualY1, actualX2, actualY2,
			actualX3, actualY3, r, g, b);	// Creates the Triangle with the specified vertices and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(t);						// Push it onto our drawing queue
	mlock.unlock();
}

const Function* CartesianCanvas::drawFunction(const Function* f) {
	std::unique_lock<std::mutex> mlock(mutex);
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
				fl_vertex(screenX, screenY);
			drawNext = false;
		}
		else {
			if (!drawNext && lastX != screenX)
				fl_vertex(lastX, lastY);
			fl_vertex(screenX, screenY);
			drawNext = true;
		}
	}
	fl_end_line();

	fl_color(defaultRed, defaultGreen, defualtBlue);
	mlock.unlock();
	return f;
}

#endif /* CARTESIANCANVAS_H_- */
