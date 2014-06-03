//
// FLTK drawing example showing simple line drawing animation
// erco 03/22/07
// Better documentation by Patrick Crain, 6/3/2014
//
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

class Canvas : public Fl_Box {
	void draw() {
		static int counter = 0;
		counter = (counter < 800) ? ++counter : counter;
		int color;
		for (int i = 0; i < counter; i++) {
			for (int j = 0; j <= 600; j++) {
				color = (i+j)*256/1200;
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
    	box(FL_FLAT_BOX);  //Create a box for drawing
		color(45);  //Initialize the background color
		Fl::add_timeout(0.01, Canvas_Callback, (void*)this);  //Adds a callback after 0.25 seconds to itself
    }
	Canvas(int x, int y, int width, int height) : Fl_Box(x,y,width,height,0) {
        box(FL_FLAT_BOX);  //Create a box for drawing
        color(45);  //Initialize the background color
        Fl::add_timeout(0.01, Canvas_Callback, (void*)this);  //Adds a callback after 0.25 seconds to itself
	}
	int start() {
	    Fl_Double_Window win(800, 600);
	    win.add(this);
	    win.show();
	    return(Fl::run());
	}
};
