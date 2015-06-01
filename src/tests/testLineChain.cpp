/*
 * testLineChain.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

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
    int holder1 = atoi(argv[1]);   //Width
    int holder2 = atoi(argv[2]);   //Height
    int width = 0;
    int height = 0;
    if (holder1 <= 0 || holder2 <= 0) {    //Check if the width and height arguments are valid
    	width = WINDOW_W;  //If not, set the default window width and height
    	height = WINDOW_H;
    } else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
    	width = WINDOW_W;
        height = WINDOW_H;
    } else {
    	width = holder1;   //Else, set them to the Canvas width and height
        height = holder2;
    }
    Canvas c2(0, 0, width, height, "", FRAME);
    c2.setBackgroundColor(BLACK);
    c2.start();
    lineChainFunction(c2);
    c2.close();
}
