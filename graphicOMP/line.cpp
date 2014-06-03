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
#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0
class MyTimer : public Fl_Box {
    void draw() {
        // COMPUTE NEW COORDS OF LINE
        static long start = time(NULL);  //static variable is computed once on first call
        long tick = time(NULL) - start;  //computed every time call is run
        char secs[80]; sprintf(secs, "%02ld:%02ld", tick/60, tick%60); //formats in xx:xx format
        float pi = 3.14 - (((tick % 60) / 60.0) * 6.28);  //computes the current radian value of the tick
        int radius = h() / 2;  //radius = height of screen / 2
        int x1 = (int)(x() + w()/2),  //coordinates of line
            y1 = (int)(y() + h()/2),
            x2 = (int)(x1 + (sin(pi) * radius)),
            y2 = (int)(y1 + (cos(pi) * radius));

        // TELL BASE WIDGET TO DRAW ITS BACKGROUND
        Fl_Box::draw();  //what they said

        // DRAW 'SECOND HAND' OVER WIDGET'S BACKGROUND
        fl_color(TICK_COLOR);
        fl_line(x1, y1, x2, y2);
        fl_color(CIRC_COLOR);
        fl_pie(x1-10, y1-10, 20, 20, 0.0, 270.0);  //arc circle, 360 degrees

        // DRAW TIMER TEXT STRING
        fl_color(TICK_COLOR);
        fl_font(FL_HELVETICA,16);
        fl_draw(secs, x()+4, y()+h()-4);
    }
    static void Timer_CB(void *userdata) {  //Callback for the timer
        MyTimer *o = (MyTimer*)userdata;  //Casts the userdata pointer as a new timer
        o->redraw();  //Redraw the timer
        Fl::repeat_timeout(0.25, Timer_CB, userdata);  //Restart the callback
    }
public:
    // CONSTRUCTOR
    MyTimer(int X,int Y,int W,int H,const char*L=0) : Fl_Box(X,Y,W,H,L) {
        box(FL_FLAT_BOX);  //Create a box for drawing
        color(BG_COLOR);  //Initialize the background color
        Fl::add_timeout(0.25, Timer_CB, (void*)this);  //Adds a callback after 0.25 seconds to itself
    }
};
// MAIN
int main() {
     Fl_Double_Window win(220, 220);
     MyTimer tim(10, 10, win.w()-20, win.h()-20);
     win.show();
     return(Fl::run());
}
