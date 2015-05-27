/*
 * testTextTwo.cpp
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

/**
 * \brief Tests to see if text is still drawn if a font is specified (just to make sure the error handling for when one is not specified didn't break anything).
 * \details Same as textFunction, but without the setFont line uncommented out.
 * \param can, Reference to the Canvas being drawn to
 */
void textFunctionTwo(Canvas& can) {
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.setFont("assets/freefont/FreeMono.ttf");  //This line is commnted out (or has been deleted) in textFunction()
    can.drawText(L"A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", 16, 150, 32, RED);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
                 32, BLUE);
    can.drawText(L"Of *what* exactly that extraordinary event was.", 16, 450, 32, GREY);
    can.drawText(L"And to that I say...oh well.", 16, 550, 32, WHITE);
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c24(10);
    c24.setBackgroundColor(GREY);
    c24.start();
    textFunctionTwo(c24);
    c24.close();
    glfwTerminate();  // Release GLFW
}
