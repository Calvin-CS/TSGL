/*
 * testColorPoints.cpp
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
 * \brief Draws a neat pattern of points to a canvas using OMP
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in \b nthreads
 * - The number of lines per thread is calculated and stored in \b myPart
 * - The starting position of each given thread is calculated and stored in \b myStart
 * - The outer for loop is set up in a block pattern, and the inner for loop runs from 100 to the Canvas width - 100
 * - Some dark voodoo magic is used to calculate the color for a given point
 * - The point is drawn to the Canvas
 * .
 * \param can Reference to the Canvas being drawn to
 */
void colorPointsFunction(Canvas& can) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        int myPart = can.getWindowHeight() / nthreads;
        int myStart = myPart * omp_get_thread_num();
        for (int j = myStart; j < myStart + myPart; j++) {
            for (int i = 100; i < can.getWindowWidth() - 100; i++) {
                if (i % 2 == 0)
                    can.drawPoint(i, j, ColorInt(j % NUM_COLORS, i % NUM_COLORS, (i * j) % 113));
                else
                    can.drawPoint(i, j, ColorInt(i % NUM_COLORS, j % NUM_COLORS, (i * j) % NUM_COLORS));
            }
            if (!can.getIsOpen()) break;
        }
    }
}


int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c1(BUFFER);
    c1.setBackgroundColor(GREY);
    c1.start();
    colorPointsFunction(c1);
    c1.close();
    glfwTerminate();  // Release GLFW
}
