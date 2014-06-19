/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/19/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Function.h"									// For drawing math functions on the screen

class CartesianCanvas : public Canvas {
	typedef long double Decimal;						// Define the variable type to use for coordinates
private:
	Decimal minX, maxX, minY, maxY;						// Bounding Cartesian coordinates for the window
	Decimal cartWidth, cartHeight;						// maxX-minX, maxY-minY
	Decimal pixelWidth, pixelHeight;					// cartWidth/window.w(), cartHeight/window.h()
	bool zoomed, canZoom;								// Whether we've zoomed since last draw or we can zoom at all
protected:
	int handle(int e) {									//Handler for mouse events
		if (!canZoom)						//If we can't zoom, don't bother handling anything
			return 1;
        static Decimal oldX = 0, oldY = 0;
        int ret = Fl_Window::handle(e);
		Decimal newX, newY, temp, aspect, mean, delta;
        switch ( e ) {
            case FL_PUSH:					// On mouse push, store the mouse position into oldX, oldY
            	getCartesianCoordinates(Fl::event_x(),Fl::event_y(),oldX, oldY);
                return(1);
            case FL_RELEASE:				// On mouse release, store the mouse position into newX, newY
            	getCartesianCoordinates(Fl::event_x(),Fl::event_y(),newX, newY);
            	if (Fl::event_button() == FL_LEFT_MOUSE) {	// On left click, zoom in
            		if (std::abs(newX-oldX) < cartWidth/32 && std::abs(newY-oldY) < cartHeight/32)
            			return(1);
            		if (oldX > newX) {	// Makes sure oldX, oldY is the topleft
            			temp = oldX;
            			oldX = newX;
            			newX = temp;
            		}
            		if (oldY > newY) {
            			temp = oldY;
            			oldY = newY;
            			newY = temp;
            		}
            		aspect = ((newX-oldX)/(newY-oldY))/(cartWidth/cartHeight);	// Compute the different in aspect ratios
            		mean = (newY+oldY) / 2;				// Compute the middle of the current y dimension
            		delta = aspect * (newY-oldY) / 2;	// Compute the new y radius with the given aspect ratio
            		oldY = mean - delta;				// Adjust the Y dimensions to maintain the aspect ratio
            		newY = mean + delta;
            		recomputeDimensions(oldX,oldY,newX,newY);
            	} else if (Fl::event_button() == FL_RIGHT_MOUSE) {	// On right click, zoom out
            		recomputeDimensions(oldX-cartWidth,oldY-cartHeight,newX+cartWidth,newY+cartHeight);
            	}
                return(1);
        }
        return(ret);
	}
public:
	CartesianCanvas(unsigned int b);				// Default constructor for our CartesianCanvas
	CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin,
			Decimal yMin, Decimal xMax, Decimal yMax, unsigned int b, char *t);	// Explicit constructor for our CartesianCanvas
	void recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);	// Recomputes CartesianCanvas' size variables
	void getScreenCoordinates(Decimal cartX, Decimal cartY,
			int &screenX, int &screenY);									// Returns the equivalent screen coordinates for the specified Cartesian ones
	void getCartesianCoordinates(int screenX, int screenY,
			Decimal &cartX, Decimal &cartY);								// Returns the equivalent Cartesian coordinates for the specified screen ones
	Decimal getPixelWidth() 	{ return pixelWidth; }						// Accessor for pixelWidth
	Decimal getPixelHeight() 	{ return pixelHeight; }						// Accessor for pixelHeight
	Decimal getMinX() 			{ return minX; }							// Accessor for minX
	Decimal getMaxX() 			{ return maxX; }							// Accessor for maxX
	Decimal getMinY() 			{ return minY; }							// Accessor for minY
	Decimal getMaxY() 			{ return maxY; }							// Accessor for maxY
	Decimal getCartWidth()		{ return cartWidth; }						// Accessor for cartWidth
	Decimal getCartHeight()		{return cartHeight; }						// Accessor for cartHeight
	bool getZoomed()			{ return zoomed; }							// Accessor for zoomed
	void setZoomed(bool z)		{ zoomed = z; }								// Mutator for zoomed
	bool getCanZoom()			{ return canZoom; }							// Accessor for canZoom
	void setCanZoom(bool z)		{ canZoom = z; }							// Mutator for canZoom
	void drawPoint(Decimal x, Decimal y);									// Draws a point at the given coordinates
	void drawPointColor(Decimal x, Decimal y, RGBfloatType color);			// Draws a point at the given coordinates with the given color
	void drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2);			// Draws a line at the given coordinates
	void drawLineColor(Decimal x1, Decimal y1, Decimal x2,
						Decimal y2, RGBfloatType color);					// Draws a line at the given coordinates with the given color
	void drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h);			// Draws a rectangle at the given coordinates with the given dimensions
	void drawRectangleColor(Decimal x, Decimal y, Decimal w,
								Decimal h, RGBfloatType color);				// Draws a rectangle at the given coordinates with the given dimensions and color
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);		// Draws a triangle with the given vertices
	void drawTriangleColor(int x1, int y1, int x2, int y2,
							int x3, int y3,	RGBfloatType color);			// Draws a triangle with the given vertices and color
	void drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]);// Draws an arbitrary polygon with colored vertices
	const Function* drawFunction(const Function* f);						// Draws the Function on the screen
};

/*
 * Default constructor for the CartesianCanvas class
 * Parameter:
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 CartesianCanvas with 1-1 pixel correspondence and central origin
 */
