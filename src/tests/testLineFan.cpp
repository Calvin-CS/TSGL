/*
 * testLineFan.cpp
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
 * \brief Draws a fan of randomly colored lines at the target framerate and a dynamic number of threads using OMP
 * \details
 * - A timer is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS)
 * - While the canvas is open:
 *   - The number of threads to use is recalculated, and the process is forked.
 *   - The timer sleeps on each thread until the next frame is ready to be drawn.
 *   - An offset is calculated based on the thread's ID and a predefined arc-length.
 *   - An angle is then calculated using the offset and the Canvas' current lifespan ( as calculated by \b t.getReps() ).
 *   - The vertices of the lines to be drawn are chosen using the sines and cosines of the predetermined angle.
 *   - A random color is chosen.
 *   - The line is draw to the Canvas.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void lineFanFunction(Canvas& can) {
    const double ARC = 7.11;  //(Arbitrary) spacing between arcs of the fan
    while (can.getIsOpen()) {
        #pragma omp parallel num_threads(omp_get_num_procs())
        {
            can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
            int a, b, c, d, red, green, blue;
            double angle, offset = omp_get_thread_num() * ARC;
            angle = offset + can.getReps() * RAD;
            a = can.getWindowWidth() / 2 * (1 + sin(angle));
            b = can.getWindowHeight() / 2 * (1 + cos(angle));
            c = can.getWindowWidth() / 2 * (1 - sin(angle));
            d = can.getWindowHeight() / 2 * (1 - cos(angle));
            red = (a + can.getReps()) % NUM_COLORS;
            green = (b + can.getReps()) % NUM_COLORS;
            blue = (a * b + can.getReps()) % NUM_COLORS;
            can.drawLine(a, b, c, d, ColorInt(red, green, blue));
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c3(500, FRAME);
    c3.setBackgroundColor(GREY);
    c3.start();
    lineFanFunction(c3);
    c3.close();
    glfwTerminate();  // Release GLFW
}

