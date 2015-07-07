/*
 * testImage.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws various images on a Canvas.
 * \details Very basic test function showcasing image drawing capabilities.
 * - The first 6 images are drawn opaque.
 * - The 7th image is drawn across the entire Canvas with alpha transparency.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void imageFunction(Canvas& can) {
    int ww = can.getWindowWidth()/3, hh = can.getWindowHeight()/2;
    can.drawImage("../assets/pics/test.png", 0, 0, ww, hh);
    can.drawImage("../assets/pics/ship.bmp", ww, 0, ww, hh);
    can.drawImage("../assets/pics/shiprgb.bmp", ww*2, 0, ww, hh);
    can.drawImage("../assets/pics/sky_main.jpg", 0, hh, ww, hh);
    can.drawImage("../assets/pics/Captain-falcon.png", ww, hh, ww, hh);
    can.drawImage("../assets/pics/mini.jpg", ww*2, hh, ww, hh);

    can.drawImage("../assets/pics/bestpicture.png", ww/2, 0, ww*2, hh*2, 0.25f);    //ALPHA
}

int main() {
    int w = 1.2*Canvas::getDisplayHeight();
    int h = 0.5*w;
    Canvas c21(-1, -1, w, h ,"Images");
    c21.setBackgroundColor(GREY);
    c21.start();
    imageFunction(c21);
    c21.wait();
}
