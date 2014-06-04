// Basic Point class extending from the Shape class
//
// Last Modified: Patrick Crain, 6/4/2014

#ifndef CANVAS_H_
#define CANVAS_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "Point.h"
#include <omp.h>
#include "CQueue.h"
#include "List.h"

class Canvas : public Fl_Box {
private:
	Queue<Shape*> myShapes;
	int counter;
	int queueSize;
	int x,y,w,h;  //Positioning and sizing data for the Canvas
	int colorR, colorG, colorB; //Global color data
	Fl_Double_Window *window;
	bool started;
	inline void init(int xx, int yy, int width, int height);
	inline void draw();
	inline static void Canvas_Callback(void *userdata);
public:
	inline Canvas();
	inline Canvas(int xx, int yy, int width, int height, char* title);
	inline int start();
	inline int end();
	inline void setColor(int r, int g, int b);
	inline int getColorR();
	inline int getColorG();
	inline int getColorB();
	inline Point drawPoint(int x, int y);
	inline Point drawPointColor(int x, int y, int r, int g, int b);
	inline int getFrameNumber();
};

void Canvas::init(int xx, int yy, int width, int height) {
	started = false;  //We haven't started the window yet
	counter = 0;
	queueSize = 0;
	x = xx; y = yy; w = width; h = height;  //Initialize translation
	box(FL_FLAT_BOX);  //Create a box for drawing
	color(45);  //Initialize the background color
	setColor(0,0,0);
	Fl::add_timeout(0.01, Canvas_Callback, (void*)this);  //Adds a callback after 0.25 seconds to itself
}

void Canvas::draw() {
	counter++;
	int ql = queueSize, oldR = colorR, oldG = colorG, oldB = colorB;
	Shape *s;
	while (ql--  > 0) {
		s = myShapes.pop();
		if (s->getUsesDefaultColor()) {
			s->draw();
		}
		else {
			setColor(s->getColorR(),s->getColorG(),s->getColorB());
			s->draw();
			setColor(oldR, oldG, oldB);
		}
		myShapes.push(s);
	}
}

void Canvas::Canvas_Callback(void *userdata) {  //Callback for the timer
	Canvas *o = (Canvas*)userdata;  //Casts the userdata pointer as a new timer
    o->redraw();  //Redraw the timer
    Fl::repeat_timeout(0.01, Canvas_Callback, userdata);  //Restart the callback
}

Canvas::Canvas() : Fl_Box (0,0,800,600) {
	init(0,0,800,600);
}

Canvas::Canvas(int xx, int yy, int width, int height, char* title = 0) : Fl_Box(xx,yy,width,height,title) {
	init(xx,yy,width,height);
}

//Starts the canvas
int Canvas::start() {
	if (started)
		return -1;
	started = true;
    window = new Fl_Double_Window(w,h);
    window->add(this);
    window->show();
    return(Fl::run());
}

//Closes and frees the canvas (doesn't actually free yet)
int Canvas::end() {
	if (!started)
		return -1;
	window->hide();
	delete window;
	return 0;
}

//Sets the global color for drawing
void Canvas::setColor(int r, int g, int b) {
	colorR = r;
	colorG = g;
	colorB = b;
	fl_color(r,g,b);
}

int Canvas::getColorR() { return colorR; }

int Canvas::getColorG() { return colorG; }

int Canvas::getColorB() { return colorB; }

Point Canvas::drawPoint(int x, int y) {
	Point *p = new Point(x,y);
	myShapes.push(p);
	#pragma omp atomic
	queueSize++;
	//std::cout << myShapes.size() << std::endl;
	return *p;
}

Point Canvas::drawPointColor(int x, int y, int r, int g, int b) {
	Point *p = new Point(x,y,r,g,b);
	myShapes.push(p);
	#pragma omp atomic
	queueSize++;
	//std::cout << myShapes.size() << std::endl;
	return *p;
}

int Canvas::getFrameNumber() { return counter; }

#endif /* CANVAS_H_ */
