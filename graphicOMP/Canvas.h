/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/12/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <FL/Fl.H>						// For using basic FLTK library functions
#include <FL/Fl_Double_Window.H>	// For Fl_Double_Window, which draws our window
#include <FL/Fl_Box.H>				// For Fl_Box, from which our Canvas inherits
#include <FL/fl_draw.H>				// For FLTK's drawing function, which we implement to make our own thread-safe version.
#include "Point.h"					// Our own class for drawing single points.
#include "Line.h"					// Our own class for drawing straight lines.
#include "Rectangle.h"				// Our own class for drawing rectangles.
#include "Triangle.h"				// Our own class for drawing triangles.
#include "Array.h"					// Our own thread-safe array for buffering drawing operations.
#include <omp.h>					// For OpenMP support
#include <cmath>					// For converting HSV to RGB
#include <chrono>					// For timing drawing and FPS
#include <thread>					// For spawning rendering in a different thread
#include <mutex>					// Needed for locking the Canvas for thread-safety
#include <iostream> //DEBUGGING

#define FPS 60
#define FRAME 1.0f/FPS				// Represents a single frame

typedef struct {float R, G, B;} RGBType;
typedef struct {float H, S, V;} HSVType;
typedef std::chrono::high_resolution_clock highResClock;

class Canvas : public Fl_Box {
protected:
	Array<Shape*> * myShapes;										// Our buffer of shapes to draw
	int counter;													// Counter for the number of frames that have elapsed in the current session (for animations)
	int monitorX,monitorY,monitorWidth,monitorHeight;  				// Positioning and sizing data for the Canvas
	int defaultRed, defaultGreen, defualtBlue; 						// Our current global RGB drawing color
	int drawBufferSize;												// Maximum allowed Shapes in our drawing List
	Fl_Double_Window* window;										// The FLTK window to which we draw
	bool started;													// Whether our canvas is running and the frame counter is counting
	bool autoRefresh;												// Whether or not we automatically refresh the Canvas each frame
	std::thread renderThread;										// Thread dedicated to rendering the Canvas
	std::mutex mutex;												// Mutex for locking the Canvas so that only one thread can read/write at a time
	void init(int xx, int yy, int ww, int hh, unsigned int b);		// Method for initializing the canvas
	void draw();													// Method for drawing the canvas and the shapes within
	inline static void Canvas_Callback(void* userdata);				// Callback so that the canvas redraws periodically
	float realFPS;													// Actual FPS of drawing
	bool showFPS_;													// Flag to show DEBUGGING FPS
	highResClock::time_point cycleTime, startTime;					// Times to show FPS and render time
public:
	Canvas(unsigned int b);											// Default constructor for our Canvas
	Canvas(int xx, int yy, int w, int h, unsigned int b, char* t);	// Explicit constructor for our Canvas
	int start();													// Function to start rendering our Canvas
	int end();														// Function to end rendering our Canvas
	void setColor(int r, int g, int b);								// Sets the global drawing color
	void setAutoRefresh(bool b);									// Sets whether we automatically refresh the Canvas
	virtual void drawPoint(int x, int y);							// Draws a point at the given coordinates
	virtual void drawPointColor(int x, int y, int r, int g, int b);// Draws a point at the given coordinates with the given color
	virtual void drawLine(int x1, int y1, int x2, int y2);			// Draws a line at the given coordinates
	virtual void drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b);		// Draws a line at the given coordinates with the given color
	virtual void drawRectangle(int x, int y, int w, int h);							// Draws a rectangle at the given coordinates with the given dimensions
	virtual void drawRectangleColor(int x, int y, int w, int h, int r, int g, int b);	// Draws a rectangle at the given coordinates with the given dimensions and color
	virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);			// Draws a triangle with the given vertices
	virtual void drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3,
			int r, int g, int b);															// Draws a triangle with the given vertices and color
	virtual void drawText(const char * s, int x, int y);									// Draws a string of text at the given position
	int getWindowX() 		{ return monitorX; }					// Accessor for the window width
	int getWindowY() 		{ return monitorY; }					// Accessor for the window height
	int getWindowWidth() 	{ return monitorWidth; }				// Accessor for the window width
	int getWindowHeight() 	{ return monitorHeight; }				// Accessor for the window height
	int getColorR() 		{ return defaultRed; }					// Accessor for the red component of the global drawing color
	int getColorG() 		{ return defaultGreen; }				// Accessor for the green component of the global drawing color
	int getColorB() 		{ return defualtBlue; }					// Accessor for the number component of the global drawing color
	int getFrameNumber() 	{ return counter; }						// Accessor for the number of frames rendered so far
	int getBufferSize() 	{ return drawBufferSize; }				// Accessor for the Shape list's buffer size
	float getFPS() 			{ return realFPS; }						// Accessor for true FPS
	bool isOpen() 			{ return window->visible(); }			// Returns if the window is visible, which would mean it's closed
	void showFPS(bool b) 	{ showFPS_ = b; }						// Mutator to show debugging FPS
	double getTime();												// Returns the time since initialization
	static RGBType HSVtoRGB(HSVType HSV);
};

