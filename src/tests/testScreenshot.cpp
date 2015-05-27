/*
 * testScreenshot.cpp
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

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

const int IPF = 1000;  //For those functions that need it

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Draws a bunch of triangles and outputs each frame to an image.
 * \details
 * - Declare and initialize variables to keep track of each of three vertices for a triangle.
 * - Set the Canvas to record screenshots for 30 seconds (1800 frames).
 * - Set up a timer to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep the timer until the Canvas is ready to draw.
 *   - Set the old coordinates to last frame's middle ones.
 *   - Set the middle coordinates to last frame's new ones.
 *   - Set the new coordinates to a random position on the Canvas.
 *   - Draw a traingle on the canvas with the given coordinates and a random color.
 *   .
 * .
 * \note The details of the recordForNumFrames() function are handled automatically in Canvas, and
 * are by default written the \i frames/ directory.
 * \param can Reference to the Canvas being drawn to
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
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c29(0, 0, 800, 600, 0, 0, 800, 600, 50000, "", FRAME);
    c29.setBackgroundColor(GREY);
    c29.start();
    screenShotFunction(c29);
    c29.close();
    glfwTerminate();  // Release GLFW
}
