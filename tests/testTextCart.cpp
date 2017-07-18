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
    can.setBackgroundColor(WHITE);

    Text * t0 = new Text("A long time ago, in a galaxy far, far away.", 1, 2, 8, BLACK);
    Text * t1 = new Text("Something extraordinary happened.", 1, 1.9, 8, BLACK);
    Text * t2 = new Text("Something far more extraordinary than anything mankind has ever seen.", 1, 1.8, 8, BLACK);
    Text * t3 = new Text("Unfortunately, as nobody was around to witness the event, we are largely ignorant", 1, 1.7,
                 8, BLACK);
    Text * t4 = new Text("Of *what* exactly that extraordinary event was.", 1, 1.6, 8, BLACK);
    Text * t5 = new Text("And to that I say...oh well.", 1, 1.5, 8, BLACK);
    can.add(t0); can.add(t1); can.add(t2); can.add(t3); can.add(t4); can.add(t5);
}

//Takes command-line arguments for the width and height of the screen
//as well as for the font file for the text
int main(int argc, char * argv[]) {
    //Width and height
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w - 200.0f;
    std::string font = argv[3]; //Font
    if(w <= 0 || h <= 0) {  //Check validity of width and height
      w = 1.2 * Canvas::getDisplayHeight();
      h = 0.75 * w;
    }
    Cart c(-1, -1, w, h, 0, 0, 4, 3, "Text on a Cartesian Canvas");
    c.run(textCartFunction, font);
}
