/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/11/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include "FL/Fl.H"					// For using basic FLTK library functions
#include <FL/Fl_Double_Window.H>	// For Fl_Double_Window, which draws our window
#include <FL/Fl_Box.H>				// For Fl_Box, from which our Canvas inherits
#include <FL/fl_draw.H>				// For FLTK's drawing function, which we implement to make our own thread-safe version.
#include "Point.h"					// Our own class for drawing single points.
#include "Line.h"					// Our own class for drawing straight lines.
#include "Rectangle.h"				// Our own class for drawing rectangles.
#include <omp.h>					// For OpenMP support
#include "Array.h"					// Our own thread-safe array for buffering drawing operations.
#include <cmath>					// For converting HSV to RGB
#include <chrono>					// For timing drawing and FPS
#include <iostream>

#define FPS 60
#define FRAME 1.0f/FPS		// Represents a single frame

typedef struct {float R, G, B;} RGBType;
typedef struct {float H, S, V;} HSVType;
typedef std::chrono::high_resolution_clock highResClock;

class Canvas : public Fl_Box {
	typedef void (*fcall)(Canvas* const);	// Define a type for our callback function pointer
private:
	fcall updateFunc;						// User-defined callback function for drawing
protected:
//	List<Shape*> * myShapes;		// Our buffer of shapes to draw
	Array<Shape*> * myShapes;
	int counter;					// Counter for the number of frames that have elapsed in the current session (for animations)
	int monitorX,monitorY,monitorWidth,monitorHeight;  	//Positioning and sizing data for the Canvas
	int colorR, colorG, colorB; 	// Our current global RGB drawing color
	int drawBufferSize;				// Maximum allowed Shapes in our drawing List
	Fl_Double_Window* window;		// The FLTK window to which we draw
	bool started;					// Whether our canvas is running and the counter is counting
	bool autoRefresh;				// Whether or not we automatically refr/ (double)(CLOCKS_PER_SEC / 100)esh the Canvas each frame
	void init(int xx, int yy, int ww, int hh, unsigned int b);				// Method for initializing the canvas
	void draw();													// Method for drawing the canvas and the shapes within
	virtual void callUpdate();										// Actually calls updateFunc (needed to avoid typing errors)
	inline static void Canvas_Callback(void* userdata);				// Callback so that the canvas redraws periodically
	float realFPS;													// Actual FPS of drawing
	highResClock::time_point cycleTime, startTime;
public:
	Canvas(fcall c, unsigned int b);											// Default constructor for our Canvas
	Canvas(fcall c, int xx, int yy, int w, int h, unsigned int b, char* t);	// Explicit constructor for our Canvas
	int start();													// Function to start rendering our Canvas
	int end();														// Function to end rendering our Canvas
	void setColor(int r, int g, int b);								// Sets the global drawing color
	void setAutoRefresh(bool b);									// Sets whether we automatically refresh the Canvas
	virtual Point drawPoint(int x, int y);							// Draws a point at the given coordinates
	virtual Point drawPointColor(int x, int y, int r, int g, int b);// Draws a point at the given coordinates with the given color
	virtual Line drawLine(int x1, int y1, int x2, int y2);			// Draws a line at the given coordinates
	virtual Line drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b);		// Draws a line at the given coordinates with the given color
	virtual Rectangle drawRectangle(int x, int y, int w, int h);							// Draws a rectangle at the given coordinates with the given dimensions
	virtual Rectangle drawRectangleColor(int x, int y, int w, int h, int r, int g, int b);	// Draws a rectangle at the given coordinates with the given dimensions and color
	virtual void drawText(const char * s, int x, int y);									// Draws a string of text at the given position
	int getWindowX() { return monitorX; }						// Accessor for the window width
	int getWindowY() { return monitorY; }						// Accessor for the window height
	int getWindowWidth() { return monitorWidth; }				// Accessor for the window width
	int getWindowHeight() { return monitorHeight; }				// Accessor for the window height
	int getColorR() { return colorR; }							// Accessor for  the red component of the global drawing color
	int getColorG() { return colorG; }							// Accessor for  the green component of the global drawing color
	int getColorB() { return colorB; }							// Accessor for  the number component of the global drawing color
	int getFrameNumber() { return counter; }					// Accessor for the number of frames rendered so far
	int getBufferSize() {return drawBufferSize; }				// Accessor for the Shape list's buffer size
	float getFPS() { return realFPS; }							// Accessor for true FPS
	double getTime();											// Returns the time since initialization

	static RGBType HSVtoRGB(HSVType HSV);
};

/*
 * init initialized the Canvas with the values specified in the constructor
 * Parameters:
 * 		c, a callback to the user's own draw function
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		width, the x dimension of the Canvas window
 * 		height, the y dimension of the Canvas window
 * 		b, the buffer size for the Shapes (-1 = no limit)
 */
