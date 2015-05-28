/*
 * testImage.cpp
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
 * \brief Draws various images on a Canvas.
 * \details Very basic test function showcasing image drawing capabilities.
 * - The first 6 images are drawn opaque.
 * - The 7th image is drawn across the entire Canvas with alpha transparency.
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void imageFunction(Canvas& can) {
    can.drawImage("assets/test.png", 0, 0, 400, 300);
    can.drawImage("assets/ship.bmp", 400, 0, 400, 300);
    can.drawImage("assets/shiprgb.bmp", 800, 0, 400, 300);
    can.drawImage("assets/sky_main.jpg", 0, 300, 400, 300);
    can.drawImage("assets/Captain-falcon.png", 400, 300, 400, 300);
    can.drawImage("assets/mini.jpg", 800, 300, 400, 300);

    can.drawImage("assets/bestpicture.png", 200, 0, 800, 600, 0.25f);    //ALPHA
}

int main() {
    Canvas c21(0,0,1200,600,100, "");
    c21.setBackgroundColor(GREY);
    c21.start();
    imageFunction(c21);
    c21.close();
}
