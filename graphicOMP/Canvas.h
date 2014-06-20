/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/20/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <FL/Fl.H>					// For using basic FLTK library functions
#include <FL/Fl_Window.H>			// For inheriting Fl_Window, which draws our window
#include <FL/gl.h>					// For FLTK's built-in OpenGL functions
#include "Point.h"					// Our own class for drawing single points.
#include "Line.h"					// Our own class for drawing straight lines.
#include "Rectangle.h"				// Our own class for drawing rectangles.
#include "Triangle.h"				// Our own class for drawing triangles.
#include "ShinyPolygon.h"			// Our own class for drawing polygons with colored vertices.
#include "Polyline.h"				// Our own class for drawing polylines.
#include "Text.h"					// Our own class for drawing text.
#include "Array.h"					// Our own array for buffering drawing operations.
#include "color.h"					// Our own interface for converting color types
#include <omp.h>					// For OpenMP support
#include <cmath>					// For converting HSV to RGB and other math stuff
#include <chrono>					// For timing drawing and FPS
#include <thread>					// For spawning rendering in a different thread
#include <mutex>					// Needed for locking the Canvas for thread-safety
#include <iostream> 				// DEBUGGING

#define FPS 60
#define FRAME 1.0f/FPS				// Represents a single frame

typedef std::chrono::high_resolution_clock highResClock;
typedef std::unique_lock<std::mutex> mutexLock;

class Canvas : public Fl_Window {
protected:
	Array<Shape*> * myShapes;										// Our buffer of shapes to draw
	Array<Shape*> * myBuffer;										// Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
	int counter;													// Counter for the number of frames that have elapsed in the current session (for animations)
	int monitorX,monitorY,monitorWidth,monitorHeight;  				// Positioning and sizing data for the Canvas
	RGBfloatType defaultColor; 										// Our current global RGB drawing color
	RGBfloatType backgroundColor;									// The background color
	bool toClear;													// Flag for clearing the canvas
	bool started;													// Whether our canvas is running and the frame counter is counting
	std::thread renderThread;										// Thread dedicated to rendering the Canvas
	std::mutex shapes, buffer;										// Mutexes for locking the Canvas so that only one thread can read/write at a time
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
	void setColor(float r, float g, float b, float a);				// Sets the global drawing color
	virtual void drawPoint(int x, int y);							// Draws a point at the given coordinates
	virtual void drawPointColor(int x, int y, RGBfloatType color);	// Draws a point at the given coordinates with the given color
	virtual void drawLine(int x1, int y1, int x2, int y2);								// Draws a line at the given coordinates
	virtual void drawLineColor(int x1, int y1, int x2, int y2, RGBfloatType color);		// Draws a line at the given coordinates with the given color
	virtual void drawRectangle(int x, int y, int w, int h);								// Draws a rectangle at the given coordinates with the given dimensions
	virtual void drawRectangleColor(int x, int y, int w, int h, RGBfloatType color);	// Draws a rectangle at the given coordinates with the given dimensions and color
	virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);			// Draws a triangle with the given vertices
	virtual void drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3,
										RGBfloatType color);							// Draws a triangle with the given vertices and color
	virtual void drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]);	// Draws a polygon of with given number of vertices with shading across it
	virtual void drawText(const char * s, int x, int y);								// Draws a string of text at the given position
	virtual void drawTextColor(const char * s, int x, int y, RGBfloatType color);		// Draws a string of text at the given position, with the given color
	void setBackgroundColor(RGBfloatType color);					// Changes the background color
	void clear();													// Clears the canvas
	int getWindowX() 		{ return monitorX; }					// Accessor for the window width
	int getWindowY() 		{ return monitorY; }					// Accessor for the window height
	int getWindowWidth() 	{ return monitorWidth; }				// Accessor for the window width
	int getWindowHeight() 	{ return monitorHeight; }				// Accessor for the window height
	RGBfloatType getColor()	{ return defaultColor; }				// Accessor for the global drawing color
	float getColorR() 		{ return defaultColor.R; }				// Accessor for the red component of the global drawing color
	float getColorG() 		{ return defaultColor.G; }				// Accessor for the green component of the global drawing color
	float getColorB() 		{ return defaultColor.B; }				// Accessor for the blue component of the global drawing color
	float getColorA() 		{ return defaultColor.A; }				// Accessor for the alpha component of the global drawing color
	int getFrameNumber() 	{ return counter; }						// Accessor for the number of frames rendered so far
	float getFPS() 			{ return realFPS; }						// Accessor for true FPS
	bool isOpen() 			{ return visible(); }					// Returns if the window is visible, which would mean it's not closed
	void showFPS(bool b) 	{ showFPS_ = b; }						// Mutator to show debugging FPS
	double getTime();												// Returns the time since initialization
};