void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b) {
	started = false;  					// We haven't started the window yet
	counter = 0;						// We haven't drawn any frames yet
	autoRefresh = true;					// Default to clearing the queue every frame
	startTime = highResClock::now();	// Record the init time
	monitorX = xx; monitorY = yy; monitorWidth = ww; monitorHeight = hh;  // Initialize translation
//	myShapes = new List<Shape*>(b);							// Initialize myShapes
	myShapes = new Array<Shape*>(b);
	box(FL_FLAT_BOX);  										// Sets the box we will draw to (the only one)
	setColor(0,0,0);										// Our default global drawing color is black
	Fl::add_timeout(FRAME, Canvas_Callback, (void*)this);  	// Adds a callback after 1/60 second to the Canvas' callback function
}

/*
 * draw actually draws our Canvas and its contents to the display
 * Note: this function is called automatically by the callback and the FLTK redraw function, which is why it's private
 */
void Canvas::draw() {
	// Calculate CycleTime since draw() was last called
	highResClock::time_point end = highResClock::now();
	realFPS = 1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - cycleTime).count() * 1000000000.0;
	realFPS = realFPS-(int)realFPS > .5 ? (int)realFPS + 1 : (int)realFPS;
	cycleTime = end;

	std::cout << realFPS << '/' << FPS << std::endl;

	counter++;				// Increment the frame counter
	callUpdate();			// Call the user's callback to do work on the Canvas
	int oldR = colorR;		// Temporary variables for the initial global drawing color
	int oldG = colorG;
	int oldB = colorB;
	Shape* s;				// Pointer to the next Shape in the queue

	// Iterate through our queue until we've made it to the end
//	for (List<Shape*>::Iterator iterator = myShapes->begin(); iterator != myShapes->end();iterator++) {
	for (unsigned int i = 0; i < myShapes->size(); i++) {
//		s = *iterator;		// Get the next item
		s = myShapes->operator[](i);
		if (s->getUsesDefaultColor()) {
			s->draw();		// If our shape uses the default color, just draw it
		}
		else {				// Otherwise, the color must be set before and after drawing
			setColor(s->getColorR(),s->getColorG(),s->getColorB());
			s->draw();
			setColor(oldR, oldG, oldB);
		}
	}
	if (autoRefresh) {
		myShapes->clear();
	}
}

/*
 * callUpdate simply calls the update function for the Canvas class
 */
void Canvas::callUpdate() {
	updateFunc(this);		// Call the user's callback to do work on the Canvas
}

/*
 * Canvas_Callback is a callback function called (by default) every 1/60 of a second (update cycle) to redraw the canvas
 * Parameters:
 * 		userdata, a pointer to the Canvas class that we're calling back
 */
void Canvas::Canvas_Callback(void* userdata) {
	Canvas *o = (Canvas*)userdata;  						// Casts the userdata pointer as a Canvas pointer
    o->redraw();  											// Redraw the canvas
    Fl::repeat_timeout(FRAME, Canvas_Callback, userdata);  	// Restart the fcall
}

/*
 * Default constructor for the canvas class
 * Parameter:
 * 		c, a callback to the user's own draw function
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 Canvas on the topleft of the screen with no title
 */
Canvas::Canvas(fcall c, unsigned int b) : Fl_Box (0,0,800,600) {
	if (c == NULL) {
		updateFunc = [](Canvas* c){};			// Empty lambda function that does nothing
	} else
		updateFunc = c;							// Adds a callback to the user's own draw function
	init(0,0,800,600,b);
}

/*
 * Explicit constructor for the canvas class
 * Parameters:
 * 		c, a callback to the user's own draw function
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		w, the x dimension of the Canvas window
 * 		h, the y dimension of the Canvas window
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * 		t, the title of the window
 * Returns: a new Canvas with the specified positional data and title
 */
Canvas::Canvas(fcall c, int xx, int yy, int w, int h, unsigned int b, char* t = 0) : Fl_Box(xx,yy,w,h,t) {
	if (c == NULL) {
		updateFunc = [](Canvas* c){};			// Empty lambda function that does nothing
	} else
		updateFunc = c;							// Adds a callback to the user's own draw function
	init(xx,yy,w,h,b);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started)												// If we're already started, return error code -1
		return -1;
	//Fl::gl_visual(FL_ALPHA);
	started = true;												// We've now started
    window = new Fl_Double_Window(monitorWidth,monitorHeight);	// Instantiate our drawing window
    window->add(this);											// Add ourself (Canvas) to the drawing window
    window->show();												// Show the window
    return(Fl::run());
}

/*
 * start starts rendering the Canvas
 * Returns: 0 if exit is successful, -1 otherwise
 */