// Clean up if someone closes the window
void close_cb(Fl_Widget* w, void* v) {
	Fl_Double_Window* d = (Fl_Double_Window*) v;
	d->hide();										// Hide our window
}

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
	Fl::gl_visual(FL_ALPHA);								// Turns alpha-rendering on
	started = false;  										// We haven't started the window yet
	counter = 0;											// We haven't drawn any frames yet
	autoRefresh = true;										// Default to clearing the queue every frame
	startTime = highResClock::now();						// Record the init time
	monitorX = xx; monitorY = yy; monitorWidth = ww; monitorHeight = hh;  // Initialize translation
	myShapes = new Array<Shape*>(b);						// Initialize myShapes
	box(FL_FLAT_BOX);  										// Sets the box we will draw to (the only one)
	setColor(0,0,0);										// Our default global drawing color is black
	Fl::add_timeout(FRAME, Canvas_Callback, (void*)this);  	// Adds a callback after 1/60 second to the Canvas' callback function
	showFPS_ = false;										// Set debugging FPS to false
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

	if (showFPS_) std::cout << realFPS << '/' << FPS << std::endl;

	counter++;				// Increment the frame counter
	Shape* s;				// Pointer to the next Shape in the queue

	std::unique_lock<std::mutex> mlock(mutex);
	// Iterate through our queue until we've made it to the end
	for (unsigned int i = 0; i < myShapes->size(); i++) {
		s = myShapes->operator[](i);
		if (s->getUsesDefaultColor()) {
			s->draw();		// If our shape uses the default color, just draw it
		}
		else {				// Otherwise, the color must be set before and after drawing
			fl_color(s->getColorR(), s->getColorG(), s->getColorB());
			s->draw();
			fl_color(defaultRed, defaultGreen, defualtBlue);
		}
	}
	if (autoRefresh) {
		myShapes->clear();
	}
	mlock.unlock();
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
Canvas::Canvas(unsigned int b) : Fl_Box (0,0,800,600) {
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
Canvas::Canvas(int xx, int yy, int w, int h, unsigned int b, char* t = 0) : Fl_Box(xx,yy,w,h,t) {
	init(xx,yy,w,h,b);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started) return -1;										// If we're already started, return error code -1
	started = true;												// We've now started
    window = new Fl_Double_Window(monitorWidth,monitorHeight);	// Instantiate our drawing window
    window->add(this);											// Add ourself (Canvas) to the drawing window
	window->callback(close_cb,window);							// Add the function to call when the window is closed
    window->show();												// Show the window
    renderThread = std::thread(Fl::run);						// Spawn the rendering thread
    return 0;
}

/*
 * start starts rendering the Canvas
 * Returns: 0 if exit is successful, -1 otherwise
 */
int Canvas::end() {
	if (!started) return -1;						// If we haven't even started yet, return error code -1
	renderThread.join();							// Blocks until ready to join, which will be when the window is closed
	delete window;									// Delete our window from the heap
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
	std::unique_lock<std::mutex> mlock(mutex);
	defaultRed = r;
	defaultGreen = g;
	defualtBlue = b;
	fl_color(r,g,b);	// Updates the underlying FLTK color (critical to avoid syncing problems)
	mlock.unlock();
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
void Canvas::drawPoint(int x, int y) {
	Point* p = new Point(x,y);				// Creates the Point with the specified coordinates
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);						// Push it onto our drawing queue
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
 * 	Returns: a new point at the given position with the specified color
 */
void Canvas::drawPointColor(int x, int y, int r, int g, int b) {
	Point* p = new Point(x,y,r,g,b);		// Creates the Point with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(p);						// Push it onto our drawing queue
	mlock.unlock();
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
void Canvas::drawLine(int x1, int y1, int x2, int y2) {
	Line* l = new Line(x1,y1,x2,y2);		// Creates the Line with the specified coordinates
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(l);						// Push it onto our drawing queue
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
 * 	Returns: a new line with the given coordinates and the specified color
 */
void Canvas::drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b) {
	Line* l = new Line(x1,y1,x2,y2,r,g,b);	// Creates the Line with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(l);						// Push it onto our drawing queue
	mlock.unlock();
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
void Canvas::drawRectangle(int x, int y, int w, int h) {
	Rectangle* rec = new Rectangle(x,y,w,h);	// Creates the Rectangle with the specified coordinates
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
 * 	Returns: a new rectangle with the given coordinates, dimensions, and color
 */
void Canvas::drawRectangleColor(int x, int y, int w, int h, int r, int g, int b) {
	Rectangle* rec = new Rectangle(x,y,w,h,r,g,b);	// Creates the Rectangle with the specified coordinates and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(rec);							// Push it onto our drawing queue
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
 * 	Returns: a new triangle with the given vertices
 */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	Triangle* t = new Triangle(x1,y1,x2,y2,x3,y3);	// Creates the Triangle with the specified vertices
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
 * 	Returns: a new triangle with the given vertices and color
 */
void Canvas::drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b) {
	Triangle* t = new Triangle(x1,y1,x2,y2,x3,y3,r,g,b);	// Creates the Triangle with the specified vertices and color
	std::unique_lock<std::mutex> mlock(mutex);
	myShapes->push(t);										// Push it onto our drawing queue
	mlock.unlock();
}

/*
 * drawText prints text at the given coordinates
 * Parameters:
 * 		s, the char* to print
 * 		x, the x coordinate of the text's left edge
 * 		y, the y coordinate of the text's top edge
 */
void Canvas::drawText(const char * s, int x, int y) {
	fl_draw(s,x,y);
}

/*
 * getTime returns the time elapsed since the Canvas has started drawing (in nanoseconds)
 */
double Canvas::getTime() {
	std::unique_lock<std::mutex> mlock(mutex);
	double time = std::chrono::duration_cast<std::chrono::microseconds>(highResClock::now() - startTime).count() / 1000000.0;
	mlock.unlock();
	return time;
}

/*
 * HSVtoRGB converts a triplet of HSV colors to a triplet of RGB colors
 * Parameter:
 * 		HSV, a struct of 3 floats representing HSV color data
 * Returns: a struct of 3 floats representing the RGB equivalent of the HSV color
 */
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
