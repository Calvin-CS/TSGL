// Canvas provides a window / canvas for all of the drawing operations in the graphicOMP library
//
// Authors: Patrick Crain, Mark Vander Stel, 6/6/2014

#ifndef CANVAS_H_
#define CANVAS_H_

#include "FL/Fl.H"					//For using basic FLTK library functions
#include <FL/Fl_Double_Window.H>	//For Fl_Double_Window, which draws our window
#include <FL/Fl_Box.H>				//For Fl_Box, from which our Canvas inherits
#include <FL/fl_draw.H>				//For FLTK's drawing function, which we implement to make our own thread-safe version.
#include "Point.h"					//Our own class for drawing single points.
#include "Line.h"					//Our own class for drawing straight lines.
#include <omp.h>					//For OpenMP support
#include "List.h"					//Our own doubly-linked list for buffering drawing operations in a thread-safe manner.

const double FRAME = 1.0f/60.0f;	//Represents a single frame (@ 60Hz)

class Canvas : public Fl_Box {
	typedef void (*fcall)(Canvas* const);	//Define a type for our callback function pointer
private:
	List<Shape*> * myShapes;		//Our buffer of shapes to draw
	int counter;					//Counter for the number of frames that have elapsed in the current session (for animations)
	int x,y,w,h;  					//Positioning and sizing data for the Canvas
	int colorR, colorG, colorB; 	//Our current global RGB drawing color
	int drawBufferSize;				//Maximum allowed Shapes in our drawing List
	Fl_Double_Window *window;		//The FLTK window to which we draw
	bool started;					//Whether our canvas is running and the counter is counting
	bool autoRefresh;				//Whether or not we automatically refresh the Canvas each frame
	fcall updateFunc;				//User-defined callback function for drawing
	inline void init(fcall c, int xx, int yy, int ww, int hh, int b);	//Method for initializing the canvas
	inline void draw();											//Method for drawing the canvas and the shapes within
	inline static void Canvas_Callback(void *userdata);			//Callback so that the canvas redraws periodically
public:
	inline Canvas(fcall c, int b);										//Default constructor for our Canvas
	inline Canvas(fcall c, int xx, int yy, int w, int h, int b, char* t);	//Explicit constructor for our Canvas
	inline int start();													//Function to start rendering our Canvas
	inline int end();													//Function to end rendering our Canvas
	inline void setColor(int r, int g, int b);							//Sets the global drawing color
	inline void setAutoRefresh(bool b);									//Sets whether we automatically refresh the Canvas
	inline Point drawPoint(int x, int y);								//Draws a point at the given coordinates
	inline Point drawPointColor(int x, int y, int r, int g, int b);		//Draws a point at the given coordinates with the given color
	inline Line drawLine(int x1, int y1, int x2, int y2);				//Draws a line at the given coordinates
	inline Line drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b);	//Draws a line at the given coordinates with the given color
	inline int getColorR();												//Gets the red component of the global drawing color
	inline int getColorG();												//Gets the green component of the global drawing color
	inline int getColorB();												//Gets the blue component of the global drawing color
	inline int getFrameNumber();										//Accessor for the number of frames rendered so far
	inline int getBufferSize();											//Accessor for the Shape list's buffer size
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
void Canvas::init(fcall c, int xx, int yy, int ww, int hh, int b) {
	started = false;  	//We haven't started the window yet
	counter = 0;		//We haven't drawn any frames yet
	autoRefresh = true;	//Default to clearing the queue every frame
	x = xx; y = yy; w = ww; h = hh;  						//Initialize translation
	myShapes = new List<Shape*>(b);							//Initialize myShapes
	box(FL_FLAT_BOX);  										//Sets the box we will draw to (the only one)
	setColor(0,0,0);										//Our default global drawing color is black
	if (c == NULL) {
		updateFunc = [](Canvas *c){};						//Empty lambda function that does nothing
	} else
		updateFunc = c;										//Adds a callback to the user's own draw function
	Fl::add_timeout(FRAME, Canvas_Callback, (void*)this);  	//Adds a callback after 1/60 second to the Canvas' callback function
}

