/*
 * testImageCart.cpp
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
 * \brief Draws various images on a CartesianCanvas.
 * \details Same as imageFunction, but on a CartesianCanvas.
 * \param can, Reference to the CartesianCanvas being drawn to (Cart is a typedef for CartesianCanvas)
 */
void imageCartFunction(Cart& can) {
    can.drawImage("assets/test.png", 0, 3, 2, 1.5);
    can.drawImage("assets/ship.bmp", 2, 3, 2, 1.5);
    can.drawImage("assets/shiprgb.bmp", 4, 3, 2, 1.5);
    can.drawImage("assets/sky_main.jpg", 0, 1.5, 2, 1.5);
    can.drawImage("assets/Captain-falcon.png", 2, 1.5, 2, 1.5);
    can.drawImage("assets/mini.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("assets/bestpicture.png", 1, 3, 4, 3, 0.25f);    //ALPHA
}

int main() {
    Cart c26(0, 0, 1200, 600, 0, 0, 6, 3, "");
    c26.setBackgroundColor(GREY);
    c26.start();
    imageCartFunction(c26);
    c26.close();
}
