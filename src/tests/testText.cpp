/*
 * testText.cpp
 *
 * Usage: ./testSpectrum <width> <height> <fontPath>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws some text on a Canvas.
 * \details
 * - We changed it so that now a default text font is loaded if one is not specified.
 * - We draw a few lines of text in various colors using drawText().
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param font The font of the text.
 */
void textFunction(Canvas& can, std::string font) {
    // can.drawText("A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    // can.drawText("Something extraordinary happened.", 16, 150, 32, RED);
    // can.drawText("Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    // can.drawText("Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
    //              32, BLUE);
    // can.drawText("Of *what* exactly that extraordinary event was.", 16, 450, 32, GRAY);
    // can.drawText("And to that I say...oh well.", 16, 550, 32, WHITE);
    Text * text = new Text(0,0,0,"B", font, 100, 0,0,0, YELLOW);
    can.add(text);
    while(can.isOpen()) {
        can.sleep();
    }
    delete text;

}

//Takes command-line arguments for the width and height of the screen
//as well as for the font
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2f*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75f*w - 330.0f;
    std::string font = (argc > 3) ? argv[3] : "./assets/freefont/FreeMono.ttf";
    if(w <= 0 || h <= 0) {  //Check validity of width and height
      w = 1.2f*Canvas::getDisplayHeight();
      h = 0.75f*w;
    }
    Canvas c(-1, -1, w, h, "Text on a Canvas");
    c.run(textFunction, font);
}
