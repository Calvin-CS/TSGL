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
    Background * bg = can.getBackground();
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d, x, y, w, h;
    while (can.isOpen()) {
        can.sleep();
        a = saferand(-WW/2,WW/2); b = saferand(-WH/2, WH/2);
        c = saferand(-WW/2,WW/2); d = saferand(-WH/2, WH/2);
        x = (a + c) / 2;
        y = (b + d) / 2;
        w = abs(c - a);
        h = abs(d - b);
        bg->drawRectangle(x, y, 0, w, h, 0,0,0, ColorFloat(randfloat(), randfloat(), randfloat(), (float) 16/255));
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Fancy Rectangles", BLACK);
    c.run(alphaRectangleFunction);
}
