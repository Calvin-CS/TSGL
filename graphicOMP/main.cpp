#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Shared_Image.H"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Box.H"

#include "Canvas.h"
#include <iostream>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

int main() {
	Canvas *can = new Canvas();
	for (int i = 100; i < 700; i++) {
		for (int j = 100; j < 500; j++) {
			can->setColor(80,10,160);
			if (i % 2 == 0)
				can->drawPoint(i,j);
			else
				can->drawPointColor(i,j,i,j,i*j % 256);
		}
	}
	return (can->start());
}

//int main2() {
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
