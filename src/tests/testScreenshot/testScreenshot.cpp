/*
 * testScreenshot.cpp
 *
 * Usage: ./testScreenshot <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a bunch of triangles and outputs each frame to an image.
 * \details
 * - Declare and initialize variables to keep track of each of three vertices for a triangle.
 * - Set the Canvas to record screenshots for 30 seconds (1800 frames).
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Set the old coordinates to last frame's middle ones.
 *   - Set the middle coordinates to last frame's new ones.
 *   - Set the new coordinates to a random position on the Canvas.
 *   - Draw a triangle on the canvas with the given coordinates and a random color.
 *   .
 * .
 * \note The details of the recordForNumFrames() function are handled automatically in Canvas, and
 * are by default written to the i frames/ directory.
 * \param can Reference to the Canvas being drawn to.
 */
void screenShotFunction(Cart& can) {
    Background * bg = can.getBackground();
    int xNew = can.getCartWidth() / 2, yNew = can.getCartHeight() / 2, xMid = xNew, yMid = yNew, xOld, yOld;
    can.recordForNumFrames(FPS * 30);
    while (can.isOpen()) {  // Checks to see if the window has been closed
        can.sleep();
        xOld = xMid;
        yOld = yMid;
        xMid = xNew;
        yMid = yNew;
        xNew = saferand(0, can.getCartWidth());
        yNew = saferand(0, can.getCartHeight());
        bg->drawTriangle(xOld,yOld,0, xMid,yMid,0, xNew, yNew,0, 0,0,0, Colors::randomColor());
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char * argv[]) {
    Cart c(-1, -1, 800, 600, 0, 0, 800, 600,"Screenshot Test");
    c.run(screenShotFunction);
}
