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
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.drawText(L"A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", 16, 150, 32, RED);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
                 32, BLUE);
    can.drawText(L"Of *what* exactly that extraordinary event was.", 16, 450, 32, GRAY);
    can.drawText(L"And to that I say...oh well.", 16, 550, 32, WHITE);
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
