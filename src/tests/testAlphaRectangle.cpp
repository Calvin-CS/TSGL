/*
 * testAlphaRectangle.cpp
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
 * \brief Draws semi-transparent rectangles on a Canvas
 * \details
 * - Store the Canvas' width and height in variables for easy reuse.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 10 seconds
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Select a random position on the Canvas for a corner of a rectangle
 *   - Draw a rectangle stretching from the specified corner to another corner on the Canvas,
 *   with a random color and a transparency of 16 (~0.06).
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
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

int main() {
    Canvas c14(0, 0, WINDOW_W, WINDOW_H, "", FRAME / 10);
    c14.setBackgroundColor(BLACK);
    c14.start();
    alphaRectangleFunction(c14);
    c14.close();
}
