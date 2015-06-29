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
 * \param can Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas).
 */
void imageCartFunction(Cart& can) {
    can.drawImage("../assets/pics/test.png", 0, 3, 2, 1.5);
    can.drawImage("../assets/pics/ship.bmp", 2, 3, 2, 1.5);
    can.drawImage("../assets/pics/shiprgb.bmp", 4, 3, 2, 1.5);
    can.drawImage("../assets/pics/sky_main.jpg", 0, 1.5, 2, 1.5);
    can.drawImage("../assets/pics/Captain-falcon.png", 2, 1.5, 2, 1.5);
    can.drawImage("../assets/pics/mini.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("../assets/pics/bestpicture.png", 1, 3, 4, 3, 0.25f);    //ALPHA
}

int main() {
    int w = 1.2*Canvas::getDisplayHeight();
    int h = 0.5*w;
    Cart c26(-1, -1, w, h, 0, 0, 6, 3, "");
    c26.setBackgroundColor(GREY);
    c26.start();
    imageCartFunction(c26);
    c26.wait();
}
