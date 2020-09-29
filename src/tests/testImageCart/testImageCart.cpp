/*
 * testImageCart.cpp
 *
 * Usage: ./testImageCart <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws various images on a CartesianCanvas.
 * \details Same as imageFunction, but on a CartesianCanvas.
 * \param can Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas).
 */
void imageCartFunction(Cart& can) {
    CartesianBackground * cart = can.getBackground();
    cart->drawImage(1, 2.25, 0, "./assets/pics/test.png", 2, 1.5, 0,0,0);
    cart->drawImage(3, 2.25, 0, "./assets/pics/ship.bmp", 2, 1.5, 0,0,0); // possibly lost
    cart->drawImage(5, 2.25, 0, "./assets/pics/shiprgb.bmp", 2, 1.5, 0,0,0); // definitely lost
    cart->drawImage(1, 0.75, 0, "./assets/pics/sky_main.jpg", 2, 1.5, 0,0,0);
    cart->drawImage(3, 0.75, 0, "./assets/pics/cow.jpg", 2, 1.5, 0,0,0);
    cart->drawImage(5, 0.75, 0, "./assets/pics/colorfulKeyboard.jpg", 2, 1.5, 0,0,0);

    Image * image = new Image(3, 1.5, 0, "./assets/pics/colorful-cars-circle.jpg", 4, 3, 0,0,0, 0.25f);
    can.add(image);

    // Arrow * a = new Arrow(1, 2, 0, 5, 1, 0, 0.1, 0,0,0, BLUE);
    // can.add(a);

    while (can.isOpen()) {
        can.sleep();
    }

    delete image;
    // delete a;
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
    c.run(imageCartFunction);
}
