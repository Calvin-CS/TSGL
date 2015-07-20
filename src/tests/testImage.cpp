/*
 * testImage.cpp
 *
 * Usage: ./testImage <width> <height>
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
    can.drawImage("../assets/pics/colorfulKeyboard.jpg", ww, hh, ww, hh);
    can.drawImage("../assets/pics/cow.jpg", ww*2, hh, ww, hh);

    can.drawImage("../assets/pics/background.jpg", ww/2, 0, ww*2, hh*2, 0.25f);    //ALPHA
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.5*w;
    if(w <= 0 || h <= 0) { //Check width and height validity
      w = 1.2*Canvas::getDisplayHeight();
      h = 0.5*w;
    }
    Canvas c(-1, -1, w, h ,"Images");
    c.setBackgroundColor(GRAY);
    c.run(imageFunction);
}
