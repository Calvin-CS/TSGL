/*
 * testCosineIntegral.cpp
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
 * \brief Draws the area under a predefined function (the integral) using CartesianCanvas
 * \details
 * - Use a predetermined number of threads, storing it in: \b THREADS.
 * - Bind Q's press event to quit the rendering in case it takes too long.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 2 seconds
 * - Draw axes through the origin, with spacing PI/4 between x ticks and 0.5 between y ticks.
 * - Store the width of the canvas's pixel in \b pw to avoid thousands of multiple function calls.
 * - Initialize and draw a CosineFunction using the currently rendered area of the CartesianCanvas.
 * - Set the CartesianCanvas' font from an external font file using setFont().
 * - Draw some labels on the CartesianCanvas to make things look pretty.
 * - Set up a parallel block with OMP using \b THREADS threads.
 * - Set \b nthreads to the actual number of threads spawned.
 * - Calculate each thread's share of the work and store it in: \b offset.
 * - Calculate each thread's starting position and store it in: \b start.
 * - Calculate each thread's stopping position and store it in: \b stop.
 * - For each thread, from \b start to \b stop with step size \b pw:
 *   - If the Canvas was closed, break.
 *   - Sleep the internal timer until it's ready to render.
 *   - Draw a line from x,0 to x,f(x) for the current x.
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to
 */
void cosineIntegralFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 8;
    can.bindToButton(TSGL_Q, TSGL_PRESS, [&can](){  // Quit on press of Q
        can.end();
    });

    can.drawAxes(0, 0, PI/4, .5);
    long double pw = can.getPixelWidth();
    CosineFunction function1;
    can.drawFunction(function1);

    can.setFont("assets/freefont/FreeSerif.ttf");
    can.drawText(L"-1.5π", -1.5 * PI - .1, .25, 20);  // Note the important capital L, used to support Unicode.
    can.drawText(L"1.5\u03C0", 1.5 * PI - .2, .25, 20);
    can.drawText(L"1", .1, 1.05, 20);
    can.drawText(L"-1", .1, -1.1, 20);

    #pragma omp parallel num_threads(THREADS)
    {
        int nthreads = omp_get_num_threads();
        long double offset = 3*PI / nthreads;
        long double start = -1.5*PI + omp_get_thread_num() * offset;
        long double stop = start + offset;
        for (long double i = start; i < stop; i += pw) {
            if (!can.getIsOpen()) break;
            can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
            can.drawLine(i, 0, i, function1.valueAt(i), Colors::highContrastColor(omp_get_thread_num()));
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c12(0, 0, WINDOW_W, WINDOW_H, -5,-1.5,5,1.5, 16000, "", FRAME / 2);
    c12.setBackgroundColor(WHITE);
    c12.start();
    cosineIntegralFunction(c12);
    c12.close();
    glfwTerminate();  // Release GLFW
}
