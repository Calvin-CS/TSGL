#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"

using namespace std;

int main() {
	Fl_Window *window = new Fl_Window(800, 600, "This is the greatest window");
	Fl_Text_Buffer *sometext = new Fl_Text_Buffer();
	Fl_Text_Display *textdisplay = new Fl_Text_Display(200,200,300,300,"Display");
	textdisplay->buffer(sometext);

	window->show();

	sometext->text("This is the greatest text");

//	cout << "Sleeping..." << endl;
//	cout << "Window shown: " << window->shown() << endl;
//	cout << "DONE" << endl;

	return (Fl::run());
}
