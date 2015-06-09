/*
 * testImageCart.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

typedef CartesianCanvas Cart;

/*!
 * \brief Draws various images on a CartesianCanvas.
 * \details Same as imageFunction, but on a CartesianCanvas.
 * \param can, Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas)
 */
void imageCartFunction(Cart& can) {
    can.drawImage("assets/test.png", 0, 3, 2, 1.5);
    can.drawImage("assets/ship.bmp", 2, 3, 2, 1.5);
    can.drawImage("assets/shiprgb.bmp", 4, 3, 2, 1.5);
    can.drawImage("assets/sky_main.jpg", 0, 1.5, 2, 1.5);
    can.drawImage("assets/Captain-falcon.png", 2, 1.5, 2, 1.5);
    can.drawImage("assets/mini.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("assets/bestpicture.png", 1, 3, 4, 3, 0.25f);    //ALPHA
}

int main() {
    Cart c26(0, 0, 1200, 600, 0, 0, 6, 3, "");
    c26.setBackgroundColor(GREY);
    c26.start();
    imageCartFunction(c26);
    c26.wait();
}
