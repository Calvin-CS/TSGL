/*
 * testText.cpp
 *
 * Usage: ./testSpectrum <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws some text on a Canvas.
 * \details
 * - We declare some colors to use for drawing.
 * - We changed it so that now a default text font is loaded if one is not specified.
 * - We draw a few lines of text in various colors using drawText().
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void textFunction(Canvas& can) {
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.setBackgroundColor(WHITE);

    Text * t0 = new Text("A long time ago, in a galaxy far, far away.", 16, 50, 8, BLACK);
    Text * t1 = new Text("Something extraordinary happened.", 16, 150, 8, RED);
    Text * t2 = new Text("Something far more extraordinary than anything mankind has ever seen.", 16, 250, 8, GREEN);
    Text * t3 = new Text("Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350, 8, BLUE);
    Text * t4 = new Text("Of *what* exactly that extraordinary event was.", 16, 450, 8, GRAY);
    Text * t5 = new Text("And to that I say...oh well.", 16, 550, 8, WHITE);
    can.add( t0 ); can.add( t1 ); can.add( t2 ); can.add( t3 ); can.add( t4 ); can.add( t5 );

    while( can.isOpen() ) { can.sleep(); } //Wait until Canvas is closed...
    //Then free memory from Text pointers
    delete t0; delete t1; delete t2;
    delete t3; delete t4; delete t5;
}

//Takes command-line arguments for the width and height of the screen
//as well as for the font
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2f*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75f*w - 330.0f;
    if(w <= 0 || h <= 0) {  //Check validity of width and height
      w = 1.2f*Canvas::getDisplayHeight();
      h = 0.75f*w;
    }
    Canvas c(-1, -1, w, h, "Text on a Canvas");
    c.run(textFunction);
}
