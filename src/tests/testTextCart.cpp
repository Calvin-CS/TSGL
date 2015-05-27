/*
 * testTextCart.cpp
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
 * \brief Draws some text on a CartesianCanvas
 * \details Same as textFunction, but with a CartesianCanvas and black text.
 * \param can Reference to the CartesianCanvas being drawn to
 */
void textCartFunction(Cart& can) {
    can.setFont("assets/freefont/FreeSerif.ttf");
    can.drawText(L"A long time ago, in a galaxy far, far away.", .05, .8, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", .05, .6, 32, BLACK);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", .05, .4, 32, BLACK);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", .05, .3,
                 32, BLACK);
    can.drawText(L"Of *what* exactly that extraordinary event was.", .05, .2, 32, BLACK);
    can.drawText(L"And to that I say...oh well.", .05, .1, 32, BLACK);
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Cart c27(0, 0, WINDOW_W, WINDOW_H, 0, 0, 4, 3, 10, "");
    c27.setBackgroundColor(GREY);
    c27.start();
    textCartFunction(c27);
    c27.close();
    glfwTerminate();  // Release GLFW
}
