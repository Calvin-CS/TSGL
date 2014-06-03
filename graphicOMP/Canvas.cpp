//
// Basic Canvas class build on top of FLTK library.
// UNFINISHED
//
// Last Modified: Patrick Crain, 6/3/2014
//
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
//#include <stack>

class Canvas : public Fl_Box {
private:
	int x,y,w,h;
	Fl_Double_Window *window;
	bool started;
	void init(int xx, int yy, int width, int height) {
		started = false;  //We haven't started the window yet
		x = xx; y = yy; w = width; h = height;  //Initialize translation
		box(FL_FLAT_BOX);  //Create a box for drawing
		color(45);  //Initialize the background color
		Fl::add_timeout(0.01, Canvas_Callback, (void*)this);  //Adds a callback after 0.25 seconds to itself
	}
	void draw() {
		static int counter = 0;
		counter < 800 ? ++counter : counter;
		int color;
		for (int i = 0; i < counter; i++) {
			for (int j = 0; j <= 600; j++) {
				color = i*128/800 + j*128/600;
				fl_color(color,color,color);
				fl_point(i,j);
			}
		}
	}
    static void Canvas_Callback(void *userdata) {  //Callback for the timer
    	Canvas *o = (Canvas*)userdata;  //Casts the userdata pointer as a new timer
        o->redraw();  //Redraw the timer
        Fl::repeat_timeout(0.01, Canvas_Callback, userdata);  //Restart the callback
    }
public:
    Canvas() : Fl_Box (0,0,800,600) {
    	init(0,0,800,600);
    }
	Canvas(int xx, int yy, int width, int height, char* title = 0) : Fl_Box(xx,yy,width,height,title) {
		init(xx,yy,width,height);
	}
	int start() {
		if (started)
			return -1;
		started = true;
	    window = new Fl_Double_Window(w,h);
	    window->add(this);
	    window->show();
	    return(Fl::run());
	}
	int end() {
		if (!started)
			return -1;
		window->hide();
		return 0;
	}
};
