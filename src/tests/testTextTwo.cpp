 /*
 * testTextTwo.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

/**
 * \brief Tests to see if text is still drawn if a font is specified (just to make sure the error handling for when one is not specified didn't break anything).
 * \details Same as textFunction, but without the setFont line deleted.
 * \param can Reference to the Canvas being drawn to.
 */
void textFunctionTwo(Canvas& can) {
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.setFont("../assets/freefont/FreeMono.ttf");  //This line has been deleted in testText
    can.drawText(L"A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", 16, 150, 32, RED);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
                 32, BLUE);
    can.drawText(L"Of *what* exactly that extraordinary event was.", 16, 450, 32, GRAY);
    can.drawText(L"And to that I say...oh well.", 16, 550, 32, WHITE);
}

int main() {
    int w = 1.2f*Canvas::getDisplayHeight();
    Canvas c24(-1, -1, w, 0.75f*w, "More Text on a Canvas");
    c24.start();
    textFunctionTwo(c24);
    c24.wait();
}
