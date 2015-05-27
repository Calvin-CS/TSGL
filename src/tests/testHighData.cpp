/*
 * testHighData.cpp
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
 * \brief Draws a large number of pixels on a Canvas at a high framerate.
 * \details Very basic stress test for the Canvas' drawPoint() function.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - Set Local variables to track the internal timer's repetitions, and the Canvas' dimensions.
 * - While the Canvas is open:
 *   - Set \b reps to the timer's current number of repetitions.
 *   - Compute the blue component of the current color based on reps.
 *   - Attempt to draw every pixel with the current 1.0,1.0,blue
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void highData(Canvas& can) {
    unsigned int reps,
                 width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    while (can.getIsOpen()) {
        reps = can.getReps();  //Added a getReps() function to Canvas that gets the internal timer's reps
        float blue = (reps % 255) / 255.0f;
        for (unsigned int i = 0; i < width; i++) {
            for (unsigned int j = 0; j < height; j++) {
                can.drawPoint(i, j, ColorFloat(1.0f, 1.0f, blue, 1.0f));
            }
        }
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c22(0, 0, 1200, 900, 1201 * 900, "", FRAME);
    c22.setBackgroundColor(GREY);
    c22.start();
    highData(c22);
    c22.close();
    glfwTerminate();  // Release GLFW
}
