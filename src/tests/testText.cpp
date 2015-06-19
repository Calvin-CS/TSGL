/*
 * testText.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
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

    can.drawText(L"A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", 16, 150, 32, RED);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
                 32, BLUE);
    can.drawText(L"Of *what* exactly that extraordinary event was.", 16, 450, 32, GREY);
    can.drawText(L"And to that I say...oh well.", 16, 550, 32, WHITE);

}

int main() {
    Canvas c23;
    c23.start();
    textFunction(c23);
    c23.wait();
}