int Canvas::end() {
	if (!started)						// If we haven't even started yet, return error code -1
		return -1;
	window->hide();						// Hide our window
	delete window;						// Delete our window from the heap
	return 0;
}

/*
 * setColor sets the global drawing color
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 */
void Canvas::setColor(int r, int g, int b) {
	colorR = r;
	colorG = g;
	colorB = b;
	#pragma omp critical
	fl_color(r,g,b);	// Updates the underlying FLTK color (critical to avoid syncing problems)
}

/*
 * setAutoRefresh sets the automatic flushing of the queue on or off
 * Parameter:
 * 		b, whether or not to automatically refresh (flush the queue)
 */
void Canvas::setAutoRefresh(bool b) {
	autoRefresh = b;
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the given position
 */
Point Canvas::drawPoint(int x, int y) {
	Point* p = new Point(x,y);				// Creates the Point with the specified coordinates
	myShapes->push(p);						// Push it onto our drawing queue
	return *p;								// Return a pointer to our new Point
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new point at the given position with the specified color
 */
Point Canvas::drawPointColor(int x, int y, int r, int g, int b) {
	Point* p = new Point(x,y,r,g,b);		// Creates the Point with the specified coordinates and color
	myShapes->push(p);						// Push it onto our drawing queue
	return *p;								// Return a pointer to our new Point
}

/*
 * drawLine draws a line at the given coordinates
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 	Returns: a new line with the given coordinates
 */
Line Canvas::drawLine(int x1, int y1, int x2, int y2) {
	Line* l = new Line(x1,y1,x2,y2);		// Creates the Line with the specified coordinates
	myShapes->push(l);						// Push it onto our drawing queue
	return *l;								// Return a pointer to our new Line
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
 * 	Returns: a new line with the given coordinates and the specified color
 */
Line Canvas::drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b) {
	Line* l = new Line(x1,y1,x2,y2,r,g,b);	// Creates the Line with the specified coordinates and color
	myShapes->push(l);						// Push it onto our drawing queue
	return *l;								// Return a pointer to our new Line
}

/*
 * drawRectangle draws a rectangle with the given coordinates and dimensions
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 	Returns: a new rectangle with the given coordinates and dimensions
 */
Rectangle Canvas::drawRectangle(int x, int y, int w, int h) {
	Rectangle* rec = new Rectangle(x,y,w,h);	// Creates the Rectangle with the specified coordinates
	myShapes->push(rec);						// Push it onto our drawing queue
	return *rec;								// Return a pointer to our new Rectangle
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
 * 	Returns: a new rectangle with the given coordinates, dimensions, and color
 */
Rectangle Canvas::drawRectangleColor(int x, int y, int w, int h, int r, int g, int b) {
	Rectangle* rec = new Rectangle(x,y,w,h,r,g,b);	// Creates the Rectangle with the specified coordinates and color
	myShapes->push(rec);							// Push it onto our drawing queue
	return *rec;									// Return a pointer to our new Rectangle
}

/*
 * drawText() prints text at the given coordinates
 * Parameters:
 * 		s, the char* to print
 * 		x, the x coordinate of the text's left edge
 * 		y, the y coordinate of the text's top edge
 */
void Canvas::drawText(const char * s, int x, int y) {
	fl_draw(s,x,y);
}

double Canvas::getTime() {
	return std::chrono::duration_cast<std::chrono::microseconds>(highResClock::now() - startTime).count() / 1000000.0;
}

RGBType Canvas::HSVtoRGB(HSVType HSV) {
 	// H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1].
	// RGB are each returned on [0, 1].
	float h = HSV.H, s = HSV.S, v = HSV.V, m, n, f;
	int i;
	RGBType RGB;

	if (h == -1) {
		RGB.R = v;
		RGB.G = v;
		RGB.B = v;
		return RGB;
	}
	i = floor(h);
	f = h - i;
	if ( !(i&1) ) f = 1 - f; 		// if i is even
	m = v * (1 - s);
	n = v * (1 - s * f);
	switch (i) {
		case 6:
		case 0:
			RGB.R = v;
			RGB.G = n;
			RGB.B = m;
			return RGB;
		case 1:
			RGB.R = n;
			RGB.G = v;
			RGB.B = m;
			return RGB;
		case 2:
			RGB.R = m;
			RGB.G = v;
			RGB.B = n;
			return RGB;
		case 3:
			RGB.R = m;
			RGB.G = n;
			RGB.B = v;
			return RGB;
		case 4:
			RGB.R = n;
			RGB.G = m;
			RGB.B = v;
			return RGB;
		case 5:
			RGB.R = v;
			RGB.G = m;
			RGB.B = n;
			return RGB;
		default:
			throw std::out_of_range("Bad H value");
	}
}

#endif /* CANVAS_H_ */
