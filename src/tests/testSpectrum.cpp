/*
 * testSpectrum.cpp
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

/*!
 * \brief Draws the full spectrum across the x, y, and time dimensions at the given framerate
 * and a static number of threads using OMP
 * \details
 * - A timer is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS).
 * - A parallel block is set up with OMP, using one thread per processor.
 * - The actual number of threads spawned is stored in \b nthreads.
 * - While the canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - An outer for loop from 0 to 255 is set up in a per-thread striping pattern.
 *   - An inner for loop runs from 0 to the 255 normally.
 *   - Each point is drawn to the canvas, with x, y, and time representing red, green, and blue respectively.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void spectrumFunction(Canvas& can) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        while (can.getIsOpen()) {
            can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
            for (int i = omp_get_thread_num(); i < NUM_COLORS; i += nthreads)
                for (int j = 0; j < NUM_COLORS; j++)
                    can.drawPoint(i, j, ColorInt(i, j, can.getReps() % NUM_COLORS));
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c4(0,0,255,255,65536, "", FRAME);
    c4.setBackgroundColor(GREY);
    c4.start();
    spectrumFunction(c4);
    c4.close();
    glfwTerminate();  // Release GLFW
}

