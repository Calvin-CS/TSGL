/*
 * testAlphaRectangle.cpp
 *
 * Usage: ./testAlphaRectangle <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws semi-transparent rectangles on a Canvas.
 * \details
 * - Store the Canvas' width and height in variables for easy reuse.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 10 seconds.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Select a random position on the Canvas for a corner of a rectangle.
 *   - Draw a rectangle stretching from the specified corner to another corner on the Canvas,
 *     with a random color and a transparency of 16 (~0.06).
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void alphaRectangleFunction(Canvas& can) {
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;
    while (can.isOpen()) {
<<<<<<< HEAD
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        a = rand() % WINDOW_W;
        b = rand() % WINDOW_H;
        c = rand() % WINDOW_W;
        d = rand() % WINDOW_H;
=======
        can.sleep();
        a = rand() % WW; b = rand() % WH;
        c = rand() % WW; d = rand() % WH;
>>>>>>> 537c46ba6c9b4aff4c592277352ca791cf994e5a
        can.drawRectangle(a, b, c, d, ColorInt(rand()%MAX_COLOR, rand()%MAX_COLOR, rand()%MAX_COLOR, 16));
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Fancy Rectangles");
    c.setBackgroundColor(BLACK);
    c.run(alphaRectangleFunction);
}
