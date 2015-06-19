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
    can.drawImage("../assets/test.png", 0, 0, 400, 300);
    can.drawImage("../assets/ship.bmp", 400, 0, 400, 300);
    can.drawImage("../assets/shiprgb.bmp", 800, 0, 400, 300);
    can.drawImage("../assets/sky_main.jpg", 0, 300, 400, 300);
    can.drawImage("../assets/Captain-falcon.png", 400, 300, 400, 300);
    can.drawImage("../assets/mini.jpg", 800, 300, 400, 300);

    can.drawImage("../assets/bestpicture.png", 200, 0, 800, 600, 0.25f);    //ALPHA
}

int main() {
    int w = 1.2*Canvas::getDisplayHeight();
    int h = 0.5*w;
    Canvas c21(-1, -1, w, h ,"");
    c21.setBackgroundColor(GREY);
    c21.start();
    imageFunction(c21);
    c21.wait();
}
