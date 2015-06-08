/*
 * testLineChain.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

/*!
 * \brief Draws a chain of randomly colored lines at the target framerate (default 60 FPS)
 * \details
 * - \b xNew and \b yNew are set to the middle of the canvas
 * - The internal timer of the Canvas is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS)
 * - While the canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn
 *   - \b xOld and \b yOld are set to \b xNew and \b yNew, while \b xNew and \b yNew are set to random positions
 *   - A random color is chosen
 *   - The line is drawn to the Canvas
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void lineChainFunction(Canvas& can) {
    int xOld, yOld, xNew = can.getWindowWidth() / 2, yNew = can.getWindowHeight() / 2, red, green, blue;
    while (can.getIsOpen()) {  // Checks to see if the window has been closed
        can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
        xOld = xNew;
        yOld = yNew;
        xNew = rand() % can.getWindowWidth();
        yNew = rand() % can.getWindowHeight();
        red = rand() % NUM_COLORS;
        green = rand() % NUM_COLORS;
        blue = rand() % NUM_COLORS;
        can.drawLine(xOld, yOld, xNew, yNew, ColorInt(red, green, blue));
    }
}

//Takes command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1200;
    int h = (argc > 2) ? atoi(argv[2]) : 900;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1200;
      h = 900;                  //If not, set the width and height to a default value
    }
    Canvas c2(0, 0, w, h, "", FRAME);
    c2.setBackgroundColor(BLACK);
    c2.start();
    lineChainFunction(c2);
    c2.wait();
}
