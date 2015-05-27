/*
 * testColorWheel.cpp
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
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and private per-thread variables
 * \details
 * - \b THREADS is set to a number greater than the number of physical processors
 * - The center of the canvas is computed and stored.
 * - The radius of the wheel is computed (using the minimum of the Canvas width / height) and stored.
 * - The size of the \b GRADIENT is computed and stored.
 * - Variables for the second and third vertices of a triangle and the current shading are initialized.
 * - The internal timer of the Canvas is set up to expire every \b FRAME seconds.
 * - The predetermined number of parallel threads is forked off using OMP, pulling along the coordinate
 * and shading variables.
 * - The actual number of threads is stored in: \b nthreads.
 * - A color \b delta is computed.
 * - Each thread's thread id ( \b tid ) is stored.
 * - Each thread's shading is computed using it's id and \b nthreads
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn
 *   - Each thread's \b start is computed using the timer's lifetime and it's current id
 *   - The second and third coordinates of a triangle approximating an arc of a circle are
 *   computed using the \b GRADIENT and the thread's \b start position.
 *   A triangle is drawn on the Canvas for each thread, with the first vertex in the center, and
 *   the second and third vertices as computed above, with a hue based on the precomputed \b start,
 *   full saturation, and a value of \b shading.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void colorWheelFunction(Canvas& can) {
    const int THREADS = 64,                           // Number of threads to compute with
              WINDOW_CW = can.getWindowWidth() / 2,   // Set the center of the window
              WINDOW_CH = can.getWindowHeight() / 2;
    const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,  // Radius of wheel
    GRADIENT = 2 * PI / NUM_COLORS;                   // Gap between wedges
    float x2, x3, y2, y3, shading;
    #pragma omp parallel num_threads(THREADS) private(x2,x3,y2,y3,shading)
    {
        int nthreads = omp_get_num_threads();
        int delta = NUM_COLORS / nthreads;           // Distance between threads to compute
        int tid = omp_get_thread_num();
        shading = 1 - (float) tid / nthreads;
        while (can.getIsOpen()) {
            can.sleep();
            int start = (NUM_COLORS - can.getReps() % NUM_COLORS + tid * delta) % NUM_COLORS;
            x2 = WINDOW_CW + RADIUS * sin(GRADIENT * start);
            y2 = WINDOW_CH + RADIUS * cos(GRADIENT * start);
            x3 = WINDOW_CW + RADIUS * sin(GRADIENT * (start + 1));
            y3 = WINDOW_CH + RADIUS * cos(GRADIENT * (start + 1));
            can.drawTriangle(WINDOW_CW, WINDOW_CH, x2, y2, x3, y3,
                             ColorHSV(start * 6.0f / NUM_COLORS, 1.0f, shading));
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c10(0, 0, WINDOW_W, WINDOW_H, 512, "", FRAME);
    c10.setBackgroundColor(GREY);
    c10.start();
    colorWheelFunction(c10);
    c10.close();
    glfwTerminate();  // Release GLFW
}