/*
 * draw actually draws our Canvas and its contents to the display
 * Note: this function is called automatically by the callback and the FLTK redraw function, which is why it's private
 */
void Canvas::draw() {
	counter++;				//Increment the frame counter
	updateFunc(this);		//Call the user's callback to do work on the Canvas
	//Temporary variables for the initial global drawing color
	int oldR = colorR;
	int oldG = colorG;
	int oldB = colorB;
	Shape *s;				//Pointer to the next Shape in the queue
	//Iterate through our queue until we've made it to the end
	for (List<Shape*>::Iterator iterator = myShapes->begin(); iterator != myShapes->end();iterator++) {
		s = *iterator;		//Get the next item
//		if (s->forDeletion) {
//			iterator.removePrevious();
//		}
		if (s->getUsesDefaultColor()) {
			s->draw();		//If our shape uses the default color, just draw it
		}
		else {				//Otherwise, the color must be set before and after drawing
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
 * Canvas_Callback is a callback function called (by default) every 1/60 of a second (update cycle) to redraw the canvas
 * Parameters:
 * 		userdata, a pointer to the Canvas class that we're calling back
 */
void Canvas::Canvas_Callback(void *userdata) {
	Canvas *o = (Canvas*)userdata;  						//Casts the userdata pointer as a Canvas pointer
    o->redraw();  											//Redraw the canvas
    Fl::repeat_timeout(FRAME, Canvas_Callback, userdata);  	//Restart the fcall
}

/*
 * Default constructor for the canvas class
 * Parameter:
 * 		c, a callback to the user's own draw function
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 Canvas on the topleft of the screen with no title
 */
Canvas::Canvas(fcall c, int b = -1) : Fl_Box (0,0,800,600) {
	init(c,0,0,800,600,b);
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
Canvas::Canvas(fcall c, int xx, int yy, int w, int h, int b = -1, char* t = 0) : Fl_Box(xx,yy,w,h,t) {
	init(c,xx,yy,w,h,b);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started)						//If we're already started, return error code -1
		return -1;
	started = true;						//We've now started
    window = new Fl_Double_Window(w,h);	//Instantiate our drawing window
    window->add(this);					//Add ourself (Canvas) to the drawing window
    window->show();						//Show the window
    return(Fl::run());
}

/*
 * start starts rendering the Canvas
 * Returns: 0 if exit is successful, -1 otherwise
 */
int Canvas::end() {
	if (!started)	//If we haven't even started yet, return error code -1
		return -1;
	window->hide();	//Hide our window
	delete window;	//Delete our window from the heap
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
	fl_color(r,g,b);	//Updates the underlying FLTK color (critical to avoid syncing problems)
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
	Point *p = new Point(x,y);	//Creates the Point with the specified coordinates
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
 * 	Returns: a new point at the given position with the specified color
 */
Point Canvas::drawPointColor(int x, int y, int r, int g, int b) {
	Point *p = new Point(x,y,r,g,b);	//Creates the Point with the specified coordinates and color
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
 * 	Returns: a new line with the given coordinates
 */
Line Canvas::drawLine(int x1, int y1, int x2, int y2) {
	Line *l = new Line(x1,y1,x2,y2);//Creates the Point with the specified coordinates
	myShapes->push(l);				//Push it onto our drawing queue
	return *l;						//Return a pointer to our new Point
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
	Line *l = new Line(x1,y1,x2,y2,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(l);						//Push it onto our drawing queue
	return *l;								//Return a pointer to our new Point
}

// Accessor functions for the individual components of the global drawing color
int Canvas::getColorR() { return colorR; }
int Canvas::getColorG() { return colorG; }
int Canvas::getColorB() { return colorB; }

// Accessor function for the current render frame number
int Canvas::getFrameNumber() { return counter; }

#endif /* CANVAS_H_ */
