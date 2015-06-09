/*
 * testTextCart.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

typedef CartesianCanvas Cart;

/*!
 * \brief Draws some text on a CartesianCanvas
 * \details Same as textFunction, but with a CartesianCanvas and black text.
 * \param can, Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas)
 */
void textCartFunction(Cart& can) {
    can.setFont("../assets/freefont/FreeSerif.ttf");
    can.drawText(L"A long time ago, in a galaxy far, far away.", .05, .8, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", .05, .6, 32, BLACK);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", .05, .4, 32, BLACK);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", .05, .3,
                 32, BLACK);
    can.drawText(L"Of *what* exactly that extraordinary event was.", .05, .2, 32, BLACK);
    can.drawText(L"And to that I say...oh well.", .05, .1, 32, BLACK);
}

int main() {
    Cart c27(0, 0, 1200, 900, 0, 0, 4, 3, "");
    c27.start();
    textCartFunction(c27);
    c27.wait();
}
