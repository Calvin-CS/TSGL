/*
 * testImageCart.cpp
 *
 * Usage: ./testImageCart <width> <height>
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
    can.drawImage("../assets/pics/cow.jpg", 2, 1.5, 2, 1.5);
    can.drawImage("../assets/pics/colorfulKeyboard.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("../assets/pics/colorful-cars-circle.jpg", 1, 3, 4, 3, 0.25f);    //ALPHA
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.5*w;
    if(w <= 0 || h <= 0) {  //Check width and height validity
      w = 1.2 * Canvas::getDisplayHeight();
      h = 0.5 * w;
    }
    Cart c(-1, -1, w, h, 0, 0, 6, 3, "Cartesian Images");
    c.setBackgroundColor(GRAY);
    c.run(imageCartFunction);
}
