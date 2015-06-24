/*
 * testAlphaRectangle.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
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
 *   with a random color and a transparency of 16 (~0.06).
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void alphaRectangleFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the center of the window
              WINDOW_H = can.getWindowHeight();
    int a, b;
    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        a = rand() % WINDOW_W;
        b = rand() % WINDOW_H;
        can.drawRectangle(a, b, rand() % (WINDOW_W - a), rand() % (WINDOW_H - b),
                          ColorInt(rand() % MAX_COLOR, rand() % MAX_COLOR, rand() % MAX_COLOR, 16));
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c14(-1, -1, w, h, "", FRAME / 10);
    c14.setBackgroundColor(BLACK);
    c14.start();
    alphaRectangleFunction(c14);
    c14.wait();
}