CartesianCanvas::CartesianCanvas(unsigned int b) : Canvas(b) {
	recomputeDimensions(-400,-300,400,300);
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
			Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, unsigned int b, char* t = 0) :
			Canvas(xx, yy, w, h, b, t) {
	recomputeDimensions(xMin, yMin, xMax, yMax);
}

/*
 * recomputeDimensions recomputes the size variables of CartesianCanvas according to new bounds
 * Parameters:
 * 		xMin, a real number corresponding to the new left edge of the CartesianCanvas
 * 		YMin, a real number corresponding to the new top edge of the CartesianCanvas
 * 		xMax, a real number corresponding to the new right edge of the CartesianCanvas
 * 		xMax, a real number corresponding to the new bottom edge of the CartesianCanvas
 */
void CartesianCanvas::recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax) {
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	cartWidth = maxX - minX;
	cartHeight = maxY - minY;
	Decimal xError = cartWidth / monitorWidth;
	Decimal yError = cartHeight / monitorHeight;
	pixelWidth = (cartWidth - xError) / (monitorWidth + xError);
	pixelHeight = (cartHeight  - yError) / (monitorHeight + yError);
	zoomed = true;
}

/*
 * getScreenCoordinates takes a pair of Cartesian coordinates and translates them to on-screen coordinates
 * Parameters:
 * 		cartX, the Cartesian x coordinate
 * 		cartY, the Cartesian y coordinate
 * 		screenX, a reference variable to be filled with cartX's window position
 * 		screenY, a reference variable to be filled with cartY's window position
 */
void CartesianCanvas::getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY) {
	std::unique_lock<std::mutex> mlock(mutex);
	screenX = ceil((cartX - minX) / cartWidth * monitorWidth);
	screenY = ceil(h() - (cartY - minY) / cartHeight * monitorHeight);
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
void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY) {
	std::unique_lock<std::mutex> mlock(mutex);
	cartX = (screenX * cartWidth) / monitorWidth + minX;
	cartY = minY-(screenY - h())*cartHeight/monitorHeight;
	mlock.unlock();
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 */
void CartesianCanvas::drawPoint(Decimal x, Decimal y) {
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
 * 		a, the alpha component
 */
void CartesianCanvas::drawPointColor(Decimal x, Decimal y, RGBfloatType color) {
	int actualX, actualY;
	getScreenCoordinates(x, y, actualX, actualY);
	Point* p = new Point(actualX, actualY, color);	// Creates the Point with the specified coordinates and color
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
 */
void CartesianCanvas::drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
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
 * 		a, the alpha component
 */
void CartesianCanvas::drawLineColor(Decimal x1, Decimal y1, Decimal x2, Decimal y2, RGBfloatType color) {
	int actualX1, actualY1, actualX2, actualY2;
	getScreenCoordinates(x1, y1,actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	Line* l = new Line(actualX1, actualY1, actualX2, actualY2, color);	// Creates the Line with the specified coordinates and color
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
 */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h) {
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
 * 		a, the alpha component
 */
void CartesianCanvas::drawRectangleColor(Decimal x, Decimal y, Decimal w, Decimal h, RGBfloatType color) {
	int actualX, actualY, actualW, actualH;
	getScreenCoordinates(x, y, actualX, actualY);
	getScreenCoordinates(w, h, actualW, actualH);
	Rectangle* rec = new Rectangle(x, y, w, h, color);	// Creates the Rectangle with the specified coordinates and color
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
 * 		a, the alpha component
 */
void CartesianCanvas::drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color) {
	int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
	getScreenCoordinates(x1, y1, actualX1, actualY1);
	getScreenCoordinates(x2, y2, actualX2, actualY2);
	getScreenCoordinates(x3, y3, actualX3, actualY3);
	Triangle* t = new Triangle(actualX1, actualY1, actualX2, actualY2,
								actualX3, actualY3, color);			// Creates the Triangle with the specified vertices and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(t);												// Push it onto our drawing queue
	mlock.unlock();
}

/*
 * drawShinyPolygon draws a ShinyPolygon with the given vertex data
 * Parameters:
 * 		size, the number of vertices in the polygon
 * 		x, an array of x positions of the vertices
 * 		y, an array of y positions of the vertices
 * 		r, an array of red components for the vertices
 * 		g, an array of green components for the vertices
 * 		b, an array of blue components for the vertices
 * 		a, an array of alpha components for the vertices
 */
void CartesianCanvas::drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]) {
	int actualX, actualY;
	ShinyPolygon* p = new ShinyPolygon(size);
	for (int i = 0; i < size; i++) {
		getScreenCoordinates(x[i], y[i], actualX, actualY);
		p->addVertex(actualX, actualY, color[i]);
	}
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);										// Push it onto our drawing queue
	mlock.unlock();
}

const Function* CartesianCanvas::drawFunction(const Function* f) {
	int screenX = 0, screenY = 0;
	Polyline *p = new Polyline(1 + (maxX-minX) / pixelWidth);
	for (Decimal x = minX; x <= maxX; x += pixelWidth) {
		getScreenCoordinates(x, f->valueAt(x), screenX, screenY);
		std::unique_lock<std::mutex> mlock(mutex);
		p->addVertex(screenX, screenY);
		mlock.unlock();
	}
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);										// Push it onto our drawing queue
	mlock.unlock();

	return f;
}

#endif /* CARTESIANCANVAS_H_- */
