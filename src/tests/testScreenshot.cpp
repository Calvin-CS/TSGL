/*
 * testScreenshot.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

typedef CartesianCanvas Cart;

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
 * are by default written to the \i frames/ directory.
 * \param can, Reference to the Canvas being drawn to
 */
void screenShotFunction(Canvas& can) {
    int xNew = can.getWindowWidth() / 2, yNew = can.getWindowHeight() / 2, xMid = xNew, yMid = yNew, xOld, yOld;
    can.recordForNumFrames(FPS * 30);
    while (can.getIsOpen()) {  // Checks to see if the window has been closed
        can.sleep();
        xOld = xMid;
        yOld = yMid;
        xMid = xNew;
        yMid = yNew;
        xNew = rand() % can.getWindowWidth();
        yNew = rand() % can.getWindowHeight();
        can.drawTriangle(xOld, yOld, xMid, yMid, xNew, yNew, Colors::randomColor(), true);
    }
}

int main() {
    Cart c29(0, 0, 800, 600, 0, 0, 800, 600,"", FRAME);
    c29.start();
    screenShotFunction(c29);
    c29.wait();
}
