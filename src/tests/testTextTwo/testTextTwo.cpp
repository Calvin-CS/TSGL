/*
 * testTextTwo.cpp
 *
 * Usage: ./testTextTwo <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/**
 * \brief Tests to see if text is still drawn if a font is not specified.
 * \details Same as textFunction, but with the setFont line deleted.
 * \param can Reference to the Canvas being drawn to.
 */
void textFunctionTwo(Canvas& can) {
    Background * bg = can.getBackground();
    float WW = can.getWindowWidth() / 2; float WH = can.getWindowHeight() / 2;
    std::string font = "./assets/freefont/FreeMonoBold.ttf";
    bg->drawText(0, WH * 5/6, 0, L"A long time ago, in a galaxy far, far away.", font, 16, 0,0,0, BLACK);
    bg->drawText(0, WH * 3/6, 0, L"Something extraordinary happened.", font, 16, 0,0,0, RED);
    bg->drawText(0, WH * 1/6, 0,L"Something far more extraordinary than anything mankind has ever seen.", font, 16, 0,0,0, GREEN);
    bg->drawText(0,-WH * 1/6, 0,L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", font, 16, 0,0,0, BLUE);
    bg->drawText(0,-WH * 3/6, 0,L"Of *what* exactly that extraordinary event was.", font, 16, 0,0,0, PURPLE);
    bg->drawText(0,-WH * 5/6, 0,L"And to that I say...oh well.", font, 16, 0,0,0, WHITE);
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char * argv[]) {
    //Width and height
    int w = (argc > 1) ? atoi(argv[1]) : 1.2f*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75f*w - 300.0f;
    if(w <= 0 || h <= 0) { //Check the validity of the width and height
      w = 1.2f*Canvas::getDisplayHeight();
      h = 0.75f*w;
    }
    Canvas c(-1, -1, w, h, "More Text on a Canvas");
    c.run(textFunctionTwo);
}
