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
//#include <stack>
#include <unistd.h>
#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0
class MyTimer : public Fl_Box {
    void draw() {
    	static int counter = 0;
    	counter = (counter < 600) ? ++counter : counter;
    	int color;
    	for (int i = 0; i < counter; i++) {
    		for (int j = 0; j <= 600; j++) {
    			color = (i+j)*256/1200;
    			fl_color(color,color,color);
    			fl_point(i,j);
    		}
    	}
    }
    static void Timer_CB(void *userdata) {  //Callback for the timer
        MyTimer *o = (MyTimer*)userdata;  //Casts the userdata pointer as a new timer
        o->redraw();  //Redraw the timer
        Fl::repeat_timeout(0.01, Timer_CB, userdata);  //Restart the callback
    }
public:
    // CONSTRUCTOR
    MyTimer(int X,int Y,int W,int H,const char*L=0) : Fl_Box(X,Y,W,H,L) {
        box(FL_FLAT_BOX);  //Create a box for drawing
        color(BG_COLOR);  //Initialize the background color
        Fl::add_timeout(0.01, Timer_CB, (void*)this);  //Adds a callback after 0.25 seconds to itself
    }
};
// MAIN
int main2() {
     Fl_Double_Window win(600, 600);
     MyTimer tim(10, 10, win.w()-20, win.h()-20);
     win.show();
     return(Fl::run());
}

// OLDER MAIN
//int main3() {
//	fl_register_images();
//	Fl_Window *window = new Fl_Window(WINDOW_W, WINDOW_H, "This is the greatest window");
//	Fl_Text_Buffer *sometext = new Fl_Text_Buffer();
//	Fl_Text_Display *textdisplay = new Fl_Text_Display(200,200,300,300,"Display");
//	Fl_Box *box = new Fl_Box(250,100,WINDOW_W-100,WINDOW_H-100);
//	Fl_PNG_Image myjpeg("data/bestpicture.png");
//
//	textdisplay->buffer(sometext);
//	box->image(myjpeg);
//	sometext->text("This is the greatest text");
//
//	window->show();
//	return (Fl::run());
//}
