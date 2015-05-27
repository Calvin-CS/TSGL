/*
 * testGraydient.cpp
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
 * \brief Draws a diagonal black-to-white gradient using OMP
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in \b nthreads
 * - The outer for loop is set up in a striping pattern, and the inner for loop runs from 0 to the Canvas height
 * - The color is set to a shade of gray based on its distance from the top left of the canvas
 * - The point is drawn to the Canvas
 * .
 * \param can Reference to the Canvas being drawn to
 */
void graydientFunction(Canvas& can) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        int color;
        for (int i = omp_get_thread_num(); i < can.getWindowWidth(); i += nthreads) {
            for (int j = 0; j < can.getWindowHeight(); j++) {
                color = i * MAX_COLOR / 2 / can.getWindowWidth() + j * MAX_COLOR / 2 / can.getWindowHeight();
                can.drawPoint(i, j, ColorInt(color, color, color));
            }
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c(BUFFER);
    c.setBackgroundColor(GREY);
    c.start();
    graydientFunction(c);
    c.close();
    glfwTerminate();  // Release GLFW
}
