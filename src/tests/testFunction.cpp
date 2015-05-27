/*
 * testFunction.cpp
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
 * \brief Draws the outputs of some functions using CartesianCanvas
 * \details
 * - Draw axes through the origin, with spacing 1.0 between x ticks and 5.0 between y ticks.
 * - Initialize a CosineFunction, and draw it using the currently rendered area of the CartesianCanvas.
 * - Initialize a PowerFunction with argument 2 (square), and draw it using
 * the currently rendered area of the CartesianCanvas.
 * - Declare a new function that computes some bizarre polynomial.
 * - Initialize the new function, and draw it using the currently rendered area of the CartesianCanvas.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void functionFunction(CartesianCanvas& can) {
    can.drawAxes(0, 0, 1, 5);

    CosineFunction function1;
    can.drawFunction(function1);

    PowerFunction function2(2);
    can.drawFunction(function2);

    class myFunction : public Function {
     public:
        virtual long double valueAt(long double x) const {
            return 5 * pow(x, 4) + 2 * pow(x, 3) + x + 15;
        }
    };

    myFunction function3;
    can.drawFunction(function3);
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c11(0, 0, WINDOW_W, WINDOW_H, -5,-5,5,50, 100, "");
    c11.setBackgroundColor(WHITE);
    c11.start();
    functionFunction(c11);
    c11.close();
    glfwTerminate();  // Release GLFW
}