//// Clean up if someone closes the window
//void close_cb(Fl_Widget* w, void* v) {
//	OmpWindow* d = (OmpWindow*) v;
//	d->hide();										// Hide our window
//}

/*
 * init initializes the Canvas with the values specified in the constructor
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
//	glutInitDisplayMode (GLUT_DOUBLE);
	glDisable(GL_DEPTH_TEST);								// Turn off 3D depth-testing
	glDisable(GL_POINT_SMOOTH);
	gl_font(FL_HELVETICA, 12);
	backgroundColor = {0.75f, 0.75f, 0.75f};				// Set the default background color
	toClear = false;										// Don't need to clear at the start
	started = false;  										// We haven't started the window yet
	counter = 0;											// We haven't drawn any frames yet
	startTime = highResClock::now();						// Record the init time
	monitorX = xx; monitorY = yy; monitorWidth = ww; monitorHeight = hh;  // Initialize translation
	myShapes = new Array<Shape*>(b);						// Initialize myShapes
	myBuffer = new Array<Shape*>(b);
	box(FL_FLAT_BOX);  										// Sets the box we will draw to (the only one)
	setColor(0, 0, 0, 255);										// Our default global drawing color is black
	Fl::add_timeout(FRAME, Canvas_Callback, (void*)this);  	// Adds a callback after 1/60 second to the Canvas' callback function
	showFPS_ = false;										// Set debugging FPS to false
}

/*
 * draw actually draws our Canvas and its contents to the display
 * Note: this function is called automatically by the callback and the FLTK redraw function, which is why it's private
 */
void Canvas::draw() {
	gl_start();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w() - (1.0f/w()), h(), 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, 0.0);		// Set the background
	if (toClear) {
		glClear(GL_COLOR_BUFFER_BIT);
		toClear = false;
	}

	// Calculate CycleTime since draw() was last called
	highResClock::time_point end = highResClock::now();
	realFPS = round(1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - cycleTime).count() * 1000000000.0);
	cycleTime = end;

	if (showFPS_) std::cout << realFPS << '/' << FPS << std::endl;

	counter++;				// Increment the frame counter
	Shape* s;				// Pointer to the next Shape in the queue

	mutexLock mBufferLock(buffer);						// Time to flush our buffer
	if (myBuffer->size() > 0) {							// But only if there is anything to flush
		for (unsigned int i = 0; i < myBuffer->size(); i++) {
			myShapes->push(myBuffer->operator[](i));
		}
		myBuffer->shallowClear();						// We want to clear the buffer but not delete those objects as we still need to draw them
	}
	mBufferLock.unlock();

	gl_draw(" ",-100,100);								// OpenGl likes drawing the first string with a ? prepended, so get that out of the way

	if (myShapes->size() == 0) {						// If there is nothing to render...
		glBegin(GL_POINTS);								// OpenGL won't keep our drawings unless we pretend
		glVertex2f(-1, -1);								// 	to render stuff
		glEnd();1064
	} else {
		bool pointList = false;
		// Iterate through our queue until we've made it to the end
		for (unsigned int i = 0; i < myShapes->size(); i++) {
			s = myShapes->operator[](i);
			if (!pointList && s->getIsPoint()) {			// Check if we are going to render a point
				pointList = true;
				glBegin(GL_POINTS);							// Start drawing in point mode
			}
			else if (pointList && !s->getIsPoint()) {		// If we have been drawing points and now won't be...
				pointList = false;
				glEnd();									// Stop drawing in point mode
			}

			if (s->getUsesDefaultColor()) {
				glColor4f(defaultColor.R, defaultColor.G, defaultColor.B, defaultColor.A);
				s->draw();									// If our shape uses the default color, just draw it
			} else {										// Otherwise, the color must be got from the shape
				glColor4f(s->getColorR(),s->getColorG(),s->getColorB(),s->getColorA());
				s->draw();
			}
		}
		if (pointList)										// If we didn't finish drawing points...
			glEnd();										// Stop drawing in point mode
		myShapes->clear();									// Clear the buffer of shapes to be drawn
	}

	gl_finish();
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
Canvas::Canvas(unsigned int b) : Fl_Window (0,0,800,600) {
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
Canvas::Canvas(int xx, int yy, int w, int h, unsigned int b, char* t = 0) : Fl_Window(xx,yy,w,h,t) {
	init(xx,yy,w,h,b);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started) return -1;										// If we're already started, return error code -1
	started = true;												// We've now started
//	callback(close_cb,window);									// Add the function to call when the window is closed
    show();														// Show the window
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
	return 0;
}

