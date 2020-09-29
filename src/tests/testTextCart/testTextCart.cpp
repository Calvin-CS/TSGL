/*
 * testTextCart.cpp
 *
 * Usage: ./testTextCart <width> <height> <fontPath>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws some text on a CartesianCanvas.
 * \details Same as textFunction, but with a CartesianCanvas and black text.
 * \param can Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas).
 * \param font The font of the text.
 */
void textCartFunction(Cart& can, std::string font) {
    CartesianBackground * cart = can.getBackground();
    float w = can.getCartWidth();
    float h = can.getCartHeight();
    // Text * t = new Text(w/2, h/2, 0,  L"1.5\u03C0", font, (float) 1/7, 0,0,0, BLACK);
    // can.add(t);
    cart->drawText(w/2, h * 11/12, 0, L"A long time ago, in a galaxy far, far away.", font, (float) 3/14, 0,0,0, BLACK);
    cart->drawText(w/2, h * 9/12, 0, L"Something extraordinary happened.", font, (float) 3/14, 0,0,0, BLACK);
    cart->drawText(w/2, h * 7/12, 0, L"Something far more extraordinary than anything mankind has ever seen.", font, (float) 3/14, 0,0,0, BLACK);
    cart->drawText(w/2, h * 5/12, 0, L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", font, (float) 3/14, 0,0,0, BLACK);
    cart->drawText(w/2, h * 3/12, 0, L"Of *what* exactly that extraordinary event was.", font, (float) 3/14, 0,0,0, BLACK);
    cart->drawText(w/2, h * 1/12, 0, L"And to that I say...oh well.", font, (float) 3/14, 0,0,0, BLACK);
    while(can.isOpen()) {
        can.sleep();
    }
}

//Takes command-line arguments for the width and height of the screen
//as well as for the font file for the text
int main(int argc, char * argv[]) {
    //Width and height
    int w = 960;
    int h = 420;
    std::string font = "./assets/freefont/FreeSerif.ttf";// argv[3]; //Font
    Cart c(-1, -1, w, h, 0, 0, 4, 3, "Text on a Cartesian Canvas");
    c.run(textCartFunction, font);
}
