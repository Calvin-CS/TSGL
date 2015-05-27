/*
 * testNova.cpp
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
 * \brief Draws a gradient Nova set on a CartesianCanvas
 * \details Same as gradientMandelbrotFunction(), but with a nova fractal ( more time-consuming
 * to compute; see http://en.wikipedia.org/wiki/Nova_fractal ).
 * \param can, Reference to the CartesianCanvas being drawn to
 */
void novaFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 32;
    const unsigned int DEPTH = 200;
    const long double R = 1.0l;
    #pragma omp parallel num_threads(THREADS)
    {
        unsigned int nthreads = omp_get_num_threads();
        double BLOCKSTART = (can.getMaxY() - can.getMinY()) / nthreads;
        unsigned int iterations;
        double smooth;
        for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
            long double row = BLOCKSTART * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
            for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                complex c(col, row);
                complex z(1, 0);
                smooth = exp(-std::abs(z));
                complex n, d, c1;
                complex r(1, 0);
                iterations = 0;
                while (std::abs(z) < 2.0l && iterations != DEPTH) {
                    iterations++;
                    n = z * z * z - 1.0l;
                    d = z * z * 3.0l;
                    z = z + c - (R * n / d);
                    smooth += exp(-std::abs(z));
                }
                for (int i = 0; i < 20; i++) {
                    iterations++;
                    n = z * z * z - 1.0l;
                    d = z * z * 3.0l;
                    z = z + c - (R * n / d);
                    smooth += exp(-std::abs(z));
                }
                smooth /= DEPTH;
                if (smooth != smooth || smooth < 0)  // Check to see if smooth is NAN
                smooth = 0;
                while (smooth > 1)
                    smooth -= 1;
                can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, (float) smooth, 1.0f));
            }
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c17(0, 0, WINDOW_W, WINDOW_H, -1, -0.5, 0, 0.5, BUFFER, "");
    c17.setBackgroundColor(GREY);
    c17.start();
    novaFunction(c17);
    c17.close();
    glfwTerminate();  // Release GLFW
}