/*
 * setColor sets the global drawing color
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 		a, the alpha component
 */
void Canvas::setColor(float r, float g, float b, float a) {
	defaultColor.R = r;
	defaultColor.G = g;
	defaultColor.B = b;
	defaultColor.A = a;
}

/*
 * setBackgroundColor sets the background color
 * Parameters:
 * 		color, the RGBfloatType with the color. The alpha channel is ignored
 */
void Canvas::setBackgroundColor(RGBfloatType color) {
	backgroundColor.R = color.R;
	backgroundColor.G = color.R;
	backgroundColor.B = color.R;
}

/*
 * clear removes all shapes and sets the background to the set color
 */
void Canvas::clear() {
	toClear = true;
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 */
void Canvas::drawPoint(int x, int y) {
	Point* p = new Point(x,y);						// Creates the Point with the specified coordinates
	mutexLock mlock(buffer);
	myBuffer->push(p);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		color, the color with which to draw the text
 */
void Canvas::drawPointColor(int x, int y, RGBfloatType color) {
	Point* p = new Point(x,y,color);				// Creates the Point with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(p);								// Push it onto our drawing buffer
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
void Canvas::drawLine(int x1, int y1, int x2, int y2) {
	Line* l = new Line(x1,y1,x2,y2);				// Creates the Line with the specified coordinates
	mutexLock mlock(buffer);
	myBuffer->push(l);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawLineColor draws a line at the given coordinates with the given color
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 		color, the color with which to draw the text
 */
void Canvas::drawLineColor(int x1, int y1, int x2, int y2, RGBfloatType color) {
	Line* l = new Line(x1,y1,x2,y2,color);			// Creates the Line with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(l);								// Push it onto our drawing buffer
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
void Canvas::drawRectangle(int x, int y, int w, int h) {
	Rectangle* rec = new Rectangle(x,y,w,h);		// Creates the Rectangle with the specified coordinates
	mutexLock mlock(buffer);
	myBuffer->push(rec);							// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawRectangleColor draws a rectangle with the given coordinates, dimensions, and color
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 		color, the color with which to draw the text
 */
void Canvas::drawRectangleColor(int x, int y, int w, int h, RGBfloatType color) {
	Rectangle* rec = new Rectangle(x,y,w,h,color);	// Creates the Rectangle with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(rec);							// Push it onto our drawing buffer
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
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	Triangle* t = new Triangle(x1,y1,x2,y2,x3,y3);	// Creates the Triangle with the specified vertices
	mutexLock mlock(buffer);
	myBuffer->push(t);								// Push it onto our drawing buffer
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
 * 		color, the color with which to draw the triangle
 */
void Canvas::drawTriangleColor(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color) {
	Triangle* t = new Triangle(x1,y1,x2,y2,x3,y3,color);	// Creates the Triangle with the specified vertices and color
	mutexLock mlock(buffer);
	myBuffer->push(t);										// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawShinyPolygon draws a ShinyPolygon with the given vertex data
 * Parameters:
 * 		size, the number of vertices in the polygon
 * 		x, an array of x positions of the vertices
 * 		y, an array of y positions of the vertices
 * 		color, an array of colors for the vertices
 */
void Canvas::drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]) {
	ShinyPolygon* p = new ShinyPolygon(size);
	for (int i = 0; i < size; i++) {
		p->addVertex(x[i],y[i],color[i]);
	}
	mutexLock mlock(buffer);
	myBuffer->push(p);										// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawText prints text at the given coordinates
 * Parameters:
 * 		s, the string to print
 * 		x, the x coordinate of the text's left edge
 * 		y, the y coordinate of the text's top edge
 */
void Canvas::drawText(const char * s, int x, int y) {
	Text* t = new Text(s,x,y);						// Creates the Text with the specified string and coordinates
	mutexLock mlock(buffer);
	myBuffer->push(t);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawTextColor prints text at the given coordinates with the given color
 * Parameters:
 * 		s, the string to print
 * 		x, the x coordinate of the text's left edge
 * 		y, the y coordinate of the text's top edge
 * 		color, the color with which to draw the text
 */
void Canvas::drawTextColor(const char * s, int x, int y, RGBfloatType color) {
	Text* t = new Text(s,x,y,color);					// Creates the Text with the specified string and coordinates
	mutexLock mlock(buffer);
	myBuffer->push(t);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * getTime returns the time elapsed since the Canvas has started drawing (in microseconds)
 */
double Canvas::getTime() {
	return std::chrono::duration_cast<std::chrono::microseconds>(highResClock::now() - startTime).count() / 1000000.0;
}

#endif /* CANVAS_H_ */
