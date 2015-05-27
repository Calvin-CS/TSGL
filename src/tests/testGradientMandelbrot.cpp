/*
 * testGradientMandelbrot.cpp
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
 * \brief Draws a gradient Mandelbrot set on a CartesianCanvas
 * \details Same as mandelbrotFunction(), but with smoother shading ( see
 * http://linas.org/art-gallery/escape/smooth.html ).
 * \param can Reference to the CartesianCanvas being drawn to
 */
void gradientMandelbrotFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 32;
    const unsigned int DEPTH = 32;
    Decimal firstX, firstY, secondX, secondY;
    bool toRender = true;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &toRender]() {
        can.clear();
        toRender = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, &firstX, &firstY]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), firstX, firstY);
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, &firstX, &firstY, &secondX, &secondY, &toRender]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), secondX, secondY);
        can.zoom(firstX, firstY, secondX, secondY);
        toRender = true;
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, &toRender]() {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        can.zoom(x, y, 1.5);
        toRender = true;
    });
    can.bindToScroll([&can, &toRender](double dx, double dy) {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        Decimal scale;
        if (dy == 1) scale = .5;
        else scale = 1.5;
        can.zoom(x, y, scale);
        toRender = true;
    });

    while (toRender) {
        toRender = false;
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int nthreads = omp_get_num_threads();
            double blockstart = can.getCartHeight() / nthreads;
            unsigned int iterations;
            double smooth;
            for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
                long double row = blockstart * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
                for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                    complex c(col, row);
                    complex z(col, row);
                    smooth = exp(-std::abs(z));
                    iterations = 0;
                    while (std::abs(z) < 2.0l && iterations != DEPTH) {
                        iterations++;
                        z = z * z + c;
                        smooth += exp(-std::abs(z));
                    }
                    int i;
                    for (i = 0; i < 2; i++) {
                        iterations++;
                        z = z * z + c;
                        smooth += exp(-std::abs(z));
                    }
                    smooth /= (DEPTH + i + 1);
                    can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, 1.0f, 1.0f));
                    if (toRender) break;
                }
                if (toRender) break;
            }
        }
        while (can.getIsOpen() && !toRender)
            can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c16(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER, "", FRAME / 2);
    c16.setBackgroundColor(GREY);
    c16.start();
    gradientMandelbrotFunction(c16);
    c16.close();
    glfwTerminate();  // Release GLFW
}
